using Newtonsoft.Json;
using YaeAchievement.res;

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
                Console.WriteLine(App.ConfigInitGotPath, gameInstallPath);
                Console.WriteLine(App.ConfigInitPathConfirm);
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
