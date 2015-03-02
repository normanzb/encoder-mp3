#ifndef AS3API_H
#define AS3API_H

#include "as3helper.h"

void AS3_Compress() __attribute__((
    used,
    annotate("as3sig:public function compress(wave:ByteArray):ByteArray"),
    annotate("as3package:im.norm.data.encoders.MP3Encoder.shine"),
    annotate("as3import:flash.utils.ByteArray")
));

#endif
