#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>

int read_fileb(char search_q[], char output[]) { // Finds strings in files.
				  // Returns -1 on error, 1 if the string is found, 0 if it isn't.
	FILE* path_s = fopen("/usr/.database", "r");
	char file_text[50000];
	if (path_s == NULL) {
		perror("Could not read keyring.");
		return (-1);
	}

	fread(file_text, 5000, 5000, path_s);
	if (strstr(file_text, search_q) != NULL) {
		char *occurence = strstr(file_text, search_q);
		char keyholder[100], key[2048];
		sscanf(occurence, "%s %s", keyholder, key);
		strcpy(output, key);
		fclose(path_s);
		return 1;
	}	

	fclose(path_s);
	return 0;
} 
