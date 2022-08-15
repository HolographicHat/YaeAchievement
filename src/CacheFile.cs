using System.IO.Compression;
using Google.Protobuf;

namespace YaeAchievement; 

public class CacheFile {

    private readonly string _cacheName;
    private CacheItem? _content;
    
    public DateTime LastWriteTime => Exists() ? File.GetLastWriteTimeUtc(_cacheName) : DateTime.UnixEpoch;

    public CacheFile(string identifier) {
        Directory.CreateDirectory(Path.Combine(GlobalVars.AppPath, "cache"));
        _cacheName = Path.Combine(GlobalVars.AppPath, $"cache/{identifier.MD5Hash()[..16]}.miko");
    }

    public bool Exists() => File.Exists(_cacheName);

    public CacheItem Read() {
        if (_content == null) {
            using var fInput = File.OpenRead(_cacheName);
            using var dInput = new GZipStream(fInput, CompressionMode.Decompress);
            _content = CacheItem.Parser.ParseFrom(dInput);
        }
        return _content;
    }

    public void Write(byte[] data, string? etag = null) {
        using var fOut = File.OpenWrite(_cacheName);
        using var cOut = new GZipStream(fOut, CompressionLevel.SmallestSize);
        new CacheItem {
            Etag = etag ?? string.Empty,
            Version = 3,
            Checksum = data.MD5Hash(),
            Content = ByteString.CopyFrom(data)
        }.WriteTo(cOut);
    }
}
