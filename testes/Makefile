#
# Makefile para programas exemplo
#

CC=gcc
LIB_DIR=../lib/
INC_DIR=../include/

all: able_tree semaphore threads

able_tree: able_tree.c $(LIB_DIR)libcthread.a
	$(CC) -o able_tree.out able_tree.c -I$(INC_DIR) -L$(LIB_DIR) -lcthread -Wall

semaphore: semaphore.c $(LIB_DIR)libcthread.a
	$(CC) -o semaphore.out semaphore.c -I$(INC_DIR) -L$(LIB_DIR) -lcthread -Wall

threads: threads.c $(LIB_DIR)libcthread.a
	$(CC) -o threads.out threads.c -I$(INC_DIR) -L$(LIB_DIR) -lcthread -Wall

clean:
	rm -r *.out
