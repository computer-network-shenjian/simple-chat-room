CXXFLAGS := -std=c++11 -Wall -Wextra

SRC_LIB = $(wildcard src/*.cpp)
HEADER_LIB = $(wildcard include/*.hpp)
OBJ_LIB := $(SRC_LIB:.cpp=.o)
TARGET_LIB := server try

RM := rm -f

all: $(OBJ_LIB)
	$(CXX) $(CXXFLAGS) -o server $^

$(OBJ_LIB) : $(HEADER_LIB)

%.o: %.cpp $(HEADER_LIB)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%: %.o %.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

conf: src/conf.o include/conf.hpp
	$(CXX) $(CXXFLAGS) -o try $<

clean :
	$(RM) $(TARGET_LIB) $(OBJ_LIB)

.PHONY : all clean conf
