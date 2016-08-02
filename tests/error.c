/*
 * Error reporting test
 */

#if 0

int main(int argc, char *argv[])
{
	prontf("Hello, world!\n");
}

#endif


#include "../zebu.h"

int main(int argc, char *argv[])
{
	zz_error("prontf is not a function", "error.c", 9, 2, 9, 7);
	exit(EXIT_SUCCESS);
}
