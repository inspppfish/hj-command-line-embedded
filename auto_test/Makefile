CC = gcc
BASE = ../Core
SOURCE = $(BASE)/Src/sys_app/command_line.c
INCLUDE = $(BASE)/Inc


clean:
	rm command_line.o

main1.o: main1.c
	@$(CC) -c main1.c -I$(INCLUDE)

main2.o: main2.c
	@$(CC) -c main2.c -I$(INCLUDE)

command_line.o: $(SOURCE)
	@$(CC) -c $(SOURCE) -I$(INCLUDE)

test: test1 test2
	@rm command_line.o

test1: main1.o command_line.o
	@$(CC) -o test1 command_line.o main1.o
	@rm main1.o
	@./test1 > output1
	@diff correct_1 output1
	@rm test1 output1

test2: main2.o command_line.o
	@$(CC) -o test2 command_line.o main2.o
	@rm main2.o
	@./test2 > output2
	@diff correct_2 output2
	@rm test2 output2
