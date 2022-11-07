#include <stdio.h>
#include <gmp.h>
#include "include/imath.h"

void gmp_ferm() {
	char comp[30000];
	printf("Enter composite (base 10):\n>> ");
	scanf("%s", comp);

	mpf_t mp_comp, p, q;
	mpf_inits(p, q, NULL);
	mpf_init_set_str(mp_comp, comp, 10);

	printf("Commencing factorization...\n");
	gmp_fermat_factorization(mp_comp, p, q);

	gmp_printf("%.Ff\n%.Ff\n", p, q);

	return;
}

void pollard_rho() {
	mpz_t N, a, b;
	char N_str[100000];
	mpz_inits(N, a, b, NULL);

	printf("Enter composite (base 10):\n>> ");
	scanf("%s", N_str);
	mpz_set_str(N, N_str, 10);
	
	printf("Commencing factorization...\n");
	pollard_rho_f(N, a, b);

	return;
}
