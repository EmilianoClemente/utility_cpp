#include "MemoryIo.h"
#include <string.h>

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


#ifdef EXECUTE_TEST
#include <stdio.h>
int testMemoryReader(){
	char data[] = "Hello world!";
	enum{DataSize = sizeof(data)};
	int64_t newoffset;

	io::MemoryReader reader(data, DataSize);

	for(int i=0; i<2; i++){
		// check read
		char buffer[DataSize * 2];
		int rlen = reader.Read(buffer, sizeof(buffer));

		if(rlen != DataSize){
			printf("read error\n");
			return -1;
		}

		if(memcmp(buffer, data, DataSize) != 0){
			printf("read error\n");
			return -1;
		}

		reader.Seek(0, io::Seeker::Start, newoffset);
	}

	{
		// check seek
		char buffer[DataSize * 2];
		reader.Seek(5, io::Seeker::Start, newoffset);
		if(newoffset != 5){
			printf("seek error\n");
			return -1;
		}

		if(reader.Read(buffer, sizeof(buffer)) != DataSize - 5){
			printf("seek error\n");
			return -1;
		}

		if(memcmp(buffer, &(data[5]), DataSize-5) != 0){
			printf("seek error\n");
			return -1;
		}

		if(io::Seeker::eOk != reader.Seek(-10, io::Seeker::End, newoffset)){
			printf("seek error\n");
			return -1;
		}

		if(newoffset != (DataSize-10)){
			printf("seek error\n");
			return -1;
		}

		if(10 != reader.Read(buffer, sizeof(buffer))){
			printf("seek error\n");
			return -1;
		}

		if(memcmp(buffer, &(data[DataSize-10]), 10) != 0){
			printf("seek error\n");
			return -1;
		}
	}

	printf("io::MemoryReader test ok\n");
	return 0;
}

int testMemoryWriter(){
	char data[] = "Hello world";
	enum{DataSize = sizeof(data)};
	char buffer[4 * DataSize];
	int64_t newoffset;

	io::MemoryWriter w(buffer, sizeof(buffer));

	if(w.Write(data, DataSize) != DataSize){
		printf("write error\n");
		return -1;
	}

	if(w.Written() != DataSize){
		printf("write error\n");
		return -1;
	}

	if(memcmp(buffer, data, DataSize) != 0){
		printf("write error\n");
		return -1;
	}

	if(w.Seek(0, io::Seeker::End, newoffset)!=io::Seeker::eOk){
		printf("seek error\n");
		return -1;
	}

	if(newoffset != DataSize){
		printf("seek error\n");
		return -1;
	}

	printf("io::MemoryWriter test ok\n");
	return 0;
}

int main(int argc, const char* argv[]){
	if(testMemoryReader() != 0){
		printf("io::MemoryReader test fail\n");
	}

	if(testMemoryWriter() != 0){
		printf("io::MemoryWriter test fail\n");
	}

	return 0;
}
#endif
