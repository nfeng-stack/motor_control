TARGET = output
BUILT_DIR =built
#编译器定义
CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc -x assembler-with-cpp
CP = arm-none-eabi-objcopy
SZ = arm-none-eabi-size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#编译参数
CPPFLAG = -IApplication/inc -IDriver/bsp/inc \
           -IDriver/CMSIS/cortex-m3 -IDriver/CMSIS/device/inc       \
		   -IDriver/Hal_lib/inc -IDriver/Hal_lib/inc/Legacy \
			-IRTOS/include -IRTOS/components/finsh/inc -IRTOS

CFLAG = -mthumb -mcpu=cortex-m3 -DUSE_HAL_DRIVER -DSTM32F103xB -O2 -Wall \
        -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP

LFLAG = -mthumb -mcpu=cortex-m3 -T./en_config/STM32F103XB_FLASH.ld \
         -specs=nano.specs -lc -lm -lnosys -Wl,-Map=$(BUILT_DIR)/$(TARGET).map,--cref -Wl,--gc-sections \
		-u _printf_float -Wl,--no-warn-rwx-segments

SOURCE_PATH = Application/src Driver/bsp/src Driver/CMSIS/device/src Driver/Hal_lib/src Driver/Hal_lib/Legacy \
			  RTOS/components/finsh/src RTOS/libcpu RTOS/src RTOS

C_SOURCE =$(foreach dir,$(SOURCE_PATH),$(wildcard $(dir)/*.c))
A_SOURCE =$(foreach dir,$(SOURCE_PATH),$(wildcard $(dir)/*.S))
A_OBJS =$(patsubst %.S,$(BUILT_DIR)/%.o,$(notdir $(A_SOURCE)))
OBJS = $(patsubst %.c,$(BUILT_DIR)/%.o,$(notdir $(C_SOURCE)))
OBJS +=$(A_OBJS)
DEPS =$(OBJS:.o=.d)	
vpath %.c Application/src:Driver/bsp/src:Driver/CMSIS/device/src:Driver/Hal_lib/src:Driver/Hal_lib/Legacy:RTOS/components/finsh/src:RTOS/libcpu:RTOS/src:RTOS
vpath %.S Application/src:Driver/bsp/src:Driver/CMSIS/device/src:Driver/Hal_lib/src:Driver/Hal_lib/Legacy:RTOS/components/finsh/src:RTOS/libcpu:RTOS/src:RTOS 

all:$(BUILT_DIR)/$(TARGET).hex $(BUILT_DIR)/$(TARGET).bin

$(BUILT_DIR)/$(TARGET).hex : $(BUILT_DIR)/$(TARGET).elf | $(BUILT_DIR)
	@$(HEX) $< $@
$(BUILT_DIR)/$(TARGET).bin : $(BUILT_DIR)/$(TARGET).elf | $(BUILT_DIR)
	@$(BIN) $< $@
$(BUILT_DIR)/$(TARGET).elf : $(OBJS) 
	@$(CC) $(OBJS) $(LFLAG) -o $@
	@$(SZ) $@
$(BUILT_DIR)/%.o : %.c 
	@$(CC) $(CPPFLAG) $(CFLAG) -c $< -o $@
$(BUILT_DIR)/%.o : %.S
	@$(CC) $(CPPFLAG) $(CFLAG) -c $< -o $@
$(BUILT_DIR):
	mkdir  $@	
-include $(DEPS)
clean:
	rm -rf $(BUILT_DIR)/*
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).bin
.PHONY: clean tt all


