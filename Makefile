CC  = g++

CFLAGS = -Wall -g

all: linecounter

linecounter.o: linecounter.cpp
	$(CC) $(CFLAGS) -c linecounter.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

linecounter: linecounter.o main.o
	@echo "Linking application"
	@echo
	$(CC) $(CFLAGS) main.o linecounter.o -o $@

clean:
	@echo "Removing build files"
	@echo
	rm -rf main.o linecounter.o linecounter.exe
