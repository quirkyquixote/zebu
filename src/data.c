
#include "data.h"

#include "dict.h"

static struct zz_dict *strings = NULL;

const struct zz_data zz_null = { ZZ_NULL };

struct zz_data zz_string(const char *str)
{
	struct zz_data data = { ZZ_STRING };
	strings = zz_dict_insert(strings, str, &data.data.string_val);
	return data;
}

void zz_data_destroy(struct zz_data x)
{
	if (x.type == ZZ_STRING)
		strings = zz_dict_delete(strings, x.data.string_val);
}

struct zz_data zz_data_copy(struct zz_data x)
{
	struct zz_data y = x;
	if (x.type == ZZ_STRING)
		strings = zz_dict_insert(strings, x.data.string_val, &y.data.string_val);
	return y;
}
