MCU    = atmega8
F_CPU  = 11059200
OPT    = z
TARGET = main
CFLAGS =

all: clean $(TARGET).elf size

$(TARGET).elf: src/$(TARGET).c
	@avr-gcc src/$(TARGET).c -o src/$(TARGET).elf \
		$(CFLAGS) \
		-DF_CPU=$(F_CPU) \
		-O$(OPT) \
		-ffreestanding \
		-funsigned-char \
		-funsigned-bitfields \
		-fpack-struct \
		-fshort-enums \
		-Wno-array-bounds \
		-std=gnu99 \
		-pedantic \
		-Wall \
		-Wextra \
		-Wstrict-prototypes \
		-mmcu=$(MCU)

size: $(TARGET).elf
	@avr-size -C --mcu=$(MCU) src/$(TARGET).elf
	@avr-nm --radix d --size-sort src/$(TARGET).elf | grep " b "
	@avr-nm --radix d --size-sort src/$(TARGET).elf | grep " d "

connect:
	@avrdude  -c arduino -P /dev/ttyUSB0 -p m328p

flash:
	@avrdude  -c arduino -P /dev/ttyUSB0 -p m328p -U src/main.elf

clean:
	@rm -f main.elf

sim:
	gcc src/simulator.c -o sim -Wall -Wextra -g -O3 -lSDL2
