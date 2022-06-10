namespace YaeAchievement.Win32; 

public static class MemoryProtection {
    public const int Execute = 0x10;
    public const int ExecuteRead = 0x20;
    public const int ExecuteReadWrite = 0x40;
    public const int ExecuteWriteCopy = 0x80;
    public const int NoAccess = 0x01;
    public const int ReadOnly = 0x02;
    public const int ReadWrite = 0x04;
    public const int WriteCopy = 0x08;
    public const int GuardModifierFlag = 0x100;
    public const int NoCacheModifierFlag = 0x200;
    public const int WriteCombineModifierFlag = 0x400;
}
