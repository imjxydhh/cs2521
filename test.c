#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void test(char *text){
	printf("%d\n",strcmp(text, "a"));
}
int main(void){
	test("");
	int a = 5;
	printf("%d",a);
	a = 10;
	a = 20;
}


