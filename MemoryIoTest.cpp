#include <stdio.h>
#include <string.h>
#include "common/MemoryIo.h"

using namespace common;

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
