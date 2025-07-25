#include <sys/serial.hh>

extern "C" [[noreturn]] void _start() {
	serial::init(0x3F8);
	serial::write(reinterpret_cast<const u8*>("Hello, World!\n"), 15, 0x3F8);
	for (;;)
		;
}