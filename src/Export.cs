using Newtonsoft.Json;
using static AchievementAllDataNotify.Types.Achievement.Types;

namespace YaeAchievement; 

public static class Export {
    
    public static void Choose(AchievementAllDataNotify data) {
        Console.Write(@"导出至: 
        [0] 椰羊 (https://cocogoat.work/achievement)
        [1] SnapGenshin
        [2] Paimon.moe
        [3] Seelie.me
        [4] 表格文件 (默认)
        输入一个数字(0-4): ".Split("\n").Select(s => s.Trim()).JoinToString("\n") + " ");
        if (!int.TryParse(Console.ReadLine(), out var num)) num = 4;
        Action<AchievementAllDataNotify> act = num switch {
            0 => ToCocogoat,
            1 => ToSnapGenshin,
            2 => ToPaimon,
            3 => ToSeelie,
            7 => ToRawJson,
            _ => ToCSV
        };
        act(data);
    }

    private static void ToCocogoat(AchievementAllDataNotify data) {
        throw new NotImplementedException();
    }
    
    private static void ToSnapGenshin(AchievementAllDataNotify data) {
        throw new NotImplementedException();
    }
    
    private static void ToPaimon(AchievementAllDataNotify data) {
        var info = LoadAchievementInfo();
        var output = new Dictionary<uint, Dictionary<uint, bool>>();
        var path = Path.GetFullPath($"export-{DateTime.Now:yyyyMMddHHmmss}-paimon.json");
        foreach (var ach in data.List.Where(a => a.Status is Status.Finished or Status.RewardTaken)) {
            if (!info.Items.TryGetValue(ach.Id, out var achInfo) || achInfo == null) {
                Console.WriteLine($"Unable to find {ach.Id} in metadata.");
                continue;
            }
            var map = output.GetValueOrDefault(achInfo.Group, new Dictionary<uint, bool>());
            map[ach.Id == 81222 ? 81219 : ach.Id] = true;
            output[achInfo.Group] = map;
        }
        var final = new Dictionary<string, Dictionary<uint, Dictionary<uint, bool>>> {
            ["achievement"] = output.OrderBy(pair => pair.Key).ToDictionary(pair => pair.Key, pair => pair.Value)
        };
        File.WriteAllText(path, JsonConvert.SerializeObject(final));
        Console.WriteLine($"成就数据已导出至 {path}");
    }
    
    private static void ToSeelie(AchievementAllDataNotify data) {
        var path = Path.GetFullPath($"export-{DateTime.Now:yyyyMMddHHmmss}-seelie.json");
        Console.WriteLine($"成就数据已导出至 {path}");
    }

    // ReSharper disable once InconsistentNaming
    private static void ToCSV(AchievementAllDataNotify data) {
        var info = LoadAchievementInfo();
        var path = Path.GetFullPath($"achievement-{DateTime.Now:yyyyMMddHHmmss}.csv");
        var outList = new List<List<object>>();
        foreach (var ach in data.List.OrderBy(a => a.Id)) {
            if (UnusedAchievement.Contains(ach.Id)) continue;
            if (!info.Items.TryGetValue(ach.Id, out var achInfo) || achInfo == null) {
                Console.WriteLine($"Unable to find {ach.Id} in metadata.");
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
        File.WriteAllText(path, $"\uFEFF{string.Join("\n", output)}");
        Console.WriteLine($"成就数据已导出至 {path}");
    }

    private static void ToRawJson(AchievementAllDataNotify data) {
        var path = Path.GetFullPath($"export-{DateTime.Now:yyyyMMddHHmmss}-raw.json");
        File.WriteAllText(path, JsonConvert.SerializeObject(data, Formatting.Indented));
        Console.WriteLine($"成就数据已导出至 {path}");
    }

    // ReSharper disable once InconsistentNaming
    private static string ExportToUIAFApp(AchievementAllDataNotify data) {
        return "";
    }
    
    private static string JoinToString(this IEnumerable<object> list, string separator) {
        return string.Join(separator, list);
    }
    
    private static readonly List<uint> UnusedAchievement = new() { 84517 };

    private static string ToDesc(this Status status) {
        return status switch {
            Status.Invalid => "未知",
            Status.Finished => "已完成但未领取奖励",
            Status.Unfinished => "未完成",
            Status.RewardTaken => "已完成",
            _ => throw new ArgumentOutOfRangeException(nameof(status), status, null)
        };
    }

    private static AchievementInfo LoadAchievementInfo() {
        var b = Utils.GetBucketFileAsByteArray("schicksal/metadata");
        return AchievementInfo.Parser.ParseFrom(b);
    }
    
}