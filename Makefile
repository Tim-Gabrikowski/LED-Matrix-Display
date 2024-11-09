MCU    = atmega328p
F_CPU  = 16000000
OPT    = z
TARGET = src/main
CFLAGS =

all: clean $(TARGET).elf size

$(TARGET).elf: $(TARGET).c
	@avr-gcc $(TARGET).c -o $(TARGET).elf \
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
	@avr-size -C --mcu=$(MCU) $(TARGET).elf
	@avr-nm --radix d --size-sort $(TARGET).elf | grep " b "
	@avr-nm --radix d --size-sort $(TARGET).elf | grep " d "

connect:
	@avrdude  -c arduino -P /dev/ttyUSB0 -p m328p

flash:
	@avrdude  -c arduino -P /dev/ttyUSB0 -p m328p -U src/main.elf

clean:
	@rm -f main.elf

sim:
	gcc simulator.c -o sim -Wall -Wextra -g -O3 -lSDL2