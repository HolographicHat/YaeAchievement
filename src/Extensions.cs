using System.Collections.Specialized;
using System.Diagnostics.CodeAnalysis;
using System.Net;
using System.Security.Cryptography;
using System.Text;
using System.Web;
using Newtonsoft.Json;

namespace YaeAchievement;

[SuppressMessage("ReSharper", "MemberCanBePrivate.Global")]
public static class Extensions {

    // ReSharper disable once InconsistentNaming
    private static readonly Lazy<Aes> aes = new (Aes.Create);
    // ReSharper disable once InconsistentNaming
    private static readonly Lazy<MD5> md5 = new (MD5.Create);
    // ReSharper disable once InconsistentNaming
    private static readonly Lazy<SHA1> sha1 = new (SHA1.Create);
    // ReSharper disable once InconsistentNaming
    private static readonly Lazy<HttpClient> defaultClient = new (() => new HttpClient(new HttpClientHandler {
        Proxy = GlobalVars.DebugProxy ? new WebProxy("http://127.0.0.1:8888") : null
    }) {
        DefaultRequestHeaders = {{ "User-Agent", "UnityPlayer/2017.4.30f1 (UnityWebRequest/1.0, libcurl/7.51.0-DEV)" }}
    });

    public static byte[] ToBytes(this string text) {
        return Encoding.UTF8.GetBytes(text);
    }
    
    public static string DecodeToString(this byte[] bytes) {
        return Encoding.UTF8.GetString(bytes);
    }
    
    // ReSharper disable once InconsistentNaming
    public static string MD5Hash(this string text) {
        return md5.Value.ComputeHash(text.ToBytes()).ToHex();
    }
    
    // ReSharper disable once InconsistentNaming
    public static string SHA1Hash(this string text, bool base64 = true) {
        var bytes = sha1.Value.ComputeHash(text.ToBytes());
        return base64 ? bytes.ToBase64() : bytes.ToHex();
    }

    public static HttpResponseMessage Send(this HttpRequestMessage message, HttpClient? client = null) {
        Logger.Trace($"{message.Method} {message.RequestUri?.GetFullPath()}");
        return (client ?? defaultClient.Value).Send(message); // dispose message?
    }
    
    public static T? Send<T>(this HttpRequestMessage message, HttpClient? client = null, Func<string, string>? onPreDeserialize = null) {
        Logger.Trace($"{message.Method} {message.RequestUri?.GetFullPath()}");
        using var response = (client ?? defaultClient.Value).Send(message);
        var text = response.Content.ReadAsStringAsync().Result;
        if (onPreDeserialize != null) {
            text = onPreDeserialize.Invoke(text);
        }
        return JsonConvert.DeserializeObject<T>(text);
    }
    
    public static string ToQueryString(this NameValueCollection collection, bool escape = true) {
        var items = collection.AllKeys
            .Select(key => escape ? 
                $"{HttpUtility.UrlEncode(key)}={HttpUtility.UrlEncode(collection[key])}" :
                $"{key}={collection[key]}");
        return string.Join("&", items);
    }
    
    public static string ToQueryString(this IEnumerable<KeyValuePair<string, object>> dict, bool escape = true) {
        var items = dict
            .Select(pair => escape ? 
                $"{HttpUtility.UrlEncode(pair.Key)}={HttpUtility.UrlEncode(pair.Value.ToString())}" :
                $"{pair.Key}={pair.Value}");
        return string.Join("&", items);
    }

    public static string ToJsonString<TKey, TValue>(this Dictionary<TKey, TValue> data) where TKey : notnull {
        return JsonConvert.SerializeObject(data);
    }

    public static string GetFullPath(this Uri uri) {
        return $"{uri.Scheme}://{uri.Host}{uri.AbsolutePath}";
    }
    
    public static IEnumerable<KeyValuePair<string, string>> ToKeyValuePairs(this NameValueCollection collection) {
        return collection.AllKeys
            .Select(key => new KeyValuePair<string, string>(key!, collection[key] ?? string.Empty))
            .ToArray();
    }

    public static string UrlEncode(this string text) {
        return HttpUtility.UrlEncode(text);
    }
    
    public static string ToHex(this byte[] bytes) {
        return Convert.ToHexString(bytes);
    }
    
    public static string ToBase64(this byte[] bytes) {
        return Convert.ToBase64String(bytes);
    }
    
    public static byte[] FromBase64(this string text) {
        return Convert.FromBase64String(text);
    }
}