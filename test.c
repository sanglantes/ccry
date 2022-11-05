#include <stdio.h>
#include <stdlib.h>

void main() {
	char lol[200];
	scanf("%s", lol);
	char test[500];
	fgets(test, sizeof(test), stdin);
	puts(test);
}
