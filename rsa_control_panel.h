#include <stdio.h>
#include <assert.h>
#include "include/rsa_suite.h"
#include <unistd.h>

int generate_rsa_profile() {
	unsigned int bit_size;
	char path[100];
	getcwd(path, 100);

	puts("Enter wanted bit size (160 to 4096 bits):");
	printf(">> ");
	scanf("%i", &bit_size);
	assert(bit_size > 159 && bit_size < 4097);

	puts("Generating key pair...");

	if (rsa_profile(bit_size) == 1) {
		printf("Keys generated and stored at %s/public_key and %s/private_key.\n", path, path);
		rsa_interface();
	}
	else {
		rsa_profile(bit_size);
		return (-1);
	}
	return 0;
}
int sign_rsa_message() {

	return 0;
}
int verify_rsa_message() {}
int add_to_keyring() {}
int encrypt_message() {}
