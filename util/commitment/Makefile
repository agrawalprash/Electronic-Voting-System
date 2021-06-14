# This makefile assumes that you have:
# a) followed gmp install instructions from https://gmplib.org/#DOWNLOAD
# b) followed pbc install instructions from https://crypto.stanford.edu/pbc/howto.html

INCLUDE_DIR=$(HOME)/.local/include/pbc
LIB_DIR=$(HOME)/.local/lib
RPATH=$(LIB_DIR)
TARGET=main

build:
	gcc -I$(INCLUDE_DIR) -o $(TARGET) $(TARGET).cpp -L$(LIB_DIR) -Wl,-rpath $(RPATH) -lpbc -lgmp

all:
	echo $(HOME)
	g++ -I$(INCLUDE_DIR) -o $(TARGET) $(TARGET).cpp -L$(LIB_DIR) -Wl,-rpath $(RPATH) -lpbc -lgmp
	./$(TARGET)


clean:
	rm -rf $(TARGET)
