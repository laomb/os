include config.mk

LIMINE_DIR := $(EXTERN_DIR)/limine
LIMINE_REPO := https://github.com/limine-bootloader/limine.git

all: disk

kernel: $(LIMINE_DIR)
	@make -C kernel/ BUILD_DIR=$(BUILD_DIR) LIMINE_DIR=$(LIMINE_DIR)

disk: kernel $(LIMINE_DIR)/BOOTX64.EFI
	@mkdir -p $(BUILD_DIR)
	@rm -rf $(BUILD_DIR)/image.hdd
	@dd if=/dev/zero bs=1M count=0 seek=64 of=$(BUILD_DIR)/image.hdd
	@sgdisk $(BUILD_DIR)/image.hdd -n 1:2048 -t 1:ef00
	@mformat -i $(BUILD_DIR)/image.hdd@@1M
	@mmd -i $(BUILD_DIR)/image.hdd@@1M ::/EFI ::/EFI/BOOT
	@mcopy -i $(BUILD_DIR)/image.hdd@@1M limine.conf ::/limine.conf
	@mcopy -i $(BUILD_DIR)/image.hdd@@1M $(LIMINE_DIR)/BOOTX64.EFI ::/EFI/BOOT
	@mcopy -i $(BUILD_DIR)/image.hdd@@1M $(BUILD_DIR)/krnl ::/

debug: disk $(EXTERN_DIR)/ovmf-code-x86_64.fd
	@qemu-system-x86_64 -drive format=raw,file=$(BUILD_DIR)/image.hdd \
		-drive if=pflash,unit=0,format=raw,file=$(EXTERN_DIR)/ovmf-code-x86_64.fd,readonly=on \
		$(QEMUFLAGS) \
		-gdb tcp::1234 -S & \
		sleep 1 && \
		gdb -ex "file $(BUILD_DIR)/krnl" -ex "target remote localhost:1234"

run: disk $(EXTERN_DIR)/ovmf-code-x86_64.fd
	@qemu-system-x86_64 -drive format=raw,file=$(BUILD_DIR)/image.hdd \
			-drive if=pflash,unit=0,format=raw,file=$(EXTERN_DIR)/ovmf-code-x86_64.fd,readonly=on \
			$(QEMUFLAGS) \
			--no-reboot --no-shutdown \
			-D $(BUILD_DIR)/qemu_log.txt

$(LIMINE_DIR):
	@git clone --depth=1 --branch=v9.x-binary $(LIMINE_REPO) $(LIMINE_DIR)

clean:
	@rm -rf $(BUILD_DIR)

reset:
	@make clean
	@make

$(EXTERN_DIR)/ovmf-code-x86_64.fd:
	mkdir -p ovmf
	curl -Lo $@ https://github.com/osdev0/edk2-ovmf-nightly/releases/latest/download/ovmf-code-x86_64.fd

deps: $(EXTERN_DIR)/ovmf-code-x86_64.fd $(LIMINE_DIR)
.PHONY: all kernel disk run clean reset deps
