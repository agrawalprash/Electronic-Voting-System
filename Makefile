CXX = g++
CXXFLAGS   := -g3 -ggdb -O0 -std=c++17 -Wall -Wno-unused -fno-stack-protector
OUTPUT     := output
BUILD      := build
SRC        := src
INCLUDE    := include
SHARED_LIB := shared_lib
LIB        := 
LFLAGS     := -lpbc -lgmp -lpython3.8

MAIN := main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
BUILDDIRS   := $(addprefix $(BUILD)/, $(SOURCEDIRS))
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)


FIXPATH = $1
RM      = rm -rf
MD	   := mkdir -p

INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))
SOURCES		:= $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))
OBJECTS		:= $(addprefix $(BUILD)/, $(SOURCES:.cpp=.o))

# PYBIND11 INCLUDES
INCLUDE_PYBIND := -I ./pybind11/include/pybind11/
PYTHON_H := -I/usr/include/python3.8/

# PBC INCLUDES
INCLUDE_PBC  := -I/usr/local/include/pbc 
LIB_PBC      := -L/usr/local/lib
LIB_PBC_PATH := -Wl,-rpath /usr/local/lib

# FILE_TO_RUN
TARGET        := main

# Google Test
GOOGLE_TEST   := googletest
GOOGLE_BUILD  := build

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(BUILDDIRS) $(MAIN)
	@echo Executing 'all' complete!

full: clean all run shared

$(OUTPUT):
	$(MD) $(OUTPUT)

$(BUILDDIRS):
	$(MD) $(BUILDDIRS)

$(MAIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDE_PBC) $(INCLUDES) $(PYTHON_H) $(INCLUDE_PYBIND) $(LIB_PBC) $(LIB_PBC_PATH) $(OBJECTS) $(TARGET).cpp $(LFLAGS) -o $(OUTPUTMAIN)

$(BUILD)/%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDE_PBC) $(INCLUDES) -c $< -o $@ 

.PHONY: clean
# clean:
# 	$(RM) $(OUTPUTMAIN)
# 	$(RM) $(call FIXPATH,$(OBJECTS))
# 	@echo Cleanup complete!

shared: all
	echo "Builind shared library ..."
	mkdir -p $(BUILD)
	mkdir -p $(SHARED_LIB)
	cd $(BUILD);\
	cmake ..;\
	make;\
	cp -r voting.cpython-38-x86_64-linux-gnu.so ../$(SHARED_LIB)/voting.cpython-38-x86_64-linux-gnu.so;\
	cp -r voting.cpython-38-x86_64-linux-gnu.so ../testing_shared_lib/voting.cpython-38-x86_64-linux-gnu.so;\
	cp -r voting.cpython-38-x86_64-linux-gnu.so ../app/server/voting.cpython-38-x86_64-linux-gnu.so;\
	

clean:
	$(RM) $(BUILD)
	$(RM) $(OUTPUT)
	$(RM) $(SHARED_LIB)
	@echo Cleanup complete!

run: all
	./$(OUTPUTMAIN) ${args}
	@echo Executing 'run: all' complete!

test:all install
	echo "Building files ..."
	mkdir $(BUILD)/$(GOOGLE_BUILD); \
	cmake -S testing/ -B $(BUILD)/$(GOOGLE_BUILD); \
	cd $(BUILD)/$(GOOGLE_BUILD); \
	make;\
	./test

install:
	mkdir testing/$(BUILD);\
	cd testing/$(BUILD);\
	if mkdir $(GOOGLE_TEST); then \
		echo "Installing Googletest ... ";\
		cp -r ../../googletest .;\
		# # wget https://github.com/google/googletest/archive/refs/heads/master.zip --no-check-certificate;\
		# rm -rf googletest-master;\
		# rm -rf googletest;\
		# unzip master.zip;\
		# mv googletest-master googletest;\
	else\
		echo "Already installed";\
	fi

help:
	@echo "usage: make <command>"
	@echo "\nFor execution: make run"
	@echo "Options:"
	@echo "\n positional arguments:"
	@echo "  command"

	@echo "    all:\tcompile all files and generate the executable target file"
	@echo "    run:\tcompile the main file and run the executable target file"
	@echo "    test:\tunit testing"
	@echo "    full:\trebuild the object files and the executable target file"
	@echo "    install:\tinstalls googletest in build"
	@echo "    clean:\tremove build, output directory"
	@echo "    help:\tdisplay help"