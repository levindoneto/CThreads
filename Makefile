#
# Makefile 
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#

CC=gcc
CFLAGS=-Wall
LIB_DIR=./lib/
INC_DIR=./include/
BIN_DIR=./bin/
SRC_DIR=./src/

all: $(BIN_DIR)red_black_tree.o

$(BIN_DIR)red_black_tree.o: $(SRC_DIR)red_black_tree.c
	$(CC) $(CFLAGS) -c -o $(BIN_DIR)red_black_tree.o -I$(INC_DIR) $(SRC_DIR)red_black_tree.c 

## Copy of support.o because it's removed in clean up process
clean:
	rm -rf $(BIN_DIR)*.o $(SRC_DIR)*~ $(INC_DIR)*~ *~ && cp ./resources/support.o $(BIN_DIR)support.o  

