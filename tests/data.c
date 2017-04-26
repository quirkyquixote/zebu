
#include <string.h>

#include "../src/zebu.h"

int main(int argc, char *argv[])
{
	struct zz_tree t;
	struct zz_data d;

	zz_tree_init(&t, sizeof(struct zz_node));

	d = zz_null;
	assert(d.type == ZZ_NULL);

	d = zz_int(-42);
	assert(zz_to_int(d) == -42);

	d = zz_uint(42);
	assert(zz_to_uint(d) == 42);

	d = zz_double(42);
	assert(zz_to_double(d) == 42);

	d = zz_string(&t, "forty-two");
	assert(strcmp(zz_to_string(d), "forty-two") == 0);

	d = zz_pointer(&t);
	assert(zz_to_pointer(d) == &t);

	zz_tree_destroy(&t);
	exit(EXIT_SUCCESS);
}
