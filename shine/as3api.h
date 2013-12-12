#ifndef AS3API_H
#define AS3API_H

#define AS3_GetByteArray(__source, byteArrayPtr, byteArraySize) \
    do { \
        inline_as3 \
        ( \
            "var dataPtr:int = CModule.malloc(__source.length);\n" \
            "CModule.writeBytes(dataPtr, __source.length, __source);\n" \
            "var dataSize:int = __source.length;\n" \
            : : \
        ); \
        AS3_GetScalarFromVar(byteArrayPtr, dataPtr); \
        AS3_GetScalarFromVar(byteArraySize, dataSize); \
    } while (0)

#define AS3_SetByteArray(varName, byteArrayPtr, byteArraySize) \
    do { \
        inline_as3 \
        ( \
            "var varName:ByteArray = new ByteArray();\n" \
            "CModule.readBytes(%0, %1, varName);\n" \
            : : "r"(byteArrayPtr), "r"(byteArraySize) \
        ); \
    } while (0)

void AS3_Compress() __attribute__((
    used,
    annotate("as3sig:public function compress(wave:ByteArray):ByteArray"),
    annotate("as3package:im.norm.data.encoders.MP3Encoder"),
    annotate("as3import:flash.utils.ByteArray")
));

#endif
