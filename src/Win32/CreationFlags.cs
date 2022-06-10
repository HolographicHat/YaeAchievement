namespace YaeAchievement.Win32; 

[Flags]
public enum CreationFlags : uint {
    CreateSuspended = 0x00000004,
    DetachedProcess = 0x00000008,
    CreateNoWindow = 0x08000000,
    ExtendedStartupInfoPresent = 0x00080000
}
