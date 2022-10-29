#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>

char* sha1_hash(char tstring[]) {
	char* hash_string = (char *) malloc(sizeof(char) * 512);
//	SHA1(tstring, strlen(tstring), hash_string);
	return hash_string;
}
