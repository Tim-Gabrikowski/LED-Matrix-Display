/* Stacker Game */

#define STACKER_INIT_BLOCKS 4

typedef struct
{
	uint8_t x, y;
} StackerBlock;

typedef struct
{
	uint8_t count;
	uint8_t color[4];
	StackerBlock blocks[STACKER_INIT_BLOCKS];
} Stacker;

static uint8_t stacker_left(Stacker *stacker)
{
	for(uint8_t i = 0; i < stacker->count; ++i)
	{
		--stacker->blocks[i].x;
	}

	return stacker->blocks[0].x == 0;
}

static uint8_t stacker_right(Stacker *stacker)
{
	for(uint8_t i = 0; i < stacker->count; ++i)
	{
		++stacker->blocks[i].x;
	}

	return stacker->blocks[stacker->count - 1].x != LED_WIDTH - 1;
}

static uint8_t stacker_move(Stacker *stacker, uint8_t dir)
{
	return dir ? stacker_right(stacker) : stacker_left(stacker);
}

static void stacker_begin(Stacker *stacker, uint8_t row, uint8_t *color)
{
	random_color(color, stacker->color);
	for(uint8_t i = 0; i < stacker->count; ++i)
	{
		StackerBlock *block = stacker->blocks + i;
		block->x = i;
		block->y = row;
	}
}

static void stacker_board_set(uint8_t *board, uint8_t x, uint8_t y)
{
	field_set(board, x, y, 1);
}

static void stacker_board_shift(uint8_t *board)
{
	for(uint8_t y = LED_HEIGHT - 1; y > 0; --y)
	{
		for(uint8_t x = 0; x < LED_WIDTH; ++x)
		{
			board[y * LED_WIDTH + x] = board[(y - 1) * LED_WIDTH + x];
		}
	}

	for(uint8_t x = 0; x < LED_WIDTH; ++x)
	{
		board[x] = 0;
	}
}

static uint8_t stacker_place(Stacker *stacker, uint8_t *board)
{
	uint8_t remain = stacker->count;
	for(uint8_t i = 0; i < stacker->count; ++i)
	{
		StackerBlock *block = stacker->blocks + i;
		if(field_get(board, block->x, block->y + 1))
		{
			stacker_board_set(board, block->x, block->y);
		}
		else
		{
			--remain;
		}
	}

	return remain;
}

static void stacker_blocks_render(Stacker *stacker)
{
	for(uint8_t i = 0; i < stacker->count; ++i)
	{
		led_put_pixel(stacker->blocks[i].x, stacker->blocks[i].y, stacker->color);
	}
}

static void stacker_board_render(uint8_t *board, uint8_t *color)
{
	for(uint8_t y = 0; y < LED_HEIGHT; ++y)
	{
		for(uint8_t x = 0; x < LED_WIDTH; ++x)
		{
			if(field_get(board, x, y))
			{
				led_put_pixel(x, y, color);
			}
		}
	}
}

static void stacker_render(Stacker *stacker, uint8_t *board, uint8_t *color)
{
	led_clear_black();
	stacker_blocks_render(stacker);
	stacker_board_render(board, color);
	led_update();
}

static void stacker_board_init(uint8_t *board)
{
	field_clear(board);
	for(uint8_t i = 0; i < STACKER_INIT_BLOCKS; ++i)
	{
		stacker_board_set(board, i + 3, LED_HEIGHT - 1);
	}
}

static void stacker(void)
{
	uint8_t color[4];
	uint8_t board[LED_PIXELS];
	Stacker stacker;
	uint16_t ticks = 0;
	uint16_t ticks_update = 340;
	uint8_t row = LED_HEIGHT - 2;
	uint8_t dir = 1;
	uint16_t height = 0;

	color[3] = 0;
	random_color(color, color);

	stacker.count = STACKER_INIT_BLOCKS;
	stacker_board_init(board);
	stacker_begin(&stacker, row, color);
	stacker_render(&stacker, board, color);
	for(;;)
	{
		uint8_t key;
		while((key = key_get()))
		{
			switch(key)
			{
			case KEY_ESCAPE_PRESSED:
				return;

			case KEY_CENTER_PRESSED:
				stacker.count = stacker_place(&stacker, board);
				if(!stacker.count)
				{
					gameover_score(height);
					return;
				}

				dir = 1;
				if(row == 1)
				{
					stacker_board_shift(board);
				}
				else
				{
					--row;
				}

				if(ticks_update > 100)
				{
					ticks_update -= 10;
				}

				memcpy(color, stacker.color, 4);
				stacker_begin(&stacker, row, color);
				++height;
				ticks = 0;
				stacker_render(&stacker, board, color);
				break;
			}
		}

		if(++ticks >= ticks_update)
		{
			ticks = 0;
			dir = stacker_move(&stacker, dir);
			stacker_render(&stacker, board, color);
		}

		delay1();
	}
}
