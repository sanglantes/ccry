#include <stdio.h>
#include <stdlib.h>
#include "rsa_suite.h"

int main() {
	mpz_t sign;
	mpz_init(sign);
	rsa_sign_msg("Hello", "private_key", sign);
	printf("%d\n", verify_signature("Hello", "public_key", NULL, sign));
	return 0;
}
