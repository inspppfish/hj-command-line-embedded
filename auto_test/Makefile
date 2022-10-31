CC = gcc
BASE = ../Core
SOURCE = $(BASE)/Src/sys_app/command_line.c
INCLUDE = $(BASE)/Inc

local_test: main.o command_line.o
	@$(CC) -o local_test command_line.o main.o
	@rm main.o command_line.o

clean:
	rm main.o command_line.o local_test

main.o: main.c
	@$(CC) -c main.c -I$(INCLUDE)

command_line.o: $(SOURCE)
	@$(CC) -c $(SOURCE) -I$(INCLUDE)

test1: local_test
	@./local_test > test1
	@diff correct_1 test1
	@rm test1
