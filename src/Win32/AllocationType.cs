namespace YaeAchievement.Win32;

[Flags]
public enum AllocationType : uint {
    Commit     = 0x00001000,
    Reserve    = 0x00002000,
    Reset      = 0x00080000,
    TopDown    = 0x00100000,
    WriteWatch = 0x00200000,
    Physical   = 0x00400000,
    Rotate     = 0x00800000,
    ResetUndo  = 0x01000000,
    LargePages = 0x20000000,
    Decommit   = 0x00004000,
    Release    = 0x00008000
}
