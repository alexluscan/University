using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace pdp_lab4;

public static class DirectCallbacks
{
    public static WaitHandle[] StartDownloads(IEnumerable<DownloadRequest> requests)
    {
        return requests.Select(StartSingleDownload).ToArray();
    }

    private static WaitHandle StartSingleDownload(DownloadRequest request)
    {
        var completion = new ManualResetEvent(false);
        try
        {
            var entry = Dns.GetHostEntry(request.Host);
            var address = SelectAddress(entry.AddressList);
            var socket = new Socket(address.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            var endpoint = new IPEndPoint(address, request.Port);
            var state = new State(socket, request, completion);

            socket.BeginConnect(endpoint, ConnectCallback, state);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[{request.DisplayName}] Failed to start download: {ex.Message}");
            completion.Set();
        }

        return completion;
    }

    private static IPAddress SelectAddress(IReadOnlyList<IPAddress> addresses)
    {
        return addresses.FirstOrDefault(a => a.AddressFamily == AddressFamily.InterNetwork)
               ?? addresses.First();
    }

    private static void ConnectCallback(IAsyncResult ar)
    {
        var state = (State)ar.AsyncState!;
        try
        {
            state.Socket.EndConnect(ar);
            var requestBytes = state.Request.BuildRequestBytes();
            state.Socket.BeginSend(requestBytes, 0, requestBytes.Length, SocketFlags.None, SendCallback, state);
        }
        catch (Exception ex)
        {
            state.Fail("connect", ex);
        }
    }

    private static void SendCallback(IAsyncResult ar)
    {
        var state = (State)ar.AsyncState!;
        try
        {
            state.Socket.EndSend(ar);
            state.Socket.BeginReceive(state.Buffer, 0, state.Buffer.Length, SocketFlags.None, ReceiveCallback, state);
        }
        catch (Exception ex)
        {
            state.Fail("send", ex);
        }
    }

    private static void ReceiveCallback(IAsyncResult ar)
    {
        var state = (State)ar.AsyncState!;
        try
        {
            var bytesReceived = state.Socket.EndReceive(ar);
            if (bytesReceived <= 0)
            {
                state.Complete("server closed connection");
                return;
            }

            state.Parser.Append(state.Buffer.AsSpan(0, bytesReceived));

            if (state.Parser.BodyComplete)
            {
                state.Complete("Content-Length reached");
                return;
            }

            state.Socket.BeginReceive(state.Buffer, 0, state.Buffer.Length, SocketFlags.None, ReceiveCallback, state);
        }
        catch (Exception ex)
        {
            state.Fail("receive", ex);
        }
    }

    private sealed class State
    {
        public State(Socket socket, DownloadRequest request, ManualResetEvent completion)
        {
            Socket = socket;
            Request = request;
            Completion = completion;
            Parser = new HttpResponseBuffer(request.DisplayName);
        }

        public Socket Socket { get; }
        public DownloadRequest Request { get; }
        public ManualResetEvent Completion { get; }
        public HttpResponseBuffer Parser { get; }
        public byte[] Buffer { get; } = new byte[HttpResponseBuffer.BufferLength];

        public void Complete(string reason)
        {
            Parser.PrintSummary(reason);
            Cleanup();
        }

        public void Fail(string stage, Exception ex)
        {
            Console.WriteLine($"[{Request.DisplayName}] {stage} failed: {ex.Message}");
            Cleanup();
        }

        private void Cleanup()
        {
            try
            {
                Socket.Shutdown(SocketShutdown.Both);
            }
            catch
            {
                // ignore cleanup errors
            }

            Socket.Close();
            Completion.Set();
        }
    }
}