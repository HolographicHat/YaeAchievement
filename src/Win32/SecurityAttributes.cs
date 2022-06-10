using System.Runtime.InteropServices;

// ReSharper disable MemberCanBePrivate.Global
// ReSharper disable FieldCanBeMadeReadOnly.Global

namespace YaeAchievement.Win32; 

[StructLayout(LayoutKind.Sequential)]
public struct SecurityAttributes {
    public int nLength;
    public IntPtr lpSecurityDescriptor;
}
