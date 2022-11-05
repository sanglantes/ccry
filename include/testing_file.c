#include <stdio.h>
#include <stdlib.h>
#include "rsa_suite.h"

int main() {
	mpz_t sign;
	mpz_init(sign);
	rsa_profile(161);
	rsa_add_profile("public_key", "Jon"); 
	return 0;
}
