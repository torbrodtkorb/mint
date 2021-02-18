# Makefile for the Mint bootloader

TOP := $(shell pwd)
BUILDDIR := $(TOP)/build
TARGET := boot

# Compilers
CC      = arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump
OBJCOPY = arm-none-eabi-objcopy
ARM_ASM = arm-none-eabi-as
GDB     = arm-none-eabi-gdb


# Set the compiler flags
# -x defines the code language c, -O1 defines the optimization level
CFLAGS   += -x c -O1 -g3 -ffunction-sections -fdata-sections -mlong-calls -Wall
CFLAGS   += -std=gnu99 -mcpu=cortex-a5 -ffreestanding
CFLAGS   += -Wno-unused-function -Wno-unused-variable

# Set the linker flags 
LDFLAGS  += -Wl,--gc-sections -mcpu=cortex-a5
LDFLAGS  += -T$(TOP)/arch/linker.ld -Wl,--print-memory-usage

# Set the assembler flags 
ASMFLAGS += -mcpu=cortex-a5 -g3

# Relative path after bootloader
SRC :=
ASM :=
include $(TOP)/entry/Makefile
include $(TOP)/drivers/Makefile
include $(TOP)/lib/Makefile
include $(TOP)/network/Makefile

# C preprosessor flags
CPFLAGS := 
include $(TOP)/include/Makefile

DEPS_GLOBAL = $(addprefix $(TOP)/include/, $(DEPS))

# Global path for all the source fils and assembly files
SRC_GLOBAL := $(addprefix $(BUILDDIR)/, $(SRC))
ASM_GLOBAL := $(addprefix $(BUILDDIR)/, $(ASM))

# Add all .s and .c files innto OBJ and change to .o
OBJ := $(SRC_GLOBAL:.c=.o) $(ASM_GLOBAL:.s=.o)

# Prevents Make from delete object files in the entry folder 
.SECONDARY: $(OBJ)
.PHONY: all elf bin lss clean

# Defines elf, bin, lss as prerequisite 
all: elf bin lss 

elf: $(BUILDDIR)/$(TARGET).elf
bin: $(BUILDDIR)/$(TARGET).bin
lss: $(BUILDDIR)/$(TARGET).lss

debug: $(BUILDDIR)/$(TARGET).elf
	@echo
	@echo Starting the debugger
	@$(GDB) -f $< -x $(TOP)/scripts/debug.gdb


# Rule for convert object files to elf file
# Using compiler for linking
%.elf: $(OBJ)
	@echo 
	@$(CC) $(LDFLAGS) $^ -o $@

# Rule for convert elf to binary
%.bin: %.elf
	@$(OBJCOPY) -O binary $< $@

# Rule for convert elf to lss
%.lss: %.elf
	@$(OBJDUMP) -h -S $< > $@

# Rule for compiling the source files into object files
$(BUILDDIR)/%.o: $(TOP)/%.c $(DEPS_GLOBAL)
	@echo " " CC $(patsubst $(TOP)/%, $(notdir $(TOP))/%, $<)
	@mkdir -p $(dir $@)
	@$(CC) $(CPFLAGS) $(CFLAGS) -c $< -o $@

# Rule for compiling the assembly files into object files
$(BUILDDIR)/%.o: $(TOP)/%.s $(DEPS_GLOBAL)
	@echo " " AS $(patsubst $(TOP)/%, $(notdir $(TOP))/%, $<)
	@mkdir -p $(dir $@)
	@$(ARM_ASM) $(ASMFLAGS) -c $< -o $@

# Remove the build directory
clean:
	@rm -r -f $(BUILDDIR)
