#include <stdio.h>
#include <stdlib.h> // Needed for strtol and malloc.
#include <gmp.h>
#include "include/ifermat.h"
#include "include/irandom.h"

void isqrt(mpq_t N) {
	// Function that finds the square root of an arbitrary length mpq_t type.

}

int main(int argc, char *argv[])
{
	// Convert argv to uint and assign it to range.
	// Pointer throwaway_ptr points to strings in argv which is irrelevant for our inteded purposes.
	char *throwaway_ptr;
	long double N_int = strtol(argv[1], &throwaway_ptr, 10);
	mpf_set_default_prec(4096);
	mpf_t N, p, q;
	mpf_inits(N, p, q, NULL);
	mpz_t k;
	mpz_init_set_str(k, "18446744073709551617", 10);
	mpf_set_z(N, k);
	gmp_fermat_factorization(N, p, q);
	gmp_printf("%Ff\n%Ff", p, q);
return 0;
}
