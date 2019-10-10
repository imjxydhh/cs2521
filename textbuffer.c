
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "textbuffer.h"

typedef struct textNode {
	char *data;
	struct textNode *next;
	struct textNode *prev;
} *Node;

struct textbuffer {
	Node first;
	Node curr;
	Node last;
	int length;
};

/**
 * Allocate a new textNode whose contents is initialised with the text
 * in the given string.
 */
static Node newNode(char *text, int size){
	char *textCopy = malloc(sizeof(char) * (size + 1));
	if(textCopy == NULL){
	// no enough memory 
		return NULL;
	}
	*textCopy = 0;
	Node new = malloc(sizeof(*new));
	if(new == NULL){
	// no enough memory 
		return NULL;
	}
	if(strcmp(text, "\n") && strcmp(text, "")){
		strcpy(textCopy, text);
	}
	new->data = textCopy;	
	new->prev = new->next = NULL;
	return new;
}

/**
 * Allocate a new textbuffer whose contents is initialised with the text
 * in the given string.
 */
TB newTB (char *text) {
	if(text == NULL){
		fprintf(stderr, "argument can not be null.\n");
		return NULL;
	}
	TB new = malloc(sizeof(*new));
	if(new == NULL){
		fprintf(stderr, "memory allocation failed.\n");
		return NULL;
	}
	new->first = new->curr = new->last = NULL;
	new->length = 0;
	if(!strcmp(text, "")){
	// text is an empty string
		return new;
	}
	char *start = text;
	char *end = text; 
	while(*start != '\0'){
	// start processing the text 
		Node nNode = NULL;
		if(*start == '\n'){
		// there is an empty line
			nNode = newNode("\n", 0);
			if(nNode == NULL){
				fprintf(stderr, "memory allocation failed.\n");
				releaseTB(new);
				return NULL;
			}
			start++;
			end = start + 1;
		}else{
			if(*end == '\n'){
			// the end of a line is encountered
			    char oneLine[end - start + 1];
				strncpy(oneLine, start, end - start);
				oneLine[end - start] = 0;
				nNode = newNode(oneLine, end -start); 
				if(nNode == NULL){
					fprintf(stderr, "memory allocation failed.\n");
					releaseTB(new);
					return NULL;
				}
				start = end + 1;
				end = start + 1;
			}else{
				end++;
			}
		}
		if(nNode != NULL){
			if(new->curr == NULL){
			// condition that the textbuffer is still empty
				new->first = new->curr = new->last = nNode;
				new->length = 1;
			}else{
				new->last->next = nNode;
				nNode->prev = new->last;
				new->last = nNode;
				new->length++;
			}
			
		}
	}
	// used to test, remember to delete 
	/*Node current = new->first;
	while(current != NULL){
		printf("*%s*\n", current->data);
		current = current->next;
	}*/
	return new;
}

/**
 * Free  the  memory occupied by the given textbuffer. It is an error to
 * access the buffer afterwards.
 */
void releaseTB (TB tb) {
	if(tb == NULL){
		return;
	}
	// first release all nodes
	Node curr = tb->first;
	while(curr != NULL){
		Node tmp = curr->next;
		free(curr->data);
		free(curr);
		curr = tmp;
	}
	// finally release TB itself
	free(tb);	
}

/**
 * Allocate and return a string with a larger capacity 
 */
static char *allocStr(char* str, int len){
	if(str == NULL){
	// initializing 
		str = malloc(sizeof(char) * 25);
		str[0] = 0;
		return str;
	}

	// expand the capacity
	char *tmp = str;
	str = malloc(sizeof(char) * (len + 100));
	str[0] = 0;
	strcpy(str, tmp);
	free(tmp);
	return str;
}

/**
 * Allocate and return a string representing the prefix line number 
 */
static char *getPrefix(int size, int position){
	char str[14];
	sprintf(str, "%d. ", position); 
	char *prefix = malloc(sizeof(char) * (size + strlen(str)));
	strcpy(prefix, str);
	return prefix;
}

/**
 * Allocate  and return a string containing all of the text in the given
 * textbuffer. If showLineNumbers is true, add a prefix corresponding to
 * the line number.
 */
