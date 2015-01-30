INCLUDE = -I${ORACLE_HOME}/rdbms/public -I${ORACLE_HOME}/rdbms/demo
LIBS = -L${ORACLE_HOME}/lib
BASES = -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS = -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -lclntsh

all:
	gcc -o demo1 demo1.c $(BASES) $(INCLUDE) $(LIBS) $(CFLAGS) 

clean:
	rm -rf demo1 
