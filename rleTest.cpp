
#include <stdio.h>
#include <string.h>

#include "common/rle.h"

using namespace common;

class DataReader : public io::Reader{
    public:
        DataReader(const char* data, int data_len){
            r_ = data;
            rest_ = data_len;
        }

		virtual int Read(void* buffer, int buffer_size){
            if(rest_ <= 0){
                return 0;
            }

            if(buffer_size > rest_){
                buffer_size = rest_;
            }

            memcpy(buffer, r_, buffer_size);
            rest_ -= buffer_size;
            r_ += buffer_size;

            return buffer_size;
        }
    private:
        const char* r_;
        int rest_;
};

class BufferWriter : public io::Writer{
    public:
        BufferWriter(char* buffer, int buffer_size){
            head_ = buffer;
            w_ = buffer;
            end_ = &(buffer[buffer_size]);
        }

		virtual int Write(const void* data, int length){
            int rest = end_ - w_;
            if(length > rest){
                length = rest;
            }

            memcpy(w_, data, length);
            w_ += length;

            return length;
        }

        int Written(){
            return w_ - head_;
        }
    private:
        char* head_;
        char* w_;
        char* end_;
};


int testRle8(){
    // data should be 10bytes after compress
    char data[] = "aaaaabbbbbbbccccdddddddd";
    enum{DataSize = sizeof(data)};
    char buffer[DataSize * 2];
    char decompress_buffer[DataSize*2];

    DataReader reader(data, DataSize);
    BufferWriter writer(buffer, sizeof(buffer));
    BufferWriter decompress_writer(decompress_buffer, sizeof(decompress_buffer));

    if(rle::eOk != rle::Compress8(&reader, &writer)){
        printf("rle 8bit test fail 1\n");
        return 1;
    }

    if(writer.Written() != 10){
        printf("rle 8bit test fail 2\n");
        printf("written:%d\n", writer.Written());
        return 1;
    }

    DataReader compress_data_reader(buffer, writer.Written());

    if(rle::eOk != rle::Decompress8(&compress_data_reader, &decompress_writer)){
        printf("rle 8bit test fail 3\n");
        return 1;
    }

    if(decompress_writer.Written() != DataSize){
        printf("rle 8bit test fail 4\n");
        return 1;
    }

    if(memcmp(decompress_buffer, data, DataSize) != 0){
        printf("rle 8bit test fail 5\n");
        return 1;
    }

    printf("rle 8bit test ok\n");
    return 0;
}

int main(int argc, const char* argv[]){
    if(testRle8()!=0){
        printf("rle 8bit test fail\n");
        return -1;
    }

	return 0;
}
