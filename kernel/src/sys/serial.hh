#ifndef SERIAL_HH
#define SERIAL_HH

#include <arch/io.hh>

namespace serial {

template <typename T, u16 Offset> struct Reg {
	u16 base;
	constexpr Reg(u16 base) : base(base) {}
	inline void write(T value) const { io::out<T>(base + Offset, value); }
	inline T read() const { return io::in<T>(base + Offset); }
};

struct Device {
	const u16 base;

	explicit constexpr Device(u16 base)
		: base(base), THR(base), RBR(base), DLL(base), DLM(base), IER(base),
		  IIR(base), FCR(base), LCR(base), MCR(base), LSR(base), MSR(base),
		  SCR(base) {}

	const Reg<u8, 0x0> THR;
	const Reg<u8, 0x0> RBR;
	const Reg<u8, 0x0> DLL;
	const Reg<u8, 0x1> DLM;
	const Reg<u8, 0x1> IER;
	const Reg<u8, 0x2> IIR;
	const Reg<u8, 0x2> FCR;
	const Reg<u8, 0x3> LCR;
	const Reg<u8, 0x4> MCR;
	const Reg<u8, 0x5> LSR;
	const Reg<u8, 0x6> MSR;
	const Reg<u8, 0x7> SCR;
};

inline void init(Device& port) {
	port.IER.write(0x00);
	port.LCR.write(0x80);
	port.DLL.write(0x03);
	port.DLM.write(0x00);
	port.LCR.write(0x03);
	port.FCR.write(0xC7);
	port.MCR.write(0x0B);
}

inline bool is_transmit_ready(const Device& port) {
	return port.LSR.read() & 0x20;
}

inline void write_byte(u8 byte, Device& port) {
	while (!is_transmit_ready(port)) {
	}
	port.THR.write(byte);
}

inline bool is_received(const Device& port) { return port.LSR.read() & 0x01; }

inline u8 read_byte(Device& port) {
	while (!is_received(port)) {
	}
	return port.RBR.read();
}

inline void write(const u8* data, usize len, Device& port) {
	for (usize i = 0; i < len; ++i) {
		write_byte(data[i], port);
	}
}

inline void read(u8* buffer, usize len, Device& port) {
	for (usize i = 0; i < len; ++i) {
		buffer[i] = read_byte(port);
	}
}

} // namespace serial

#endif // SERIAL_HH
