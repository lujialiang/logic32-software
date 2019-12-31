#ifndef __PACK_TYPES_H__
#define __PACK_TYPES_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
	uint8_t buff[4];
	int32_t value;
} int32_buffer_t;

typedef union {
	uint8_t buff[4];
	uint32_t value;
} uint32_buffer_t;

typedef union {
	uint8_t buff[2];
	uint16_t value;
} uint16_buffer_t;

typedef union {
	uint8_t buff[2];
	int16_t value;
} int16_buffer_t;

typedef union {
	uint8_t buff[4];
	float value;
} float_buffer_t;

typedef union {
	uint8_t buff[8];
	double value;
} double_buffer_t;

#ifdef __cplusplus
}
#endif

#endif

