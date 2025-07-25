#ifndef IO_HH
#define IO_HH

#include <assert.h>

namespace io {

template <typename T>
[[gnu::always_inline]] [[deprecated(
	"Use io::port_io<u16 Port, T>::read() instead")]]
inline T in(u16 port) {
	T value;
	if constexpr (sizeof(T) == 1) {
		asm volatile("inb %w1, %b0" : "=a"(value) : "d"(port));
	} else if constexpr (sizeof(T) == 2) {
		asm volatile("inw %w1, %w0" : "=a"(value) : "d"(port));
	} else if constexpr (sizeof(T) == 4) {
		asm volatile("inl %w1, %l0" : "=a"(value) : "d"(port));
	} else {
		static_assert(sizeof(T) != 1 && sizeof(T) != 2 && sizeof(T) != 4,
					  "Invalid size passed to `T io::in<T>(u16 port)");
	}
	return value;
}

template <typename T>
[[gnu::always_inline]] [[deprecated(
	"Use io::port_io<u16 Port, T>::write() instead")]]
inline void out(u16 port, T value) {
	if constexpr (sizeof(T) == 1) {
		asm volatile("outb %b0, %w1" : : "a"(value), "d"(port));
	} else if constexpr (sizeof(T) == 2) {
		asm volatile("outw %w0, %w1" : : "a"(value), "d"(port));
	} else if constexpr (sizeof(T) == 4) {
		asm volatile("outl %l0, %w1" : : "a"(value), "d"(port));
	} else {
		static_assert(sizeof(T) != 1 && sizeof(T) != 2 && sizeof(T) != 4,
					  "Invalid size passed to `io::out<T>(u16 port, T value)");
	}
}

template <u16 Port, typename T> struct port_io {
	static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4,
				  "io::port_io<u16 Port, T> constructed with invalid size");

	[[gnu::always_inline]]
	static inline T read() {
		IO_DIAG_PUSH
		IO_DIAG_IGNORE_DEPR
		return in<T>(Port);
		IO_DIAG_POP
	}

	[[gnu::always_inline]]
	static inline void write(T value) {
		IO_DIAG_PUSH
		IO_DIAG_IGNORE_DEPR
		out<T>(Port, value);
		IO_DIAG_POP
	}
};

} // namespace io

#endif // IO_HH
