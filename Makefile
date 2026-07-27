# Compiler settings
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Project name and directories
TARGET = baremetal_stm32
BUILD_DIR = build
SRC_DIRS = app drivers/src
INC_DIRS = drivers/inc

# Compile flags
MCU = -mcpu=cortex-m4 -mthumb
CFLAGS = $(MCU) -std=c99 -Wall -Wextra -Werror -O0 -g3 $(addprefix -I,$(INC_DIRS))

# Source and Object files
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))

# Search paths for source files
vpath %.c $(SRC_DIRS)

.PHONY: all clean

all: $(BUILD_DIR)/$(TARGET).elf

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	@echo "Linking object files..."
	$(CC) $(MCU) $(OBJS) -nostdlib -o $@
	$(SIZE) $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
