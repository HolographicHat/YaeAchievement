namespace YaeAchievement; 

public static class Logger {
    
    public enum Level {
        Trace, Debug, Info, Warn, Error
    }

    public static void Error(string msg) {
        Log(msg, Level.Error);
    }
    
    public static void Warn(string msg) {
        Log(msg, Level.Warn);
    }
    
    public static void Info(string msg) {
        Log(msg, Level.Info);
    }
    
    public static void Debug(string msg) {
        Log(msg, Level.Debug);
    }
    
    public static void Trace(string msg) {
        Log(msg, Level.Trace);
    }
    
    private static void Log(string msg, Level level) {
        if (level >= GlobalVars.LogLevel) {
            Console.WriteLine($"{DateTime.Now:MM/dd HH:mm:ss} {level.ToString().ToUpper().PadLeft(5)} : {msg}");
        }
    }

    public static void WriteLog(string msg, Level level = Level.Info) {
        if (level >= GlobalVars.LogLevel) {
            Console.Write($"{DateTime.Now:MM/dd HH:mm:ss} {level.ToString().ToUpper().PadLeft(5)} : {msg}");
        }
    }
}
