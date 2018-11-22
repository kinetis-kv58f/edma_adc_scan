// © 2011. Ravi Teja Gudapati. All rights reserved.

#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

#include <stdint.h>

typedef uint8_t bool_t;

#define FALSE 0U

#define TRUE 1U

typedef unsigned int uint_t;

typedef int int_t;

#ifndef NULL
#define NULL ((void *) 0)
#endif

/// Returns lower nibble from a byte
/// \param byte Input byte
/// \returns Lower nibble
static inline uint8_t uint8_get_nibble0(uint8_t const byte);
static inline uint8_t uint8_get_nibble0(uint8_t const byte) {
	return byte & 0x0F;
}

/// Returns higher nibble from a byte
/// \param byte Input byte
/// \returns Higher nibble
static inline uint8_t uint8_get_nibble1(uint8_t const byte);
static inline uint8_t uint8_get_nibble1(uint8_t const byte) {
	return (byte >> 4) & 0x0F;
}

/// Returns higher nibble from a byte
/// \param byte Input byte
/// \returns Higher nibble
static inline uint8_t uint8_make_from_nibbles(uint8_t const hiNib, uint8_t const loNib);
static inline uint8_t uint8_make_from_nibbles(uint8_t const hiNib, uint8_t const loNib) {
	return (uint8_t)((loNib) | (hiNib << 4));
}

/// Returns halfword with bytes swapped
/// \param bytes Input halfword
/// \returns Halfword with bytes swapped
static inline uint16_t uint16_swap_bytes(uint16_t const bytes);
static inline uint16_t uint16_swap_bytes(uint16_t const bytes) {
	return (uint16_t)(((bytes >> 8U) & 0xFFU) | (uint16_t) ((bytes & 0xFFU) << 8U));
}

/// Returns lower byte of the halfword
/// \param data Input halfword
/// \returns Lower byte
static inline uint8_t uint16_get_hi_byte(uint16_t const data);
static inline uint8_t uint16_get_hi_byte(uint16_t const data) {
	return (uint8_t)(data >> 8U);
}

/// Returns higher byte of the halfword
/// \param data Input halfword
/// \returns Higher byte
static inline uint8_t uint16_get_lo_byte(uint16_t const data);
static inline uint8_t uint16_get_lo_byte(uint16_t const data) {
	return (uint8_t)(data & 0xFFU);
}

/// Returns byte0 of 64-bit data
/// \param data Input 64-bit data
/// \returns Byte0
static inline uint8_t uint64_get_byte0(uint64_t const data);
static inline uint8_t uint64_get_byte0(uint64_t const data) {
	return (uint8_t)(data & 0xFFU);
}

/// Returns byte1 of 64-bit data
/// \param data Input 64-bit data
/// \returns Byte1
static inline uint8_t uint64_get_byte1(uint64_t const data);
static inline uint8_t uint64_get_byte1(uint64_t const data) {
	return (uint8_t)((data >> 8U) & 0xFFU);
}

/// Returns byte1 of 64-bit data
/// \param data Input 64-bit data
/// \returns Byte1
static inline uint8_t uint64_get_byte2(uint64_t const data);
static inline uint8_t uint64_get_byte2(uint64_t const data) {
	return (uint8_t)((data >> 16U) & 0xFFU);
}

/// Returns byte3 of 64-bit data
/// \param data Input 64-bit data
/// \returns Byte3
static inline uint8_t uint64_get_byte3(uint64_t const data);
static inline uint8_t uint64_get_byte3(uint64_t const data) {
	return (uint8_t)((data >> 24U) & 0xFFU);
}

/// Returns byte4 of 64-bit data
/// \param data Input 64-bit data
/// \returns Byte4
static inline uint8_t uint64_get_byte4(uint64_t const data);
static inline uint8_t uint64_get_byte4(uint64_t const data) {
	return (uint8_t)((data >> 32U) & 0xFFU);
}

/// Returns byte5 of 64-bit data
/// \param data Input 64-bit data
/// \returns Byte5
static inline uint8_t uint64_get_byte5(uint64_t const data);
static inline uint8_t uint64_get_byte5(uint64_t const data) {
	return (uint8_t)((data >> 40U) & 0xFFU);
}

/// Returns byte6 of 64-bit data
/// \param data Input 64-bit data
/// \returns Byte5
static inline uint8_t uint64_get_byte6(uint64_t const data);
static inline uint8_t uint64_get_byte6(uint64_t const data) {
	return (uint8_t)((data >> 48U) & 0xFFU);
}

/// Returns byte7 of 64-bit data
/// \param data Input 64-bit data
/// \returns Byte5
static inline uint8_t uint64_get_byte7(uint64_t const data);
static inline uint8_t uint64_get_byte7(uint64_t const data) {
	return (uint8_t)((data >> 56U) & 0xFFU);
}

/// Returns Halfword0 of 64-bit data
/// \param data Input 64-bit data
/// \returns Halfword0
static inline uint16_t uint64_get_hword0(uint64_t const data);
static inline uint16_t uint64_get_hword0(uint64_t const data) {
	return (uint16_t)(data & 0xFFFFU);
}

/// Returns Halfword1 of 64-bit data
/// \param data Input 64-bit data
/// \returns Halfword1
static inline uint16_t uint64_get_hword1(uint64_t const data);
static inline uint16_t uint64_get_hword1(uint64_t const data) {
	return (uint16_t)((data >> 16U) & 0xFFFFU);
}

