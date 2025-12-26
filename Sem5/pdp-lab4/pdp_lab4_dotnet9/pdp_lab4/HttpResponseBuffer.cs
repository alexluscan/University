using System;
using System.Text;

namespace pdp_lab4;

/// <summary>
/// Very small helper that accumulates HTTP response data, extracts header lines,
/// and tracks completion based on the Content-Length header.
/// </summary>
public sealed class HttpResponseBuffer
{
    public const int BufferLength = 4096;

    private readonly StringBuilder _builder = new();
    private readonly string _label;

    private bool _headersParsed;
    private int _headerLength;
    private int? _contentLength;

    public HttpResponseBuffer(string label)
    {
        _label = label;
    }

    public bool HeadersParsed => _headersParsed;
    public bool HasContentLength => _contentLength.HasValue;
    public bool BodyComplete => _headersParsed && _contentLength.HasValue &&
                                (_builder.Length - _headerLength) >= _contentLength.Value;

    public void Append(ReadOnlySpan<byte> data)
    {
        var textChunk = Encoding.ASCII.GetString(data);
        _builder.Append(textChunk);

        if (!_headersParsed)
        {
            TryParseHeaders();
        }
    }

    private void TryParseHeaders()
    {
        var snapshot = _builder.ToString();
        var markerIndex = snapshot.IndexOf("\r\n\r\n", StringComparison.Ordinal);
        if (markerIndex < 0)
        {
            return;
        }

        _headersParsed = true;
        _headerLength = markerIndex + 4;
        var headerLines = snapshot[..markerIndex].Split("\r\n", StringSplitOptions.RemoveEmptyEntries);
        foreach (var line in headerLines)
        {
            if (line.StartsWith("Content-Length:", StringComparison.OrdinalIgnoreCase) &&
                int.TryParse(line["Content-Length:".Length..].Trim(), out var length))
            {
                _contentLength = length;
            }
        }
    }

    public void PrintSummary(string completionReason)
    {
        var snapshot = _builder.ToString();
        var headers = _headersParsed ? snapshot[.._headerLength] : snapshot;
        var previewStart = _headersParsed ? _headerLength : snapshot.Length;
        var previewLength = Math.Min(200, Math.Max(0, snapshot.Length - previewStart));
        var bodyPreview = previewLength > 0 ? snapshot.Substring(previewStart, previewLength) : string.Empty;

        Console.WriteLine($"[{_label}] Download finished ({completionReason}).");
        Console.WriteLine($"[{_label}] ---- Headers ----");
        Console.WriteLine(headers);
        Console.WriteLine($"[{_label}] ---- Body (first {previewLength} chars) ----");
        Console.WriteLine(bodyPreview);
        Console.WriteLine();
    }
}

