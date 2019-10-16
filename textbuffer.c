
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
int linesTB (TB tb) { 
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
	//printf("**now tb1:\nfirst %s\ncurr %s\nlast %s\n",tb1->first->data,tb1->curr->data, tb1->last->data); ******only for testing******
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
 * Compare against the text itselft to
 * calculate the prefix function of a given string
 */ 
static void prefixFunction(int *result, char *text){
	result[0] = 0;
	int k = 0;
	for(int q = 1;q < strlen(text) - 1; q++){
	/** 
	 * here the threshold is strlen(text) - 1 beacause
	 * each results will not overlab with each other. If
	 * we find a full match, we will just start searching 
	 * next match from the end of this match
	 */
		while(text[k] != text[q] && k > 0){
			k = result[k - 1];
		}
		if(text[k] == text[q]){
			k++;
		}
		result[q] = k;
	}
}

/** 
 * Get a new initialized Match Node
 */ 
static Match newMatchNode(int line, int col){
	Match newNode = malloc(sizeof(*newNode));
	newNode->columnNumber = col;
	newNode->lineNumber = line;
	newNode->next = NULL;
	return newNode;
}

/**
 * Return a linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB (TB tb, char *search) {
	if(tb == NULL){
		fprintf(stderr, "The textBuffer doesn't exist.\n");
		abort();
	}else if(search == NULL){
		fprintf(stderr, "The search string can not be NULL");
		abort();
	}else if(!strcmp(search, "")){
	// the case that the search text is an empty string
		return NULL;
	}
	Match result = NULL;
	Match currMatch = result;
	Node curr = tb->first;
	int lineNum = 1;

	/** 
	 * Because the results should not overlap, we don't need to know the
	 * longest prefix of a entirely matched string, so we only need an
	 * array with a length of "strlen(search) - 1"
	 */
	int prefixFunc[strlen(search) - 1];
	prefixFunction(prefixFunc, search);

	while(curr != NULL){
	// search all nodes in the list
		char *currStr = curr->data;

		// Number of characters that are currently matched
		int matchNum = 0;

		for(int shift = 0;shift - matchNum <= strlen(currStr) - strlen(search);shift++){
			while(search[matchNum] != currStr[shift] && matchNum > 0){
			/** 
			 * when next character could not match, find the longest suffix via prefix funtion
			 * as the new beginning of a match
			 */
				matchNum = prefixFunc[matchNum - 1];
			}
			if(search[matchNum] == currStr[shift]){
			// if next character can match then just move to next character in the string
				matchNum++;
			}
			if(matchNum == strlen(search)){
			// if find a full match then add the node to our result list
				if(result == NULL){
				// the case that there is no node in the list yet
					result = newMatchNode(lineNum, shift - strlen(search) + 2);
					currMatch = result;
				}else{
				// the case that the list contains at least one node
					currMatch->next = newMatchNode(lineNum, shift - strlen(search) + 2);
					currMatch = currMatch->next;
				}

				// reset the number of characters that are already matched
				matchNum = 0;
			}
		}
		lineNum++;
		curr = curr->next;
	}
	return result;
}
/**
 * Acknowlege: the core algorithm(kmp) of searchTB() and prefixFunction() 
 * is written based on the pseudocode on page 1005 and page 1006 of the book 
 * Introduction to Algorithms(3rd Edition) by Thomas H. Cormnen, et al
 */



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
 * Replace special characters at specific position with specific content
 * @param left : the position of the left one special sign(*, _ or #)
 * @param right : the postion of the right one special sign(* or _), or 
 * 				  strlen(text) if the special sign is #
 * @return A new string which is a copy of the original string and the 
 * 		   special sign on the specified place are repleaced with corresponding
 *         rich text
 */ 
static char *replaceRichText(char *text, int left, int right){
	char *newStr = NULL;
	char leftStr[5];
	char rightStr[6];
	int len = strlen(text);

	switch (text[left]){
	case '#':
		sprintf(leftStr, "<h1>");
		sprintf(rightStr, "</h1>");

		// there are 9 new character + a '\0' so it's (len + 10)
		newStr = malloc(sizeof(char) * (len + 10));
		break;
	case '*':
		sprintf(leftStr, "<b>");
		sprintf(rightStr, "</b>");

		// there are 7 new character + a '\0' so it's (len + 8)
		newStr = malloc(sizeof(char) * (len + 8));
		break;
	case '_':
		sprintf(leftStr, "<i>");
		sprintf(rightStr, "</i>");
		newStr = malloc(sizeof(char) * (len + 8));
		break;
	default:
		fprintf(stderr, "replaceRichText() Switch error!!\n");
		break;
	}

	// null-terminate the new string so that string function can be applied
	newStr[0] = 0;

	if(left > 0){
	// The case that the left position is not on the edge of the string
	// copy the part on the left hand side of the left special sign to the new string
		char leftPart[left + 1];
		strncpy(leftPart, text, left);
		leftPart[left] = 0;
		strcat(newStr, leftPart);
	}
	strcat(newStr, leftStr);
	char midPart[right - left];
	strncpy(midPart, text + left + 1, right - left - 1);
	midPart[right - left - 1] = 0;
	strcat(newStr, midPart);
	strcat(newStr, rightStr);
	if(right < len - 1){
	// The case that the right position is not on the edge of the string
	// copy the part on the right hand side of the left special sign to the new string
		char rightPart[len - right];
		strncpy(rightPart, text + right + 1, len - right - 1);
		rightPart[len - right - 1] = 0;
		strcat(newStr, rightPart);
	}
	return newStr;
}

/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
void formRichText (TB tb) {
	if(tb == NULL){
		fprintf(stderr, "The textBuffer doesn't exist.\n");
		abort();
	}
	Node curr = tb->first;
	
	while(curr != NULL){
	// Search all nodes in the textBuffer
		char *str = curr->data;
		
		// Used to record the which sign we are considering now
		// can be '*' or '_'
		char mode = 0;

		int backPoint = 0;

		//used to record the position of left special sign
		int left = -1;

		if(str[0] == '#' && strlen(str) > 1){
		/** 
		 * if there is # sign at the beginning of the string and the string
		 * has other content, we can ignore all other internal special signs
		 * as we ignore nesting
		 */
			curr->data = replaceRichText(str, 0, strlen(str)+1);
			free(str);
		}else{
			for(int i = 0;i < strlen(str);i++){
				if(str[i] == '*' || str[i] == '_'){
					if(left == -1){
					/** 
					 * the case that no special signs have been encountered 
					 * or a pair of special signs have just been replaced
					 */
						left = i;
						mode = str[i];
						backPoint = i;
					}else if(str[i] == mode){
					// the case that we have found a pair of special signs
						if(i != left + 1){
							char *tmp = str;
							str = replaceRichText(str, left, i);
							free(tmp);
							i += 5;
							mode = 0;
							left = -1;
						}else{
						/** 
						 * The case that there is no content between this pair
						 * of special signs. The use the right special sign as
						 * the new left special sign
						 */

							left = i;
						}
					}
				}
				if(mode && i == strlen(str) - 1){
				/** 
				 * A special case that there is not a corresponding right sign
				 * to a * or _ in the string. Then we go back to the next position
				 * of that left special sign to inspect if there is the other kind of
				 * special sign in the string
				 */	
					i = backPoint;
					mode = 0;
					left = -1;
				}
			}
			// here curr->data may be freed
			curr->data = str;
		}
		curr = curr->next;
	}
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

