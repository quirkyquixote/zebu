
#include <assert.h>
#include <string.h>

#include "../src/dict.h"

void empty_dict(void)
{
	struct zz_dict *dict;

	dict = NULL;
	dict = zz_dict_delete(dict, "Hello, World!");
	zz_dict_destroy(dict);
}

void insert_vals(void)
{
	struct zz_dict *dict;
	static const char c1[] = "This is a string";
	static const char c2[] = "Insert this in the dict";
	static const char c3[] = "I'm made of characters";
	static const char c4[] = "I'm a sequence";
	const char *s1, *s2, *s3, *s4, *s;

	dict = NULL;
	dict = zz_dict_insert(dict, c1, &s1);
	dict = zz_dict_insert(dict, c2, &s2);
	dict = zz_dict_insert(dict, c3, &s3);
	dict = zz_dict_insert(dict, c4, &s4);

	assert(c1 != s1);
	assert(strcmp(c1, s1) == 0);
	assert(zz_dict_lookup(dict, c1, &s) == 1);
	assert(s == s1);
	assert(c2 != s2);
	assert(strcmp(c2, s2) == 0);
	assert(zz_dict_lookup(dict, c2, &s) == 1);
	assert(s == s2);
	assert(c3 != s3);
	assert(strcmp(c3, s3) == 0);
	assert(zz_dict_lookup(dict, c3, &s) == 1);
	assert(s == s3);
	assert(c4 != s4);
	assert(strcmp(c4, s4) == 0);
	assert(zz_dict_lookup(dict, c4, &s) == 1);
	assert(s == s4);
	zz_dict_destroy(dict);
}

void delete_vals(void)
{
	struct zz_dict *dict;
	static const char c1[] = "This is a string";
	static const char c2[] = "Insert this in the dict";
	static const char c3[] = "I'm made of characters";
	static const char c4[] = "I'm a sequence";
	const char *s1, *s2, *s3, *s4, *s;

	dict = NULL;
	dict = zz_dict_insert(dict, c1, &s1);
	dict = zz_dict_insert(dict, c2, &s2);
	dict = zz_dict_insert(dict, c3, &s3);
	dict = zz_dict_insert(dict, c4, &s4);

	dict = zz_dict_delete(dict, c1);
	assert(zz_dict_lookup(dict, c1, &s1) == 0);
	assert(zz_dict_lookup(dict, c2, &s2) == 1);
	assert(zz_dict_lookup(dict, c3, &s3) == 1);
	assert(zz_dict_lookup(dict, c4, &s4) == 1);
	assert(strcmp(c2, s2) == 0);
	assert(strcmp(c3, s3) == 0);
	assert(strcmp(c4, s4) == 0);

	dict = zz_dict_delete(dict, c2);
	assert(zz_dict_lookup(dict, c1, &s1) == 0);
	assert(zz_dict_lookup(dict, c2, &s2) == 0);
	assert(zz_dict_lookup(dict, c3, &s3) == 1);
	assert(zz_dict_lookup(dict, c4, &s4) == 1);
	assert(strcmp(c3, s3) == 0);
	assert(strcmp(c4, s4) == 0);

	dict = zz_dict_delete(dict, c3);
	assert(zz_dict_lookup(dict, c1, &s1) == 0);
	assert(zz_dict_lookup(dict, c2, &s2) == 0);
	assert(zz_dict_lookup(dict, c3, &s3) == 0);
	assert(zz_dict_lookup(dict, c4, &s4) == 1);
	assert(strcmp(c4, s4) == 0);

	dict = zz_dict_delete(dict, c4);
	assert(zz_dict_lookup(dict, c1, &s1) == 0);
	assert(zz_dict_lookup(dict, c2, &s2) == 0);
	assert(zz_dict_lookup(dict, c3, &s3) == 0);
	assert(zz_dict_lookup(dict, c4, &s4) == 0);

	zz_dict_destroy(dict);
}

void insert_twice(void)
{
	struct zz_dict *dict;
	static const char str[] = "This is a string";
	const char *s1, *s2;

	dict = NULL;
	dict = zz_dict_insert(dict, str, &s1);
	dict = zz_dict_insert(dict, str, &s2);

	assert(str != s1);
	assert(strcmp(str, s1) == 0);
	assert(s1 == s2);
	assert(zz_dict_lookup(dict, str, &s2) == 1);
	assert(s1 == s2);

	dict = zz_dict_delete(dict, str);
	assert(zz_dict_lookup(dict, str, &s2) == 1);
	assert(s1 == s2);

	dict = zz_dict_delete(dict, str);
	assert(zz_dict_lookup(dict, str, &s2) == 0);

	zz_dict_destroy(dict);
}

int main(int argc, char *argv[])
{
	empty_dict();
	insert_vals();
	delete_vals();
	insert_twice();
	exit(EXIT_SUCCESS);
}
