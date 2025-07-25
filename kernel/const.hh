#ifndef CONST_H
#define CONST_H

#include <stdint.h>

#define unreachable() __builtin_unreachable() // TODO assert
#define counted_by(counter) [[gnu::counted_by(counter)]]

#if defined(__GNUC__)
#define IO_DIAG_PUSH _Pragma("GCC diagnostic push")
#define IO_DIAG_IGNORE_DEPR                                                    \
	_Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#define IO_DIAG_POP _Pragma("GCC diagnostic pop")
#else
#define IO_DIAG_PUSH
#define IO_DIAG_IGNORE_DEPR
#define IO_DIAG_POP
#endif

namespace common {
template <typename... Ts> constexpr void unused(Ts&&...) noexcept {}

template <typename T> constexpr const T& min(const T& a, const T& b) {
	return (b < a) ? b : a;
}
} // namespace common

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;

#if defined(__SIZEOF_INT128__)
using u128 = unsigned __int128;
using i128 = signed __int128;
#endif

using f32 = float;
using f64 = double;

using usize = decltype(sizeof(void*));
using isize = i64;

static_assert(sizeof(u8) == 1, "u8 must be 1 byte");
static_assert(sizeof(u16) == 2, "u16 must be 2 bytes");
static_assert(sizeof(u32) == 4, "u32 must be 4 bytes");
static_assert(sizeof(u64) == 8, "u64 must be 8 bytes");
#if defined(__SIZEOF_INT128__)
static_assert(sizeof(u128) == 16, "u128 must be 16 bytes");
#endif
static_assert(sizeof(usize) >= sizeof(void*), "usize must hold a pointer");
static_assert(sizeof(isize) >= sizeof(void*), "isize must hold a pointer");

#endif // CONST_H
