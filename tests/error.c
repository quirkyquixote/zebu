/*
 * Error reporting test
 */

#if 0

int main(int argc, char *argv[])
{
        prontf("Hello, world!\n");

        (overly_long_function_name_to_ensure_line_break() +
         yet_another_overly_long_function_name()) = foo();
}

#endif


#include "../src/zebu.h"

int main(int argc, char *argv[])
{
	zz_error("prontf is not a function", "error.c", 9, 9, 9, 14);
	zz_error("expected l-value", "error.c", 11, 9, 12, 49);
	zz_error("error past end of file", "error.c", 27, 1, 27, 1);
	exit(EXIT_SUCCESS);
}