/// Returns Halfword2 of 64-bit data
/// \param data Input 64-bit data
/// \returns Halfword2
static inline uint16_t uint64_get_hword2(uint64_t const data);
static inline uint16_t uint64_get_hword2(uint64_t const data) {
	return (uint16_t)((data >> 32U) & 0xFFFFU);
}

/// Returns Halfword3 of 64-bit data
/// \param data Input 64-bit data
/// \returns Halfword3
static inline uint16_t uint64_get_hword3(uint64_t const data);
static inline uint16_t uint64_get_hword3(uint64_t const data) {
	return (uint16_t)((data >> 48U) & 0xFFFFU);
}

/// Returns Word 0 of 64-bit data
/// \param data Input 64-bit data
/// \returns Word 0
static inline uint32_t uint64_get_word0(uint64_t const data);
static inline uint32_t uint64_get_word0(uint64_t const data) {
	return (uint32_t)(data & 0xFFFFFFFFU);
}

/// Returns Word 1 of 64-bit data
/// \param data Input 64-bit data
/// \returns Word 1
static inline uint32_t uint64_get_word1(uint64_t const data);
static inline uint32_t uint64_get_word1(uint64_t const data) {
	return (uint32_t)((data >> 32U) & 0xFFFFFFFFU);
}

/// Returns Byte0 of 32-bit data
/// \param data Input 32-bit data
/// \returns Byte0
static inline uint8_t uint32_get_byte0(uint32_t const data);
static inline uint8_t uint32_get_byte0(uint32_t const data) {
	return (uint8_t)(data & 0xFFU);
}

/// Returns Byte1 of 32-bit data
/// \param data Input 32-bit data
/// \returns Byte1
static inline uint8_t uint32_get_byte1(uint32_t const data);
static inline uint8_t uint32_get_byte1(uint32_t const data) {
	return (uint8_t)((data >> 8U) & 0xFFU);
}

/// Returns Byte2 of 32-bit data
/// \param data Input 32-bit data
/// \returns Byte2
static inline uint8_t uint32_get_byte2(uint32_t const data);
static inline uint8_t uint32_get_byte2(uint32_t const data) {
	return (uint8_t)((data >> 16U) & 0xFFU);
}

/// Returns Byte3 of 32-bit data
/// \param data Input 32-bit data
/// \returns Byte3
static inline uint8_t uint32_get_byte3(uint32_t const data);
static inline uint8_t uint32_get_byte3(uint32_t const data) {
	return (uint8_t)((data >> 24U) & 0xFFU);
}

/// Returns Halfword0 of 32-bit data
/// \param data Input 32-bit data
/// \returns Halfword0
static inline uint16_t uint32_get_hword0(uint32_t const data);
static inline uint16_t uint32_get_hword0(uint32_t const data) {
	return (uint16_t)(data & 0xFFFFU);
}

/// Returns Halfword1 of 32-bit data
/// \param data Input 32-bit data
/// \returns Halfword1
static inline uint16_t uint32_get_hword1(uint32_t const data);
static inline uint16_t uint32_get_hword1(uint32_t const data) {
	return (uint16_t)((data >> 16U) & 0xFFFFU);
}

/// Returns 16-bit data from two bytes
/// \param hi Higher byte
/// \param lo Lower byte
/// \returns 16-bit data
static inline uint16_t uint16_make_from_bytes(uint8_t const hi, uint8_t const lo);
static inline uint16_t uint16_make_from_bytes(uint8_t const hi, uint8_t const lo) {
	uint16_t ret = hi;

	ret = (uint16_t)(ret << 8U);
	ret = (uint16_t)(ret | lo);

	return ret;
}

/// Returns 32-bit data from two halfwords
/// \param hi Higher halfword
/// \param lo Lower halfword
/// \returns 16-bit data
static inline uint32_t uint32_make_from_uin16(uint16_t const hi, uint16_t const lo);
static inline uint32_t uint32_make_from_uin16(uint16_t const hi, uint16_t const lo) {
	uint32_t ret = hi;
	ret <<= 16U;
	ret |= (uint16_t) lo;
	return ret;
}

/// Returns 64-bit data from 4 halfwords
/// \param hw0 Halfword0
/// \param hw1 Halfword1
/// \param hw2 Halfword2
/// \param hw3 Halfword3
/// \returns 64-bit data
static inline uint64_t uint64_make_from_uint16(uint16_t const hw0, uint16_t const hw1,
		uint16_t hw2, uint16_t hw3);
static inline uint64_t uint64_make_from_uint16(uint16_t const hw0, uint16_t const hw1,
		uint16_t hw2, uint16_t hw3) {
	uint64_t ret = hw3;
	ret <<= 16U;
	ret |= (uint16_t) hw2;
	ret <<= 16U;
	ret |= (uint16_t) hw1;
	ret <<= 16U;
	ret |= (uint16_t) hw0;

	return ret;
}

/// Returns 64-bit data from 2 words
/// \param w0 Word0
/// \param w1 Word1
/// \returns 64-bit data
static inline uint64_t uint64_make_from_uint32(uint32_t const w0, uint32_t const w1);
static inline uint64_t uint64_make_from_uint32(uint32_t const w0, uint32_t const w1) {
	uint64_t ret = w1;
	ret <<= 32U;
	ret |= (uint32_t) w0;

	return ret;
}

/// Returns absolute subtraction of two bytes
/// \param inp1 Input 1
/// \param inp2 Input2
/// \returns Absolute subtraction of two bytes
static inline uint16_t uint8_abs_subtract(uint8_t const inp1, uint8_t const inp2) {
	if(inp2 >= inp1) {
		return inp2 - inp1;
	} else {
		return inp1 - inp2;
	}
}

#endif /* SRC_TYPES_H_ */
