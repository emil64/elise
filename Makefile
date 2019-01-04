# Find the OS platform using the uname command.
Linux := $(findstring Linux, $(shell uname -s))
MacOS := $(findstring Darwin, $(shell uname -s))
Windows := $(findstring NT, $(shell uname -s))

# Specify what typing 'make' on its own will compile
default: elisa

# For Windows, add the .exe extension
ifdef Windows

%: %.c
	clang -Dtest_$@ -std=c11 -Wall -pedantic -g $@.c -o $@.exe -lssl -lcrypto

# For Linux/MacOS, include the advanced debugging options
else

%: %.c
	gcc -std=c11 -Wall -pedantic -g -pthread \
	 	$@.c filescan.c server.c crypto.c logic.c play.c -o $@ \
	    -fsanitize=undefined -fsanitize=address  -lssl -lcrypto -pthread `pkg-config --cflags --libs gtk+-3.0`  \
		-rdynamic -export-dynamic -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

endif
