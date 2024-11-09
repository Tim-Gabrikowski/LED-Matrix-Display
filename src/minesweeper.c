#define MINESWEEPER_MINE         0x0F
#define MINESWEEPER_MINES_COUNT  20

static uint8_t minesweeper_has_mine(uint8_t *field, uint8_t x, uint8_t y)
{
	return field_get(field, x, y) == MINESWEEPER_MINE;
}

static void minesweeper_mark(uint8_t *field, uint8_t x, uint8_t y)
{
	if(x >= LED_WIDTH || y >= LED_HEIGHT) { return; }
	field[y * LED_WIDTH + x] |= 0xC0;
}

static void minesweeper_surround(uint8_t *field, uint8_t x, uint8_t y)
{
	minesweeper_mark(field, x - 1, y - 1);
	minesweeper_mark(field, x - 1, y);
	minesweeper_mark(field, x - 1, y + 1);

	minesweeper_mark(field, x,     y - 1);
	minesweeper_mark(field, x,     y + 1);

	minesweeper_mark(field, x + 1, y - 1);
	minesweeper_mark(field, x + 1, y);
	minesweeper_mark(field, x + 1, y + 1);
}

static uint8_t minesweeper_reveal(uint8_t *field, uint8_t x, uint8_t y)
{
	uint8_t f = field_get(field, x, y);
	if(f == MINESWEEPER_MINE)
	{
		return 1;
	}

	minesweeper_mark(field, x, y);

	uint8_t remain = 1;
	while(remain)
	{
		remain = 0;
		for(uint8_t y = 0; y < LED_HEIGHT; ++y)
		{
			for(uint8_t x = 0; x < LED_WIDTH; ++x)
			{
				uint8_t *p = field + y * LED_WIDTH + x;
				uint8_t front = *p & 0xC0;
				uint8_t back = *p & 0x3F;
				if(front == 0xC0)
				{
					if(!back)
					{
						minesweeper_surround(field, x, y);
					}

					remain = 1;
					front = 0x80;
					if(back <= 0x08)
					{
						back += 0x10;
					}

					*p = front | back;
				}
			}
		}
	}

	/* Unmark all */
	for(uint8_t y = 0; y < LED_HEIGHT; ++y)
	{
		for(uint8_t x = 0; x < LED_WIDTH; ++x)
		{
			field[y * LED_WIDTH + x] &= 0x3F;
		}
	}

	return 0;
}

static void minesweeper_inc(uint8_t *field, uint8_t x, uint8_t y)
{
	if(x >= LED_WIDTH || y >= LED_HEIGHT || minesweeper_has_mine(field, x, y)) { return; }
	++field[y * LED_WIDTH + x];
}

static void minesweeper_init(uint8_t *field, uint8_t cx, uint8_t cy)
{
	field_clear(field);
	for(uint8_t i = 0; i < MINESWEEPER_MINES_COUNT; ++i)
	{
		uint8_t x, y;
		do
		{
			x = rand() % LED_WIDTH;
			y = rand() % LED_HEIGHT;
		}
		while((x == cx && y == cy) || minesweeper_has_mine(field, x, y));

		field_set(field, x, y, MINESWEEPER_MINE);

		minesweeper_inc(field, x - 1, y - 1);
		minesweeper_inc(field, x,     y - 1);
		minesweeper_inc(field, x + 1, y - 1);

		minesweeper_inc(field, x - 1, y);
		minesweeper_inc(field, x + 1, y);

		minesweeper_inc(field, x - 1, y + 1);
		minesweeper_inc(field, x,     y + 1);
		minesweeper_inc(field, x + 1, y + 1);
	}
}

static uint16_t minesweeper_count(uint8_t *field)
{
	uint16_t count = 0;
	for(uint16_t i = 0; i < LED_PIXELS; ++i)
	{
		count += (field[i] < 0x10);
	}

	return count;
}

static void minesweeper_set_color(uint8_t v)
{
	switch(v)
	{
	case 0x10: led_set_color(  0,   0,   0); break;
	case 0x11: led_set_color(  0, 255,   0); break;
	case 0x12: led_set_color(127, 255,   0); break;
	case 0x13: led_set_color(255, 255,   0); break;
	case 0x14: led_set_color(255, 127,   0); break;
	case 0x15: led_set_color(255,   0,   0); break;
	case 0x16: led_set_color(255,   0, 127); break;
	case 0x17: led_set_color(255,   0, 255); break;
	case 0x18: led_set_color(127,   0, 255); break;
	case 0x19: led_set_color(255,   0,   0); break;
	default:   led_set_color( 25,  25,  25); break;
	}
}

static void minesweeper_show_mines(uint8_t *field, uint8_t r, uint8_t g, uint8_t b)
{
	led_clear_black();
	for(uint8_t x = 0; x < LED_WIDTH; ++x)
	{
		for(uint8_t y = 0; y < LED_HEIGHT; ++y)
		{
			uint8_t v = field_get(field, x, y);
			if(v == MINESWEEPER_MINE)
			{
				led_set_color(r, g, b);
			}
			else
			{
				minesweeper_set_color(v);
			}

			led_set_pixel(x, y);
		}
	}
}

