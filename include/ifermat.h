#include <stdio.h>
#include <gmp.h>

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

void fermat_factorization(mpf_t N, mpf_t a, mpf_t b) {
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
	mpf_t a2, temp_a;
	mpf_inits(a2, temp_a, NULL);
	mpf_sqrt(a, N); mpf_ceil(a, a); // Set a to square root of N. Apply ceiling function to a.
	mpf_mul(a2, a, a); mpf_sub(a2, a2, N); // Set a2 to a^2 - N.
	mpf_sqrt(b, a2); // b = sqrt(a^2 - N)
	
	while (mpf_integer_p(b) == 0) { // While b is not an integer.
		mpf_add_ui(a, a, 1);
		mpf_mul(a2, a, a); mpf_sub(a2, a2, N); // Set a2 to a^2 - N.
		mpf_sqrt(b, a2); // b = sqrt(a^2 - N)
	}
	mpf_set(temp_a, a);
	mpf_add(a, a, b);
	mpf_sub(temp_a, a, b); mpf_sub(temp_a, a, b);
	mpf_set(b, temp_a);
}

