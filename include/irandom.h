#include <gmp.h>
#include <sys/random.h>
#include <stdint.h>

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

