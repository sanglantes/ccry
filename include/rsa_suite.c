#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include "irandom.h"
#include "fileop.h"
#include <openssl/sha.h>
#define MAX_KEY_LENGTH 4096

void rsa_profile(mp_bitcnt_t bit_size) {
	// Generate RSA profile and output results to file.
	
	// Initialization
	mpz_t p, q, n, e, o, d;
	mpz_inits(p, q, n, e, o, d);
	mpz_set_ui(e, 65537);

	// Prime number generation
	impz_random(p, bit_size); // p
	impz_random(q, bit_size); // q
	mpz_mul(n, p, q); // n = pq

	// phi(n) = (p - 1)(q - 1)
	mpz_sub_ui(p, p, 1); mpz_sub_ui(q, q, 1);
	mpz_mul(o, p, q);
	
	// d is the multiplicative modular inverse of e mod o.
	// Find a d such that ed = 1 (mod o)
	mpz_invert(d, e, o);

	// Create file streams
	FILE* private_fptr = fopen("private_key", "w+");
	FILE* public_fptr = fopen("public_key", "w+");
	gmp_fprintf(private_fptr, "%Zx\n%Zx\n", o, d);
	fclose(private_fptr);
	fclose(public_fptr);
}

int rsa_sign_message(char message[], char key_location[]) {
	// Signs a hash with private RSA key.
	// (m^d)^e === m mod n
	// (m^e)^d === m mod n
	mpz_t o, d;
	FILE* key_fptr = fopen(key_location, "r");
	if (key_fptr == NULL) {
		perror("Could not read from file");
		return (-1);
	}
	char phi_value[MAX_KEY_LENGTH], private_exponent[MAX_KEY_LENGTH];
	fscanf(key_fptr, "%s %s", phi_value, private_exponent);
	mpz_init_set_str(o, phi_value, 16);
	mpz_init_set_str(d, private_exponent, 16);

	sha1_hash(message);
	return 0;
}

int rsa_add_profile(char path_to_file[],  char profile_name[]) {
	// Reads a public key from a file or string.
	FILE* path_ptr = fopen(path_to_file, "r");
	if (path_ptr == NULL) {
		perror("Could not read public key profile");
		return (-1);
	}
	char public_key[MAX_KEY_LENGTH]; // Read public key from file and put it in a string.
	fscanf(path_ptr, "%s", public_key);
	fclose(path_ptr);

	FILE* keyring = fopen("/usr/.database", "a");
	if (keyring == NULL) {
		perror("Could not access keyring file");
		return (-1);
	}
	
	if (read_fileb(public_key) < 1) {
		printf("Public key already exists in keyring.\n");
		return 0;
	}
	
	fputs(profile_name, keyring);
	fputs("\t", keyring);
	fputs(public_key, keyring);
	fputs("\n", keyring);

	fclose(keyring);
	return 1;
}

int main() {
	mp_bitcnt_t bit = 4096;
//	rsa_profile(bit);
	rsa_add_profile("public_key", "John");
return 0;
}