char *dumpTB (TB tb, bool showLineNumbers) {
	if(tb == NULL){
	// the case that textBuffer doesn't exist.
		fprintf(stderr, "The textBuffer doesn't exist.\n");
		return NULL;
	}else if(tb->curr == NULL){
	// the case that tb is empty
		char *empty = malloc(sizeof(char));
		*empty = 0;
		return empty;
	}

	int position = 1;
	Node curr = tb->first;
	char *str = NULL;
	int size = 25;
	str = allocStr(str, size);
	if(str == NULL){
		fprintf(stderr, "memory allocation failed.\n");
		return NULL;
	}
	while(curr != NULL){
		if((int)strlen(curr->data) >= (int)(size - strlen(str) - (showLineNumbers?4:1))){
		// if there is no enough space in str, allocate more memory
		/** 
		 * "- (showLineNumbers?4:1)" to ensure that 
		 * there is enough space for line number and \n
		 */
			size = size + strlen(curr->data);
			str = allocStr(str, size);
			size += 100;
			if(str == NULL){
				fprintf(stderr, "memory allocation failed.\n");
				return NULL;
			}
		}
		char *thisLine = NULL;
		if(showLineNumbers){
			char *prefix = getPrefix(strlen(curr->data) + 1, position);
			if(prefix == NULL){
				fprintf(stderr, "memory allocation failed.\n");
				free(str);
				return NULL;
			}
			thisLine = strcat(prefix, curr->data);
		}else{
			thisLine = curr->data;
		}
		strcat(str, thisLine);
		if(showLineNumbers){
		/** 
		 * in this condition, thisLine is seperately allocated and 
		 * will not be used again. it needs to be freeed
		 */	
			free(thisLine);
		}
		strcat(str, "\n");
		curr = curr->next;
		position++;
	}
	return str;
}

/** 
 * Return the number of lines of the given textbuffer.
 */
int linesTB (TB tb) { //DONE
	return tb->length;
}

/**
 * There are a lot of functions which need boundary checking so boundary 
 * check is abstracted as a function.
 * textBuffer mainTB is the target to check bound. There is another textBuffer
 * as parameter because some funtions have two textBuffer as parameters,
 * when out of bound error is detected, both of these two textBuffer need to be 
 * freed.
 * The position need to be checked is passed as pointer because if there is
 * only one position which needs to be checked, we could set one of the arugument
 * as NULL so that only one position will be checked
 */

static void CheckInBound(TB mainTB, TB maybe, int *mainPos, int *maybePos){
	// A judgement
	int state = 0; 
	
	if(mainTB == NULL){
	// textbuffer does not exist
		fprintf(stderr, "The textBuffer doesn't exist.\n");
		state = 1;
	}else if(maybePos == NULL){
		if(*mainPos < 1 || *mainPos > mainTB->length){
				// index out of bound
			fprintf(stderr, "Index out of boundary!\n");
			releaseTB(mainTB);
			state = 1;
		}
	}else if(*mainPos < 1 || *maybePos > mainTB->length){
	// index out of bound
		fprintf(stderr, "Index out of boundary!\n");
		releaseTB(mainTB);
		state = 1;
	}else if(*mainPos > *maybePos){
	// left point bigger than right point
		fprintf(stderr, "invalid range: left point > right point\n");
		releaseTB(mainTB);
		state = 1;
	}
	if(state){
		if(maybe != NULL){
			releaseTB(maybe);
		}
		abort();
	}
}


/**
 * Add a given prefix to all lines between 'from' and 'to', inclusive.
 * - The  program  should abort() wih an error message if 'from' or 'to'
 *   is out of range. The first line of a textbuffer is at position 1.
 */
void addPrefixTB (TB tb, int from, int to, char *prefix) {
	if(prefix == NULL){
		printf("The prefix string is NULL!\n");
		releaseTB(tb);
		abort();
	}
	CheckInBound(tb, NULL, &from, &to);
	if(!strcmp(prefix, "")){
	// if prefix is an empty string, directly return to speed up
		return;
	}
	Node curr = tb->first;

	// calculate how many lines are needed to add prefix
	int num = to - from; 
	
	while(from > 1){
	// move curr to the line corresponding to from 
		curr = curr->next;
		from--;
	}
	while(num >= 0){
	// add prefix for each line in the range
		char *newStr = malloc(sizeof(char) * (strlen(prefix) + strlen(curr->data) + 1));
		strcpy(newStr, prefix);
		strcat(newStr, curr->data);
		char *tmp = curr->data;
		curr->data = newStr;
		free(tmp);
		num--;
		curr = curr->next;
	}
}

