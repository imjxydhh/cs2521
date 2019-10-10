// testTextbuffer.c
// A stub file for you to test your textbuffer implementation.
// Note that you will NOT be submitting this - this is simply for you to
// test  your  functions as you go. We will have our own testTexbuffer.c
// for testing your submission.

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

//static void testNewTB(void);
//static void showList(TB tb);
//static void testAddPrefix();
//static void testMergeTB(void);
//static void testPasteTB(void);
static void testCutTB(void);

int main(void) {
	//testMergeTB();
	//testNewTB();
	//testAddPrefix();
	//testPasteTB();
	testCutTB();
	// TODO: Call more test functions
	
	
	printf("All tests passed! You are awesome!\n");
}

/*static void testMergeTB(void){
	printf("-----------------------------------------\n"
	       "              mergeTB tests              \n"
	       "-----------------------------------------\n");
	TB tb1 = newTB("abcdefghijklmnopqrst\n2\n3\n4\n5\n");
	TB tb2 = newTB("");
	int totLen = linesTB(tb1) + linesTB(tb2);
	mergeTB(tb1, 7, tb2);
	assert(linesTB(tb1) == totLen);
	char *text1 = dumpTB(tb1, true);
	//printf("*%s*\n", text1);	
	assert(strcmp("1. abcdefghijklmnopqrst\n2. 2\n3. 3\n4. 4\n5. 5\n", text1) == 0);
	//assert(strcmp("6\n7\n8\n9\n1\n2\n3\n4\n5\n", text1) == 0);
	free(text1);
	releaseTB(tb1);
	printf("mergeTB tests passed!\n");
}*/

/*static void testPasteTB(void){
	printf("-----------------------------------------\n"
	       "              mergeTB tests              \n"
	       "-----------------------------------------\n");
	TB tb1 = newTB("abcdefghijklmnopqrst\n2\n3\n4\n5\n");
	TB tb2 = newTB("asdsadasd\n");
	int totLen = linesTB(tb1) + linesTB(tb2);
	pasteTB(tb1, 6, tb2);
	assert(linesTB(tb1) == totLen);
	char *text1 = dumpTB(tb1, true);
	//printf("*%s*\n", text1);	
	assert(strcmp("1. abcdefghijklmnopqrst\n2. 2\n3. 3\n4. 4\n5. 5\n6. asdsadasd\n", text1) == 0);
	//assert(strcmp("6\n7\n8\n9\n1\n2\n3\n4\n5\n", text1) == 0);
	free(text1);
	releaseTB(tb1);
	releaseTB(tb2);
	printf("mergeTB tests passed!\n");
}*/

/*static void testNewTB(void) {
	printf("-----------------------------------------\n"
	       "               newTB tests               \n"
	       "-----------------------------------------\n");
	
	// Calling dumpTB immediately after newTB, without modifying the TB
	TB tb1 = newTB("\naa\n\naa\n\n");
	assert(linesTB(tb1) == 5);
	char *text1 = dumpTB(tb1, true); // Don't show line numbers
	//printf("*%s*\n", text1);
	assert(strcmp("1. \n2. aa\n3. \n4. aa\n5. \n", text1) == 0);
	free(text1);
	releaseTB(tb1);
	
	// TODO: Add more tests
	
	
	printf("newTB tests passed!\n");
}*/

static void testCutTB(void) {
	printf("-----------------------------------------\n"
	       "               newTB tests               \n"
	       "-----------------------------------------\n");
	
	// Calling dumpTB immediately after newTB, without modifying the TB
	TB tb1 = newTB("1\n2\n3\n4\n5\n");
	assert(linesTB(tb1) == 5);
	char *text1 = dumpTB(tb1, false); // Don't show line numbers
	
	//printf("*%s*\n", text1);
	assert(strcmp("1\n2\n3\n4\n5\n", text1) == 0);
	free(text1);
	TB tb2 = cutTB(tb1, 1, 3);
	text1 = dumpTB(tb1, false);
	char *text2 = dumpTB(tb2, false);
	assert(strcmp("4\n5\n", text1) == 0);
	assert(strcmp("1\n2\n3\n", text2) == 0);
	printf("test case 1 succeed\n");
	free(text1);
	free(text2);
	mergeTB(tb1, 1, tb2);

	tb2 = cutTB(tb1, 2, 3);
	text1 = dumpTB(tb1, false);
	text2 = dumpTB(tb2, false);
	assert(strcmp("1\n4\n5\n", text1) == 0);
	assert(strcmp("2\n3\n", text2) == 0);
	printf("test case 2 succeed\n");
	free(text1);
	free(text2);
	mergeTB(tb1, 2, tb2);

	tb2 = cutTB(tb1, 2, 5);
	text1 = dumpTB(tb1, false);
	text2 = dumpTB(tb2, false);
	assert(strcmp("1\n", text1) == 0);
	assert(strcmp("2\n3\n4\n5\n", text2) == 0);
	printf("test case 3 succeed\n");
	free(text1);
	free(text2);
	mergeTB(tb1, 2, tb2);

	tb2 = cutTB(tb1, 1, 5);
	text1 = dumpTB(tb1, false);
	text2 = dumpTB(tb2, false);
	assert(strcmp("", text1) == 0);
	assert(strcmp("1\n2\n3\n4\n5\n", text2) == 0);
	printf("test case 4 succeed\n");

	free(text1);
	free(text2);
	releaseTB(tb1);
	releaseTB(tb2);
	
	// TODO: Add more tests
	
	
	printf("newTB tests passed!\n");
}

/*static void testAddPrefix(){
	printf("-----------------------------------------\n"
	       "            addPrefixTB tests            \n"
	       "-----------------------------------------\n");
	
	// Calling dumpTB immediately after newTB, without modifying the TB
	TB tb1 = newTB("\naa\n\n\naa\n");
	assert(linesTB(tb1) == 5);
	addPrefixTB(tb1, 0, 7 , "prefix");
	char *text1 = dumpTB(tb1, false); // Don't show line numbers
	//printf("*%s*\n", text1);
	assert(strcmp("prefix\nprefixaa\nprefix\n\naa\n", text1) == 0);
	free(text1);
	releaseTB(tb1);
	
	// TODO: Add more tests
	
	
	printf("newTB tests passed!\n");
}*/

// TODO: Add more test functions here
/*static void showList(TB tb){
		
	while()
}*/

