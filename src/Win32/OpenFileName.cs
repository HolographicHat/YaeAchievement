using System.Runtime.InteropServices;

// ReSharper disable MemberCanBePrivate.Global
// ReSharper disable FieldCanBeMadeReadOnly.Global

namespace YaeAchievement.Win32; 
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]  
public struct OpenFileName {
    public int     size;
    public IntPtr  owner; 
    public IntPtr  instance;
    public string  filter;
    public IntPtr  customFilter;
    public int     maxCustFilter;
    public int     filterIndex;
    public IntPtr  file;
    public int     maxFile;
    public IntPtr  fileTitle;
    public int     maxFileTitle;
    public string  initialDir;
    public string  title;
    public OpenFileFlags flags; 
    public short   fileOffset;
    public short   fileExtension;
    public string  defExt;
    public IntPtr  custData;  
    public IntPtr  hook;
    public string  templateName;
    public IntPtr  reservedPtr; 
    public int     reservedInt;
    public int     flagsEx;
}
