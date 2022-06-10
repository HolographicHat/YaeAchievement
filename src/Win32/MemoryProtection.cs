namespace YaeAchievement.Win32;

[Flags]
public enum MemoryProtection : uint {
    Execute = 0x10,
    ExecuteRead = 0x20,
    ExecuteReadWrite = 0x40,
    ExecuteWriteCopy = 0x80,
    NoAccess = 0x01,
    ReadOnly = 0x02,
    ReadWrite = 0x04,
    WriteCopy = 0x08,
    GuardModifierFlag = 0x100,
    NoCacheModifierFlag = 0x200,
    WriteCombineModifierFlag = 0x400
}
