#define GAMES_COUNT 4

static void preview_render(uint8_t idx)
{
	static const uint8_t gp[] PROGMEM =
	{
		'T', 1,
		'S', 2,
		//'R', 4,
		'M', 3,
		'P', 5
	};

	const uint8_t *p = gp + 2 * idx;
	uint8_t c = pgm_read_byte(p);
	uint8_t color = pgm_read_byte(p + 1);

	led_clear_black();
	set_bits_color(color);
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
				if(game == GAMES_COUNT - 1) { game = 0; } else { ++game; }
				preview_render(game);
				break;

			case KEY_LEFT_PRESSED:
				if(!game) { game = GAMES_COUNT - 1; } else { --game; }
				preview_render(game);
				break;

			case KEY_CENTER_PRESSED:
				switch(game)
				{
				default:
				case 0: tetris();      break;
				case 1: snake();       break;
				//case 2: stacker();     break;
				case 2: minesweeper(); break;
				case 3: sokoban();     break;
				}

				/* After game finished */
				preview_render(game);
				break;
			}
		}

		delay1();
	}
}
