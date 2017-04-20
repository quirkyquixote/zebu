

#include "dict.h"

#include <string.h>
#include <unistd.h>

static struct zz_dict *zz_dict_skew(struct zz_dict *t)
{
	struct zz_dict *l;

	if (t == NULL) {
		return NULL;
	} else if (t->left == NULL) {
		return t;
	} else if (t->left->level == t->level) {
		l = t->left;
		t->left = l->right;
		l->right = t;
		return l;
	} else {
		return t;
	}
}

static struct zz_dict *zz_dict_split(struct zz_dict *t)
{
	struct zz_dict *r;

	if (t == NULL) {
		return NULL;
	} else if (t->right == NULL || t->right->right == NULL) {
		return t;
	} else if (t->level == t->right->right->level) {
		r = t->right;
		t->right = r->left;
		r->left = t;
		++r->level;
		return r;
	} else {
		return t;
	}
}

struct zz_dict *zz_dict_insert(struct zz_dict *t, const char *data,
		const char **rval)
{
	int cmp;

	if (t == NULL) {
		t = calloc(1, sizeof(*t) + strlen(data) + 1);
		t->level = 1;
		strcpy(t->data, data);
		if (rval != NULL)
			*rval = t->data;
		return t;
	}
	cmp = strcmp(data, t->data);
	if (cmp < 0) {
		t->left = zz_dict_insert(t->left, data, rval);
	} else if (cmp > 0) {
		t->right = zz_dict_insert(t->right, data, rval);
	} else {
		if (rval != NULL)
			*rval = t->data;
	}
	t = zz_dict_skew(t);
	t = zz_dict_split(t);
	return t;
}

void zz_dict_destroy(struct zz_dict *t)
{
	if (t != NULL) {
		zz_dict_destroy(t->left);
		zz_dict_destroy(t->right);
		free(t);
	}
}

