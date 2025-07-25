include ../config.mk

OBJ_DIR    := ../build/kernel/obj
TARGET     := ../build/krnl

SRCS       := $(shell find . -type f \( -name '*.c' -o -name '*.cc' -o -name '*.S' -o -name '*.asm' \) | sed 's|^\./||')
C_SRCS     := $(filter %.c,$(SRCS))
CXX_SRCS   := $(filter %.cc,$(SRCS))
ASM_SRCS   := $(filter %.S,$(SRCS))
NASM_SRCS  := $(filter %.asm,$(SRCS))

OBJ := \
  $(patsubst %.c,$(OBJ_DIR)/%.c.o,$(C_SRCS)) \
  $(patsubst %.cc,$(OBJ_DIR)/%.cc.o,$(CXX_SRCS)) \
  $(patsubst %.S,$(OBJ_DIR)/%.S.o,$(ASM_SRCS)) \
  $(patsubst %.asm,$(OBJ_DIR)/%.asm.o,$(NASM_SRCS))

DEPS := $(OBJ:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $(KERNEL_LDFLAGS) $^ -o $@
ifeq ($(STRIP_BINARIES),1)
	$(STRIP) $@
endif

-include $(DEPS)

$(OBJ_DIR)/%.c.o: %.c
	@mkdir -p $(@D)
	$(CC) $(KERNEL_CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.cc.o: %.cc
	@mkdir -p $(@D)
	$(CXX) $(KERNEL_CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.S.o: %.S
	@mkdir -p $(@D)
	$(CC) -D__ASSEMBLY__ $(KERNEL_CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.asm.o: %.asm
	@mkdir -p $(@D)
	nasm $(KERNEL_NASMFLAGS) $< -o $@

limine.h: $(LIMINE_DIR)
	@cp $(LIMINE_DIR)/limine.h limine.h

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(DEPS)
