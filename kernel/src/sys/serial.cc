#include <sys/serial.hh>

namespace serial {

void init(port p) {
	io::out<u8>(p + static_cast<u16>(reg_offset::ier), 0x00);
	io::out<u8>(p + static_cast<u16>(reg_offset::lcr), 0x80);
	io::out<u8>(p + static_cast<u16>(reg_offset::dll), 0x03);
	io::out<u8>(p + static_cast<u16>(reg_offset::dlm), 0x00);
	io::out<u8>(p + static_cast<u16>(reg_offset::lcr), 0x03);
	io::out<u8>(p + static_cast<u16>(reg_offset::fcr), 0xC7);
	io::out<u8>(p + static_cast<u16>(reg_offset::mcr), 0x0B);
}

bool is_transmit_ready(port p) {
	return io::in<u8>(p + static_cast<u16>(reg_offset::lsr)) & 0x20;
}

void write_byte(u8 byte, port p) {
	while (!is_transmit_ready(p)) {
	}
	io::out<u8>(p + static_cast<u16>(reg_offset::thr), byte);
}

bool is_received(port p) {
	return io::in<u8>(p + static_cast<u16>(reg_offset::lsr)) & 0x01;
}

u8 read_byte(port p) {
	while (!is_received(p)) {
	}
	return io::in<u8>(p + static_cast<u16>(reg_offset::rbr));
}

void write(const u8* data, usize len, port p) {
	for (usize i = 0; i < len; ++i) {
		write_byte(data[i], p);
	}
}

void read(u8* buffer, usize len, port p) {
	for (usize i = 0; i < len; ++i) {
		buffer[i] = read_byte(p);
	}
}

} // namespace serial