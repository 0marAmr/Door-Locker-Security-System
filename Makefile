CC := avr-gcc
CC_FLAGS := -O0 -w -std=c99	 #disable optimization. disable warnings. compiler switch ISO-C
MCU := atmega32
FREQUENCY := 8000000UL
HMI_ECU_SRC_FILES := .\HMI_ECU\main.c  .\HMI_ECU\APP\app.c .\HMI_ECU\MCAL\GPIO\gpio.c .\HMI_ECU\MCAL\USART\usart.c .\HMI_ECU\MCAL\Timer\timer.c .\HMI_ECU\HAL\LCD\lcd.c .\HMI_ECU\HAL\Keypad\keypad.c 
CONTROL_ECU_SRC_FILES := .\CONTROL_ECU\main.c  .\CONTROL_ECU\APP\app.c .\CONTROL_ECU\MCAL\GPIO\gpio.c .\CONTROL_ECU\MCAL\I2C\twi.c .\CONTROL_ECU\MCAL\USART\usart.c .\CONTROL_ECU\MCAL\Timer\timer.c .\CONTROL_ECU\HAL\Buzzer\buzzer.c .\CONTROL_ECU\HAL\EEPROM\eeprom_24c16.c .\CONTROL_ECU\HAL\Motors\DC_Motor\dc_motor.c
HMI_EXECUTABLE := HMI
CONTROL_EXECUTABLE := CONTROL

default: compile

compile:
	@echo "Compiling.." 
	@$(CC) $(CC_FLAGS) -mmcu=$(MCU) -DF_CPU=$(FREQUENCY)  $(HMI_ECU_SRC_FILES) -o $(HMI_EXECUTABLE).elf	
	@avr-objcopy -j .text -j .data -O ihex $(HMI_EXECUTABLE).elf $(HMI_EXECUTABLE).hex
	@echo "------HMI ECU Promgram Size------"
	@avr-size --format=avr --mcu=$(MCU) $(HMI_EXECUTABLE).elf

	@$(CC) $(CC_FLAGS) -mmcu=$(MCU) -DF_CPU=$(FREQUENCY)  $(CONTROL_ECU_SRC_FILES) -o $(CONTROL_EXECUTABLE).elf	
	@avr-objcopy -j .text -j .data -O ihex $(CONTROL_EXECUTABLE).elf $(CONTROL_EXECUTABLE).hex
	@echo "------CONTROL ECU Promgram Size------"
	@avr-size --format=avr --mcu=$(MCU) $(CONTROL_EXECUTABLE).elf

clean:
	@echo "removing compilation files.." 
	@rm $(HMI_EXECUTABLE).elf $(HMI_EXECUTABLE).hex $(CONTROL_EXECUTABLE).elf $(CONTROL_EXECUTABLE).hex