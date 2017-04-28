

#include "dict.h"

#include <string.h>
#include <unistd.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define SWAP(a, b)\
do {\
	typeof(a) tmp = a;\
	a = b;\
	b = tmp;\
} while (0)

static struct zz_dict *skew(struct zz_dict *t)
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

static struct zz_dict *split(struct zz_dict *t)
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

static struct zz_dict *sucessor(struct zz_dict *t)
{
	for (t = t->right; t->left; t = t->left)
		continue;
	return t;
}

static struct zz_dict *predecessor(struct zz_dict *t)
{
	for (t = t->left; t->right; t = t->right)
		continue;
	return t;
}

static struct zz_dict *decrease_level(struct zz_dict *t)
{
	int right_level = t->right ? t->right->level : 0;
	int left_level = t->left ? t->left->level : 0;
	int should_be = MIN(left_level, right_level) + 1;
	if (should_be < t->level) {
		t->level = should_be;
		if (should_be < right_level)
			t->right->level = should_be;
	}
	return t;
}

int zz_dict_lookup(struct zz_dict *t, const char *data, const char **rval)
{
	int cmp;

	while (t != NULL) {
		cmp = strcmp(data, t->data);
		if (cmp < 0) {
			t = t->left;
		} else if (cmp > 0) {
			t = t->right;
		} else {
			if (rval != NULL)
				*rval = t->data;
			return 1;
		}
	}
	return 0;
}

struct zz_dict *zz_dict_insert(struct zz_dict *t, const char *data,
		const char **rval)
{
	int cmp;

	if (t == NULL) {
		t = calloc(1, sizeof(*t));
		t->level = 1;
		t->ref_count = 1;
		t->data = strdup(data);
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
		++t->ref_count;
		if (rval != NULL)
			*rval = t->data;
	}
	t = skew(t);
	t = split(t);
	return t;
}

struct zz_dict *zz_dict_delete(struct zz_dict *t, const char *data)
{
	int cmp;

	if (t == NULL)
		return t;
	cmp = strcmp(data, t->data);
	if (cmp > 0) {
		t->right = zz_dict_delete(t->right, data);
	} else if (cmp < 0) {
		t->left = zz_dict_delete(t->left, data);
	} else {
		if (t->ref_count > 1) {
			--t->ref_count;
			return t;
		}
		if (t->left == NULL) {
			if (t->right == NULL) {
				free(t->data);
				free(t);
				return NULL;
			}
			struct zz_dict *l = sucessor(t);
			SWAP(t->data, l->data);
			SWAP(t->ref_count, l->ref_count);
			t->right = zz_dict_delete(t->right, l->data);
		} else {
			struct zz_dict *l = predecessor(t);
			SWAP(t->data, l->data);
			SWAP(t->ref_count, l->ref_count);
			t->left = zz_dict_delete(t->left, l->data);
		}
	}

	/* Rebalance the tree. Decrease the level of all nodes in this level if
	 * necessary, and then skew and split all nodes in the new level. */
	t = decrease_level(t);
	t = skew(t);
	t->right = skew(t->right);
	if (t->right != NULL)
		t->right->right = skew(t->right->right);
	t = split(t);
	t->right = split(t->right);
	return t;
}

void zz_dict_destroy(struct zz_dict *t)
{
	if (t != NULL) {
		zz_dict_destroy(t->left);
		zz_dict_destroy(t->right);
		free(t->data);
		free(t);
	}
}

