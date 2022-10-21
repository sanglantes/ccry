#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/random.h>
#include <gmp.h>


//typedef struct RSAPROFILE
//{
//	char q, p, public_exp, phi, mod_inv [];
//} rsa_p;
//
//mpz_t* extended_gcd(mpz_t public_exp, mpz_t phi)
//{
//	mpz_t inverse;
//	mpz_set_ui(inverse, 1077);
//	mpz_t* inv = &inverse;
//	return inv;	
//}
//
//char rsa_gen(uint16_t bit_size, char* mr_flag) // blueprint for generating RSA key pairs.
//{
//	rsa_p rsaprofile;
//
//	rsaprofile.p = pgen(bit_size);
//	rsaprofile.q = pgen(bit_size);
//	rsaprofile.public_exp = "65537";
//	if (mr_flag != NULL) {
//		rsaprofile.p = miller_rabin(rsaprofile.p);
//		rsaprofile.q = miller_rabin(rsaprofile.q);
//	}
//
//	rsaprofile.phi = phi_func(rsaprofile.p, rsaprofile.q);
//	rsaprofile.mod_inv =  extended_gcd(rsaprofile.public_exp, rsaprofile.p);
//}

uint64_t rand_seed() { // Generates seed which can be used by libgmp to generate larger random integers.
	uint64_t buffer;
	size_t buf_len = 8; // Buffer length in bytes. Variable buffer is uint64_t, therefore 8 bytes should be read from /dev/urandom.
	unsigned int flag = 0; // Set bit to 0. Prevents blocking if entropy is low.
	getrandom(&buffer, buf_len, flag);
	return buffer;
}

void impz_random(mpz_t rop, mp_bitcnt_t s_range) { // Generate random number with GMP.
	// Initialization of random state.
	gmp_randstate_t state;
	gmp_randinit_default(state);
	// Random state seeding
	gmp_randseed_ui(state, rand_seed());
	// Random number generation
	mpz_urandomb(rop, state, s_range); // Using mpz_urandomb to generate an integer between 0 and 2^n-1. Alternatively, use mpz_urandomm for 0 through n-1.
}

void fermat_algorithm(mpz_t p) {
	// An algorithm that utilizes Euler's generalization of Fermat's little theorem for finding primes.
	// This theorem is not resistant against strong pseudoprimes such as Carmichael numbers.
	// Use Miller-Rabin algorithm to confirm with k certainty that p is prime.
	
	mpz_t rop, p_minus_one, one, base;
	mpz_inits(rop, p_minus_one, one, base, NULL); // Initialize variables.
	mpz_set_ui(base, 2);
	mpz_set_ui(one, 1);
	if (mpz_cmp_ui(p, 0) == 0) {
		mpz_add_ui(p, p, 1); // Checks if p is odd. If not, add 1 to p and return (recursion).
		fermat_algorithm(p);
	}

	else {
		mpz_sub_ui(p_minus_one, p, 1); // Sets p_minus_one to p-1.
		mpz_powm(rop, base, p_minus_one, p); // Checks theorem: a^p-1 cong 1 mod p
		if (mpz_cmp_ui(rop, 1) == 0) {
			gmp_printf("%Zd", p);
		}
		else {
			mpz_add_ui(p, p, 2);
			fermat_algorithm(p);
		}
	}
}

int main(int argc, char *argv[])
{
	// Convert argv to uint and assign it to range. Pointer throwaway_ptr points to chars in argv which is irrelevant for our inteded purposes.
	char *throwaway_ptr;
	mp_bitcnt_t range = strtol(argv[1], &throwaway_ptr, 10);
	// Initialize rop variable.
	mpz_t random_number;
	mpz_init(random_number); 
	mpz_set_ui(random_number, 6);
//	impz_random(random_number, range);

	fermat_algorithm(random_number);
return 0;
}
