#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include "irandom.h"
#include "fileop.h"
#include "strutils.h"
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
	// Write (n, d) to private profile. Although n is public, it is used in digital signatures. Storing it together with d facilitates the signing process.
	// Write (n) to public profile.
	gmp_fprintf(private_fptr, "%Zx\n%Zx\n", d, n);
	gmp_fprintf(public_fptr, "%Zx\n", n);

	fclose(private_fptr);
	fclose(public_fptr);
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

int mpz_rsa_read_msg(mpz_t signed_hash, char original_message[], char path_to_publisher[]) {
	FILE* pptr = fopen(path_to_publisher, "r"); // Init file ptr.
	if (pptr == NULL) {
		perror("Could not read publisher key");	
		return (-1);
	}
	char str_publisher_key[MAX_KEY_LENGTH]; // String to be converted to mpz_t
	fscanf(pptr, "%s", str_publisher_key); // str_publisher_key is set to publisher public key

	mpz_t publisher_key, decrypted_hash;
	mpz_init_set_str(publisher_key, str_publisher_key, 16); // Set str ver to mpz type
	mpz_init(decrypted_hash);
	// Hash the orignal message to independently verify the signed hash.
	char independent_hash[41];
	mpz_t mpz_independent_hash;
	sha1_hash(original_message, independent_hash);

	// Decrypt the signed hash.
	char str_decrypted_hash[4096];
	char final_d_hash[4096];
	mpz_powm_ui(decrypted_hash, signed_hash, 65537, publisher_key);
	gmp_sprintf(str_decrypted_hash, "%s", decrypted_hash);
//	sha1_hash(str_decrypted_hash, final_d_hash);

	// decrypted_hash and independent_hash should be equal, if not, the message does not match the signature.
	mpz_init_set_str(mpz_independent_hash, independent_hash, 16);

	printf("%s\n\n\n\n\n%s\n", final_d_hash, independent_hash);

	fclose(pptr);
	return 0;
}

void rsa_read_msg(char signed_hash[], char original_message[], char path_to_publisher[]) {
	// Simply a macro for the equivalent mpz function. Facilitates conversions.
	mpz_t n_signed_hash;
	mpz_init_set_str(n_signed_hash, signed_hash, 16);
	mpz_rsa_read_msg(n_signed_hash, original_message, path_to_publisher);
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
	mpz_t sign;
	mpz_init(sign);
	rsa_sign_msg("Hello", "private_key", sign);

	mpz_rsa_read_msg(sign, "Hello", "public_key");
return 0;
}

