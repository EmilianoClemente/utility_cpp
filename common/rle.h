#ifndef RLE_H
#define RLE_H

#include "io.h"

namespace rle{

enum Result{
	eOk=0,
	eBufferOver,
};

Result Compress8(common::io::Reader* in, common::io::Writer* out);

Result Decompress8(common::io::Reader* in, common::io::Writer* out);

Result Compress16(common::io::Reader* in, common::io::Writer* out);

Result Decompress16(common::io::Reader* in, common::io::Writer* out);

};

#endif
