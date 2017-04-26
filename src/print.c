

#include "print.h"

void zz_print(struct zz_node *node, FILE * f)
{
	struct zz_node *iter;

	fprintf(f, "[%s", node->token);

	switch (node->data.type) {
	case ZZ_NULL:
		break;
	case ZZ_INT:
		fprintf(f, " %d", node->data.data.int_val);
		break;
	case ZZ_UINT:
		fprintf(f, " %u", node->data.data.uint_val);
		break;
	case ZZ_DOUBLE:
		fprintf(f, " %f", node->data.data.double_val);
		break;
	case ZZ_STRING:
		fprintf(f, " \"%s\"", node->data.data.string_val);
		break;
	case ZZ_POINTER:
		fprintf(f, " %p", node->data.data.pointer_val);
		break;
	}

	zz_list_foreach_entry(iter, &node->children, siblings) {
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
