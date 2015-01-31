INCLUDE = -I${ORACLE_HOME}/rdbms/public -I${ORACLE_HOME}/rdbms/demo 
LIBS = -lstdc++
LIBRARY = -L${ORACLE_HOME}/lib
BASES = -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS = -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -lclntsh

all:
	gcc -o demo2 demo2.cpp $(BASES) $(INCLUDE) $(LIBS) $(LIBRARY) $(CFLAGS) 

clean:
	rm -rf demo2
