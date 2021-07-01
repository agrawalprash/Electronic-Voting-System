CXX = g++
CXXFLAGS := -g3 -ggdb -O0 -std=c++17 -Wall -Wno-unused -fno-stack-protector
OUTPUT   := output
BUILD    := build
SRC      := src
INCLUDE  := include
LIB      := 
LFLAGS   := -lpbc -lgmp

MAIN := main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
BUILDDIRS   := $(addprefix $(BUILD)/, $(SOURCEDIRS))
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)

FIXPATH = $1
RM      = rm -r
MD	   := mkdir -p

INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))
SOURCES		:= $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))
OBJECTS		:= $(addprefix $(BUILD)/, $(SOURCES:.cpp=.o))

# PBC INCLUDES
INCLUDE_PBC  := -I$(HOME)/.local/include/pbc 
LIB_PBC      := -L$(HOME)/.local/lib
LIB_PBC_PATH := -Wl,-rpath $(HOME)/.local/lib

# FILE_TO_RUN
TARGET        := main

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(BUILDDIRS) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(BUILDDIRS):
	$(MD) $(BUILDDIRS)

$(MAIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDE_PBC) $(INCLUDES) $(LIB_PBC) $(LIB_PBC_PATH) $(OBJECTS) $(TARGET).cpp $(LFLAGS) -o $(OUTPUTMAIN)

$(BUILD)/%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDE_PBC) $(INCLUDES) -c $< -o $@ 

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	@echo Cleanup complete!

complete_clean:
	$(RM) build
	$(RM) output

run: all
	./$(OUTPUTMAIN) ${args}
	@echo Executing 'run: all' complete!

help:
	@echo "usage: make <command>"
	@echo "\nFor execution: make run"
	@echo "Options:"
	@echo "\n positional arguments:"
	@echo "  command"

	@echo "    all:\tcompile all files and generate the executable target file"
	@echo "    run:\tcompile the main file and run the executable target file"
	@echo "    clean:\tremove build, output directory"
	@echo "    help:\tdisplay help"