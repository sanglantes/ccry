#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	char str1[4000]; char str2[4000];
	FILE* ptr = fopen("public_key", "r");
	fscanf(ptr, "%s %s", str1, str2);
	printf("%s\n%s\n", str1, str2);
	fclose(ptr);
return 0;
}

