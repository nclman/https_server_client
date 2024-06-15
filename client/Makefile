SHELL=/bin/bash

CXX = g++
CPPFLAGS = -O2 -std=c++11 -Wall -Wextra
CPP_DEFINES = -DCPPHTTPLIB_OPENSSL_SUPPORT

INCLUDES = -I/usr/local/opt/openssl@3/include
LIBS = -L/usr/local/opt/openssl@3/lib

LINKER_OPTS = -lssl -lcrypto

all:
	$(CXX) -o client $(CPPFLAGS) -I.. client.cc $(CPP_DEFINES) $(INCLUDES) $(LIBS) $(LINKER_OPTS)

clean:
	rm -f client
