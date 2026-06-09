# ZKernel Makefile

NAME := ZKernel

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
ISO_ROOT := $(BUILD_DIR)/iso_root
ISO := $(BUILD_DIR)/$(NAME).iso
KERNEL := $(BUILD_DIR)/$(NAME).elf

EXTERN_DIR := extern
LIMINE_DIR := $(EXTERN_DIR)/limine
LIMINE_PROTOCOL_DIR := $(EXTERN_DIR)/limine-protocol
FLANTERM_DIR := $(EXTERN_DIR)/flanterm

CC ?= x86_64-elf-gcc
LD ?= $(CC)

CFLAGS := \
	-std=gnu11 \
	-O2 \
	-g \
	-Wall \
	-Wextra \
	-ffreestanding \
	-fno-asynchronous-unwind-tables \
	-fno-unwind-tables \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-pic \
	-m64 \
	-no-pie \
	-march=x86-64 \
	-mno-red-zone \
	-mcmodel=large \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-I include \
	-I src \
	-I $(FLANTERM_DIR)/src \
	-I $(LIMINE_PROTOCOL_DIR)/include \
	-I $(LIMINE_DIR) \
	-DZK_ARCH_X86_64

LDFLAGS := \
	-nostdlib \
	-static \
	-z max-page-size=0x1000 \
	-T linker.ld

C_SRC := $(shell /usr/bin/env find src -name '*.c' 2>/dev/null)
ASM_SRC := $(shell /usr/bin/env find src -name '*.S' 2>/dev/null)

FLANTERM_SRC := \
	$(FLANTERM_DIR)/src/flanterm.c \
	$(FLANTERM_DIR)/src/flanterm_backends/fb.c

SOURCES := $(C_SRC) $(FLANTERM_SRC)
C_OBJECTS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SOURCES))
ASM_OBJECTS := $(patsubst %.S,$(OBJ_DIR)/%.o,$(ASM_SRC))
OBJECTS := $(C_OBJECTS) $(ASM_OBJECTS)

