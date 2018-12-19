CXXFLAGS := -std=c++11 -Wall -Wextra

SRC_SERVER := server.cpp

TARGET_SERVER := $(SRC_SERVER:.cpp=)

SRC_LIB := 
HEADER_LIB := $(SRC_LIB:.cpp=.hpp)
OBJ_LIB := $(SRC_LIB:.cpp=.o)

RM := rm -rf

all: $(TARGET_SERVER) $(TARGET_CLIENT) 

$(TARGET_SERVER): $(SRC_SERVER) $(OBJ_LIB) $(HEADER_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_CLIENT): $(SRC_CLIENT) $(OBJ_LIB) $(HEADER_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean :
	$(RM) $(TARGET_SERVER) $(TARGET_CLIENT) *.o
	$(RM) server_txt/ client_txt/

.PHONY : clean all 
