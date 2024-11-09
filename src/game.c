#if 0

#define R 0x03
#define G 0x0C
#define B 0x30
#define Y 0x0F
#define C 0x3C
#define M 0x33
#define W 0x3F

static const uint8_t previews[4][LED_PIXELS] PROGMEM =
{
	{
		G,G,0,0,0,0,0,0,G,G,
		G,0,R,R,R,R,R,0,0,G,
		0,0,0,0,R,0,0,0,0,0,
		0,0,0,0,R,0,0,0,0,0,
		0,0,0,0,R,0,0,0,0,0,
		0,0,0,0,R,0,0,0,0,0,
		0,0,0,0,R,0,0,0,0,0,
		0,0,0,0,R,0,0,0,0,0,
		G,0,0,0,R,0,0,0,0,G,
		G,G,0,0,0,0,0,0,G,G,
	},
	{
		G,G,0,0,0,0,0,0,G,G,
		G,0,0,0,G,G,G,G,0,G,
		0,0,0,G,0,0,0,0,0,0,
		0,0,0,G,0,0,0,0,0,0,
		0,0,0,0,G,G,G,0,0,0,
		0,0,0,0,0,0,0,G,0,0,
		0,0,0,0,0,0,0,G,0,0,
		0,0,0,G,G,G,G,0,0,0,
		G,0,0,0,0,0,0,0,0,G,
		G,G,0,0,0,0,0,0,G,G,
	},
	{
		G,G,0,0,0,0,0,0,G,G,
		G,0,B,B,B,B,0,0,0,G,
		0,0,B,0,0,0,B,0,0,0,
		0,0,B,0,0,0,B,0,0,0,
		0,0,B,B,B,B,0,0,0,0,
		0,0,B,B,0,0,0,0,0,0,
		0,0,B,0,B,0,0,0,0,0,
		0,0,B,0,0,B,0,0,0,0,
		G,0,B,0,0,0,B,0,0,G,
		G,G,0,0,0,0,0,0,G,G,
	},
	{
		G,G,0,0,0,0,0,0,G,G,
		G,Y,0,0,0,0,0,Y,0,G,
		0,Y,Y,0,0,0,Y,Y,0,0,
		0,Y,0,Y,0,Y,0,Y,0,0,
		0,Y,0,0,Y,0,0,Y,0,0,
		0,Y,0,0,0,0,0,Y,0,0,
		0,Y,0,0,0,0,0,Y,0,0,
		0,Y,0,0,0,0,0,Y,0,0,
		G,Y,0,0,0,0,0,Y,0,G,
		G,G,0,0,0,0,0,0,G,G,
	},
};

static void preview_render(uint8_t idx)
{
	const uint8_t *field = previews[idx];
	led_clear_black();
	for(uint8_t y = 0; y < LED_HEIGHT; ++y)
	{
		const uint8_t *row = field + y * LED_WIDTH;
		for(uint8_t x = 0; x < LED_WIDTH; ++x)
		{
			uint8_t color = pgm_read_byte(row + x);
			led_set_color((color & 0x03) << 6, (color & 0x0C) << 4, (color & 0x30) << 2);
			led_set_pixel(x, y);
		}
	}

	led_update();
}

#endif

static void preview_render(uint8_t idx)
{
	led_clear_black();
	char c;
	switch(idx)
	{
	default:
	case 0: c = 'T'; led_set_color(255,   0,   0); break;
	case 1: c = 'S'; led_set_color(  0, 255,   0); break;
	case 2: c = 'R'; led_set_color(  0,   0, 255); break;
	case 3: c = 'M'; led_set_color(255, 255,   0); break;
	}

	render_char(c);
	led_update();
}

static void game(void)
{
	uint8_t game = 0;
	preview_render(game);
	for(;;)
	{
		uint8_t key;
		while((key = key_get()))
		{
			switch(key)
			{
			case KEY_RIGHT_PRESSED:
				if(game == 3) { game = 0; } else { ++game; }
				preview_render(game);
				break;

			case KEY_LEFT_PRESSED:
				if(!game) { game = 3; } else { --game; }
				preview_render(game);
				break;

			case KEY_CENTER_PRESSED:
				switch(game)
				{
				default:
				case 0:
					tetris();
					break;

				case 1:
					snake();
					break;

				case 2:
					stacker();
					break;

				case 3:
					minesweeper();
					break;
				}

				/* After game finished */
				preview_render(game);
				break;
			}
		}

		_delay_ms(1);
	}
}
