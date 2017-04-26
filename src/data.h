
#ifndef ZEBU_DATA_H_
#define ZEBU_DATA_H_

#include <assert.h>

/*
 * Tagged union that can hold values of different types
 */
struct zz_data {
	/* Type of data */
	enum {
		ZZ_NULL,
		ZZ_INT,
		ZZ_UINT,
		ZZ_DOUBLE,
		ZZ_STRING,
		ZZ_POINTER
	} type;
	/* Payload */
	union {
		int int_val;
		unsigned int uint_val;
		double double_val;
		const char *string_val;
		void *pointer_val;
	} data;
};

/*
 * Reset to null
 */
extern const struct zz_data zz_null;
/*
 * Reset to int
 *
 * @data new data for node
 */
static inline struct zz_data zz_int(int data)
{
	return (struct zz_data){ ZZ_INT, { .int_val = data }};
}
/*
 * Reset to uint
 *
 * @data new data for node
 */
static inline struct zz_data zz_uint(unsigned int data)
{
	return (struct zz_data){ ZZ_UINT, { .uint_val = data }};
}
/*
 * Reset to double
 *
 * @data new data for node
 */
static inline struct zz_data zz_double(double data)
{
	return (struct zz_data){ ZZ_DOUBLE, { .double_val = data }};
}
/*
 * Reset to pointer
 *
 * @data new data for node
 */
static inline struct zz_data zz_pointer(void *data)
{
	return (struct zz_data){ ZZ_POINTER, { .pointer_val = data }};
}
/*
 * Get int
 *
 * @x a zz_data
 * @return int held by x
 */
static inline int zz_to_int(struct zz_data x)
{
	assert(x.type == ZZ_INT);
	return x.data.int_val;
}
/*
 * Get unsigned int
 *
 * @x a zz_data
 * @return data held by x
 */
static inline unsigned int zz_to_uint(struct zz_data x)
{
	assert(x.type == ZZ_UINT);
	return x.data.uint_val;
}
/*
 * Get double
 *
 * @x a zz_data
 * @return data held by x
 */
static inline double zz_to_double(struct zz_data x)
{
	assert(x.type == ZZ_DOUBLE);
	return x.data.double_val;
}
/*
 * Get string
 *
 * @x a zz_data
 * @return data held by x
 */
static inline const char *zz_to_string(struct zz_data x)
{
	assert(x.type == ZZ_STRING);
	return x.data.string_val;
}
/*
 * Get pointer
 *
 * @x a zz_data
 * @return data held by x
 */
static inline void *zz_to_pointer(struct zz_data x)
{
	assert(x.type == ZZ_POINTER);
	return x.data.pointer_val;
}

#endif       // ZEBU_DATA_H_
