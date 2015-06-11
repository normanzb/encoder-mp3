#ifndef AS3HELPER_H
#define AS3HELPER_H

#include "AS3/AS3.h"

#define AS3_GetByteArray(varName, byteArrayPtr, byteArraySize) \
    do { \
        inline_as3 \
        ( \
            #varName".position = 0;\n" \
            "var dataSize:int = "#varName".length;\n" \
            "var dataPtr:int = CModule.malloc(dataSize);\n" \
            "CModule.writeBytes(dataPtr, dataSize, "#varName");\n" \
            : : \
        ); \
        AS3_GetScalarFromVar(byteArrayPtr, dataPtr); \
        AS3_GetScalarFromVar(byteArraySize, dataSize); \
    } while (0)

#define AS3_SetByteArray(varName, byteArrayPtr, byteArraySize) \
    do { \
        inline_as3 \
        ( \
            "var "#varName":ByteArray = new ByteArray();\n" \
            "CModule.readBytes(%0, %1, "#varName");\n" \
            : : "r"(byteArrayPtr), "r"(byteArraySize) \
        ); \
    } while (0)

#define AS3_DebugCharPtr(name, value) \
    do { \
        inline_as3 \
        ( \
            "trace(CModule.readString(%0, %1) + ' : ' + CModule.readString(%2, %3));\n" \
            : : "r"(name), "r"(strlen(name)), "r"(value), "r"(strlen(value)) \
        ); \
    } while(0)

#define AS3_DebugInt(name, value) \
    do { \
        inline_as3 \
        ( \
            "trace(CModule.readString(%0, %1) + ' : ' + %2);\n" \
            : : "r"(name), "r"(strlen(name)), "r"(value) \
        ); \
    } while(0)

#define AS3_Throw(msg) \
    do { \
        inline_as3 \
        ( \
            "throw new Error(CModule.readString(%0, %1));\n" \
            : : "r"(msg), "r"(strlen(msg)) \
        ); \
    } while(0)

#endif