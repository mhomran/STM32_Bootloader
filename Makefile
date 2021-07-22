# CONFIG
CPU           := cortex-m4
DEFINES       := STM32F407xx

BUILD_DIR     := build
SRC_DIR       := src

SRCS          := $(shell find $(SRC_DIR) -name "*.c" -or -name "*.s" -or -name "*.S")
OBJS          := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS          := $(OBJS:.o=.d)
LSCRIPT       := $(shell find $(SRC_DIR) -name "*.ld")
INC_DIRS      := $(shell find $(SRC_DIR) -type d)
INC_FLAGS     := $(addprefix -I,$(INC_DIRS))
DEFINES_FLAGS := $(addprefix -D,$(DEFINES))

TARGET        := output
CC            := arm-none-eabi-gcc
OBJCOPY       := arm-none-eabi-objcopy
GDB           := arm-none-eabi-gdb
SIZE          := arm-none-eabi-size
CFLAGS        := -mcpu=$(CPU) -mthumb -Wall -O0 -g -std=gnu99 -MMD -MP $(INC_FLAGS) $(DEFINES_FLAGS)
LDFLAGS       := -mcpu=$(CPU) -mthumb -nostdlib -T $(LSCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@.elf
	$(OBJCOPY) -j .text -j .data -O ihex $@.elf $@.hex
	$(SIZE) $@.elf

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: openocd
openocd:
	openocd -f board/stm32f4discovery.cfg 

.PHONY: qemu
qemu: 
	qemu-system-gnuarmeclipse -s -S -board STM32F4-Discovery -image $(BUILD_DIR)/$(TARGET).elf

.PHONY: gdb
gdb: 
	$(GDB) $(BUILD_DIR)/$(TARGET).elf
