using System.Text.Json;
using System.Text.Json.Serialization;
using Google.Protobuf;
using YaeAchievement.res;

namespace YaeAchievement.Parsers;

public enum AchievementStatus {
    Invalid,
    Unfinished,
    Finished,
    RewardTaken,
}

public class AchievementItem {
    
    public uint Id { get; init; }
    public uint TotalProgress { get; init; }
    public uint CurrentProgress { get; init; }
    public uint FinishTimestamp { get; init; }
    public AchievementStatus Status { get; init; }
    
}

public class AchievementAllDataNotify {

    public List<AchievementItem> AchievementList { get; private init; } = [];

    public static bool OnReceive(byte[] bytes) {
        GlobalVars.AchievementDataCache.Write(bytes);
        Export.Choose(ParseFrom(bytes));
        return true;
    }

    public static AchievementAllDataNotify ParseFrom(byte[] bytes) {
        using var stream = new CodedInputStream(bytes);
        var data = new List<Dictionary<uint, uint>>();
        var errTimes = 0;
        try {
            uint tag;
            while ((tag = stream.ReadTag()) != 0) {
                if ((tag & 7) == 2) { // is LengthDelimited
                    var dict = new Dictionary<uint, uint>();
                    using var eStream = stream.ReadLengthDelimitedAsStream();
                    try {
                        while ((tag = eStream.ReadTag()) != 0) {
                            if ((tag & 7) != 0) { // not VarInt
                                dict = null;
                                break;
                            }
                            dict[tag >> 3] = eStream.ReadUInt32();
                        }
                        if (dict != null) {
                            data.Add(dict);
                        }
                    } catch (InvalidProtocolBufferException) {
                        if (errTimes++ > 0) { // allows 1 fail on 'reward_taken_goal_id_list'
                            throw;
                        }
                    }
                }
            }
        } catch (InvalidProtocolBufferException) {
            // ReSharper disable once LocalizableElement
            Console.WriteLine("Parse failed");
            File.WriteAllBytes("achievement_raw_data.bin", bytes);
            Environment.Exit(0);
        }
        if (data.Count == 0) {
            return new AchievementAllDataNotify();
        }
        uint tId, sId, iId, currentId, totalId;
        if (data.Count > 20) { /* uwu */
            (tId, var cnt) = data //        ↓ 2020-09-15 04:15:14
                .GroupKeys(value => value > 1600114514).Select(g => (g.Key, g.Count())).MaxBy(p => p.Item2);
            sId = data //           FINISHED ↓    ↓ REWARD_TAKEN
                .GroupKeys(value => value is 2 or 3).First(g => g.Count() == cnt).Key;
            iId = data //                                 ↓ id: 8xxxx
                .GroupKeys(value => value / 10000 % 10 == 8).MaxBy(g => g.Count())!.Key;
            (currentId, totalId) = data
                .Where(d => d[sId] is 2 or 3)
                .Select(d => d.ToDictionary().RemoveValues(tId, sId, iId).ToArray())
                .Where(d => d.Length == 2 && d[0].Value != d[1].Value)
                .GroupBy(a => a[0].Value > a[1].Value ? (a[0].Key, a[1].Key) : (a[1].Key, a[0].Key))
                .Select(g => (FieldIds: g.Key, Count: g.Count()))
                .MaxBy(p => p.Count)
                .FieldIds;
            #if DEBUG
            // ReSharper disable once LocalizableElement
            Console.WriteLine($"Id={iId}, Status={sId}, Total={totalId}, Current={currentId}, Timestamp={tId}");
            #endif
        } else {
            var info = GlobalVars.AchievementInfo.PbInfo; // ...
            iId = info.Id;
            tId = info.FinishTimestamp;
            sId = info.Status;
            totalId = info.TotalProgress;
            currentId = info.CurrentProgress;
            if (data.Any(dict => !dict.ContainsKey(iId) || !dict.ContainsKey(sId) || !dict.ContainsKey(totalId))) {
                Console.WriteLine(App.WaitMetadataUpdate);
                Environment.Exit(0);
            }
        }
        return new AchievementAllDataNotify {
            AchievementList = data.Select(dict => new AchievementItem {
                Id = dict[iId],
                Status = (AchievementStatus) dict[sId],
                TotalProgress = dict[totalId],
                CurrentProgress = dict.GetValueOrDefault(currentId),
                FinishTimestamp = dict.GetValueOrDefault(tId),
            }).ToList()
        };
    }

}

[JsonSerializable(typeof(AchievementAllDataNotify))]
[JsonSourceGenerationOptions(
    WriteIndented = true,
    GenerationMode = JsonSourceGenerationMode.Serialization,
    PropertyNamingPolicy = JsonKnownNamingPolicy.SnakeCaseLower
)]
public partial class AchievementRawDataSerializer : JsonSerializerContext {

    public static string Serialize(AchievementAllDataNotify ntf) {
        return JsonSerializer.Serialize(ntf, Default.AchievementAllDataNotify);
    }
}
