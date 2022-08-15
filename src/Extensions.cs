using System.Security.Cryptography;
using System.Text;

namespace YaeAchievement;

// ReSharper disable MemberCanBePrivate.Global
public static class Extensions {

    // ReSharper disable once InconsistentNaming
    private static readonly Lazy<MD5> md5 = new (MD5.Create);
    // ReSharper disable once InconsistentNaming
    private static readonly Lazy<SHA1> sha1 = new (SHA1.Create);
    
    public static byte[] ToBytes(this string text) {
        return Encoding.UTF8.GetBytes(text);
    }
    
    // ReSharper disable once InconsistentNaming
    public static string MD5Hash(this string text) {
        return text.ToBytes().MD5Hash();
    }
    
    // ReSharper disable once InconsistentNaming
    public static string MD5Hash(this byte[] data) {
        return md5.Value.ComputeHash(data).ToHex().ToLower();
    }
    
    // ReSharper disable once InconsistentNaming
    public static string SHA1Hash(this string text, bool base64 = true) {
        var bytes = sha1.Value.ComputeHash(text.ToBytes());
        return base64 ? bytes.ToBase64() : bytes.ToHex();
    }

    public static string ToHex(this byte[] bytes) {
        return Convert.ToHexString(bytes);
    }
    
    public static string ToBase64(this byte[] bytes) {
        return Convert.ToBase64String(bytes);
    }
}
