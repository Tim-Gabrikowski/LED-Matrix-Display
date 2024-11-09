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
#include "game.c"

int main(void)
{
	/* Init */
	adc_init();
	random_init();
	buttons_init();

#if 0
	/* Detect corners */
	led_set_color(255, 0, 0);
	led_set_pixel(0, 0);

	led_set_color(0, 255, 0);
	led_set_pixel(LED_WIDTH - 1, 0);

	led_set_color(0, 0, 255);
	led_set_pixel(0, LED_HEIGHT - 1);

	led_set_color(255, 255, 0);
	led_set_pixel(LED_WIDTH - 1, LED_HEIGHT - 1);

	led_update();
	_delay_ms(5000);
#endif

#if 1
	/* Test */
	led_set_color(255, 255, 0);
	text_P(PSTR("Welcome!"));
#endif

	game();

	return 0;
}
