

#include "print.h"

void zz_print(struct zz_node *node, FILE * f)
{
	struct zz_node *iter;

	fprintf(f, "[%s", node->token);

	if (node->type == ZZ_INT)
		fprintf(f, " %d", node->data.int_val);
	else if (node->type == ZZ_UINT)
		fprintf(f, " %u", node->data.uint_val);
	else if (node->type == ZZ_DOUBLE)
		fprintf(f, " %f", node->data.double_val);
	else if (node->type == ZZ_STRING)
		fprintf(f, " \"%s\"", node->data.str_val);
	else if (node->type == ZZ_POINTER)
		fprintf(f, " %p", node->data.ptr_val);

	zz_foreach_child(iter, node) {
		fprintf(f, " ");
		zz_print(iter, f);
	}

	fprintf(f, "]");
}

void zz_error(const char *msg, const char *file, size_t first_line,
		size_t first_column, size_t last_line, size_t last_column)
{
	FILE *f;
	int i;
	int c;
	int r;

	if (file == NULL) {
		fprintf(stderr, "<file>:%zu: %s\n", first_line, msg);
		return;
	}
	fprintf(stderr, "%s:%zu: %s", file, first_line, msg);
	f = fopen(file, "r");
	if (f == NULL)
		return;
	fseek(f, 0, SEEK_SET);
	for (i = 1; i < first_line; ++i) {
		while (fgetc(f) != '\n')
			continue;
	}
	fputc('\n', stderr);
	r = ftell(f);
	for (i = 0; (c = fgetc(f)) != '\n'; ++i)
		fputc(c, stderr);
	if (last_line > first_line)
		last_column = i - 1;
	fputc('\n', stderr);
	fseek(f, r, SEEK_SET);
	for (i = 1; i < first_column; ++i)
		fputc(fgetc(f) == '\t' ? '\t' : ' ', stderr);
	for (; i <= last_column; ++i)
		fputc(fgetc(f) == '\t' ? '\t' : '^', stderr);
	fputc('\n', stderr);
	fclose(f);
}
