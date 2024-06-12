#include <stdio.h>
#include <string.h>
#include "common/base64.h"
#include "common/MemoryIo.h"

using namespace common;

int main(){
    const char text[] = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";

    const char answer[] = "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";

    char out[sizeof(text) * 4 / 3];

    io::MemoryReader r(text, sizeof(text)-1);
    io::MemoryWriter w(out, sizeof(out));

    int result = base64::Encode(&r, &w);

    out[w.Written()] = 0;
    if(memcmp(answer, out, sizeof(answer)) != 0){
        printf("encode test fail\n");
        return -1;
    }

    printf("base64 test ok\n");

    return 0;
}
