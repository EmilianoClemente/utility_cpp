all:

test:
	g++ -o  MemoryIoTest.out MemoryIo.cpp MemoryIoTest.cpp
	./MemoryIoTest.out
	g++ -o rleTest.out rle.cpp rle_test.cpp
	./rleTest.out
