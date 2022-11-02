#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include "irandom.h"
#include "fileop.h"
#include "strutils.h"
#include <openssl/sha.h>
#define MAX_KEY_LENGTH 4096

int rsa_profile(mp_bitcnt_t bit_size) {
	// Generate RSA profile and output results to file.
	
	// Initialization
	mpz_t p, q, n, e, o, d;
	mpz_inits(p, q, n, e, o, d, NULL);
	mpz_set_ui(e, 65537);

	// Prime number generation
	impz_random(p, bit_size); // p
	mpz_nextprime(p, p);
	impz_random(q, bit_size); // q
	mpz_nextprime(q, q);
	mpz_mul(n, p, q); // n = pq

	// phi(n) = (p - 1)(q - 1)
	mpz_t temp_p, temp_q;
	mpz_inits(temp_p, temp_q, NULL);
	mpz_sub_ui(temp_p, p, 1); mpz_sub_ui(temp_q, q, 1);
	mpz_mul(o, temp_p, temp_q);
	
	// d is the multiplicative modular inverse of e mod o.
	// Find a d such that ed = 1 (mod o)
	mpz_invert(d, e, o);

	// Create file streams
	FILE* private_fptr = fopen("private_key", "w");
	FILE* public_fptr = fopen("public_key", "w");
	if (private_fptr == NULL || public_fptr == NULL) {
		perror("Could not create profile.");
		return (-1);
	}
	// Write (n, d) to private profile. Although n is public, it is used in digital signatures. Storing it together with d facilitates the signing process.
	// Write (n) to public profile.
	
	gmp_fprintf(private_fptr, "%Zx\n%Zx\n", d, n);
	gmp_fprintf(public_fptr, "%Zx\n", n);

	fclose(private_fptr);
	fclose(public_fptr);
	return 1;
}

int rsa_sign_msg(char message[], char key_location[], mpz_t signature) {
	// Signs a hash with private RSA key.
	// Returns 0 on succes, -1 on failure.
	mpz_t d, n;

	FILE* key_fptr = fopen(key_location, "r");
	if (key_fptr == NULL) {
		perror("Could not read from file");
		return (-1);
	}
	char composite[MAX_KEY_LENGTH];
	char private_exponent[MAX_KEY_LENGTH];
	fscanf(key_fptr, "%s %s", private_exponent, composite);
	mpz_init_set_str(d, private_exponent, 16);
	mpz_init_set_str(n, composite, 16);
	
	char message_digest[SHA_DIGEST_LENGTH];
	sha1_hash(message, message_digest);
	mpz_t mpz_hash;
	mpz_init_set_str(mpz_hash, message_digest, 16); 
	
	mpz_powm(signature, mpz_hash, d, n);
	fclose(key_fptr);
	return 0;
}

int verify_signature(char original_message[], char path_to_publisher[], char name_of_publisher[], mpz_t signed_hash) {
	// Verify signatures of hashes. Returns 1 if the signature is valid, 0 if it is not, and -1 on failure.
	FILE* pptr = fopen(path_to_publisher, "r"); // Init file ptr.
	if (pptr == NULL && name_of_publisher == NULL) {
		perror("Could not read publisher key");	
		return (-1);
	}

	if (path_to_publisher == NULL) {
		char output[5000];
		char key[5000];
		read_fileb(name_of_publisher, output);

		mpz_t publisher_key;
		mpz_init_set_str(publisher_key, output, 16);
		// Decrypt hash
		mpz_t decrypted_hash;
		mpz_init(decrypted_hash);
		mpz_powm_ui(decrypted_hash, signed_hash, 65537, publisher_key);
	
		// Commence independent investigation
		char ind_hash[41];
		mpz_t independent_hash; 
		sha1_hash(original_message, ind_hash);
		mpz_init_set_str(independent_hash, ind_hash, 16);

		if (mpz_cmp(independent_hash, decrypted_hash) == 0) {
			return 1;		
		}
		
		else {
			return 5;
		}

	}

	char str_publisher_key[MAX_KEY_LENGTH]; // String to be converted to mpz_t
	fscanf(pptr, "%s", str_publisher_key); // str_publisher_key is set to publisher public key
	mpz_t publisher_key;
	mpz_init_set_str(publisher_key, str_publisher_key, 16);
	// Decrypt hash
	mpz_t decrypted_hash;
	mpz_init(decrypted_hash);
	mpz_powm_ui(decrypted_hash, signed_hash, 65537, publisher_key);

	// Commence independent investigation
	char ind_hash[41];
	mpz_t independent_hash; 
	sha1_hash(original_message, ind_hash);
	mpz_init_set_str(independent_hash, ind_hash, 16);
	
	fclose(pptr);

	if (mpz_cmp(independent_hash, decrypted_hash) == 0) {
		return 1;
	}
	else {
		return 0;
	}
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
	char output[5000];	
	if (read_fileb(public_key, output) != 1) {
		printf("Public key already exists in keyring.\n");
		return 0;
	}

	else if (read_fileb(public_key, output) != 0) {read_fileb(public_key, output);}
	
	fputs(profile_name, keyring);
	fputs("\t", keyring);
	fputs(public_key, keyring);
	fputs("\n", keyring);

	fclose(keyring);

	//Bodge: add names in seperate file.
	FILE* name_f = fopen("/usr/.userl", "a");
	fputs(profile_name, name_f);
	fputs("\n", name_f);
	fclose(name_f);
	return 1;
}
