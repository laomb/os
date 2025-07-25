#include <sys/serial.hh>

namespace serial {

enum class lcr_bits : u8 {
	dl_enable = 0x80,
	word_length_8 = 0x03,
};

enum class fcr_bits : u8 {
	fifo_enable = 0x01,
	reset_rx_fifo = 0x02,
	reset_tx_fifo = 0x04,
	trigger_level_14 = 0xC0,
	fcr_init = fifo_enable | reset_rx_fifo | reset_tx_fifo | trigger_level_14,
};

enum class mcr_bits : u8 {
	dtr = 0x01,
	rts = 0x02,
	out2 = 0x08,
	mcr_init = dtr | rts | out2,
};

enum class lsr_bits : u8 {
	data_ready = 0x01,
	thr_empty = 0x20,
};

enum class baud_rate : u8 {
	baud_115200 = 0x03, // Divisor for 115200 baud
};

void init(port p) {
	io::out<u8>(p + static_cast<u16>(reg_offset::ier), 0x00);
	io::out<u8>(p + static_cast<u16>(reg_offset::lcr), static_cast<u8>(lcr_bits::dl_enable));
	io::out<u8>(p + static_cast<u16>(reg_offset::dll), static_cast<u8>(baud_rate::baud_115200));
	io::out<u8>(p + static_cast<u16>(reg_offset::dlm), 0x00);
	io::out<u8>(p + static_cast<u16>(reg_offset::lcr), static_cast<u8>(lcr_bits::word_length_8));
	io::out<u8>(p + static_cast<u16>(reg_offset::fcr), static_cast<u8>(fcr_bits::fcr_init));
	io::out<u8>(p + static_cast<u16>(reg_offset::mcr), static_cast<u8>(mcr_bits::mcr_init));
}

bool is_transmit_ready(port p) {
	return io::in<u8>(p + static_cast<u16>(reg_offset::lsr)) & static_cast<u8>(lsr_bits::thr_empty);
}

void write_byte(u8 byte, port p) {
	while (!is_transmit_ready(p)) {
	}
	io::out<u8>(p + static_cast<u16>(reg_offset::thr), byte);
}

bool is_received(port p) {
	return io::in<u8>(p + static_cast<u16>(reg_offset::lsr)) &
		   static_cast<u8>(lsr_bits::data_ready);
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