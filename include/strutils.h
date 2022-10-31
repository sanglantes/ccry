#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>

void str2ascii(char str[]) {
	int i = 0;
	char ascii[sizeof(char) * 3];
	char buffer[sizeof(int) * strlen(str) * 3];
	while(str[i]) {
		sprintf(ascii, "%d", str[i++]);
		strcat(buffer, ascii);
	}
}

void sha1_hash(char message[], char rop[]) {
	unsigned char temp[SHA_DIGEST_LENGTH];
	char buf[SHA_DIGEST_LENGTH*2];
	memset(buf, 0x0, SHA_DIGEST_LENGTH*2);
	memset(temp, 0x0, SHA_DIGEST_LENGTH);
	
	SHA1((unsigned char *) message, strlen(message), temp);
	
	for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
	    sprintf((char*)&(buf[i*2]), "%02x", temp[i]);
	}
	strcpy(rop, buf);
}
