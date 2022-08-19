using Newtonsoft.Json;

namespace YaeAchievement; 

public class AppConfig {
    
    [JsonProperty(PropertyName = "location")]
    public string? Location { get; set; }

    private static AppConfig? _instance;
    
    private static readonly string FileName = Path.Combine(GlobalVars.AppPath, "conf.json");

    public static string GamePath => _instance!.Location!;
    
    internal static void Load() {
        if (File.Exists(FileName)) {
            var text = File.ReadAllText(FileName);
            _instance = JsonConvert.DeserializeObject<AppConfig>(text)!;
        }
        if (_instance?.Location == null || !Utils.CheckGamePathValid(_instance.Location)) {
            var gameInstallPath = Utils.FindGamePathFromRegistry();
            if (!string.IsNullOrEmpty(gameInstallPath)) {
                Console.WriteLine($"自动读取到游戏路径: {gameInstallPath}");
                Console.WriteLine($"如果确认路径无误，请按 Y ；若有误或需要自行选择，请按 N ");
                var key = Console.ReadKey().Key;
                gameInstallPath = key == ConsoleKey.Y ? gameInstallPath : Utils.SelectGameExecutable();
            } else {
                gameInstallPath = Utils.SelectGameExecutable();
            }
            _instance = new AppConfig {
                Location = gameInstallPath
            };
            Save();
        }
    }
    
    public static void Save() {
        File.WriteAllText(FileName, JsonConvert.SerializeObject(_instance!, Formatting.Indented));
    }
}
