CC = gcc
BASE = ../Core
SOURCE = $(BASE)/Src/sys_app/command_line.c
NUMBERS = 1 2 3
TARGET := $(foreach n, $(NUMBERS), test$(n))
OBJECT := $(foreach t, $(TARGET), $(t).o)
INCLUDE = ../Core/Inc/

.PHONY clean:
	@rm $(OBJECT)
	@rm $(TARGET)
	@rm command_line.o

.PHONY test_all: $(TARGET)

$(OBJECT): $(INCLUDE)
	@$(CC) -c $(@:.o=.c) -I$(INCLUDE)

command_line.o:
	@$(CC) -c $(SOURCE) -I$(INCLUDE)

$(TARGET): $(OBJECT) command_line.o
	@$(CC) -o $@ $@.o command_line.o
	@./$@ > $@.out
	@diff $@.correct $@.out



