CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = simulator

SRCS = main.cpp \
       memory.cpp \
       cache.cpp \
       buddy.cpp \
       virtual_memory.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)
