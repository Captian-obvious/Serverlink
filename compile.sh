#!/usr/bin/env bash

# Compile the project
echo "Compiling..."

# Compiler and flags
COMPILER=g++
FLAGS="-g -Werror -fPIC"

# Output files
OUTFILE="./sl.2"
OUTFILE2="./libsl-ext.so"

# Compile the shared library
$COMPILER $FLAGS -shared -o $OUTFILE2 ./sl-ext.cpp

# Compile the main executable and link with the shared library
$COMPILER $FLAGS ./main.cpp -L. -lsl-ext -Wl,-rpath='./' -o $OUTFILE

echo "Compiled! Granting permissions..."

# Grant execute permissions
chmod +x $OUTFILE
chmod +x $OUTFILE2

echo "Done!"
