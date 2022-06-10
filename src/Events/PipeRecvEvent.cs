namespace YaeAchievement.Events; 

public class PipeRecvEvent : IEventBase {

    private readonly byte[] _data;

    public PipeRecvEvent(string content) {
        _data = Convert.FromBase64String(content);
    }

    public byte[] GetData() {
        return _data;
    }
}