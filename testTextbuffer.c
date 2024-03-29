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

// static void testNewTB(void);
// static void showList(TB tb);
// static void testAddPrefix();
// static void testMergeTB(void);
// static void testPasteTB(void);
// static void testCutTB(void);
// static void testSearchTB();
// static void testFormRichText();
// static void testDiffTB();
static void testUnReDo();

int main(void) {
	// testMergeTB();
	// testNewTB();
	// testAddPrefix();
	// testPasteTB();
	// testCutTB();
	// testSearchTB();
	// testFormRichText();
	// testDiffTB();
	testUnReDo();
	// TODO: Call more test functions
	
	
	printf("All tests passed! You are awesome!\n");
}

static void testUnReDo(){
	printf("-----------------------------------------\n"
	       "             undo redo tests             \n"
	       "-----------------------------------------\n");
	TB tb1 = newTB("1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n");
	undoTB(tb1);
	char *text1 = dumpTB(tb1,false);
	assert(strcmp("1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n", text1) == 0);
	free(text1);

	deleteTB(tb1, 2, 5);
	undoTB(tb1);
	text1 = dumpTB(tb1,false);
	assert(strcmp("1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n", text1) == 0);
	free(text1);

	redoTB(tb1);
	text1 = dumpTB(tb1, false);
	assert(strcmp("1\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n", text1) == 0);
	free(text1);

	redoTB(tb1);
	text1 = dumpTB(tb1, false);
	assert(strcmp("1\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n", text1) == 0);
	free(text1);

	for(int i = 0;i < 9;i++){
		deleteTB(tb1, 2, 2);
	}
	redoTB(tb1);
	text1 = dumpTB(tb1, false);
	assert(strcmp("1\n15\n16\n17\n18\n19\n", text1) == 0);
	free(text1);

	deleteTB(tb1, 2, 2);

	undoTB(tb1);
	text1 = dumpTB(tb1,false);
	assert(strcmp("1\n15\n16\n17\n18\n19\n", text1) == 0);
	free(text1);

	for(int i = 0;i< 9;i++){
		undoTB(tb1);
	}
	undoTB(tb1);
	text1 = dumpTB(tb1, false);
	assert(strcmp("1\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n", text1) == 0);
	free(text1);

	for(int i = 0;i< 9;i++){
		redoTB(tb1);
	}	
	text1 = dumpTB(tb1, false);
	assert(strcmp("1\n15\n16\n17\n18\n19\n", text1) == 0);
	free(text1);

	undoTB(tb1);
	TB tb2 = newTB("insert1\ninsert2\ninsert3\n");
	pasteTB(tb1, 2, tb2);
	redoTB(tb1);
	text1 = dumpTB(tb1, false);
	//printf("*%s*", text1);	
	assert(strcmp("1\ninsert1\ninsert2\ninsert3\n14\n15\n16\n17\n18\n19\n", text1) == 0);
	free(text1);

	undoTB(tb1);
	text1 = dumpTB(tb1, false);	
	assert(strcmp("1\n14\n15\n16\n17\n18\n19\n", text1) == 0);
	free(text1);

	redoTB(tb1);
	text1 = dumpTB(tb1, false);	
	assert(strcmp("1\ninsert1\ninsert2\ninsert3\n14\n15\n16\n17\n18\n19\n", text1) == 0);


	free(text1);
	releaseTB(tb1);
	releaseTB(tb2);
	printf("undoTB redoTB tests passed!\n");
}

/* static void testDiffTB(){
	printf("-----------------------------------------\n"
	       "              diffTB tests               \n"
	       "-----------------------------------------\n");
	TB tb1 = newTB("1\n3\n1\n4\n5\n2\n7\n");
	TB tb2 = newTB("1\n3\n5\n7\n");
	char *text1 = diffTB(tb1, tb2);
	printf("*%s*\n", text1);
	free(text1);
	releaseTB(tb1);
	releaseTB(tb2);
	printf("DiffTB tests passed!\n");
} */

/* void testFormRichText(){
	printf("-----------------------------------------\n"
	       "           FormRichText tests            \n"
	       "-----------------------------------------\n");
	TB tb1 = newTB("**hello***world**\n* *\n*123\nsadsad*sad*\n_asdas__sad_\nasd**dsa\n*asd_dsasda*_\n*asd*_asd*_\n#\n#_asd_\ndas**dsa**\n");
	formRichText(tb1);
	char *text1 = dumpTB(tb1, false);
	//printf("*%s*\n", text1);
	assert(strcmp("*<b>hello</b>*<b>world</b>*\n<b> </b>\n*123\nsadsad<b>sad</b>\n<i>asdas</i><i>sad</i>\nasd**dsa\n<b>asd_dsasda</b>_\n<b>asd</b><i>asd*</i>\n#\n<h1>_asd_</h1>\ndas*<b>dsa</b>*\n", text1) == 0);
	free(text1);
	releaseTB(tb1);
	printf("FormRichText tests passed!\n");
} */

