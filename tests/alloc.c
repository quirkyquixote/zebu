/*
 * Test for tree allocation
 */

#include <assert.h>
#include <string.h>

#include "../zebu.h"

static const char REALLY_LONG_STRING[] =
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam vel eros"
"tortor. Pellentesque ac iaculis tellus, at maximus felis. Phasellus ut eleifend"
"leo. Proin ultricies et mauris at lobortis. Sed aliquet elit quis lectus"
"ullamcorper viverra. Praesent finibus vulputate purus, at mattis leo mattis"
"nec. Fusce eu arcu justo."
""
"Sed a sem venenatis enim pharetra pharetra. Phasellus porttitor ex hendrerit"
"consequat pulvinar. Nunc et diam rutrum, mattis nulla at, porttitor lorem."
"Maecenas vitae velit id lectus iaculis ultricies in vitae eros. Duis finibus,"
"sem non semper aliquet, metus leo finibus ante, eget tincidunt nisi dui quis"
"nulla. Praesent viverra ligula diam. In nisl diam, efficitur nec dui eu,"
"accumsan scelerisque risus. Aliquam leo leo, tempus ut felis sit amet, congue"
"interdum magna. Sed convallis arcu sodales, gravida risus non, vehicula orci."
"Nulla vel urna dignissim, imperdiet purus ut, vestibulum est. Ut sit amet ex id"
"diam finibus hendrerit."
""
"Pellentesque vehicula orci vel lorem consequat malesuada. Proin ullamcorper"
"enim et velit rutrum, at auctor ex eleifend. Sed commodo tempor risus, id"
"lacinia nulla facilisis vel. Maecenas massa neque, elementum eget egestas sit"
"amet, dapibus at quam. Nullam sapien dui, lobortis quis ornare posuere,"
"convallis non nisi. Nulla facilisi. Mauris in diam feugiat, vulputate tellus"
"non, eleifend augue. Aliquam erat volutpat. Sed ligula metus, ullamcorper sed"
"ultrices at, aliquam sed neque. Praesent ultrices dui a accumsan varius. Nullam"
"feugiat arcu nec suscipit eleifend. Duis pretium semper dolor, vehicula"
"condimentum turpis porttitor nec. Nulla leo ante, egestas id cursus quis,"
"maximus ut sem. Sed luctus porta nunc, vel pharetra elit molestie at."
""
"Suspendisse potenti. Etiam hendrerit gravida orci, quis tempor dolor porta eu."
"Sed sed metus eget diam pellentesque tempor. Nulla facilisi. Mauris nibh nisl,"
"scelerisque sed mauris ac, consequat vulputate ipsum. Etiam fringilla sit amet"
"metus eu condimentum. Etiam at convallis diam. Phasellus sagittis sapien nec"
"orci tincidunt viverra sed non nunc. Nullam at viverra turpis. Ut leo arcu,"
"accumsan ut consequat sed, vehicula eu erat. Integer sit amet suscipit lorem."
"Nam egestas, tortor vitae mollis lobortis, metus urna aliquet sapien, dictum"
"rutrum dui metus imperdiet erat. Phasellus non justo ut justo mattis ultrices."
"Nulla dignissim sit amet tortor posuere egestas."
""
"Cras ullamcorper mi id tempor rhoncus. Nullam ac nulla in ligula iaculis"
"molestie. Aliquam sed ornare eros, et pharetra ligula. Sed tincidunt aliquam"
"bibendum. Phasellus iaculis massa vel eros rutrum dignissim. Cras at elementum"
"felis. Aliquam id aliquam tellus. Proin malesuada diam eu dapibus rutrum. Sed"
"luctus elit quam. Sed sem orci, pharetra vel placerat a, congue vel dolor."
"Quisque pretium massa non ullamcorper convallis. ";

static const char *TOK_FOO = "foo";
static const char *TOK_BAR = "bar";
static const char *TOK_BAZ = "baz";

/* Try allocating until surpassing the size of a blob */
int allocate_beyond_blob(void)
{
	struct zz_tree tree;
	size_t len;
	struct zz_node **nodes;
	size_t i;

	zz_tree_init(&tree, sizeof(struct zz_node));
	len = ZZ_BLOB_SIZE / sizeof(*nodes) + 10;
	nodes = calloc(len, sizeof(*nodes));
	for (i = 0; i < len; ++i)
		nodes[i] = zz_uint(&tree, TOK_BAZ, i);
	for (i = 0; i < len; ++i)
		assert(zz_to_uint(nodes[i]) == i);
	zz_tree_destroy(&tree);
	return 0;
}

/* Try allocating a string bigger than the size of a blob */
int allocate_huge_string(void)
{
	struct zz_tree tree;
	const char *str;
	size_t len;
	struct zz_node *n1, *n2;

	zz_tree_init(&tree, sizeof(struct zz_node));
	str = REALLY_LONG_STRING;
	len = strlen(REALLY_LONG_STRING);
	n1 = zz_string(&tree, TOK_FOO, str);
	n2 = zz_string(&tree, TOK_BAR, str);
	assert(n1 != n2);
	assert(zz_to_string(n1) != str);
	assert(zz_to_string(n2) != str);
	assert(strcmp(zz_to_string(n1), str) == 0);
	assert(strcmp(zz_to_string(n2), str) == 0);
	assert(strcmp(zz_to_string(n1), zz_to_string(n2)) == 0);
	zz_tree_destroy(&tree);
	return 0;
}

int main(int argc, char *argv[])
{
	allocate_beyond_blob();
	allocate_huge_string();
}

