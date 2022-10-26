#include <stdio.h>
#include <gmp.h>
#include "irandom.h"

void rsa_profile(mp_bitcnt_t bit_size) {
	// Generate RSA profile and output results to file.
	
	// Initialization
	mpz_t p, q, n, e, o, d;
	mpz_inits(p, q, n, e, o, d);
	mpz_set_ui(e, 65537);
	mpz_set_str(o, "2502100440", 10);

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
	gmp_fprintf(private_fptr, "%Zd\n%Zd\n", o, d);
	fclose(private_fptr);
	gmp_fprintf(public_fptr, "%Zd", n); // Key is automatically assumed to be 65537
	fclose(public_fptr);
}

void read_profile() {

}



void main() {
	mp_bitcnt_t bit = 4096;
	rsa_profile(bit);
}
