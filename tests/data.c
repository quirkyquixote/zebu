
#include <string.h>
#include <stdlib.h>

#include "../src/data.h"

int main(int argc, char *argv[])
{
	struct zz_data d;

	d = zz_null;
	assert(d.type == ZZ_NULL);

	d = zz_int(-42);
	assert(zz_to_int(d) == -42);

	d = zz_uint(42);
	assert(zz_to_uint(d) == 42);

	d = zz_double(42);
	assert(zz_to_double(d) == 42);

	d = zz_string("forty-two");
	assert(strcmp(zz_to_string(d), "forty-two") == 0);

	d = zz_pointer(&d);
	assert(zz_to_pointer(d) == &d);

	exit(EXIT_SUCCESS);
}
