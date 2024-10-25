# Makefile for compiling sl and sl-ext

all: lib sl permissions
# Variables
COMPILER = g++
DEBUG_FLAGS = -g -Werror -fPIC
FLAGS = -Os -Werror -fPIC
OUTFILE = ./sl
OUTFILE2 = ./libsl-ext.so

# Target to compile the shared library
lib: ./sl-ext.cpp
	$(COMPILER) $(FLAGS) -shared -o $(OUTFILE2) ./sl-ext.cpp

# Target to compile the main executable and link with the shared library
sl: ./main.cpp libsl-ext.so
	$(COMPILER) $(FLAGS) ./main.cpp -L. -lsl-ext -Wl,-rpath='./' -o $(OUTFILE)
main:
	bash ./compile.sh
# Target to grant permissions
permissions: sl libsl-ext.so
	chmod +x $(OUTFILE)
	chmod +x $(OUTFILE2)
	echo "Done!"
check:
	bash ./check.sh
distcheck:
	./sl quit

# Clean up compiled files
clean:
	rm -f $(OUTFILE) $(OUTFILE2)