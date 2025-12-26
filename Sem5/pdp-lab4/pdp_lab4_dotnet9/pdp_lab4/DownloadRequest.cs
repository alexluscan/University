using System.Text;

namespace pdp_lab4;

public sealed class DownloadRequest
{
    public DownloadRequest(string host, int port, string path, string displayName)
    {
        Host = host;
        Port = port;
        Path = string.IsNullOrWhiteSpace(path) ? "/" : path;
        DisplayName = displayName;
    }

    public string Host { get; }
    public int Port { get; }
    public string Path { get; }
    public string DisplayName { get; }

    public byte[] BuildRequestBytes()
    {
        var requestText =
            $"GET {Path} HTTP/1.1\r\nHost: {Host}\r\nConnection: close\r\nUser-Agent: pdp-lab4/1.0\r\nAccept: */*\r\n\r\n";
        return Encoding.ASCII.GetBytes(requestText);
    }

    public override string ToString() => $"{DisplayName} ({Host}{Path})";
}

