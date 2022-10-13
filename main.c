#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lib/gmp/gmp.h"



int factorial(int n)
{
	int c = 1;
	for (int i = 1; i <= n; i++) 
	{
		c = c * i;
		printf("%d \t %d \n", c, c*i);
	}
	return c;
}


int main(int argc)
{
	factorial(5);
	char inputstr[4096];
	mpz_t gmp_num;
	int flag;
	scanf("%s", inputstr);
	
	mpz_init(gmp_num);
	mpz_set_ui(gmp_num, 0);
	flag = mpz_set_str(gmp_num, inputstr, 10);
	assert(flag == 0);
	mpz_add_ui(gmp_num, gmp_num, 3);
	mpz_out_str(stdout, 10, gmp_num);


	return 0;
}
