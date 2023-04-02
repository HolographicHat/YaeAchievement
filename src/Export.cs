using System.ComponentModel;
using System.Diagnostics;
using System.Net;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using Microsoft.Win32;
using Proto;
using YaeAchievement.AppCenterSDK;
using YaeAchievement.res;
using static Proto.Achievement.Types;

namespace YaeAchievement; 

public static class Export {

    public static uint ExportTo { get; set; } = uint.MaxValue;

    public static void Choose(AchievementAllDataNotify data) {
        if (ExportTo == uint.MaxValue) {
            Console.Write(App.ExportChoose);
            while (Console.KeyAvailable) {
                Console.ReadKey(false);
            }
            if (!uint.TryParse(Console.ReadLine(), out var num)) num = 0;
            ExportTo = num;
        }
        ((Action<AchievementAllDataNotify>) (ExportTo switch {
            1 => ToHuTao,
            2 => ToPaimon,
            3 => ToSeelie,
            4 => ToCSV,
            5 => ToXunkong,
            6 => ToWxApp1,
            7 => ToUIAFJson,
            8 => ToRawJson,
            _ => ToCocogoat
        })).Invoke(data);
    }

    private class CocogoatResponse {
        [JsonPropertyName("key")] public string Code { get; set; } = null!;
    }
    
    private static void ToCocogoat(AchievementAllDataNotify data) {
        var result = JsonSerializer.Serialize(ExportToUIAFApp(data));
        using var request = new HttpRequestMessage {
            Method = HttpMethod.Post,
            RequestUri = new Uri($"https://77.cocogoat.cn/v1/memo?source={App.AllAchievement}"),
            Content = new StringContent(result, Encoding.UTF8, "application/json")
        };
        using var response = Utils.CHttpClient.Send(request);
        if (response.StatusCode != HttpStatusCode.Created) {
            Console.WriteLine(App.ExportToCocogoatFail);
            return;
        }
        var responseText = response.Content.ReadAsStringAsync().GetAwaiter().GetResult();
        var responseJson = JsonSerializer.Deserialize<CocogoatResponse>(responseText)!;
        Console.WriteLine(Utils.ShellOpen($"https://cocogoat.work/achievement?memo={responseJson.Code}")
            ? App.ExportToCocogoatSuccess
            : $"https://cocogoat.work/achievement?memo={responseJson.Code}");
    }
    
    private static void ToWxApp1(AchievementAllDataNotify data) {
        var id = Guid.NewGuid().ToString("N").Substring(20, 8);
        var result = JsonSerializer.Serialize(new Dictionary<string, object> {
            { "key", id },
            { "data", ExportToUIAFApp(data) }
        });
        using var request = new HttpRequestMessage {
            Method = HttpMethod.Post,
            RequestUri = new Uri("https://api.qyinter.com/achievementRedis"),
            Content = new StringContent(result, Encoding.UTF8, "application/json")
        };
        using var response = Utils.CHttpClient.Send(request);
        Console.WriteLine(App.ExportToWxApp1Success, id);
    }

    private static void ToHuTao(AchievementAllDataNotify data) {
        Utils.CopyToClipboard(JsonSerializer.Serialize(ExportToUIAFApp(data)));
        Utils.ShellOpen("hutao://achievement/import");
        Console.WriteLine(App.ExportToSnapGenshinSuccess);
    }
    
    // ReSharper disable once InconsistentNaming
    private static void ToUIAFJson(AchievementAllDataNotify data) {
        var path = Path.GetFullPath($"uiaf-{DateTime.Now:yyyyMMddHHmmss}.json");
        if (TryWriteToFile(path, JsonSerializer.Serialize(ExportToUIAFApp(data)))) {
            Console.WriteLine(App.ExportToFileSuccess, path);
        }
    }

    private static void ToPaimon(AchievementAllDataNotify data) {
        var info = LoadAchievementInfo();
        var output = new Dictionary<uint, Dictionary<uint, bool>>();
        foreach (var ach in data.List.Where(a => a.Status is Status.Finished or Status.RewardTaken)) {
            if (!info.Items.TryGetValue(ach.Id, out var achInfo) || achInfo == null) {
                Console.WriteLine($@"Unable to find {ach.Id} in metadata.");
                continue;
            }
            var map = output.GetValueOrDefault(achInfo.Group, new Dictionary<uint, bool>());
            map[ach.Id == 81222 ? 81219 : ach.Id] = true;
            output[achInfo.Group] = map;
        }
        var final = new Dictionary<string, Dictionary<uint, Dictionary<uint, bool>>> {
            ["achievement"] = output.OrderBy(pair => pair.Key).ToDictionary(pair => pair.Key, pair => pair.Value)
        };
        var path = Path.GetFullPath($"export-{DateTime.Now:yyyyMMddHHmmss}-paimon.json");
        if (TryWriteToFile(path, JsonSerializer.Serialize(final))) {
            Console.WriteLine(App.ExportToFileSuccess, path);
        }
    }
    
    private static void ToSeelie(AchievementAllDataNotify data) {
        var output = new Dictionary<uint, Dictionary<string, bool>>();
        foreach (var ach in data.List.Where(a => a.Status is Status.Finished or Status.RewardTaken)) {
            output[ach.Id == 81222 ? 81219 : ach.Id] = new Dictionary<string, bool> {
                ["done"] = true
            };
        }
        var final = new Dictionary<string, Dictionary<uint, Dictionary<string, bool>>> {
            ["achievements"] = output.OrderBy(pair => pair.Key).ToDictionary(pair => pair.Key, pair => pair.Value)
        };
        var path = Path.GetFullPath($"export-{DateTime.Now:yyyyMMddHHmmss}-seelie.json");
        if (TryWriteToFile(path, JsonSerializer.Serialize(final))) {
            Console.WriteLine(App.ExportToFileSuccess, path);
        }
    }

