using System.Net;
using System.Text;
using Microsoft.Win32;
using Newtonsoft.Json;
using YaeAchievement.res;
using static AchievementAllDataNotify.Types.Achievement.Types;

namespace YaeAchievement; 

public static class Export {
    
    public static void Choose(AchievementAllDataNotify data) {
        Console.Write(App.ExportChoose);
        if (!int.TryParse(Console.ReadLine(), out var num)) num = 0;
        ((Action<AchievementAllDataNotify>) (num switch {
            1 => ToSnapGenshin,
            2 => ToPaimon,
            3 => ToSeelie,
            4 => ToCSV,
            5 => ToXunkong,
            6 => ToWxApp1,
            7 => ToRawJson,
            _ => ToCocogoat
        })).Invoke(data);
    }

    private static void ToCocogoat(AchievementAllDataNotify data) {
        var result = JsonConvert.SerializeObject(ExportToUIAFApp(data));
        using var request = new HttpRequestMessage {
            Method = HttpMethod.Post,
            RequestUri = new Uri($"https://77.cocogoat.work/v1/memo?source={App.AllAchievement}"),
            Content = new StringContent(result, Encoding.UTF8, "application/json")
        };
        using var response = Utils.CHttpClient.Value.Send(request);
        if (response.StatusCode != HttpStatusCode.Created) {
            Console.WriteLine(App.ExportToCocogoatFail);
            return;
        }
        dynamic memo = JsonConvert.DeserializeObject(response.Content.ReadAsStringAsync().Result)!;
        Console.WriteLine(Utils.ShellOpen($"https://cocogoat.work/achievement?memo={memo.key}")
            ? App.ExportToCocogoatSuccess
            : $"https://cocogoat.work/achievement?memo={memo.key}");
    }
    
    private static void ToWxApp1(AchievementAllDataNotify data) {
        var id = Guid.NewGuid().ToString("N").Substring(20, 8);
        var result = JsonConvert.SerializeObject(new Dictionary<string, object> {
            { "key", id },
            { "data", ExportToUIAFApp(data) }
        });
        using var request = new HttpRequestMessage {
            Method = HttpMethod.Post,
            RequestUri = new Uri("https://api.qyinter.com/achievementRedis"),
            Content = new StringContent(result, Encoding.UTF8, "application/json")
        };
        using var response = Utils.CHttpClient.Value.Send(request);
        Console.WriteLine(App.ExportToWxApp1Success, id);
    }

    private static void ToSnapGenshin(AchievementAllDataNotify data) {
        if (CheckSnapScheme()) {
            Utils.CopyToClipboard(JsonConvert.SerializeObject(ExportToUIAFApp(data)));
            Utils.ShellOpen("snapgenshin://achievement/import/uiaf");
            Console.WriteLine(App.ExportToSnapGenshinSuccess);
        } else {
            Console.WriteLine(App.ExportToSnapGenshinNeedUpdate);
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
        File.WriteAllText(path, JsonConvert.SerializeObject(final));
        Console.WriteLine(App.ExportToFileSuccess, path);
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
        File.WriteAllText(path, JsonConvert.SerializeObject(final));
        Console.WriteLine(App.ExportToFileSuccess, path);
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
                finishAt = DateTimeOffset.FromUnixTimeSeconds(ts).ToString("yyyy/MM/dd HH:mm:ss");
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
        File.WriteAllText(path, $"\uFEFF{string.Join("\n", output)}");
        Console.WriteLine(App.ExportToFileSuccess, path);
    }

    private static void ToXunkong(AchievementAllDataNotify data) {
        if (CheckXunkongScheme()) {
            Utils.CopyToClipboard(JsonConvert.SerializeObject(ExportToUIAFApp(data)));
            Utils.ShellOpen("xunkong://import-achievement?caller=YaeAchievement&from=clipboard");
            Console.WriteLine(App.ExportToXunkongSuccess);
        } else {
            Console.WriteLine(App.ExportToXunkongNeedUpdate);
            Utils.ShellOpen("ms-windows-store://pdp/?productid=9N2SVG0JMT12");
        }
    }

    private static void ToRawJson(AchievementAllDataNotify data) {
        var path = Path.GetFullPath($"export-{DateTime.Now:yyyyMMddHHmmss}-raw.json");
        File.WriteAllText(path, JsonConvert.SerializeObject(data, Formatting.Indented));
        Console.WriteLine(App.ExportToFileSuccess, path);
    }

    // ReSharper disable once InconsistentNaming
    private static Dictionary<string, object> ExportToUIAFApp(AchievementAllDataNotify data) {
        var output = data.List
            .Where(a => (uint)a.Status > 1 || a.Current > 0)
            .Select(ach => new Dictionary<string, uint> {
                ["id"] = ach.Id,
                ["status"] = (uint) ach.Status,
                ["current"] = ach.Current,
                ["timestamp"] = ach.Timestamp,
            })
            .ToList();
        return new Dictionary<string, object> {
            ["info"] = new Dictionary<string, object> {
                ["export_app"] = "YaeAchievement",
                ["export_timestamp"] = DateTimeOffset.Now.ToUnixTimeMilliseconds(),
                ["export_app_version"] = GlobalVars.AppVersionName,
                ["uiaf_version"] = "v1.1"
            },
            ["list"] = output
        };
    }

    #pragma warning disable CA1416
    private static bool CheckSnapScheme() {
        return (string?)Registry.ClassesRoot.OpenSubKey("snapgenshin")?.GetValue("") == "URL:snapgenshin";
    }

    private static bool CheckXunkongScheme() {
        return (string?)Registry.ClassesRoot.OpenSubKey("xunkong")?.GetValue("") == "URL:xunkong";
    }
    #pragma warning restore CA1416

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
}
