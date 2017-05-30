/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#ifndef ZEBU_DATA_H_
#define ZEBU_DATA_H_

#include <assert.h>

/**
 * Data
 * ----
 *
 * A tagged union that can represent a number of types, and is used as payload
 * by nodes.
 *
 * Type may be any of the following:
 *
 *    +--------------------+
 *    | ``ZZ_NULL``        |
 *    +--------------------+
 *    | ``ZZ_INT``         |
 *    +--------------------+
 *    | ``ZZ_UINT``        |
 *    +--------------------+
 *    | ``ZZ_DOUBLE``      |
 *    +--------------------+
 *    | ``ZZ_STRING``      |
 *    +--------------------+
 *    | ``ZZ_POINTER``     |
 *    +--------------------+
 */

/**
 * A field to indicate type and another to hold the data
 *
 */
struct zz_data {
	enum {
		ZZ_NULL,
		ZZ_INT,
		ZZ_UINT,
		ZZ_DOUBLE,
		ZZ_STRING,
		ZZ_POINTER
	} type;
	union {
		int int_val;
		unsigned int uint_val;
		double double_val;
		const char *string_val;
		void *pointer_val;
	} data;
};

/**
 * Null data
 *
 */
extern const struct zz_data zz_null;
/**
 * Create data from a given value
 */
static inline struct zz_data zz_int(int data)
{
	return (struct zz_data){ ZZ_INT, { .int_val = data }};
}
static inline struct zz_data zz_uint(unsigned int data)
{
	return (struct zz_data){ ZZ_UINT, { .uint_val = data }};
}
static inline struct zz_data zz_double(double data)
{
	return (struct zz_data){ ZZ_DOUBLE, { .double_val = data }};
}
struct zz_data zz_string(const char *data);
static inline struct zz_data zz_pointer(void *data)
{
	return (struct zz_data){ ZZ_POINTER, { .pointer_val = data }};
}
/**
 * Destroy data
 */
void zz_data_destroy(struct zz_data x);
/**
 * Copy data
 */
struct zz_data zz_data_copy(struct zz_data x);
/**
 * Cast data to type
 */
static inline int zz_to_int(struct zz_data x)
{
	assert(x.type == ZZ_INT);
	return x.data.int_val;
}
static inline unsigned int zz_to_uint(struct zz_data x)
{
	assert(x.type == ZZ_UINT);
	return x.data.uint_val;
}
static inline double zz_to_double(struct zz_data x)
{
	assert(x.type == ZZ_DOUBLE);
	return x.data.double_val;
}
static inline const char *zz_to_string(struct zz_data x)
{
	assert(x.type == ZZ_STRING);
	return x.data.string_val;
}
static inline void *zz_to_pointer(struct zz_data x)
{
	assert(x.type == ZZ_POINTER);
	return x.data.pointer_val;
}

#endif       // ZEBU_DATA_H_