static uint8_t minesweeper_set_if(uint8_t *field, uint8_t x, uint8_t y)
{
	if(x >= LED_WIDTH || y >= LED_HEIGHT)
	{
		return 0;
	}

	uint8_t f = field_get(field, x, y);
	if(f == MINESWEEPER_MINE)
	{
		field_set(field, x, y, 0x19);
	}
	else if(f <= 0x08)
	{
		field_set(field, x, y, f + 0x10);
	}

	led_set_pixel(x, y);
	if(x == 0)
	{
		if(y == 0) { return 1; }
		if(y == LED_HEIGHT - 1) { return 2; }
	}

	if(x == LED_WIDTH - 1)
	{
		if(y == 0) { return 4; }
		if(y == LED_HEIGHT - 1) { return 8; }
	}

	return 0;
}

static uint8_t minesweeper_rhombus(uint8_t *field, uint8_t cx, uint8_t cy, uint8_t size)
{
	uint8_t flags = 0;
	uint8_t t = cy + size;
	uint8_t b = cy - size;
	for(uint8_t i = 0; i <= size; ++i)
	{
		flags |= minesweeper_set_if(field, cx + i, t - i);
		flags |= minesweeper_set_if(field, cx - i, t - i);

		flags |= minesweeper_set_if(field, cx + i, b + i);
		flags |= minesweeper_set_if(field, cx - i, b + i);
	}

	return flags;
}

static void minesweeper_explode(uint8_t *field, uint8_t cx, uint8_t cy)
{
	uint8_t flags = 0;
	for(uint8_t i = 0; flags != 0xF; ++i)
	{
		minesweeper_show_mines(field, 25, 25, 25);
		led_set_color(255, 0, 0);
		flags |= minesweeper_rhombus(field, cx, cy, i);
		led_update();
		_delay_ms(80);
	}

	minesweeper_show_mines(field, 25, 25, 25);
	led_update();

	_delay_ms(2000);
}

static void minesweeper_flash(uint8_t *field)
{
	for(uint8_t i = 0; i < 3; ++i)
	{
		minesweeper_show_mines(field, 25, 25, 25);
		led_update();
		_delay_ms(150);
		minesweeper_show_mines(field, 255, 255, 255);
		led_update();
		_delay_ms(150);
	}

	_delay_ms(2000);
}

static void minesweeper_render(uint8_t *field, uint8_t cx, uint8_t cy, uint8_t on)
{
	led_clear_black();
	for(uint8_t x = 0; x < LED_WIDTH; ++x)
	{
		for(uint8_t y = 0; y < LED_HEIGHT; ++y)
		{
			minesweeper_set_color(field_get(field, x, y));
			led_set_pixel(x, y);
		}
	}

	if(on)
	{
		led_set_color(0, 0, 255);
		led_set_pixel(cx, cy);
	}

	led_update();
}

static void minesweeper(void)
{
	uint8_t field[LED_PIXELS];
	uint8_t cx = 0, cy = 0;
	uint8_t blink = 0;
	uint8_t ticks = 200;
	uint8_t first = 1;

	field_clear(field);
	minesweeper_render(field, cx, cy, 1);
	for(;;)
	{
		uint8_t update = 0;
		uint8_t key;
		while((key = key_get()))
		{
			switch(key)
			{
			case KEY_ESCAPE_PRESSED:
				return;
			
			case KEY_LEFT_PRESSED:
				update = 1;
				if(cx == 0)
				{
					cx = LED_WIDTH - 1;
				}
				else
				{
					--cx;
				}
				break;

			case KEY_RIGHT_PRESSED:
				update = 1;
				if(cx == LED_WIDTH - 1)
				{
					cx = 0;
				}
				else
				{
					++cx;
				}
				break;

			case KEY_UP_PRESSED:
				update = 1;
				if(cy == 0)
				{
					cy = LED_HEIGHT - 1;
				}
				else
				{
					--cy;
				}
				break;

			case KEY_DOWN_PRESSED:
				update = 1;
				if(cy == LED_HEIGHT - 1)
				{
					cy = 0;
				}
				else
				{
					++cy;
				}
				break;

			case KEY_CENTER_PRESSED:
				if(first)
				{
					first = 0;
					do
					{
						minesweeper_init(field, cx, cy);
						minesweeper_reveal(field, cx, cy);
					}
					while(minesweeper_count(field) == 0);
				}
				else
				{
					if(minesweeper_reveal(field, cx, cy))
					{
						minesweeper_explode(field, cx, cy);
						gameover();
						return;
					}

					if(minesweeper_count(field) == 0)
					{
						minesweeper_flash(field);
						youwin();
						return;
					}
				}

				update = 1;
				break;
			}
		}

		if(update)
		{
			minesweeper_render(field, cx, cy, 1);
			blink = 0;
			ticks = 0;
		}
		else
		{
			if(++ticks >= 200)
			{
				ticks = 0;
				minesweeper_render(field, cx, cy, blink);
				blink = !blink;
			}
		}

		_delay_ms(1);
	}
}
