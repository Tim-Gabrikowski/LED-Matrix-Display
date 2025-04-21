#define FALL_SPEED_DEFAULT 400
#define FALL_SPEED_FAST     30

#define WINDOW_WIDTH          (LED_WIDTH * BLOCK_SIZE + 6 * BLOCK_SIZE)
#define WINDOW_HEIGHT         (LED_HEIGHT * BLOCK_SIZE)

enum { I, J, L, O, S, T, Z };

typedef struct
{
	int8_t X, Y, Rotation;
	uint8_t Type;
} Piece;

typedef struct
{
	uint16_t Blocks[4];
	struct { uint8_t R, G, B; } Color;
} PieceDef;

static const PieceDef Pieces[] PROGMEM =
{
	{
		/* I */
		{ 0x0F00, 0x2222, 0x00F0, 0x4444 },
		{ 0x00, 0xFF, 0xFF } /* Cyan */
	},
	{
		/* J */
		{ 0x44C0, 0x8E00, 0x6440, 0x0E20 },
		{ 0x00, 0x00, 0xFF } /* Blue */
	},
	{
		/* L */
		{ 0x4460, 0x0E80, 0xC440, 0x2E00 },
		{ 0xFF, 0x7F, 0x00 } /* Orange */
	},
	{
		/* O */
		{ 0xCC00, 0xCC00, 0xCC00, 0xCC00 },
		{ 0xFF, 0xFF, 0x00 } /* Yellow */
	},
	{
		/* S */
		{ 0x06C0, 0x8C40, 0x6C00, 0x4620 },
		{ 0x00, 0xFF, 0x00 } /* Green */
	},
	{
		/* T */
		{ 0x0E40, 0x4C40, 0x4E00, 0x4640 },
		{ 0xFF, 0x00, 0xFF } /* Purple */
	},
	{
		/* Z */
		{ 0x0C60, 0x4C80, 0xC600, 0x2640 },
		{ 0xFF, 0x00, 0x00 } /* Red */
	}
};

static uint8_t tetris_field_get(uint8_t *field, int8_t x, int8_t y)
{
	if(x < 0 || x >= LED_WIDTH || y >= LED_HEIGHT)
	{
		return -1;
	}

	if(y < 0)
	{
		return 0;
	}

	return field_get(field, x, y);
}

static void tetris_shift(uint8_t *field, uint8_t row)
{
	for(uint8_t y = row; y > 0; --y)
	{
		for(uint8_t x = 0; x < LED_WIDTH; ++x)
		{
			field[y * LED_WIDTH + x] =
				field[(y - 1) * LED_WIDTH + x];
		}
	}
}

static uint8_t tetris_row_full(uint8_t *row)
{
	for(uint8_t x = 0; x < LED_WIDTH; ++x)
	{
		if(!row[x])
		{
			return 0;
		}
	}

	return 1;
}

static void tetris_set_color(uint8_t i)
{
	led_set_color(
		pgm_read_byte(&Pieces[i].Color.R),
		pgm_read_byte(&Pieces[i].Color.G),
		pgm_read_byte(&Pieces[i].Color.B));
}

static void tetris_flash_rows(uint8_t *field)
{
	uint8_t full[LED_HEIGHT];
	for(uint8_t y = 0; y < LED_HEIGHT; ++y)
	{
		full[y] = tetris_row_full(field + y * LED_WIDTH);
	}

	for(uint8_t i = 0; i < 3; ++i)
	{
		led_clear_black();
		for(uint8_t y = 0; y < LED_HEIGHT; ++y)
		{
			if(full[y])
			{
				continue;
			}

			for(uint8_t x = 0; x < LED_WIDTH; ++x)
			{
				uint8_t v = field_get(field, x, y);
				if(v)
				{
					tetris_set_color(v - 1);
					led_set_pixel(x, y);
				}
			}
		}

		led_update();
		_delay_ms(80);

		led_clear_black();
		for(uint8_t y = 0; y < LED_HEIGHT; ++y)
		{
			for(uint8_t x = 0; x < LED_WIDTH; ++x)
			{
				uint8_t v = field_get(field, x, y);
				if(v)
				{
					tetris_set_color(v - 1);
					led_set_pixel(x, y);
				}
			}
		}

		led_update();
		_delay_ms(80);
	}
}

static uint8_t tetris_rows(uint8_t *field)
{
	tetris_flash_rows(field);

	static const uint8_t scores[] PROGMEM = { 0, 1, 3, 5, 8 };
	uint8_t score = 0;
	for(uint8_t y = 0; y < LED_HEIGHT; ++y)
	{
		if(tetris_row_full(field + y * LED_WIDTH))
		{
			tetris_shift(field, y);
			++score;
		}
	}

	return pgm_read_byte(scores + score);
}

