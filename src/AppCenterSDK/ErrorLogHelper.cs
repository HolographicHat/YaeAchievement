using YaeAchievement.AppCenterSDK.Models;

namespace YaeAchievement.AppCenterSDK;

public static class ErrorLogHelper {
    public static MException CreateModelExceptionAndBinaries(Exception exception) {
        var modelException = new MException {
            Type = exception.GetType().ToString(),
            Message = exception.Message,
            StackTrace = exception.StackTrace
        };
        if (exception is AggregateException aggregateException) {
            if (aggregateException.InnerExceptions.Count != 0) {
                modelException.InnerExceptions = new List<MException>();
                foreach (var innerException in aggregateException.InnerExceptions) {
                    modelException.InnerExceptions.Add(CreateModelExceptionAndBinaries(innerException));
                }
            }
        } else if (exception.InnerException != null) {
            modelException.InnerExceptions ??= new List<MException>();
            modelException.InnerExceptions.Add(CreateModelExceptionAndBinaries(exception.InnerException));
        }
        return modelException;
    }
}