    // ReSharper disable once InconsistentNaming
    private static void ToCSV(AchievementAllDataNotify data) {
        var info = LoadAchievementInfo();
        var outList = new List<List<object>>();
        foreach (var ach in data.List.OrderBy(a => a.Id)) {
            if (UnusedAchievement.Contains(ach.Id)) continue;
            if (!info.Items.TryGetValue(ach.Id, out var achInfo) || achInfo == null) {
                Console.WriteLine($@"Unable to find {ach.Id} in metadata.");
                continue;
            }
            var finishAt = "";
            if (ach.Timestamp != 0) {
                var ts = Convert.ToInt64(ach.Timestamp);
                finishAt = DateTimeOffset.FromUnixTimeSeconds(ts).ToLocalTime().ToString("yyyy/MM/dd HH:mm:ss");
            }
            var current = ach.Status != Status.Unfinished ? ach.Current == 0 ? ach.Total : ach.Current : ach.Current;
            outList.Add(new List<object> {
                ach.Id, ach.Status.ToDesc(), achInfo.Group, achInfo.Name, 
                achInfo.Description, current, ach.Total, finishAt
            });
        }
        var output = new List<string> { "ID,状态,特辑,名称,描述,当前进度,目标进度,完成时间" };
        output.AddRange(outList.OrderBy(v => v[2]).Select(item => {
            item[2] = info.Group[(uint) item[2]];
            return item.JoinToString(",");
        }));
        var path = Path.GetFullPath($"achievement-{DateTime.Now:yyyyMMddHHmmss}.csv");
        if (TryWriteToFile(path, $"\uFEFF{string.Join("\n", output)}")) {
            Console.WriteLine(App.ExportToFileSuccess, path);
            Process.Start("explorer.exe", $"{Path.GetDirectoryName(path)}");
        }
    }

    private static void ToXunkong(AchievementAllDataNotify data) {
        if (CheckXunkongScheme()) {
            Utils.CopyToClipboard(JsonSerializer.Serialize(ExportToUIAFApp(data)));
            Utils.ShellOpen("xunkong://import-achievement?caller=YaeAchievement&from=clipboard");
            Console.WriteLine(App.ExportToXunkongSuccess);
        } else {
            Console.WriteLine(App.ExportToXunkongNeedUpdate);
            Utils.ShellOpen("ms-windows-store://pdp/?productid=9N2SVG0JMT12");
        }
    }

    private static void ToRawJson(AchievementAllDataNotify data) {
        var path = Path.GetFullPath($"export-{DateTime.Now:yyyyMMddHHmmss}-raw.json");
        var text = JsonSerializer.Serialize(data, new JsonSerializerOptions {
            WriteIndented = true
        });
        if (TryWriteToFile(path, text)) {
            Console.WriteLine(App.ExportToFileSuccess, path);
        }
    }

    // ReSharper disable once InconsistentNaming
    private static Dictionary<string, object> ExportToUIAFApp(AchievementAllDataNotify data) {
        var output = data.List
            .Where(a => (uint)a.Status > 1 || a.Current > 0)
            .Select(ach => new Dictionary<string, uint> {
                ["id"] = ach.Id,
                ["status"] = (uint) ach.Status,
                ["current"] = ach.Current,
                ["timestamp"] = ach.Timestamp
            })
            .ToList();
        return new Dictionary<string, object> {
            ["info"] = new Dictionary<string, object> {
                ["export_app"] = "YaeAchievement",
                ["export_timestamp"] = DateTimeOffset.Now.ToUnixTimeSeconds(),
                ["export_app_version"] = GlobalVars.AppVersionName,
                ["uiaf_version"] = "v1.1"
            },
            ["list"] = output
        };
    }

    private static bool CheckXunkongScheme() {
        return (string?)Registry.ClassesRoot.OpenSubKey("xunkong")?.GetValue("") == "URL:xunkong";
    }

    private static string JoinToString(this IEnumerable<object> list, string separator) {
        return string.Join(separator, list);
    }
    
    private static readonly List<uint> UnusedAchievement = new() { 84517 };

    private static string ToDesc(this Status status) {
        return status switch {
            Status.Invalid => App.StatusInvalid,
            Status.Finished => App.StatusFinished,
            Status.Unfinished => App.StatusUnfinished,
            Status.RewardTaken => App.StatusRewardTaken,
            _ => throw new ArgumentOutOfRangeException(nameof(status), status, null)
        };
    }

    private static AchievementInfo LoadAchievementInfo() {
        var b = Utils.GetBucketFileAsByteArray("schicksal/metadata");
        return AchievementInfo.Parser.ParseFrom(b);
    }

    public static int PrintMsgAndReturnErrCode(this Win32Exception ex, string msg) {
        // ReSharper disable once LocalizableElement
        Console.WriteLine($"{msg}: {ex.Message}");
        AppCenter.TrackCrash(ex, false);
        return ex.NativeErrorCode;
    }

    private static bool TryWriteToFile(string path, string contents) {
        try {
            File.WriteAllText(path, contents);
            return true;
        } catch (UnauthorizedAccessException) {
            Console.WriteLine(App.NoWritePermission, path);
            return false;
        }
    }
}
