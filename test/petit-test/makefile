CC := g++
RM := rm -f
OBJS := test.o
TARGETS := test

all : $(TARGETS)

test : test.o
	$(CC) -o test test.o

clean : 
	$(RM) $(TARGETS) $(OBJS)

.PHONY : all clean