CPP = g++
CPPFLAGS = -o2

TARGET = main
SRCS = main.cc metrics.cc schedulerChoise.cc read_file.cc cpu.cc process.cc schedulers/priority.cc schedulers/priorityNoPreemption.cc schedulers/FCFS.cc schedulers/SJF.cc schedulers/roundRobin.cc
HDRS = cpu.h metrics.h schedulerChoise.h process.h schedulers/priority.h schedulers/priorityNoPreemption.h schedulers/FCFS.h schedulers/SJF.h schedulers/roundRobin.h

OBJS = $(SRCS:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CPP) $(CPPFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cc
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
