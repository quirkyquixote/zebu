/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

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

	zz_foreach_child(iter, node) {
		fprintf(f, " ");
		zz_print(iter, f);
	}

	fprintf(f, "]");
}

void zz_error(const char *msg, const char *file, size_t first_line,
		size_t first_column, size_t last_line, size_t last_column)
{
	if (file == NULL) {
		fprintf(stderr, "<file>:%zu: %s\n", first_line, msg);
		return;
	}
	fprintf(stderr, "%s:%zu: %s", file, first_line, msg);
	FILE *f = fopen(file, "r");
	if (f == NULL)
		return;
	fseek(f, 0, SEEK_SET);
	for (int i = 1; i < first_line; ++i) {
                for (;;) {
                        int c = fgetc(f);
                        if (c == '\n')
                                break;
                        if (c == EOF)
                                break;
                }
	}
        char *buf = NULL;
        int size = 0;
        int alloc = 0;
	fputc('\n', stderr);
        for (int i = first_line; i <= last_line; ++i) {
                size = 0;
                for (int j = 1;; ++j) {
                        int c = fgetc(f);
                        if (c == EOF)
                                c = '\n';
                        fputc(c, stderr);
                        if (size == alloc) {
                                alloc = alloc ? alloc * 2 : 2;
                                buf = realloc(buf, alloc);
                        }
                        if (c == '\t' || c == '\n' || c == ' ')
                                buf[size++] = c;
                        else if ((i == first_line && j < first_column) ||
                                        (i == last_line && j > last_column))
                                buf[size++] = ' ';
                        else
                                buf[size++] = '^';
                        if (c == '\n')
                                break;
                }
                buf[size] = 0;
                fputs(buf, stderr);
        }
        free(buf);
	fclose(f);
}
