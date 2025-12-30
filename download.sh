#!/bin/bash
openocd -f ./en_config/cmsis-dap.cfg -f ./en_config/stm32f1x.cfg -c "program ./output.elf verify reset exit"