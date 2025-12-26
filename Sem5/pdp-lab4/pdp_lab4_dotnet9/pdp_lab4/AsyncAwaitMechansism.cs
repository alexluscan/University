using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace pdp_lab4;

public static class AsyncAwaitMechansism
{
    public static Task RunAsync(IEnumerable<DownloadRequest> requests)
    {
        var downloadTasks = requests.Select(DownloadAsync);
        return Task.WhenAll(downloadTasks);
    }

    private static async Task DownloadAsync(DownloadRequest request)
    {
        var address = await ResolveAsync(request.Host);
        using var socket = new Socket(address.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
        var endpoint = new IPEndPoint(address, request.Port);
        var parser = new HttpResponseBuffer(request.DisplayName);

        await ConnectAsync(socket, endpoint);
        var payload = request.BuildRequestBytes();
        await SendAsync(socket, payload);
        await ReceiveAsync(socket, parser);

        parser.PrintSummary("async/await pipeline completed");
    }

    private static async Task<IPAddress> ResolveAsync(string host)
    {
        var entry = await Dns.GetHostEntryAsync(host);
        return entry.AddressList.FirstOrDefault(ip => ip.AddressFamily == AddressFamily.InterNetwork)
               ?? entry.AddressList[0];
    }

    private static Task ConnectAsync(Socket socket, EndPoint endpoint)
    {
        var tcs = new SocketCompletionSource();
        socket.BeginConnect(endpoint, ar =>
        {
            try
            {
                socket.EndConnect(ar);
                tcs.SetResult();
            }
            catch (Exception ex)
            {
                tcs.SetException(ex);
            }
        }, null);
        return tcs.Task;
    }

    private static Task SendAsync(Socket socket, byte[] payload)
    {
        var tcs = new SocketCompletionSource();
        socket.BeginSend(payload, 0, payload.Length, SocketFlags.None, ar =>
        {
            try
            {
                socket.EndSend(ar);
                tcs.SetResult();
            }
            catch (Exception ex)
            {
                tcs.SetException(ex);
            }
        }, null);
        return tcs.Task;
    }

    private static async Task ReceiveAsync(Socket socket, HttpResponseBuffer parser)
    {
        var buffer = new byte[HttpResponseBuffer.BufferLength];
        while (true)
        {
            var bytes = await ReceiveChunkAsync(socket, buffer);
            if (bytes <= 0)
            {
                break;
            }

            parser.Append(buffer.AsSpan(0, bytes));
            if (parser.BodyComplete)
            {
                break;
            }
        }
    }

    private static Task<int> ReceiveChunkAsync(Socket socket, byte[] buffer)
    {
        var tcs = new TaskCompletionSource<int>(TaskCreationOptions.RunContinuationsAsynchronously);
        socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, ar =>
        {
            try
            {
                var bytes = socket.EndReceive(ar);
                tcs.SetResult(bytes);
            }
            catch (Exception ex)
            {
                tcs.SetException(ex);
            }
        }, null);
        return tcs.Task;
    }

    private sealed class SocketCompletionSource : TaskCompletionSource<bool>
    {
        public SocketCompletionSource() : base(TaskCreationOptions.RunContinuationsAsynchronously)
        {
        }

        public void SetResult() => SetResult(true);
    }
}
