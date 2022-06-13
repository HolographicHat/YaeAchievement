namespace YaeAchievement.Win32; 

[Flags]
public enum OpenFileFlags : uint {
    Explorer = 0x00080000,
    NoChangeDir = 0x00000008,
    FileMustExist = 0x00001000,
    NoNetworkButton = 0x00020000,
}
