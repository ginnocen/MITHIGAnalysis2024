default: all

all: Setup library/DrawRandom.o library/TauHelperFunctions3.o library/Messenger.o library/DimuonMessenger.o binary/Reformat binary/DHAdd binary/DHRemove binary/DHQuery binary/DHSet binary/DumpDHFile

Setup:
	mkdir -p library
	mkdir -p binary

library/Messenger.o: source/Messenger.cpp include/Messenger.h
	g++ source/Messenger.cpp -Iinclude -c -o library/Messenger.o `root-config --cflags`

library/DimuonMessenger.o: source/DimuonMessenger.cpp include/DimuonMessenger.h
	g++ source/DimuonMessenger.cpp -Iinclude -c -o library/DimuonMessenger.o `root-config --cflags`

library/TauHelperFunctions3.o: source/TauHelperFunctions3.cpp include/TauHelperFunctions3.h
	g++ source/TauHelperFunctions3.cpp -Iinclude -c -o library/TauHelperFunctions3.o -I${RootMacrosBase}/ -std=c++11

library/DrawRandom.o: source/DrawRandom.cpp include/DrawRandom.h
	g++ source/DrawRandom.cpp -Iinclude -c -fpic -o library/DrawRandom.o -I${RootMacrosBase}/ -std=c++11


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
