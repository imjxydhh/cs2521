#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void test(char *text){
	printf("%d\n",strcmp(text, "a"));
}
int main(void){
	int a[10] = {0};
	a[0] = 0;
	printf("%d", a[5] == 0);
}


