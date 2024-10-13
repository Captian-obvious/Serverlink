# Makefile for compiling sl and sl-ext

# Variables
COMPILER = g++
FLAGS = -g -Werror -fPIC
OUTFILE = ./sl
OUTFILE2 = ./libsl-ext.so

# Target to compile the shared library
libsl-ext.so: ./sl-ext.cpp
	$(COMPILER) $(FLAGS) -shared -o $(OUTFILE2) ./sl-ext.cpp

# Target to compile the main executable and link with the shared library
sl: ./main.cpp libsl-ext.so
	$(COMPILER) $(FLAGS) ./main.cpp -L. -lsl-ext -Wl,-rpath='./' -o $(OUTFILE)

# Target to grant permissions
permissions: sl libsl-ext.so
	chmod +x $(OUTFILE)
	chmod +x $(OUTFILE2)
	echo "Done!"
check:
	ls
distcheck:
	./sl quit

# Default target to compile the project and set permissions
all: libsl-ext.so sl permissions

# Clean up compiled files
clean:
	rm -f $(OUTFILE) $(OUTFILE2)
