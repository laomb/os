#include <sys/serial.hh>

inline serial::Device com1{0x3f8};

extern "C" [[noreturn]] void _start() {
	serial::init(com1);
	serial::write(reinterpret_cast<const u8*>("Hello, World!\n"), 15, com1);
	for (;;)
		;
}