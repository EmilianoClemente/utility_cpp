#ifndef IO_H
#define IO_H

#include <stdint.h>

namespace common{
namespace io{

class Writer{
	public:
		virtual ~Writer(){}

		/* return bytes number has been written */
		virtual int Write(const void* data, int length)=0;
};

class Reader{
	public:
		virtual ~Reader(){}

		/* return bytes number has been read */
		virtual int Read(void* buffer, int buffer_size)=0;
};

class Closer{
	public:
		virtual ~Closer(){}
		/*
		   return 0 if close is ok,otherwise is error 
		   user should not access the Closer object after close
		   cause the object may not exist anymore.
		*/
		virtual int Close()=0;
};

class Seeker{
	public:
		enum Whence{ Start=0, Current, End };

		enum ErrorCode{
			eOk = 0,
			eOutOfRange,
		};

		virtual ~Seeker(){}
		/* 
		   return 0 when ok,otherwise return error code.
		   new offset return the position after seek operation .
		   the new offset is always base on the Whence::Start
		*/
		virtual int Seek(int64_t offset, Whence whence, int64_t &new_offset)=0;
};

/* combination of interfaces defined above */
class WriteCloser : 
	public Writer,
	public Closer{};

class ReadCloser :
	public Reader,
	public Closer{};

class WriteSeeker :
	public Writer,
	public Seeker{};

class ReadSeeker : 
	public Reader, 
	public Seeker{};

class ReadWriter :
	public Reader,
	public Writer{};

class ReadWriteCloser :
	public Reader,
	public Writer,
	public Closer{};

class ReadSeekCloser :
	public Reader,
	public Seeker, 
	public Closer{};

class WriteSeekCloser:
	public Writer,
	public Seeker,
	public Closer{};

class ReadWriteSeeker:
	public Reader,
	public Writer,
	public Seeker {};

class ReadWriteSeekCloser:
	public Reader,
	public Writer,
	public Seeker,
	public Closer {};

}
}

#endif
