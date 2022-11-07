#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "include/rsa_suite.h"
#include <string.h>
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
	}
	else {
		rsa_profile(bit_size);
		return (-1);
	}
	return 0;
}
int sign_rsa_message() {
	char message[500];
	char key_location[150];
	mpz_t signature;
	mpz_init(signature);
	fgets(message, sizeof(message), stdin); // BUG
	printf("Enter message:\n>> ");
	fgets(message, sizeof(message), stdin);
	
	printf("Enter private key location:\n>> ");
	scanf("%s", key_location);

	rsa_sign_msg(message, key_location, signature);
	gmp_printf("\nSignature: \n%Zx\n", signature);

	return 0;
}

int verify_rsa_message() {
	char profile_choice[100];
	char original_message[100000];
	char signature[20000];
	FILE* profile_list = fopen("/usr/.userl", "r");

	if (profile_list == NULL) {
		perror("Could not fetch profiles");
		return (-1);
	}

	fgets(profile_choice, sizeof(profile_choice), stdin);
	printf("Select profile: \n\n");
	char c;
	c = fgetc(profile_list);
	while (c != EOF) {
	        printf ("%c", c);
	       	c = fgetc(profile_list);
	}

	printf("\n>> ");
	fgets(profile_choice, sizeof(profile_choice), stdin);
	profile_choice[strlen(profile_choice) - 1] = '\0';

	printf("Enter original message:\n>> ");

	fgets(original_message, sizeof(original_message), stdin);

	printf("Enter signature: \n>> ");
	scanf("%s", signature);
	mpz_t mp_sig;
	mpz_init_set_str(mp_sig, signature, 16);

	if (verify_signature(original_message, NULL, profile_choice, mp_sig) == 1) {
		printf("Signature matches message.\n");
		return 0;
	}
	else if (verify_signature(original_message, NULL, profile_choice, mp_sig) == -1) { perror("Could not read publisher key."); return -1; }

	else {
		printf("Signature does not match message\n");
		return 1;
	}
}

int add_to_keyring() {
	char key_loc[100];
	char profile_name[200];

	printf("Enter key location: \n>> ");
	scanf("%s", key_loc);

	printf("\nEnter name for profile:\n>> ");
	fgets(profile_name, sizeof(profile_name), stdin);
	fgets(profile_name, sizeof(profile_name), stdin);

	profile_name[strlen(profile_name) - 1] = '\0'; // Remove null-termination
					     
	rsa_add_profile(key_loc, profile_name);

	return 0;
}
