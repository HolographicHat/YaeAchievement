using System.Net;
using System.Net.Http.Json;
using YaeAchievement.AppCenterSDK.Models;
using YaeAchievement.AppCenterSDK.Models.Serialization;

namespace YaeAchievement.AppCenterSDK; 

public static class AppCenter {

    private const string AppSecret = "648b83bf-d439-49bd-97f4-e1e506bdfe39";
    private const string ApiUrl = "https://in.appcenter.ms/logs?api-version=1.0.0";

    // ReSharper disable InconsistentNaming
    public static Guid? SessionID { get; private set; }
    public static readonly string DeviceID;
    public static readonly Device DeviceInfo;
    private static List<Log> Queue;
    private static readonly Lazy<HttpClient> httpClient = new(() => new HttpClient(new HttpClientHandler {
        Proxy = GlobalVars.DebugProxy ? new WebProxy("http://127.0.0.1:8888") : null
    }) {
        DefaultRequestHeaders = {
            { "Install-ID", DeviceID },
            { "App-Secret", AppSecret }
        }
    });

    static AppCenter() {
        Queue = new List<Log>();
        DeviceID = DeviceHelper.GetDeviceID();
        DeviceInfo = new Device();
        var running = true;
        Task.Run(() => {
            // ReSharper disable once LoopVariableIsNeverChangedInsideLoop
            while (running) {
                Upload();
                Thread.Sleep(5 * 1000);
            }
        });
        AppDomain.CurrentDomain.ProcessExit += (_, _) => {
            running = false;
        };
    }
    
    // ReSharper restore InconsistentNaming
    public static void Upload() {
        if (Queue.Count == 0) return;
        var uploadStatus = "";
        do {
            Queue = Queue.Select(log => {
                log.Status = LogStatus.Uploading;
                return log;
            }).ToList();
            using var uploadContent = new StringContent(Queue.ToJson());
            try {
                using var response = httpClient.Value.PostAsync(ApiUrl, uploadContent).Result;
                var result = response.Content.ReadFromJsonAsync<LogUploadResult>().GetAwaiter().GetResult();
                uploadStatus = result!.Status;
            } catch (Exception) {
                // ignored
            }
        } while (uploadStatus != "Success");
        Queue.RemoveAll(log => log.Status == LogStatus.Uploading);
    }
    
    public static void Init() {
        new StartServiceLog("Analytics", "Crashes").Enqueue();
        SessionID = Guid.NewGuid();
        new StartSessionLog().Enqueue();
    }

    public static void TrackCrash(Exception exception, bool fatal = true) {
        new ManagedErrorLog(exception, fatal).Enqueue();
    }

    public static void Enqueue(this Log log) {
        Queue.Add(log);
    }

    private static string ToJson(this IEnumerable<Log> queue) {
        return LogSerializer.Serialize(new LogContainer(queue));
    }
}
