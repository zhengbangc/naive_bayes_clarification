EXE = mp3
OBJS = mp3.cpp 

COMPILER = g++
COMPILER_OPTS = -c -g -std=c++11 -Wall -pedantic -Werror 
LINKER = g++

all : $(EXE) 

$(EXE) : $(OBJS)
	$(LINKER) $(OBJS) -o $(EXE)

textClassification.o : textClassification.h textClassification.cpp
	$(COMPILER) $(COMPILER_OPTS) textClassification.cpp

clean :
	-rm -f *.o $(EXE)