static void tetris_new_piece(Piece *p)
{
	static uint8_t idx = 6;
	static uint8_t bag[7];

	if(idx == 6)
	{
		uint8_t i, j, tmp;
		for(i = 0; i < 7; ++i)
		{
			bag[i] = i;
		}

		for(i = 0; i < 7; ++i)
		{
			j = rand() % 7;
			tmp = bag[i];
			bag[i] = bag[j];
			bag[j] = tmp;
		}

		idx = 0;
	}

	p->X = 4;
	p->Y = -4;
	p->Rotation = 0;
	p->Type = bag[idx++];
}

static void tetris_field_draw(uint8_t *field)
{
	for(uint8_t y = 0; y < LED_HEIGHT; ++y)
	{
		for(uint8_t x = 0; x < LED_WIDTH; ++x)
		{
			uint8_t v = field_get(field, x, y);
			if(v)
			{
				tetris_set_color(v - 1);
				led_set_pixel(x, y);
			}
		}
	}
}

static uint16_t tetris_blocks(Piece *p)
{
	return pgm_read_word(&Pieces[p->Type].Blocks[p->Rotation]);
}

static void tetris_piece_draw(Piece *p)
{
	uint8_t row = 0;
	uint8_t col = 0;
	uint16_t blocks = tetris_blocks(p);
	tetris_set_color(p->Type);
	for(uint16_t bit = 0x8000; bit; bit >>= 1)
	{
		if(blocks & bit)
		{
			int8_t y = p->Y + row;
			if(y >= 0)
			{
				led_set_pixel(p->X + col, y);
			}
		}

		if(++col == 4)
		{
			col = 0;
			++row;
		}
	}
}

static uint8_t tetris_valid_position(uint8_t *field, Piece *p)
{
	uint8_t row = 0;
	uint8_t col = 0;
	uint16_t blocks = tetris_blocks(p);
	for(uint16_t bit = 0x8000; bit > 0; bit >>= 1)
	{
		if(blocks & bit)
		{
			if(tetris_field_get(field, p->X + col, p->Y + row))
			{
				return 1;
			}
		}

		if(++col == 4)
		{
			col = 0;
			++row;
		}
	}

	return 0;
}

static uint8_t tetris_to_field(uint8_t *field, Piece *p)
{
	uint8_t row = 0;
	uint8_t col = 0;
	uint16_t blocks = tetris_blocks(p);
	for(uint16_t bit = 0x8000; bit > 0; bit >>= 1)
	{
		if(blocks & bit)
		{
			int8_t y = p->Y + row;
			if(y < 0)
			{
				return 1;
			}

			field[y * LED_WIDTH + (p->X + col)] =
				p->Type + 1;
		}

		if(++col == 4)
		{
			col = 0;
			++row;
		}
	}

	return 0;
}

static void tetris(void)
{
	Piece cp;
	uint16_t ticks = 0;
	uint16_t ticks_update = 400;
	uint16_t score = 0;
	uint8_t pause = 0;
	uint8_t field[LED_PIXELS];

	field_clear(field);
	tetris_new_piece(&cp);
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

			case KEY_CENTER_PRESSED:
				pause = 1;
				break;

			case KEY_DOWN_PRESSED:
				pause = 0;
				ticks_update = FALL_SPEED_FAST;
				break;

			case KEY_UP_PRESSED:
				if(--cp.Rotation == -1)
				{
					cp.Rotation = 3;
				}

				if(tetris_valid_position(field, &cp))
				{
					if(++cp.Rotation == 4)
					{
						cp.Rotation = 0;
					}
				}
				else
				{
					update = 1;
					pause = 0;
				}
				break;

			case KEY_LEFT_PRESSED:
				--cp.X;
				if(tetris_valid_position(field, &cp))
				{
					++cp.X;
				}
				else
				{
					update = 1;
					pause = 0;
				}
				break;

			case KEY_RIGHT_PRESSED:
				++cp.X;
				if(tetris_valid_position(field, &cp))
				{
					--cp.X;
				}
				else
				{
					update = 1;
					pause = 0;
				}
				break;

			case KEY_DOWN_RELEASED:
				ticks_update = FALL_SPEED_DEFAULT;
				break;
			}
		}

		if(!pause)
		{
			if(++ticks >= ticks_update)
			{
				ticks = 0;
				++cp.Y;
				if(tetris_valid_position(field, &cp))
				{
					--cp.Y;
					if(tetris_to_field(field, &cp))
					{
						gameover_score(score);
						return;
					}

					score += tetris_rows(field);
					tetris_new_piece(&cp);
					ticks_update = FALL_SPEED_DEFAULT;
				}

				update = 1;
			}

			if(update)
			{
				led_clear_black();
				tetris_piece_draw(&cp);
				tetris_field_draw(field);
				led_update();
			}
		}

		delay1();
	}
}
