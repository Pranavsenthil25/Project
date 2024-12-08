CC = gcc
CFLAGS = -Wall -g

# Directories
SRC = .
LIB = lib

# Targets
TARGETS = test_server sensor_node

# Object Files
OBJS = $(LIB)/tcpsock.o datamgr.o connmgr.o

# Default Target
all: $(TARGETS)

# Build test_server
test_server: test_server.c $(OBJS)
	$(CC) $(CFLAGS) -o test_server test_server.c $(OBJS) -lpthread

# Build sensor_node
sensor_node: sensor_node.c $(LIB)/tcpsock.o
	$(CC) $(CFLAGS) -o sensor_node sensor_node.c $(LIB)/tcpsock.o

# Build tcpsock.o
$(LIB)/tcpsock.o: $(LIB)/tcpsock.c $(LIB)/tcpsock.h
	$(CC) $(CFLAGS) -c $(LIB)/tcpsock.c -o $(LIB)/tcpsock.o

# Build datamgr.o
datamgr.o: datamgr.c datamgr.h
	$(CC) $(CFLAGS) -c datamgr.c -o datamgr.o

# Build connmgr.o
connmgr.o: connmgr.c connmgr.h
	$(CC) $(CFLAGS) -c connmgr.c -o connmgr.o

# Clean up
clean:
	rm -f $(TARGETS) $(OBJS)
