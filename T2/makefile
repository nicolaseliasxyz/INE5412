CPP = g++
CPPFLAGS = -O2

TARGET = simulador
SRCS = main.cc PageReplacementAlgorithms/FIFO.cc PageReplacementAlgorithms/LRU.cc PageReplacementAlgorithms/OPT.cc Calculator.cc
HDRS = RAM.h PageReplacementAlgorithms/FIFO.h PageReplacementAlgorithms/LRU.h PageReplacementAlgorithms/OPT.h PageReplacementAlgorithms/PageReplacementAlgorithm.h Calculator.h

OBJS = $(SRCS:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CPP) $(CPPFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cc $(HDRS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
