PROJECT	= $(shell pwd)
BIN 	= $(PROJECT)/bin
CC		= gcc

all : $(BIN)/server $(BIN)/client

$(BIN)/server : $(PROJECT)/server.cpp
	[ -e $(BIN) ] || mkdir $(BIN)
	g++ $^ -o $@

$(BIN)/client : $(PROJECT)/client.c
	gcc $^ -o $@

.PHONY :

clean:
	rm -rf $(BIN)
