#include <stdio.h>
#include <gmp.h>
#include <math.h>

void gmp_fermat_ltheorem(mpz_t p) {
	mpz_t rop;
	mpz_t a;
	mpz_t p_min;
	mpz_inits(a, rop, p_min, NULL);
	mpz_set_ui(a, 2);
	mpz_sub_ui(p_min, p, 1);

	if (mpz_odd_p(p) == 0) {
		mpz_add_ui(p, p, 1);
		gmp_fermat_ltheorem(p);
	}

	else {
		mpz_powm(rop, a, p_min, p);
		if (mpz_cmp_ui(rop, 1) == 0) {
			gmp_printf("%Zd is prime \n", p);
			return;
		}
		else {
			mpz_add_ui(p, p, 2);
			gmp_fermat_ltheorem(p);
		}
		
	}
}

void gmp_fermat_factorization(mpf_t N, mpf_t p, mpf_t q) { 
	// Fermat's factorization method is an algorithm for finding two factors of an integer.
	// This method is efficient when the two factors are close to each other, requiring only a few iterations to find them.
	// In a case where the factors are distant, this method is only slightly more efficient than trial division.

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
	mpf_t a2, a, b;
	mpf_inits(a2, a, b, NULL);
	mpf_sqrt(a, N); mpf_ceil(a, a); // Set a to square root of N. Apply ceiling function to a.
	mpf_mul(a2, a, a); mpf_sub(a2, a2, N); // Set a2 to a^2 - N.
	mpf_sqrt(b, a2); // b = sqrt(a^2 - N)

	while (mpf_integer_p(b) == 0) { // While b is not an integer.
		mpf_add_ui(a, a, 1);
		mpf_mul(a2, a, a); mpf_sub(a2, a2, N); // Set a2 to a^2 - N.
		mpf_sqrt(b, a2); // b = sqrt(a^2 - N)
	}
	// Set p and q to results.	
	mpf_add(p, a, b);
	mpf_sub(q, a, b); 
}

void fermat_factorization_f(long double N, long double p, long double q) {
	long double a, b, b2;
	a = sqrt(N); a = ceil(a);
	b2 = pow(a, 2); b2 -= N;
	b = sqrt(b2);
	
	while (floor(b) != ceil(b)) {
		a += 1;
		b2 = pow(a, 2); b2 -= N;
		b = sqrt(b2);
	}
	p = a + b;
	q = a - b;
}

void pollard_rho_f(mpz_t N, mpz_t a, mpz_t b) {
	mpz_t x, y, s, d;
	mpz_inits(x, y, d, s,  NULL);
	mpz_set_ui(x, 2); mpz_set_ui(y, 2); mpz_set_ui(d, 1);
	
	void g(mpz_t x, mpz_t n) {
		mpz_add_ui(x, x, 1);
		mpz_powm_ui(x, x, 2, n);
	}


	while (mpz_cmp_ui(d, 1) == 0 ) {
		// x = g(x)
		g(x, N);
		// y = g(g(y))
		g(y, N); g(y, N);
		// d = gcd(|x - y|, n)
		mpz_sub(s, x, y); mpz_abs(s, s);
		mpz_gcd(d, s, N);
	}

	if (mpz_cmp(d, N) == 0) {
		printf("Number is not suitable for factorization. Make sure N is odd.\n");	
		return;
	}
	mpz_set(a, x);
	mpz_div(b, N, x);
	gmp_printf("\n%Zd\n%Zd\n", a, b);
}

