#include <stdio.h>
#include <limits.h>
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
	mpf_mul(a2, a, a); mpf_sub(a2, a2, N); // Set a2 to a^2 - N.
	mpf_sqrt(b, a2); // b = sqrt(a^2 - N)
	
	while (mpf_integer_p(b) == 0) { // While b is not an integer.
		mpf_add_ui(a, a, 1);
		mpf_mul(a2, a, a); mpf_sub(a2, a2, N); // Set a2 to a^2 - N.
		mpf_sqrt(b, a2); // b = sqrt(a^2 - N)
	}
	mpf_add(a, a, b);
	gmp_printf("a: %Ff\n", a);
	mpf_sub(a, a, b); mpf_sub(a, a, b);
	gmp_printf("b: %Ff\n", a);
}

void isqrt(mpq_t N) {
	// Function that finds the square root of an arbitrary length mpq_t type.

}

int main(int argc, char *argv[])
{
	// Convert argv to uint and assign it to range.
	// Pointer throwaway_ptr points to chars in argv which is irrelevant for our inteded purposes.
	//char *throwaway_ptr;
	//mp_bitcnt_t range = strtol(argv[1], &throwaway_ptr, 10);
	mpf_t N;
	mpf_init(N);
	mpf_set_str(N, "1125899906842321", 10);
	fermat_factorization(N);
return 0;
}
