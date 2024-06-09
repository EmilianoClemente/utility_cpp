all:

test:
	g++ -o  ByteIo.out -DEXECUTE_TEST ByteIo.cpp
	./ByteIo.out
	g++ -o  MemoryIo.out -DEXECUTE_TEST MemoryIo.cpp
	./MemoryIo.out
