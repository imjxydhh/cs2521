#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void test(char *text){
	printf("%d\n",strcmp(text, "a"));
}
int main(void){
	test("");
}


