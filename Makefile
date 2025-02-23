# Makefile for compiling sl and sl-ext

all: init lib sl permissions
# Variables
COMPILER = g++
DEBUG_FLAGS = -g -Werror -fPIC
FLAGS = -Os -Werror -fPIC
OUTFILE = ./build/sl
OUTFILE2 = ./build/libsl-ext.so
init:
	mkdir build

# Target to compile the shared library
lib: ./sl-ext.cpp
	$(COMPILER) $(FLAGS) -shared -o $(OUTFILE2) ./sl-ext.cpp

# Target to compile the main executable and link with the shared library
sl: ./main.cpp ./build/libsl-ext.so
	$(COMPILER) $(FLAGS) ./main.cpp -L./build -lsl-ext -Wl,-rpath='./build' -o $(OUTFILE)
main:
	bash ./compile.sh
# Target to grant permissions
permissions: ./build/sl ./build/libsl-ext.so
	chmod +x $(OUTFILE)
	chmod +x $(OUTFILE2)
	echo "Done!"
check:
	bash ./check.sh
distcheck:
	./build/sl quit

# Clean up compiled files
clean:
	rm -f $(OUTFILE) $(OUTFILE2)
