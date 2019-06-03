
#include "Stdafx.h"
#include "usbctl.h"


int main(int argc, char *argv[])
{
	if (argc >= 2) {
		char *op = argv[1];
		if (*op == 'i')
			uc_initialize();
		else if (*op == 'a')
			uc_authenticate();
	}
	return 0;
}