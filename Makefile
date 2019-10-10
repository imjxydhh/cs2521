CC = gcc
CFLAGS = -Wall -Werror -std=c11 -O -lm

TEST_DRIVER = testTextbuffer.c
EXERCISE_FILE = textbuffer.c

SRC_FILES = $(TEST_DRIVER) $(EXERCISE_FILE) 

all: testTextbuffer

testTextbuffer: $(SRC_FILES)
	$(CC) $(CFLAGS) -o  testTextbuffer $(SRC_FILES)
