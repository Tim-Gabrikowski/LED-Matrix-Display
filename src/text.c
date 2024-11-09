#define TEXT_SCROLL_SPEED 60

static uint8_t wait_check_cancel(void)
{
	for(uint8_t i = 0; i < TEXT_SCROLL_SPEED; ++i)
	{
		uint8_t key;
		while((key = key_get()))
		{
			if(key == KEY_ESCAPE_PRESSED)
			{
				led_clear_instant();
				return 1;
			}
		}

		_delay_ms(1);
	}

	return 0;
}

static uint8_t text_char(uint8_t c)
{
	c -= 32;
	const uint8_t *offset = font_default_chars + c * FONT_HEIGHT;
	uint8_t width = pgm_read_byte(font_default_widths + c);
	uint8_t mask = 0x01;
	for(uint8_t col = 0; col < width; ++col)
	{
		for(uint8_t row = 0; row < FONT_HEIGHT; ++row)
		{
			if(pgm_read_byte(offset + row) & mask)
			{
				led_set_pixel(LED_WIDTH - 1, row);
			}
		}

		led_update();
		if(wait_check_cancel()) { return 1; }
		mask <<= 1;
		led_shift_left();
	}

	return 0;
}

static void render_char(uint8_t c)
{
	c -= 32;
	const uint8_t *offset = font_default_chars + c * FONT_HEIGHT;
	uint8_t width = pgm_read_byte(font_default_widths + c);
	uint8_t mask = 0x01;
	uint8_t x = LED_WIDTH / 2 - width / 2;
	for(uint8_t col = 0; col < width; ++col)
	{
		for(uint8_t row = 0; row < FONT_HEIGHT; ++row)
		{
			if(pgm_read_byte(offset + row) & mask)
			{
				led_set_pixel(col + x, row);
			}
		}

		mask <<= 1;
	}
}

static void text_out(void)
{
	for(uint8_t i = 0; i < LED_WIDTH; ++i)
	{
		led_update();
		if(wait_check_cancel()) { return; }
		led_shift_left();
	}
}

static void text_P(const char *s)
{
	led_clear_instant();
	for(uint8_t c; (c = pgm_read_byte(s)); ++s)
	{
		if(text_char(c)) { return; }
	}

	text_out();
}

static void text(const char *s)
{
	led_clear_instant();
	for(uint8_t c; (c = (uint8_t)(*s)); ++s)
	{
		if(text_char(c)) { return; }
	}

	text_out();
}

static char *itoa8(uint16_t v)
{
	static char buf[8];
	char *s = buf + sizeof(buf);
	*(--s) = '\0';
	do
	{
		*(--s) = (v % 10) + '0';
		v /= 10;
	}
	while(v);
	return s;
}

static void gameover(void)
{
	led_set_color(255, 0, 0);
	text_P(PSTR("Game Over!"));
}

static void gameover_score(uint16_t score)
{
	char buf[16] = "Score: ";
	gameover();
	led_set_color(0, 255, 0);
	strcpy(buf + 7, itoa8(score));
	text(buf);
}

static void youwin(void)
{
	led_set_color(0, 255, 0);
	text_P(PSTR("You Win!"));
}
