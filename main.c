#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/random.h>
#include <gmp.h>

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
			return;
		}
		else {
			mpz_add_ui(p, p, 2);
			fermat_gmp(p);
		}
		
	}
}

void fermat_factorization(mpf_t N) {
	// Fermat's factorization method is an algorithm for finding two factors of an odd number.
	// This method is efficient when the two factors are close to each other, requiring only a few iterations to find them.
	// In a case where the factors are distant, this method is only slightly more efficient than trial division.
	// Requirements:
	// (1): N is an odd integer with non-trivial factors.
	
	// n = a^2 - b^2
	//   = (a + b)(a - b)
	//
	// a = ceil(sqrt(N))
	// b = sqrt(a^2 - N)
	// while (b != int) {
	// 	a = a + 1
	// 	b = sqrt(a^2 - N)
	// }
	// return a - b, a + b

	mpf_t a, a2, b;
	mpf_inits(a, a2, b, NULL);
	mpf_sqrt(a, N); mpf_ceil(a, a); // Set a to square root of N. Apply ceiling function to a.
	gmp_printf("%Ff is a\n", a);
	return;
	mpf_mul(a2, a, a); mpf_sub(a2, a, N); // Set a2 to a^2 - N.
	mpf_sqrt(b, a2); // b = sqrt(a^2 - N)
	
	while (mpf_integer_p(b) != 0) { // While b is not an integer.
		mpf_add_ui(a, a, 1);
		mpf_mul(a2, a, a); mpf_sub(a2, a, N); // Set a2 to a^2 - N.
		mpf_sqrt(b, a2); // b = sqrt(a^2 - N)
	}
	mpf_sub(a, a, b);
	gmp_printf("%Ff is a\n", a);
	mpf_add(a, a, b); mpf_sub(a, b, a);
	gmp_printf("%Ff is b\n", a);
}

int main(int argc, char *argv[])
{
	// Convert argv to uint and assign it to range. Pointer throwaway_ptr points to chars in argv which is irrelevant for our inteded purposes.
	//char *throwaway_ptr;
	//mp_bitcnt_t range = strtol(argv[1], &throwaway_ptr, 10);
	//// Initialize rop variable.
	mpf_t random_number;
	mpf_init(random_number); 
	mpf_set_ui(random_number, 303);
//	impz_random(random_number, range);
	
	fermat_factorization(random_number);
//	fermat_gmp(random_number);
return 0;
}
