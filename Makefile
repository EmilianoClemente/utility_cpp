all:

test:
	g++ -o  MemoryIoTest.out MemoryIo.cpp MemoryIoTest.cpp
	./MemoryIoTest.out
	g++ -o rleTest.out rle.cpp rleTest.cpp
	./rleTest.out
	g++ -o base64Test.out MemoryIo.cpp base64.cpp base64Test.cpp
	./base64Test.out