/**
 * Merge 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the merged-in lines from 'tb2'.
 *   - 'tb2' can't be used anymore (as if we had used releaseTB() on it)
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void mergeTB (TB tb1, int pos, TB tb2) {
	if(pos != tb1->length + 1){
		CheckInBound(tb1, tb2, &pos, NULL);
	}
	if(tb2 == NULL){
		fprintf(stderr, "TextBuffer tb2 doesn't exist.\n");
		releaseTB(tb1);
		abort();
	}else if(tb1 == tb2){
	// Ignore the case that merge a textBuffer with itself
		return;
	}else if(tb1->first == NULL){
	// tb1 is empty
		tb1->first = tb2->first;
		tb1->curr = tb2->curr;
		tb1->last = tb2->last;
		tb1->length = tb2->length;
		free(tb2);
		return;
	}else if(tb2->first == NULL){
	// tb2 is empty
		free(tb2);
		return;
	}

	Node theLine = tb1->first;
	while(pos > 1){
	// move theLine to the line corresponding to pos
		theLine = theLine ->next;
		pos--;
	}
	Node preLine = NULL;
	if(theLine == NULL){
	// theLine is NULL, which means pos == tb1->length + 1
	// the case that appending tb2 to tb1
		preLine = tb1->last;
		tb1->last = tb2->last;
	}else{
		preLine = theLine->prev;
	}
	if(preLine == NULL){
	// The previous node of the node on pos is NULL, which means pos == 1
	// the case that inserting tb2 before tb1
		tb1->first = tb2->first;
	}else{
		preLine->next = tb2->first;
		tb2->first->prev = preLine;
	}
	tb2->last->next = theLine;
	if(theLine != NULL){
	/** 
	 * theLine != NULL means that we are not trying to append tb2 to tb1
	 * When appending tb2 to tb1, theLine will be NULL at last  
	 */
		theLine->prev = tb2->last->next;
	}
	tb1->length = tb1->length + tb2->length;
	//printf("**now tb1:\nfirst %s\ncurr %s\nlast %s\n",tb1->first->data,tb1->curr->data, tb1->last->data);
	free(tb2);
}

/**
 * Copy 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the pasted-in lines from 'tb2'.
 *   - 'tb2' is unmodified and remains usable independent of tb1.
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void pasteTB (TB tb1, int pos, TB tb2) {;
	if(tb2 == NULL){
		fprintf(stderr, "TextBuffer tb2 doesn't exist.\n");
		releaseTB(tb1);
		abort();
	}else if(tb2->first == NULL){
	// tb2 is empty
		return;	
	}

	// get a copy of tb2
	TB tb2Copy = NULL;  
	char *tmp = dumpTB(tb2, false);
	tb2Copy = newTB(tmp);

	free(tmp);
	mergeTB(tb1, pos, tb2Copy);
}

/**
 * Cut  the lines between and including 'from' and 'to' out of the given
 * textbuffer 'tb' into a new textbuffer.
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
TB cutTB (TB tb, int from, int to) {
	CheckInBound(tb, NULL, &from, &to);
	
	// The number of nodes which will be cut
	int num = to - from + 1;

	Node left = tb->first; 
	Node right = tb->first;
	while(from != 1){
		left = left->next;
		from--;
	}
	while(to != 1){
		right = right->next;
		to--;
	}

	TB result = newTB("");
	if(result == NULL){
	// no enough memory
		releaseTB(tb);
		abort();
	}
	result->first = result->curr = left;
	result->last = right;
	if(left == tb->first && right != tb->last){
	// The case that cutting from the head of tb1 
		tb->first = tb->curr = right->next;
		tb->first->prev = NULL;
		result->last->next = NULL;
	}else if(left != tb->first && right == tb->last){
	// The case that cutting to the end of tb1
		tb->curr = tb->first;
		tb->last = left->prev;
		tb->last->next = NULL;
		result->first->prev = NULL;
	}else if(left == tb->first && right == tb->last){
	// The case that cutting the whole tb1
		tb->first = tb->curr = tb->last = NULL;
	}else{
	// The case that cutting a internal part of tb1
		tb->curr = tb->first;
		result->first->prev->next = result->last->next;
		result->last->next->prev = result->first->prev;
		result->last->next = NULL;
		result->first->prev =NULL;
	}
	result->length = num;
	tb->length -= num;
	return result;
}

/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB (TB tb, char *search) {
	return NULL;
}

/**
 * Remove  the  lines between 'from' and 'to' (inclusive) from the given
 * textbuffer 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
void deleteTB (TB tb, int from, int to) {
	TB tmp = cutTB(tb, from, to);
	releaseTB(tmp);
}

/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
void formRichText (TB tb) {

}

////////////////////////////////////////////////////////////////////////
// Bonus challenges

char *diffTB (TB tb1, TB tb2) {
	return NULL;
}

void undoTB (TB tb) {

}

void redoTB (TB tb) {

}

