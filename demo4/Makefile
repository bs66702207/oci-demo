INCLUDE = -I${ORACLE_HOME}/rdbms/public -I${ORACLE_HOME}/rdbms/demo
LIBS = -lstdc++
LIBRARY = -L${ORACLE_HOME}/lib
BASES = -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS = -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -lclntsh
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

TARGET = main

%.o : %.cpp
	g++ -o $@ $< -c $(BASES) $(INCLUDE) $(LIBS) $(LIBRARY) $(CFLAGS)

$(TARGET): $(OBJS)
	g++ -o $@ $^ $(BASES) $(INCLUDE) $(LIBS) $(LIBRARY) $(CFLAGS)
clean:
	rm -rf *.o $(TARGET)
