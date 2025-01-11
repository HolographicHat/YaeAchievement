using System.Text.Json;
using Google.Protobuf;
using Proto;

namespace YaeAchievement.Parsers;

public class PlayerStoreNotify {

    public uint WeightLimit { get; set; }

    public StoreType StoreType { get; set; }

    public List<Item> ItemList { get; set; } = [];

    public static bool OnReceive(byte[] bytes) {
        #if DEBUG
        var ntf = ParseFrom(bytes);
        File.WriteAllText("store_data.json", JsonSerializer.Serialize(ntf, new JsonSerializerOptions {
            WriteIndented = true
        }));
        #endif
        return true;
    }

    private static PlayerStoreNotify ParseFrom(byte[] bytes) {
        using var stream = new CodedInputStream(bytes);
        var ntf = new PlayerStoreNotify();
        try {
            uint tag;
            while ((tag = stream.ReadTag()) != 0) {
                var wireType = tag & 7;
                switch (wireType) {
                    case 0: { // is VarInt
                        var value = stream.ReadUInt32();
                        if (value < 10) {
                            ntf.StoreType = (StoreType) value;
                        } else {
                            ntf.WeightLimit = value;
                        }
                        continue;
                    }
                    case 2: { // is LengthDelimited
                        using var eStream = stream.ReadLengthDelimitedAsStream();
                        while (eStream.PeekTag() != 0) {
                            ntf.ItemList.Add(Item.Parser.ParseFrom(eStream));
                        }
                        break;
                    }
                }
            }
        } catch (InvalidProtocolBufferException) {
            // ReSharper disable once LocalizableElement
            Console.WriteLine("Parse failed");
            File.WriteAllBytes("store_raw_data.bin", bytes);
            Environment.Exit(0);
        }
        return ntf;
    }

}
