#include "common/rle.h"
#include "stdint.h"

using namespace common;

template<typename T>
rle::Result compress_core(io::Reader* in, io::Writer* out){
	T read;
	T pre;
	T count=0;
	bool has_pre=false;
	
	while(in->Read(&read, sizeof(read)) == sizeof(read)){
		if(!has_pre){
			pre = read;
			has_pre = true;
			count = 1;
			continue;
		}

		if(read == pre){
			count++;
			continue;
		}

		if(count){
			out->Write(&pre, sizeof(pre));
			out->Write(&count, sizeof(count));
			pre = read;
			count = 1;
		}
	}

	out->Write(&pre, sizeof(pre));
	out->Write(&count, sizeof(count));

	return rle::eOk;
}

template<typename T>
rle::Result decompress_core(io::Reader* in, io::Writer* out){
	enum{eData=0, eCount, eSize};
	T pair[eSize];
	T data, count;
	int write_size;

	while(in->Read(pair, sizeof(pair)) == sizeof(pair)){
		data = pair[eData];
		count = pair[eCount];
		for(uint16_t i=0; i<count; i++){
			write_size = out->Write(&data, sizeof(data));
			if(write_size != sizeof(data)){
				return rle::eBufferOver;
			}
		}
	}

	return rle::eOk;
}

rle::Result rle::Compress8(io::Reader* in, io::Writer* out){
	return compress_core<uint8_t>(in, out);
}

rle::Result rle::Compress16(io::Reader* in, io::Writer* out){
	return compress_core<uint16_t>(in, out);
}

rle::Result rle::Decompress8(io::Reader* in, io::Writer* out){
	return decompress_core<uint8_t>(in, out);
}

rle::Result rle::Decompress16(io::Reader* in, io::Writer* out){
	return decompress_core<uint16_t>(in, out);
}

