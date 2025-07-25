#ifndef IO_HH
#define IO_HH

namespace io {
template <typename T>
[[gnu::always_inline]]
inline T in(u16 port) {
	T value;
	if constexpr (sizeof(T) == 1) {
		asm volatile("inb %w1, %b0" : "=a"(value) : "d"(port) : "memory");
	} else if constexpr (sizeof(T) == 2) {
		asm volatile("inw %w1, %w0" : "=a"(value) : "d"(port) : "memory");
	} else if constexpr (sizeof(T) == 4) {
		asm volatile("inl %w1, %l0" : "=a"(value) : "d"(port) : "memory");
	} else {
		static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4,
					  "Invalid size for io::in<T>: must be 1, 2, or 4 bytes");
	}
	return value;
}

template <typename T>
[[gnu::always_inline]]
inline void out(u16 port, T value) {
	if constexpr (sizeof(T) == 1) {
		asm volatile("outb %b0, %w1" : : "a"(value), "d"(port) : "memory");
	} else if constexpr (sizeof(T) == 2) {
		asm volatile("outw %w0, %w1" : : "a"(value), "d"(port) : "memory");
	} else if constexpr (sizeof(T) == 4) {
		asm volatile("outl %l0, %w1" : : "a"(value), "d"(port) : "memory");
	} else {
		static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4,
					  "Invalid size for io::out<T>: must be 1, 2, or 4 bytes");
	}
}

} // namespace io

#endif // IO_HH