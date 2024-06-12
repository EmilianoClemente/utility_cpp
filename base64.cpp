#include "common/base64.h"
#include <string.h>

namespace common{

namespace base64{

const char StdEncoding[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// URLEncoding is the alternate base64 encoding defined in RFC 4648.
// It is typically used in URLs and file names.
const char URLEncoding[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

char StdEncodingDecodeMap[128];
char URLEncodingDecodeMap[128];

class DecodeMapInitializer{
    public:
        DecodeMapInitializer(const char* encoding, int len, char* map){
            memset(map, 0xff, 128);
            for(int i=0; i<len; i++){
                map[encoding[i]] = encoding[i];
            }
        }
};

DecodeMapInitializer initStdEncodingDecodeMap(StdEncoding, sizeof(StdEncoding), StdEncodingDecodeMap);
DecodeMapInitializer initURLEncodingDecodeMap(URLEncoding, sizeof(URLEncoding), URLEncodingDecodeMap);

// 11111100->0xfc
// 11110000->0xf0
// 11000000->0xc0

int Encode(io::Reader* in, io::Writer* out){
    unsigned char data[3];
    unsigned char encoded[4];
    const char* encoding= StdEncoding;

    int byte_num;

    for(;;){
        memset(data, 0, sizeof(data));
        memset(encoded, '=', sizeof(encoded));
        byte_num = in->Read(data, sizeof(data));
        if(byte_num == 0){
            break;
        }

        switch(byte_num){
        case 3:
            encoded[3] = encoding[data[2] & 0x3f];
        case 2:
            encoded[2] = encoding[((data[1] & 0x0f) << 2) | ((data[2]&0xc0)>>6)];
        case 1:
            encoded[1] = encoding[((data[0] & 0x03) << 4) | ((data[1]&0xf0)>>4)];
            encoded[0] = encoding[(data[0] & 0xfc) >> 2];
        }

        out->Write(encoded, sizeof(encoded));
    }

    return 0;
}

int Decode(io::Reader* in, io::Writer* out){
    unsigned char decoded[3];
    unsigned char data[4];

    int byte_num;

    for(;;){
        byte_num = in->Read(data, sizeof(data));
        if(byte_num == 0){
            break;
        }
    }

    return 0;
}

}
}
