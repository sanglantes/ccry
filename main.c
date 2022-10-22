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

void fermat_gmp(mpz_t p) {
	mpz_t rop;
	mpz_t a;
	mpz_t p_min;
	mpz_inits(a, rop, p_min, NULL);
	mpz_set_ui(a, 2);
	mpz_sub_ui(p_min, p, 1);

	if (mpz_odd_p(p) == 0) {
		mpz_add_ui(p, p, 1);
		fermat_gmp(p);
	}

	else {
		mpz_powm(rop, a, p_min, p);
		if (mpz_cmp_ui(rop, 1) == 0) {
			gmp_printf("%Zd is prime \n", p);
		}
		else {
			mpz_add_ui(p, p, 2);
			fermat_gmp(p);
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
	impz_random(random_number, range);

	fermat_gmp(random_number);
return 0;
}
