#include <stdio.h>
#include <gmp.h>
#include <string.h>
int read_filec(char public_key[]) {
	// Return 1 if string already exists in file, 0 if it doesn't.
	// istring[] must have a large buffer (> character count of file)
	// Return -1 on error, 1 when profile has not been added, and 0 on success.
	char file_text[50000];
	char current_char[10000];
	FILE* path = fopen("/usr/.database", "r");
	if (path == NULL) {
		perror("fileop: Could not read keyring");
		return (-1);
	}

	while (fgets(current_char, 10000, path) != NULL) {
		fgets(current_char, 10000, path);
		strcat(file_text, current_char);
	}
	
	if (strstr(file_text, public_key) != NULL) {
		return 0;
	}
	fclose(path);

	return 1;
}
