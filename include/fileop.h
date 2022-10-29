#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
int read_filec(char public_key[]) {
	// WARNING: This function is outdated and inefficient. Use read_fileb() instead.

	// Return 1 if string already exists in file, 0 if it doesn't.
	// istring[] must have a large buffer (> character count of file)
	// Return -1 on error, 1 when profile has not been added, and 0 on success.
	char file_text[50000];
	char current_char[20000];
	FILE* path = fopen("/usr/.database", "r");
	char *MAX_CHAR_READ = (char *) malloc(sizeof(char) * 10000);
	if (path == NULL) {
		perror("fileop: Could not read keyring");
		return (-1);
	}

	while (fgets(current_char, 20000, path) != NULL) {
		fgets(current_char, *MAX_CHAR_READ, path);
		strcat(file_text, current_char);
	}

	free(MAX_CHAR_READ);
	
	if (strstr(file_text, public_key) != NULL) {
		return 0;
	}

	fclose(path);
	return 1;
}

int read_fileb(char public_key[]) {
	FILE* path = fopen("/usr/.database", "r");
	char file_text[50000];
	if (path == NULL) {
		perror("Could not read keyring.");
		return (-1);
	}

	fread(file_text, 5000, 5000, path);
	if (strstr(file_text, public_key) != NULL) {
		return 0;
	}	

	fclose(path);
	return 1;
} 
