GCC = g++
TARGET = main
FLAGS = -g3 -ggdb -O0  -Wno-unused
WARNING = -Wall -Wextra

all: $(TARGET).cpp
		$(GCC) $(FLAGS) -o $(TARGET) $(TARGET).cpp -lcryptopp -lcrypto
		./$(TARGET)

all2:
	$(TARGET).c
