#ifndef CONST_H
#define CONST_H

#include <stdint.h>
#include <stddef.h>

#define counted_by(counter) [[gnu::counted_by(counter)]]

#define unreachable() __builtin_unreachable()
#define unused(x) ((void)x)

typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
typedef signed __int128 i128;
#endif

typedef float f32;
typedef double f64;

typedef size_t usize;
typedef ptrdiff_t isize;

#endif // CONST_H
