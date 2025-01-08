// ReSharper disable once CheckNamespace
namespace System.Collections.Generic;

public static class Collection {

    public static IDictionary<TKey, TValue> RemoveValues<TKey, TValue>(
        this IDictionary<TKey, TValue> dictionary,
        params TKey[] keys
    ) {
        foreach (var key in keys) {
            dictionary.Remove(key);
        }
        return dictionary;
    }

}
