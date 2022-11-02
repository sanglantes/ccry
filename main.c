#include <stdio.h>
#include <assert.h>
#include <stdlib.h> 
#include <unistd.h>
#include <gmp.h>
#include "rsa_control_panel.h"
int main();
void rsa_interface() {
	system("clear");
	int option;
	printf("Select an option (0 to go back):\n(1) Generate RSA profile\n(2) Sign message\n(3) Verify message\n(4) Add key to keyring\n(5) Encrypt message\n\n>> ");
	scanf("%2d", &option);

	switch (option) {
		case 0: main();
			break;
		case 1:	generate_rsa_profile();
		       break;
		case 2: sign_rsa_message();
			break;
		case 3: verify_rsa_message();
			break;
		case 4: add_to_keyring();
			break;
		case 5: encrypt_message();
			break;	
		default: printf("Invalid option '%d'.\n", option);
			 assert(option >= 0 && option <= 5);
	}

}
void cryptanalysis_interface() {}

int main()
{
	system("clear");
	puts("* * * * * * * * * * * * * * * * * * * * *");
	puts("* ccry - The NIX cryptographic toolbox  *");
	puts("* * * * * * * * * * * * * * * * * * * * *");
	puts("\n");
	int option;
	printf("Select an option (0 to exit):\n(1) RSA interface\n(2) Cryptanalytic tools\n\n>> ");
	scanf("%2d", &option);
	
	switch (option)
	{
		case 0: return 0;

		case 1: rsa_interface();
			break;

		case 2: cryptanalysis_interface();
			break;

		default: printf("Invalid option '%d'. Press CTRL+C to return to menu.\n", option);
			 main();
	}
return 0;
}
