#include "common/MemoryIo.h"
#include <string.h>

namespace common{

io::MemoryReader::MemoryReader(const void* data, int size): 
	head_(static_cast<const unsigned char*>(data)), 
	end_(&(static_cast<const unsigned char*>(data)[size])) {
	r_ = head_;
}

int io::MemoryReader::Read(void* buffer, int buffer_size){
	int rest = end_ -r_;
	if(buffer_size > rest){
		buffer_size = rest;
	}

	memcpy(buffer, r_, buffer_size);
	r_ += buffer_size;

	return buffer_size;
}

int io::MemoryReader::Seek(int64_t offset, Whence whence, int64_t &new_offset){
	const unsigned char* newpos = head_;
	switch(whence){
		case Start:
			break;
		case Current:
			newpos = r_;
			break;
		case End: 
			newpos = end_; 
			break;
	}
	newpos += offset;
	if((newpos < head_)||
		(newpos > end_)){
		return io::Seeker::eOutOfRange;
	}

	r_ = newpos;
	new_offset = r_ - head_;

	return io::Seeker::eOk;
}

io::MemoryWriter::MemoryWriter(void* data, int size):
head_(static_cast<unsigned char*>(data)),
end_(&(static_cast<unsigned char*>(data)[size])){
	w_ = head_;
	eof_ = head_;
}

int io::MemoryWriter::Write(const void* data, int length){
	int rest = end_ - w_;
	if(rest < length){
		length = rest;
	}

	memcpy(w_, data, length);
	w_ += length;

	if(w_ > eof_){
		eof_ = w_;
	}

	return length;
}

int io::MemoryWriter::Seek(int64_t offset, Whence whence, int64_t &new_offset){
	unsigned char* newpos = head_;
	switch(whence){
		case Start:
			break;
		case Current:
			newpos = w_;
			break;
		case End: 
			newpos = eof_; 
			break;
	}
	newpos += offset;
	if((newpos < head_)||
		(newpos > eof_)){
		return io::Seeker::eOutOfRange;
	}

	w_ = newpos;
	new_offset = w_ - head_;

	return io::Seeker::eOk;
}

}
