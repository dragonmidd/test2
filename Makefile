#include ../makefile.env

#paths
PATH_INCLUDE_BOOST = /usr/local/include
PATH_LIB_BOOST = /usr/local/lib


CC = g++
CC_FLAG = -I$(PATH_INCLUDE_BOOST) -std=c++11 -Wno-literal-suffix -Wno-conversion-null

LIB_STD = 
LIB_BOOST = -L$(PATH_LIB_BOOST) -lboost_system -lboost_timer -lboost_thread

OBJ = $(patsubst %.cpp,%.o,$(notdir $(wildcard *.cpp))) 
OBJ_DEBUG = $(patsubst %.cpp,%_d.o,$(notdir $(wildcard *.cpp)))

TARGET = test2
TARGET_DEBUG = test2_d


$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIB_STD) $(LIB_BOOST)
#	-mkdir -p ./release
#	mv -f $@ ./release
	
$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) -g -o $@ $^ $(LIB_STD) $(LIB_BOOST)
#	-mkdir -p debug
#	mv -f $@ debug

release:$(TARGET)
debug:$(TARGET_DEBUG)
all: $(TARGET) $(TARGET_DEBUG)

clean:
	-rm -f *.o $(TARGET)
#	-rm -f ../bin/$(TARGET)
	-rm -f *_d.o $(TARGET_DEBUG)
#	-rm -f ../bin/$(TARGET_DEBUG)


%.o:%.cpp
	$(CC) $(CC_FLAG) -c $<  -o $@
	
%_d.o:%.cpp
	$(CC) -g $(CC_FLAG) -c $<  -o $@
	
	
