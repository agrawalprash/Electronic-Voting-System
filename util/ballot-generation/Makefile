# This makefile assumes that you have:
# a) followed gmp install instructions from https://gmplib.org/#DOWNLOAD
# b) followed pbc install instructions from https://crypto.stanford.edu/pbc/howto.html

# INCLUDE_DIR = $(HOME)/../../usr/local/include

INCLUDE_DIR = $(HOME)/.local/include/pbc
LIB_DIR = $(HOME)/.local/lib
RPATH = $(LIB_DIR)

GCC = g++
FLAGS = -g3 -ggdb -O0  -Wno-unused -Wall
TARGET  = main
TARGET2 = ../pairing/pair
TARGET3 = ../commitment/commitment
TARGET4 = ../bilinear-mapping/bilinear_mapping
TARGET5 = ../pairing/helper_functions
TARGET6 = ballot
TARGET7 = ../common/common
TARGET8 = ../evm/evm

all:
	echo $(HOME)
	$(GCC) $(FLAGS) -I$(INCLUDE_DIR) -o $(TARGET) $(TARGET).cpp $(TARGET2).cpp $(TARGET3).cpp $(TARGET4).cpp $(TARGET5).cpp $(TARGET6).cpp $(TARGET7).cpp $(TARGET8).cpp -L$(LIB_DIR) -Wl,-rpath $(RPATH) -lpbc -lgmp -fstack-protector
	./$(TARGET)

build:
	$(GCC) $(FLAGS) -I$(INCLUDE_DIR) -o $(TARGET) $(TARGET).cpp $(TARGET2).cpp $(TARGET3).cpp $(TARGET4).cpp $(TARGET5).cpp $(TARGET6).cpp -L$(LIB_DIR) -Wl,-rpath $(RPATH) -lpbc -lgmp -fstack-protector
