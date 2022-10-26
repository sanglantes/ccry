#include <gmp.h>
#include <stdio.h>

int main() {
	mpz_t rop, range, hk;
	mpz_inits(rop, range, hk, NULL);
	mpz_set_str(range, "50000", 10);
	mpz_nextprime(rop, range);
	mpz_set(hk, rop);
	mpz_nextprime(hk, rop);
	gmp_printf("%Zd\n %Zd\n", rop, hk);
return 0;
}
