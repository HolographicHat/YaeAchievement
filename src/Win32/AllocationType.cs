namespace YaeAchievement.Win32;

public static class AllocationType {
    public const int Commit = 0x1000;
    public const int Reserve = 0x2000;
    public const int Decommit = 0x4000;
    public const int Release = 0x8000;
    public const int Reset = 0x80000;
    public const int Physical = 0x400000;
    public const int TopDown = 0x100000;
    public const int WriteWatch = 0x200000;
    public const int LargePages = 0x20000000;
}
