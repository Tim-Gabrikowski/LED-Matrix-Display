/* Standard library */
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* AVR */
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

/* Hardware access */
#include "keys.c"
#include "adc.c"
#include "ws2812.c"

/* Utilities */
#include "random.c"
#include "led.c"
#include "font.c"
#include "text.c"
#include "field.c"
#include "hsl.c"

/* Games */
#include "snake.c"
#include "stacker.c"
#include "minesweeper.c"
#include "tetris.c"
#include "sokoban.c"
#include "game.c"

int main(void)
{
	adc_init();
	random_init();
	buttons_init();
	game();
	return 0;
}
