DIRECTORY_PATH = $(HOME)/projects/crypto
BUILD_PATH = build

all:
	-mkdir $(BUILD_PATH); \
	cmake -S . -B $(BUILD_PATH); \
	cd build; \
	make;\
	./test
