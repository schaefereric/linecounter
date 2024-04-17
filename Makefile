CC  = g++
CFLAGS = -Wall -g
CPP_STANDARD = -std=c++20

all: linecounter_win32

main.o: main.cpp
	$(CC) $(CPP_STANDARD) $(CFLAGS) -c main.cpp

linecounter_win32.o: linecounter.cpp
	$(CC) $(CPP_STANDARD) $(CFLAGS) -municode -c linecounter.cpp -o $@

linecounter_linux.o: linecounter.cpp
	$(CC) $(CPP_STANDARD) $(CFLAGS) -c linecounter.cpp -o $@

linecounter_win32: linecounter_win32.o main.o
	@echo "Linking application"
	@echo
	$(CC) $(CPP_STANDARD) $(CFLAGS) main.o linecounter_win32.o -o $@

linecounter_linux: linecounter_linux.o main.o
	@echo "Linking application"
	@echo
	$(CC) $(CPP_STANDARD) $(CFLAGS) main.o linecounter_linux.o -o $@

clean:
	@echo "Removing build files"
	@echo
	rm -rf main.o linecounter.o linecounter.exe
