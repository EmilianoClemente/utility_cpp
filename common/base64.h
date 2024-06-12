#ifndef BASE64_H
#define BASE64_H

#include "io.h"

namespace common{

namespace base64{

int Encode(io::Reader* in, io::Writer* out);

int Decode(io::Reader* in, io::Writer* out);

}

};

#endif
