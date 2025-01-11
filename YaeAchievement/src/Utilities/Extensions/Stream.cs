using System.Runtime.CompilerServices;

// ReSharper disable CheckNamespace

namespace Google.Protobuf;

public static class CodedInputStreamExtensions {

    [UnsafeAccessor(UnsafeAccessorKind.Method)]
    private static extern byte[] ReadRawBytes(CodedInputStream stream, int size);

    public static CodedInputStream ReadLengthDelimitedAsStream(this CodedInputStream stream) {
        return new CodedInputStream(ReadRawBytes(stream, stream.ReadLength()));
    }

}
