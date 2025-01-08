using System.ComponentModel;
using System.Diagnostics;
using System.Net;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using Microsoft.Win32;
using YaeAchievement.Outputs;
using YaeAchievement.Parsers;
using YaeAchievement.res;

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
            7 => ToTeyvatGuide,
            8 => ToUIAFJson,
            9 => ToRawJson,
            _ => ToCocogoat
        })).Invoke(data);
    }

    private static void ToCocogoat(AchievementAllDataNotify data) {
        var result = UIAFSerializer.Serialize(data);
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
        var responseJson = JsonSerializer.Deserialize(responseText, CocogoatResponseContext.Default.CocogoatResponse)!;
        Console.WriteLine(Utils.ShellOpen($"https://cocogoat.work/achievement?memo={responseJson.Key}")
            ? App.ExportToCocogoatSuccess
            : $"https://cocogoat.work/achievement?memo={responseJson.Key}");
    }

    private static void ToWxApp1(AchievementAllDataNotify data) {
        var id = Guid.NewGuid().ToString("N").Substring(20, 8);
        var result = WxApp1Serializer.Serialize(data, id);
        using var request = new HttpRequestMessage();
        request.Method = HttpMethod.Post;
        request.RequestUri = new Uri("https://api.qyinter.com/achievementRedis");
        request.Content = new StringContent(result, Encoding.UTF8, "application/json");
        using var response = Utils.CHttpClient.Send(request);
        Console.WriteLine(App.ExportToWxApp1Success, id);
    }

    private static void ToHuTao(AchievementAllDataNotify data) {
        if (CheckWinUIAppScheme("hutao")) {
            Utils.CopyToClipboard(UIAFSerializer.Serialize(data));
            Utils.ShellOpen("hutao://achievement/import");
            Console.WriteLine(App.ExportToSnapGenshinSuccess);
        } else {
            Console.WriteLine(App.ExportToSnapGenshinNeedUpdate);
            Utils.ShellOpen("ms-windows-store://pdp/?productid=9PH4NXJ2JN52");
        }
    }

    private static void ToXunkong(AchievementAllDataNotify data) {
        if (CheckWinUIAppScheme("xunkong")) {
            Utils.CopyToClipboard(UIAFSerializer.Serialize(data));
            Utils.ShellOpen("xunkong://import-achievement?caller=YaeAchievement&from=clipboard");
            Console.WriteLine(App.ExportToXunkongSuccess);
        } else {
            Console.WriteLine(App.ExportToXunkongNeedUpdate);
            Utils.ShellOpen("ms-windows-store://pdp/?productid=9N2SVG0JMT12");
        }
    }

    private static void ToTeyvatGuide(AchievementAllDataNotify data) {
        if (Process.GetProcessesByName("TeyvatGuide").Length != 0) {
            Utils.CopyToClipboard(UIAFSerializer.Serialize(data));
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
        if (TryWriteToFile(path, UIAFSerializer.Serialize(data))) {
            Console.WriteLine(App.ExportToFileSuccess, path);
        }
    }

    private static void ToPaimon(AchievementAllDataNotify data) {
        var path = Path.GetFullPath($"export-{DateTime.Now:yyyyMMddHHmmss}-paimon.json");
        if (TryWriteToFile(path, PaimonSerializer.Serialize(data))) {
            Console.WriteLine(App.ExportToFileSuccess, path);
        }
    }

    private static void ToSeelie(AchievementAllDataNotify data) {
        var path = Path.GetFullPath($"export-{DateTime.Now:yyyyMMddHHmmss}-seelie.json");
        if (TryWriteToFile(path, SeelieSerializer.Serialize(data))) {
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
        var text = AchievementRawDataSerializer.Serialize(data);
        if (TryWriteToFile(path, text)) {
            Console.WriteLine(App.ExportToFileSuccess, path);
        }
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

public class WxApp1Root {

    public string Key { get; init; } = null!;

    public UIAFRoot Data { get; init; } = null!;

}

[JsonSerializable(typeof(WxApp1Root))]
[JsonSourceGenerationOptions(
    GenerationMode = JsonSourceGenerationMode.Serialization,
    PropertyNamingPolicy = JsonKnownNamingPolicy.SnakeCaseLower
)]
public partial class WxApp1Serializer : JsonSerializerContext {

    public static string Serialize(AchievementAllDataNotify ntf, string key) => JsonSerializer.Serialize(new WxApp1Root {
        Key = key,
        Data = Outputs.UIAFRoot.FromNotify(ntf)
    }, Default.WxApp1Root);
}

public record CocogoatResponse(string Key);

[JsonSerializable(typeof(CocogoatResponse))]
[JsonSourceGenerationOptions(PropertyNamingPolicy = JsonKnownNamingPolicy.CamelCase)]
public partial class CocogoatResponseContext : JsonSerializerContext;
