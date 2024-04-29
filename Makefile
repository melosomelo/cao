CXX = g++
CXXFLAGS = -I. \
           -I/usr/include/sysc/communication \
           -I/usr/include/sysc/datatypes/bit \
           -I/usr/include/sysc/datatypes/int \
           -I/usr/include/sysc/datatypes/misc \
           -I/usr/include/sysc/kernel \
           -I/usr/include/sysc/tracing \
           -I/usr/include/sysc/utils
LDFLAGS = -L. \
          -L/usr/lib \
          -Wl,-rpath=/usr/lib
LDLIBS = -lsystemc -lm
SRCS = src/mem.cpp src/jcalc.cpp src/andgate.cpp src/extend.cpp src/decode.cpp src/reg.cpp src/regfile.cpp src/alu.cpp src/imem.cpp src/dmem.cpp src/shiftl2.cpp src/control.cpp src/proc.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = proc

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