# Colors
RESET := \033[0m
BOLD := \033[1m
RED := \033[31m
GREEN := \033[32m
YELLOW := \033[33m
BLUE := \033[34m
MAGENTA := \033[35m
CYAN := \033[36m

.PHONY: all deps check-deps kernel iso run clean distclean help

all: iso

help:
	@printf "$(BOLD)$(CYAN)ZKernel Makefile$(RESET)\n"
	@printf "  $(GREEN)make deps$(RESET)       Clone Limine, limine-protocol and flanterm into extern/\n"
	@printf "  $(GREEN)make kernel$(RESET)     Build kernel ELF\n"
	@printf "  $(GREEN)make iso$(RESET)        Build bootable ISO\n"
	@printf "  $(GREEN)make run$(RESET)        Run ISO in QEMU\n"
	@printf "  $(GREEN)make clean$(RESET)      Remove build/\n"
	@printf "  $(GREEN)make distclean$(RESET)  Remove build/ and extern/\n"

deps:
	@printf "$(BOLD)$(CYAN)[DEPS]$(RESET) checking extern/\n"
	@mkdir -p $(EXTERN_DIR)
	@if [ ! -d "$(LIMINE_DIR)" ]; then \
		printf "$(GREEN)[CLONE]$(RESET) limine v11.x-binary\n"; \
		git clone --depth=1 -b v11.x-binary https://github.com/limine-bootloader/limine.git $(LIMINE_DIR); \
	else \
		printf "$(YELLOW)[SKIP]$(RESET) limine already exists\n"; \
	fi
	@if [ ! -d "$(LIMINE_PROTOCOL_DIR)" ]; then \
		printf "$(GREEN)[CLONE]$(RESET) limine-protocol\n"; \
		git clone --depth=1 https://github.com/limine-bootloader/limine-protocol.git $(LIMINE_PROTOCOL_DIR); \
	else \
		printf "$(YELLOW)[SKIP]$(RESET) limine-protocol already exists\n"; \
	fi
	@if [ ! -d "$(FLANTERM_DIR)" ]; then \
		printf "$(GREEN)[CLONE]$(RESET) flanterm\n"; \
		git clone --depth=1 https://github.com/mintsuki/flanterm.git $(FLANTERM_DIR); \
	else \
		printf "$(YELLOW)[SKIP]$(RESET) flanterm already exists\n"; \
	fi
	@chmod +x $(LIMINE_DIR)/limine 2>/dev/null || true

check-deps: deps
	@if [ ! -f "$(FLANTERM_DIR)/src/flanterm.c" ]; then \
		printf "$(RED)[ERR]$(RESET) missing $(FLANTERM_DIR)/src/flanterm.c\n"; \
		exit 1; \
	fi
	@if [ ! -f "$(FLANTERM_DIR)/src/flanterm_backends/fb.c" ]; then \
		printf "$(RED)[ERR]$(RESET) missing $(FLANTERM_DIR)/src/flanterm_backends/fb.c\n"; \
		exit 1; \
	fi
	@if [ ! -f "$(LIMINE_PROTOCOL_DIR)/include/limine.h" ]; then \
		printf "$(RED)[ERR]$(RESET) missing $(LIMINE_PROTOCOL_DIR)/include/limine.h\n"; \
		exit 1; \
	fi
	@if [ ! -f "$(LIMINE_DIR)/limine" ]; then \
		printf "$(RED)[ERR]$(RESET) missing $(LIMINE_DIR)/limine\n"; \
		exit 1; \
	fi

kernel: check-deps $(KERNEL)

$(KERNEL): $(OBJECTS) linker.ld
	@printf "$(MAGENTA)[LD]$(RESET) $@\n"
	@mkdir -p $(@D)
	@$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: %.c
	@printf "$(BLUE)[CC]$(RESET) $<\n"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.S
	@printf "$(BLUE)[AS]$(RESET) $<\n"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

iso: kernel limine.conf
	@printf "$(CYAN)[ISO]$(RESET) preparing ISO root\n"
	@rm -rf $(ISO_ROOT)
	@mkdir -p $(ISO_ROOT)/boot/limine
	@mkdir -p $(ISO_ROOT)/EFI/BOOT
	@cp $(KERNEL) $(ISO_ROOT)/boot/$(NAME).elf
	@cp limine.conf $(ISO_ROOT)/boot/limine/limine.conf
	@cp $(LIMINE_DIR)/limine-bios.sys $(ISO_ROOT)/boot/limine/
	@cp $(LIMINE_DIR)/limine-bios-cd.bin $(ISO_ROOT)/boot/limine/
	@cp $(LIMINE_DIR)/limine-uefi-cd.bin $(ISO_ROOT)/boot/limine/
	@cp $(LIMINE_DIR)/BOOTX64.EFI $(ISO_ROOT)/EFI/BOOT/
	@cp $(LIMINE_DIR)/BOOTIA32.EFI $(ISO_ROOT)/EFI/BOOT/
	@printf "$(CYAN)[XORRISO]$(RESET) $(ISO)\n"
	@xorriso -as mkisofs \
		-b boot/limine/limine-bios-cd.bin \
		-no-emul-boot \
		-boot-load-size 4 \
		-boot-info-table \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part \
		--efi-boot-image \
		--protective-msdos-label \
		$(ISO_ROOT) \
		-o $(ISO)
	@printf "$(CYAN)[LIMINE]$(RESET) bios-install\n"
	@$(LIMINE_DIR)/limine bios-install $(ISO)
	@printf "$(BOLD)$(GREEN)[OK]$(RESET) ISO ready: $(ISO)\n"

run: iso
	@printf "$(GREEN)[QEMU]$(RESET) running $(ISO)\n"
	@qemu-system-x86_64 \
		-M q35 \
		-m 512M \
		-cdrom $(ISO) \
		-serial stdio \
		-no-reboot \
		-no-shutdown

clean:
	@printf "$(RED)[CLEAN]$(RESET) build/\n"
	@rm -rf $(BUILD_DIR)

distclean: clean
	@printf "$(RED)[CLEAN]$(RESET) extern/\n"
	@rm -rf $(EXTERN_DIR)
