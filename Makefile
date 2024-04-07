CC  = g++
CFLAGS = -Wall -g -municode
CPP_STANDARD = -std=c++20

all: linecounter

linecounter.o: linecounter.cpp
	$(CC) $(CPP_STANDARD) $(CFLAGS) -c linecounter.cpp

main.o: main.cpp
	$(CC) $(CPP_STANDARD) $(CFLAGS) -c main.cpp

linecounter: linecounter.o main.o
	@echo "Linking application"
	@echo
	$(CC) $(CPP_STANDARD) $(CFLAGS) main.o linecounter.o -o $@

clean:
	@echo "Removing build files"
	@echo
	rm -rf main.o linecounter.o linecounter.exe
