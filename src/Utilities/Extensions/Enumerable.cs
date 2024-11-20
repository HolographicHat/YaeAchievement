// ReSharper disable once CheckNamespace

namespace System.Linq;

public static class Enumerable {

    public static IEnumerable<IGrouping<TKey, TKey>> GroupKeys<TKey, TValue>(
        this IEnumerable<Dictionary<TKey, TValue>> source,
        Func<TValue, bool> condition
    ) where TKey : notnull {
        return source.SelectMany(dict => dict.Where(pair => condition(pair.Value)).Select(pair => pair.Key)).GroupBy(x => x);
    }

}