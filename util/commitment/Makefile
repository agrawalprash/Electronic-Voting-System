# This makefile assumes that you have:
# a) followed gmp install instructions from https://gmplib.org/#DOWNLOAD
# b) followed pbc install instructions from https://crypto.stanford.edu/pbc/howto.html

INCLUDE_DIR = $(HOME)/.local/include/pbc
LIB_DIR = $(HOME)/.local/lib
RPATH = $(LIB_DIR)
TARGET = main
CURRENT_DIR = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
GMP_FOLDER = gmp
GMP_VERSION_TAR = gmp-6.2.1.tar.lz
GMP_VERSION_NAME = gmp-6.2.1
PBC_FOLDER = pbc
PBC_VERSION_TAR = pbc-0.5.14.tar.gz
PBC_VERSION_NAME = pbc-0.5.14

build:
	gcc -I$(INCLUDE_DIR) -o $(TARGET) $(TARGET).cpp -L$(LIB_DIR) -Wl,-rpath $(RPATH) -lpbc -lgmp

all:
	echo $(HOME)
	g++ -I$(INCLUDE_DIR) -o $(TARGET) $(TARGET).cpp -L$(LIB_DIR) -Wl,-rpath $(RPATH) -lpbc -lgmp
	./$(TARGET) < a.param

dependencies:
	@echo "Installing Dependencies ...\n"
	-sudo apt-get update -y
	sudo apt-get install m4
	sudo apt-get install flex
	sudo apt-get install bison		

install: pbc	

gmp: dependencies	
	@echo "\nInstalling the GMP library ..."
	sudo wget https://gmplib.org/download/gmp/$(GMP_VERSION_TAR) --no-check-certificate

	# if cd ~/../../usr/local/include; then \

		cd ~/../../usr/local/include; \
		if sudo mkdir $(GMP_FOLDER); then \
			cd $(CURRENT_DIR); \
			sudo cp $(GMP_VERSION_TAR) ~/../../usr/local/include/$(GMP_FOLDER); \
			cd ~/../../usr/local/include/$(GMP_FOLDER); \
			sudo tar -xvf $(GMP_VERSION_TAR); \
			cd $(GMP_VERSION_NAME); \
		else \
			cd $(GMP_FOLDER)/$(GMP_VERSION_NAME);\
		fi; \
		if sudo ./configure; then \
			sudo make; \
			sudo make check; \
			sudo make install; \
		fi

	# else \
	# 	cd ~/../usr/local/include; \
	# 	sudo mkdir $(GMP_FOLDER); \
	# 	cd $(CURRENT_DIR); \
	# 	sudo cp $(GMP_VERSION_TAR) ~/../usr/local/include/$(GMP_FOLDER); \
	# 	cd ~/../usr/local/include/$(GMP_FOLDER); \
	# 	sudo ./configure; \
	# 	sudo make; \
	# 	sudo make check; \
	# 	sudo make install; \
	# fi	



pbc: gmp
	cd ~/../../usr/local/include; \

	if sudo mkdir $(PBC_FOLDER); then \
		cd $(PBC_FOLDER); \
		sudo wget https://crypto.stanford.edu/pbc/files/$(PBC_VERSION_TAR) --no-check-certificate; \
		sudo tar -xvf $(PBC_VERSION_TAR); \
		cd $(PBC_VERSION_NAME); \
	else \
		cd $(PBC_FOLDER)/$(PBC_VERSION_NAME); \
	fi; \

	if sudo ./configure; then \
		sudo make; \
		sudo make install; \
	fi	

clean:
	rm -rf pbcb
