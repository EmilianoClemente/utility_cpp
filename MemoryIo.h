#ifndef MEMORYIO_H
#define MEMORYIO_H

#include "io.h"

namespace io{

class MemoryReader : public ReadSeeker{
	public:
		MemoryReader(const void* data, int size);
		virtual int Read(void* buffer, int buffer_size);
		virtual int Seek(int64_t offset, Whence whence, int64_t &new_offset);
	private:
		const unsigned char* const head_;
		const unsigned char* r_;
		const unsigned char* const end_;
};

class MemoryWriter : public WriteSeeker{
	public:
		MemoryWriter(void* data, int size);
		virtual int Write(const void* data, int length);
		virtual int Seek(int64_t offset, Whence whence, int64_t &new_offset);

		int Written(){return eof_ - head_;}
	private:
		unsigned char* const head_;
		unsigned char* w_;
		unsigned char* eof_;
		unsigned char* const end_;
};

};

#endif
