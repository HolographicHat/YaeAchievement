using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace YaeAchievement.NativeLib;

public static unsafe class Main {

    [UnmanagedCallersOnly(EntryPoint = "ElysiaMain", CallConvs = [ typeof(CallConvStdcall) ])]
    private static uint ModuleInit(void* lpThreadParameter) {
        return 0;
    }
}