/* static void testSearchTB(){
	printf("-----------------------------------------\n"
	       "              searchTB tests             \n"
	       "-----------------------------------------\n");
	TB tb1 = newTB("Hello World My\nname is jarred lovegood\nand i love carley ray jepson\n");
	Match result = searchTB(tb1, "love");
	assert(result->lineNumber == 2);
	assert(result->columnNumber == 16);
	assert(result->next->lineNumber == 3);
	assert(result->next->columnNumber == 7);
	while(result != NULL){
		Match tmp = result;
		result = result->next;
		free(tmp);
	}
	releaseTB(tb1);
} */

/* static void testMergeTB(void){
	printf("-----------------------------------------\n"
	       "              mergeTB tests              \n"
	       "-----------------------------------------\n");
	TB tb1 = newTB("abcdefghijklmnopqrst\n2\n3\n4\n5\n");
	TB tb2 = newTB("abc\n");
	TB tb3 = newTB("abc1\n");
	int totLen = linesTB(tb1) + linesTB(tb2);
	mergeTB(tb1, 5, tb2);
	assert(linesTB(tb1) == totLen);
	char *text1 = dumpTB(tb1, false);
	//printf("*%s*\n", text1);	
	assert(strcmp("abcdefghijklmnopqrst\n2\n3\n4\nabc\n5\n", text1) == 0);
	free(text1);
	mergeTB(tb1, 6, tb3);
	text1 = dumpTB(tb1,false);
	assert(strcmp("abcdefghijklmnopqrst\n2\n3\n4\nabc\nabc1\n5\n", text1) == 0);
	free(text1);
	releaseTB(tb1);
	printf("mergeTB tests passed!\n");
} */

/* static void testPasteTB(void){
	printf("-----------------------------------------\n"
	       "              mergeTB tests              \n"
	       "-----------------------------------------\n");
	TB tb1 = newTB("abcdefghijklmnopqrst\n2\n3\n4\n5\n");
	TB tb2 = newTB("asdsadasd\n");
	int totLen = linesTB(tb1) + linesTB(tb2);
	pasteTB(tb1, 5, tb2);
	assert(linesTB(tb1) == totLen);
	char *text1 = dumpTB(tb1, true);
	//printf("*%s*\n", text1);	
	assert(strcmp("1. abcdefghijklmnopqrst\n2. 2\n3. 3\n4. 4\n5. asdsadasd\n6. 5\n", text1) == 0);
	//assert(strcmp("6\n7\n8\n9\n1\n2\n3\n4\n5\n", text1) == 0);
	free(text1);
	pasteTB(tb1, 6, tb2);
	text1 = dumpTB(tb1, false);
	//printf("*%s*\n", text1);	
	assert(strcmp("abcdefghijklmnopqrst\n2\n3\n4\nasdsadasd\nasdsadasd\n5\n", text1) == 0);
	//assert(strcmp("6\n7\n8\n9\n1\n2\n3\n4\n5\n", text1) == 0);
	free(text1);
	releaseTB(tb1);
	releaseTB(tb2);
	printf("mergeTB tests passed!\n");
} */

/* static void testNewTB(void) {
	printf("-----------------------------------------\n"
	       "               newTB tests               \n"
	       "-----------------------------------------\n");
	
	// Calling dumpTB immediately after newTB, without modifying the TB
	TB tb1 = newTB("\n");
	assert(linesTB(tb1) == 1);
	char *text1 = dumpTB(tb1, true); // Don't show line numbers
	//printf("*%s*\n", text1);
	assert(strcmp("1. \n", text1) == 0);
	free(text1);
	releaseTB(tb1);
	
	// TODO: Add more tests
	
	
	printf("newTB tests passed!\n");
} */

/*static void testCutTB(void) {
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
}*/

/* static void testAddPrefix(){
	printf("-----------------------------------------\n"
	       "            addPrefixTB tests            \n"
	       "-----------------------------------------\n");
	
	// Calling dumpTB immediately after newTB, without modifying the TB
	TB tb1 = newTB("\naa\n\n\naa\n");
	assert(linesTB(tb1) == 5);
	addPrefixTB(tb1, 1, 3 , "prefix");
	char *text1 = dumpTB(tb1, false); // Don't show line numbers
	//printf("*%s*\n", text1);
	assert(strcmp("prefix\nprefixaa\nprefix\n\naa\n", text1) == 0);
	free(text1);
	addPrefixTB(tb1, 1, 3 , "");
	text1 = dumpTB(tb1, false); // Don't show line numbers
	//printf("*%s*\n", text1);
	assert(strcmp("prefix\nprefixaa\nprefix\n\naa\n", text1) == 0);
	free(text1);
	releaseTB(tb1);
	
	// TODO: Add more tests
	
	
	printf("newTB tests passed!\n");
} */

// TODO: Add more test functions here
/*static void showList(TB tb){
		
	while()
}*/

