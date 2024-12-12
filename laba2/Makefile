CC = g++
CFLAGS = -Wall -g
EXECUTABLE = Main
INSTALL_DIR = ~/pp

all: $(EXECUTABLE)

$(EXECUTABLE): laba2.cpp ThreadPool.cpp ThreadPool.h
	$(CC) $(CFLAGS) laba2.cpp ThreadPool.cpp -o $(EXECUTABLE)

install: $(EXECUTABLE)
	mkdir -p $(INSTALL_DIR)
	cp $(EXECUTABLE) $(INSTALL_DIR)

clean:
	rm -f $(EXECUTABLE)

.PHONY: all install clean cleanI
