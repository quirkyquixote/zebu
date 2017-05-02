/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */
/**
 * @file
 * Declare zz_data struct and related functions
 */

#ifndef ZEBU_DATA_H_
#define ZEBU_DATA_H_

#include <assert.h>

/**
 * Tagged union that can hold values of different types
 *
 * @ingroup Zebu
 */
struct zz_data {
	/** Type of data */
	enum {
		ZZ_NULL,
		ZZ_INT,
		ZZ_UINT,
		ZZ_DOUBLE,
		ZZ_STRING,
		ZZ_POINTER
	} type;
	/** Payload */
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
 * @memberof zz_data
 */
extern const struct zz_data zz_null;
/**
 * Create int data
 *
 * @memberof zz_data
 * @param data new data
 * @return new zz_data
 */
static inline struct zz_data zz_int(int data)
{
	return (struct zz_data){ ZZ_INT, { .int_val = data }};
}
/**
 * Create unsigned int data
 *
 * @memberof zz_data
 * @param data new data
 * @return new zz_data
 */
static inline struct zz_data zz_uint(unsigned int data)
{
	return (struct zz_data){ ZZ_UINT, { .uint_val = data }};
}
/**
 * Create double data
 *
 * @memberof zz_data
 * @param data new data
 * @return new zz_data
 */
static inline struct zz_data zz_double(double data)
{
	return (struct zz_data){ ZZ_DOUBLE, { .double_val = data }};
}
/**
 * Create string data
 *
 * @memberof zz_data
 * @param data new data
 * @return new zz_data
 */
struct zz_data zz_string(const char *data);
/**
 * Create pointer data
 *
 * @memberof zz_data
 * @param data new data
 * @return new zz_data
 */
static inline struct zz_data zz_pointer(void *data)
{
	return (struct zz_data){ ZZ_POINTER, { .pointer_val = data }};
}
/**
 * Destroy data
 *
 * @memberof zz_data
 * @param x data to destroy
 */
void zz_data_destroy(struct zz_data x);
/**
 * Copy data
 *
 * @memberof zz_data
 * @param x data to copy
 * @return a copy of x
 */
struct zz_data zz_data_copy(struct zz_data x);
/**
 * Get int
 *
 * @memberof zz_data
 * @param x a zz_data
 * @return int held by x
 */
static inline int zz_to_int(struct zz_data x)
{
	assert(x.type == ZZ_INT);
	return x.data.int_val;
}
/**
 * Get unsigned int
 *
 * @memberof zz_data
 * @param x a zz_data
 * @return data held by x
 */
static inline unsigned int zz_to_uint(struct zz_data x)
{
	assert(x.type == ZZ_UINT);
	return x.data.uint_val;
}
/**
 * Get double
 *
 * @memberof zz_data
 * @param x a zz_data
 * @return data held by x
 */
static inline double zz_to_double(struct zz_data x)
{
	assert(x.type == ZZ_DOUBLE);
	return x.data.double_val;
}
/**
 * Get string
 *
 * @memberof zz_data
 * @param x a zz_data
 * @return data held by x
 */
static inline const char *zz_to_string(struct zz_data x)
{
	assert(x.type == ZZ_STRING);
	return x.data.string_val;
}
/**
 * Get pointer
 *
 * @memberof zz_data
 * @param x a zz_data
 * @return data held by x
 */
static inline void *zz_to_pointer(struct zz_data x)
{
	assert(x.type == ZZ_POINTER);
	return x.data.pointer_val;
}

#endif       // ZEBU_DATA_H_
