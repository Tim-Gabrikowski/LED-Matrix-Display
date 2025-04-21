#define GAMES_COUNT 4

static void preview_render(uint8_t idx)
{
	led_clear_black();

	static const uint8_t gp[] PROGMEM =
	{
		'T', 255,   0,   0,
		'S',   0, 255,   0,
		'R',   0,   0, 255,
		'M', 255, 255,   0,
		'P', 255,   0, 255
	};

	const uint8_t *p = gp + 4 * idx;
	uint8_t c = pgm_read_byte(p);
	uint8_t r = pgm_read_byte(p + 1);
	uint8_t g = pgm_read_byte(p + 2);
	uint8_t b = pgm_read_byte(p + 3);

	led_set_color(r, g, b);
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
