#
# Makefile
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
# To create static library:
#	http://www.faqs.org/docs/Linux-HOWTO/Program-Library-HOWTO.html
#	ar rcs my_library.a file1.o file2.o

CC=gcc
CFLAGS=-Wall -g
LIB_DIR=./lib/
INC_DIR=./include/
BIN_DIR=./bin/
SRC_DIR=./src/
TST_DIR=./testes/

all: $(BIN_DIR)red_black_tree.o $(BIN_DIR)cdata.o $(BIN_DIR)cthread.o\
	$(BIN_DIR)able_threads.o $(BIN_DIR)support.o

$(BIN_DIR)red_black_tree.o: $(SRC_DIR)red_black_tree.c
	$(CC) $(CFLAGS) -c -o $(BIN_DIR)red_black_tree.o -I$(INC_DIR) $(SRC_DIR)red_black_tree.c

$(BIN_DIR)cdata.o: $(SRC_DIR)cdata.c
	$(CC) $(CFLAGS) -c -o $(BIN_DIR)cdata.o -I$(INC_DIR) $(SRC_DIR)cdata.c

$(BIN_DIR)cthread.o: $(SRC_DIR)cthread.c
	$(CC) $(CFLAGS) -c -o $(BIN_DIR)cthread.o -I$(INC_DIR) $(SRC_DIR)cthread.c

$(BIN_DIR)able_threads.o: $(SRC_DIR)able_threads.c
	$(CC) $(CFLAGS) -c -o $(BIN_DIR)able_threads.o -I$(INC_DIR) $(SRC_DIR)able_threads.c

.PHONY: able_tree_tst pull init semaphore_test
able_tree_tst: all $(BIN_DIR)able_tree_test.o
	 $(CC) $(CFLAGS) $(BIN_DIR)able_tree_test.o $(BIN_DIR)red_black_tree.o\
	 $(BIN_DIR)cdata.o $(BIN_DIR)cthread.o $(BIN_DIR)able_threads.o\
	 $(BIN_DIR)support.o -o test_1

	 $(BIN_DIR)able_tree_test.o: $(TST_DIR)able_tree_test.c
	 $(CC) $(CFLAGS) -c -o $(BIN_DIR)able_tree_test.o -I$(INC_DIR) $(TST_DIR)able_tree_test.c


semaphore_test: all $(BIN_DIR)semaphore_test.o
	 $(CC) $(CFLAGS) $(BIN_DIR)semaphore_test.o $(BIN_DIR)red_black_tree.o\
	 $(BIN_DIR)cdata.o $(BIN_DIR)cthread.o $(BIN_DIR)able_threads.o\
	 $(BIN_DIR)support.o -o test_1

$(BIN_DIR)semaphore_test.o: $(TST_DIR)semaphore_test.c
	 $(CC) $(CFLAGS) -c -o $(BIN_DIR)semaphore_test.o -I$(INC_DIR) $(TST_DIR)semaphore_test.c




# VM RULES
init:
	@sudo mount -t vboxsf -o uid=1000 cthreads /home/aluno/Share/

pull:
	@rm -r ./* && cp -r ~/Share/* ./

# Copy of support.o because it's removed in clean up process
clean:
	rm -rf $(BIN_DIR)*.o $(SRC_DIR)*~ $(INC_DIR)*~ *~ && cp ./resources/support.o $(BIN_DIR)support.o
