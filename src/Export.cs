using System.ComponentModel;
using System.Diagnostics;
using System.Net;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using Microsoft.Win32;
using YaeAchievement.Parsers;
using YaeAchievement.res;

namespace YaeAchievement;

public static class Export {

    public static uint ExportTo { get; set; } = uint.MaxValue;

    private static readonly JsonSerializerOptions JsonOpts = new () {
        WriteIndented = true
    };

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
            7 => ToTeyvatGuide,
            8 => ToUIAFJson,
            9 => ToRawJson,
            _ => ToCocogoat
        })).Invoke(data);
    }

    private class CocogoatResponse {
        [JsonPropertyName("key")] public string Code { get; init; } = null!;
    }

    private static void ToCocogoat(AchievementAllDataNotify data) {
        var result = JsonSerializer.Serialize(ExportToUIAFApp(data));
        using var request = new HttpRequestMessage();
        request.Method = HttpMethod.Post;
        request.RequestUri = new Uri($"https://77.cocogoat.cn/v1/memo?source={App.AllAchievement}");
        request.Content = new StringContent(result, Encoding.UTF8, "application/json");
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
        using var request = new HttpRequestMessage();
        request.Method = HttpMethod.Post;
        request.RequestUri = new Uri("https://api.qyinter.com/achievementRedis");
        request.Content = new StringContent(result, Encoding.UTF8, "application/json");
        using var response = Utils.CHttpClient.Send(request);
        Console.WriteLine(App.ExportToWxApp1Success, id);
    }

    private static void ToHuTao(AchievementAllDataNotify data) {
        if (CheckWinUIAppScheme("hutao")) {
            Utils.CopyToClipboard(JsonSerializer.Serialize(ExportToUIAFApp(data)));
            Utils.ShellOpen("hutao://achievement/import");
            Console.WriteLine(App.ExportToSnapGenshinSuccess);
        } else {
            Console.WriteLine(App.ExportToSnapGenshinNeedUpdate);
            Utils.ShellOpen("ms-windows-store://pdp/?productid=9PH4NXJ2JN52");
        }
    }

    private static void ToXunkong(AchievementAllDataNotify data) {
        if (CheckWinUIAppScheme("xunkong")) {
            Utils.CopyToClipboard(JsonSerializer.Serialize(ExportToUIAFApp(data)));
            Utils.ShellOpen("xunkong://import-achievement?caller=YaeAchievement&from=clipboard");
            Console.WriteLine(App.ExportToXunkongSuccess);
        } else {
            Console.WriteLine(App.ExportToXunkongNeedUpdate);
            Utils.ShellOpen("ms-windows-store://pdp/?productid=9N2SVG0JMT12");
        }
    }

    private static void ToTeyvatGuide(AchievementAllDataNotify data) {
        if (Process.GetProcessesByName("TeyvatGuide").Length != 0) {
            Utils.CopyToClipboard(JsonSerializer.Serialize(ExportToUIAFApp(data)));
            Utils.ShellOpen("teyvatguide://import_uigf?app=YaeAchievement");
            Console.WriteLine(App.ExportToTauriSuccess);
        } else {
            Console.WriteLine(App.ExportToTauriFail);
            Utils.ShellOpen("ms-windows-store://pdp/?productid=9NLBNNNBNSJN");
        }
    }

    // ReSharper disable once InconsistentNaming
    private static void ToUIAFJson(AchievementAllDataNotify data) {
        var path = Path.GetFullPath($"uiaf-{DateTime.Now:yyyyMMddHHmmss}.json");
        if (TryWriteToFile(path, JsonSerializer.Serialize(ExportToUIAFApp(data)))) {
            Console.WriteLine(App.ExportToFileSuccess, path);
        }
    }

    private static void ToPaimon(AchievementAllDataNotify data) {
        var info = GlobalVars.AchievementInfo.Items.ToDictionary(pair => pair.Key, pair => pair.Value.Group);
        var final = new Dictionary<string, Dictionary<uint, Dictionary<uint, bool>>> {
            ["achievement"] = data.AchievementList
                .Where(achievement => achievement.Status is AchievementStatus.Finished or AchievementStatus.RewardTaken)
                .Where(achievement => info.ContainsKey(achievement.Id))
                .GroupBy(achievement => info[achievement.Id], achievement => achievement.Id)
                .OrderBy(group => group.Key)
                .ToDictionary(group => group.Key, group => group.ToDictionary(id => id, _ => true))
        };
        var path = Path.GetFullPath($"export-{DateTime.Now:yyyyMMddHHmmss}-paimon.json");
        if (TryWriteToFile(path, JsonSerializer.Serialize(final))) {
            Console.WriteLine(App.ExportToFileSuccess, path);
        }
    }

    private static void ToSeelie(AchievementAllDataNotify data) {
        var output = new Dictionary<uint, Dictionary<string, bool>>();
        foreach (var ach in data.AchievementList.Where(a => a.Status is AchievementStatus.Finished or AchievementStatus.RewardTaken)) {
            output[ach.Id] = new Dictionary<string, bool> {
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
        var info = GlobalVars.AchievementInfo;
        var outList = new List<List<object>>();
        foreach (var ach in data.AchievementList.OrderBy(a => a.Id)) {
            if (UnusedAchievement.Contains(ach.Id)) continue;
            if (!info.Items.TryGetValue(ach.Id, out var achInfo) || achInfo == null) {
                Console.WriteLine($@"Unable to find {ach.Id} in metadata.");
                continue;
            }
            var finishAt = "";
            if (ach.FinishTimestamp != 0) {
                var ts = Convert.ToInt64(ach.FinishTimestamp);
                finishAt = DateTimeOffset.FromUnixTimeSeconds(ts).ToLocalTime().ToString("yyyy/MM/dd HH:mm:ss");
            }
            var current = ach.Status != AchievementStatus.Unfinished
                ? ach.CurrentProgress == 0 ? ach.TotalProgress : ach.CurrentProgress
                : ach.CurrentProgress;
            outList.Add([
                ach.Id, ach.Status.ToDesc(), achInfo.Group, achInfo.Name,
                achInfo.Description, current, ach.TotalProgress, finishAt
            ]);
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

    private static void ToRawJson(AchievementAllDataNotify data) {
        var path = Path.GetFullPath($"export-{DateTime.Now:yyyyMMddHHmmss}-raw.json");
        var text = JsonSerializer.Serialize(data, JsonOpts);
        if (TryWriteToFile(path, text)) {
            Console.WriteLine(App.ExportToFileSuccess, path);
        }
    }

    // ReSharper disable once InconsistentNaming
    private static Dictionary<string, object> ExportToUIAFApp(AchievementAllDataNotify data) {
        var output = data.AchievementList
            .Where(a => (uint)a.Status > 1 || a.CurrentProgress > 0)
            .Select(ach => new Dictionary<string, uint> {
                ["id"] = ach.Id,
                ["status"] = (uint) ach.Status,
                ["current"] = ach.CurrentProgress,
                ["timestamp"] = ach.FinishTimestamp
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

    // ReSharper disable once InconsistentNaming
    private static bool CheckWinUIAppScheme(string protocol) {
        return (string?)Registry.ClassesRoot.OpenSubKey(protocol)?.GetValue("") == $"URL:{protocol}";
    }

    private static string JoinToString(this IEnumerable<object> list, string separator) {
        return string.Join(separator, list);
    }

    private static readonly List<uint> UnusedAchievement = [ 84517 ];

    private static string ToDesc(this AchievementStatus status) {
        return status switch {
            AchievementStatus.Invalid => App.StatusInvalid,
            AchievementStatus.Finished => App.StatusFinished,
            AchievementStatus.Unfinished => App.StatusUnfinished,
            AchievementStatus.RewardTaken => App.StatusRewardTaken,
            _ => throw new ArgumentOutOfRangeException(nameof(status), status, null)
        };
    }

    public static int PrintMsgAndReturnErrCode(this Win32Exception ex, string msg) {
        // ReSharper disable once LocalizableElement
        Console.WriteLine($"{msg}: {ex.Message}");
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
