default: all

all: Setup library/Messenger.o binary/Reformat binary/DHAdd binary/DHRemove binary/DHQuery binary/DHSet binary/DumpDHFile

Setup:
	mkdir -p library
	mkdir -p binary

library/Messenger.o: source/Messenger.cpp include/Messenger.h
	g++ source/Messenger.cpp -Iinclude -c -o library/Messenger.o `root-config --cflags`

binary/Reformat: source/Reformat.cpp
	g++ source/Reformat.cpp -o binary/Reformat

binary/DHAdd: source/DHAdd.cpp
	g++ source/DHAdd.cpp -Iinclude -o binary/DHAdd

binary/DHRemove: source/DHRemove.cpp
	g++ source/DHRemove.cpp -Iinclude -o binary/DHRemove

binary/DHQuery: source/DHQuery.cpp
	g++ source/DHQuery.cpp -Iinclude -o binary/DHQuery

binary/DHSet: source/DHSet.cpp
	g++ source/DHSet.cpp -Iinclude -o binary/DHSet

binary/DumpDHFile: source/DumpDHFile.cpp
	g++ source/DumpDHFile.cpp -Iinclude -o binary/DumpDHFile
