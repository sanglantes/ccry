#include <stdio.h>
#include <stdlib.h>
#include "imath.h"

int main() {
	long double N, a, b;
	N = 303;
	fermat_factorization_f(N, a, b);
	printf("%Lf\n%Lf\n", a, b);
	return 0;
}
