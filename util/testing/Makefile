DIRECTORY_PATH = $(HOME)/projects/crypto
BUILD_PATH = build

all:install
	echo "Building files ..."
	-mkdir $(BUILD_PATH); \
	cmake -S . -B $(BUILD_PATH); \
	cd build; \
	make;\
	./test


install:
	if mkdir googletest; then \
		echo "Installing Googletest ... ";\
		wget https://github.com/google/googletest/archive/refs/heads/master.zip --no-check-certificate;\
		rm -rf googletest-master;\
		rm -rf googletest;\
		unzip master.zip;\
		mv googletest-master googletest;\
	else\
		echo "Already installed";\
	fi


clean:
	rm -rf master.zip
	rm -rf googletest
	rm -rf build
