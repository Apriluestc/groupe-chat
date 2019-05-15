PROJECT_PATH 	:= /home/shiny/repository/chat/

INCLUDE_PATH  	:= $(PROJECT_PATH)include/

SRC_PATH	   	:= $(PROJECT_PATH)src/

BIN_PATH	   	:= $(PROJECT_PATH)bin/

TESTSRC_PATH   	:= $(PROJECT_PATH)test/

TESTBIN_PATH   	:= $(PROJECT_PATH)test/

DEMOSRC_PATH   	:= $(PROJECT_PATH)demo/

DEMOBIN_PATH   	:= $(PROJECT_PATH)demo/

LIB_PATH 	   	:= /usr/lib64/libjsoncpp.a

LD_FLAGS 	   	:= -lpthread -ljsoncpp

CC 			   	:= g++


all : $(BIN_PATH)server $(BIN_PATH)client $(BIN_PATH)echoserver $(TESTBIN_PATH)txproj_version
	
$(BIN_PATH)server : $(SRC_PATH)server.cpp $(INCLUDE_PATH)server.h $(INCLUDE_PATH)dataPool.h $(INCLUDE_PATH)info.h
	[ -e $(BIN_PATH) ] || mkdir $(BIN_PATH)
	$(CC) -g $^ -o $@ $(LIB_PATH) $(LD_FLAGS) -std=c++11

$(BIN_PATH)client : $(SRC_PATH)client.cpp $(INCLUDE_PATH)client.h
	$(CC) -g $^ -o $@ $(LIB_PATH) $(LD_FLAGS) -std=c++11 -lncurses

$(BIN_PATH)echoserver : $(SRC_PATH)echoserver.cpp
	$(CC) -g $^ -o $@ $(LIB_PATH) -std=c++11

$(TESTBIN_PATH)txproj_version : $(TESTSRC_PATH)txproj_version.cpp $(INCLUDE_PATH)version.h
	$(CC) -g $^ -o $@ -std=c++11 -lncurses

.PHONY:

run-client :
	./bin/client 127.0.0.1 5000

run-server :
	./bin/server 5000

run-echoserver :
	./bin/echoserver

run-test :
	./test/txproj_version

clean :
	rm $(BIN_PATH)server $(BIN_PATH)client $(BIN_PATH)echoserver rm -rf $(BIN_PATH)

clean-all :
	rm $(BIN_PATH)server $(BIN_PATH)client $(BIN_PATH)echoserver $(TESTBIN_PATH)txproj_version
