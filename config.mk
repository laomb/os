CC       := clang
CXX      := clang++
AR       := llvm-ar
LD       := ld.lld
STRIP    := llvm-strip
OBJDUMP  := llvm-objdump
OBJCOPY  := llvm-objcopy
NM       := llvm-nm

C_STD    := gnu23
CXX_STD  := gnu++23

DEBUG_FLAGS   := -g -O2 -pipe -Wall -Wextra -Werror -pedantic
RELEASE_FLAGS := -O3 -DNDEBUG
DEPFLAGS      := -MMD -MP

CFLAGS   := -std=$(C_STD) $(DEPFLAGS)
CXXFLAGS := -std=$(CXX_STD) $(DEPFLAGS)

KERNEL_COMMON := \
    -target x86_64-elf -m64 -march=x86-64 \
    -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone \
    -ffreestanding -fno-omit-frame-pointer -nostdlib \
    -I source -I . -I common -include const.hh -mcmodel=kernel -fno-pic

KERNEL_CFLAGS   := $(CFLAGS)       $(KERNEL_COMMON) -fno-lto
KERNEL_CXXFLAGS := $(CXXFLAGS)     $(KERNEL_COMMON) \
                   -Wno-delete-non-virtual-dtor \
                   -nostdinc++ -fno-use-cxa-atexit \
                   -fno-exceptions -fno-rtti -fstack-protector-strong
KERNEL_LDFLAGS  := -m elf_x86_64 -nostdlib \
                    -static -T linker.ld -z max-page-size=0x1000 \
                    --gc-sections

NASMFLAGS        := -Wall -g
KERNEL_NASMFLAGS := $(NASMFLAGS) -f elf64

BUILD_DIR        ?= $(realpath $(CURDIR)/build)
EXTERN_DIR       ?= $(realpath $(CURDIR)/extern)

QEMUFLAGS ?= \
    -m 4G -smp cores=$(shell nproc) -M q35 \
    -serial stdio -d int -M smm=off \
    -enable-kvm -cpu host,+svm

ifeq ($(RELEASE), 1)
  CFLAGS        += $(RELEASE_FLAGS)
  CXXFLAGS      += $(RELEASE_FLAGS)
  KERNEL_CFLAGS += $(RELEASE_FLAGS)
  KERNEL_CXXFLAGS += $(RELEASE_FLAGS)
  STRIP_BINARIES := 1
else
  CFLAGS        += $(DEBUG_FLAGS)
  CXXFLAGS      += $(DEBUG_FLAGS)
  KERNEL_CFLAGS += $(DEBUG_FLAGS)
  KERNEL_CXXFLAGS += $(DEBUG_FLAGS)
  STRIP_BINARIES := 0
endif
