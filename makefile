.SUFFIXES : .cpp

CC = g++
CXXFLAGS = -c -g
LIBS = -lm

OBJS = main.o Game.o Piece.o Advantage.o Agent.o AggressiveAgentStrategy.o DefaultAgentStrategy.o ErrorContext.o Exceptions.o Food.o GamingTests.o Simple.o Strategic.o Resource.o
TARGET = run 

$(TARGET): $(OBJS)
	$(CC)  -o $@ $(OBJS) $(LIBS)

.cpp.o:
	$(CC) -std=c++11 $(CXXFLAGS) $(INCDIR) $<

clean:
	rm -f $(OBJS) $(TARGET)