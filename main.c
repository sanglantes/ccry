#include <stdio.h>
#include <stdint.h>
#include <gmp.h>


//typedef struct RSAPROFILE
//{
//	char q, p, public_exp, phi, mod_inv [];
//} rsa_p;
//
//mpz_t* extended_gcd(mpz_t public_exp, mpz_t phi)
//{
//	mpz_t inverse;
//	mpz_set_ui(inverse, 1077);
//	mpz_t* inv = &inverse;
//	return inv;	
//}
//
//char rsa_gen(uint16_t bit_size, char* mr_flag) // blueprint for generating RSA key pairs.
//{
//	rsa_p rsaprofile;
//
//	rsaprofile.p = pgen(bit_size);
//	rsaprofile.q = pgen(bit_size);
//	rsaprofile.public_exp = "65537";
//	if (mr_flag != NULL) {
//		rsaprofile.p = miller_rabin(rsaprofile.p);
//		rsaprofile.q = miller_rabin(rsaprofile.q);
//	}
//
//	rsaprofile.phi = phi_func(rsaprofile.p, rsaprofile.q);
//	rsaprofile.mod_inv =  extended_gcd(rsaprofile.public_exp, rsaprofile.p);
//}

int main(int argc, int argv)
{
	mpz_t b, exp, n, rop;
//	mpz_inits(b, exp, n, NULL); init(s) is never necessary as GMP handles memory allocation. https://gmplib.org/manual/Initializing-Integers
	gmp_scanf("%Zd %Zd %Zd", &b, &exp, &n);
	
	mpz_powm(rop, b, exp, n);
	gmp_printf("%Zd\n", rop);
	return 0;
}
