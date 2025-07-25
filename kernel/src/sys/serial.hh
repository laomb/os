#ifndef SYS_SERIAL_HH
#define SYS_SERIAL_HH

#include <arch/io.hh>

namespace serial {

enum class reg_offset : u16 {
	thr = 0x0,
	rbr = 0x0,
	dll = 0x0,
	dlm = 0x1,
	ier = 0x1,
	iir = 0x2,
	fcr = 0x2,
	lcr = 0x3,
	mcr = 0x4,
	lsr = 0x5,
	msr = 0x6,
	scr = 0x7
};

using port = u16;

void init(port p);
bool is_transmit_ready(port p);
void write_byte(u8 byte, port p);
bool is_received(port p);
u8 read_byte(port p);
void write(const u8* data, usize len, port p);
void read(u8* buffer, usize len, port p);

} // namespace serial

#endif // SYS_SERIAL_HH