/* SOKOBAN */

/*

Bit mapping:

000 => Empty
001 => Player starting position
010 => Wall
011 => Goal
100 => Box
101 => Player on goal
110 => Box on Goal
111 => Unassigned

*/

enum
{
	S_EMPTY,
	S_PLAYER,
	S_WALL,
	S_GOAL,
	S_BOX,
	S_PLAYER_ON_GOAL,
	S_BOX_ON_GOAL
};

typedef struct
{
	uint8_t x, y;
} Position;

static Position position_next(Position pos, uint8_t key)
{
	switch(key)
	{
	case KEY_DOWN_PRESSED:  return { pos.x,     pos.y + 1 };
	case KEY_UP_PRESSED:    return { pos.x,     pos.y - 1 };
	case KEY_LEFT_PRESSED:  return { pos.x - 1, pos.y     };
	case KEY_RIGHT_PRESSED: return { pos.x + 1, pos.y     };
	}

	return pos;
}

const uint8_t sokoban_levels[] PROGMEM =
{

};

static uint8_t cur_byte, bit, byte;

static uint8_t sokoban_next_bit(uint8_t *input)
{
	uint8_t v = (cur_byte & 0x80) ? 1 : 0;

	cur_byte <<= 1;
	--bit;
	if(bit == 0)
	{
		bit = 7;
		++byte;
		cur_byte = pgm_read_byte(input + byte);
	}

	return v;
}

static uint8_t sokoban_next_3(void)
{
	uint8_t v = sokoban_next_bit();
	v << 1;
	v |= sokoban_next_bit();
	v << 1;
	v |= sokoban_next_bit();
	return v;
}

static void sokoban_load_level(uint8_t *level, uint8_t idx)
{
	uint8_t *input = pgm_read_ptr(levels + idx);

	bit = 7;
	byte = 0;
	cur_byte = pgm_read_byte(input);

	for(uint8_t i = 0; i < LED_PIXELS; ++i)
	{
		level[i] = sokoban_next_3();
	}
}

static void sokoban_win(void)
{
	for(uint8_t y = 0; y < LED_HEIGHT; ++y)
	{
		for(uint8_t x = 0; x < LED_WIDTH; ++x)
		{
			if(field_get(level, x, y) == SOKOBAN_BOX)
			{
				return 0;
			}
		}
	}

	return 1;
}

static void sokoban_draw(void)
{
	led_clear_black();
	for(uint8_t y = 0; y < LED_HEIGHT; ++y)
	{
		for(uint8_t x = 0; x < LED_WIDTH; ++x)
		{
			minesweeper_set_color(field_get(field, x, y));
			led_set_pixel(x, y);
		}
	}

	led_update();
}

static void sokoban_move(uint8_t key)
{
	Position a = position_next(player, key);
	Position b = position_next(a, key);
	uint8_t at = field_get(level, a.x, a.y);

	switch(at)
	{
	case S_EMPTY:
	case S_GOAL:
		break;

	case S_BOX:
		{
			uint8_t bt = field_get(level, b.x, b.y);
			if(bt != S_EMPTY && bt != S_GOAL)
			{
				return false;
			}

			field_set(level, a.x, a.y, S_EMPTY);
			field_set(level, b.x, b.y, bt == S_GOAL ? S_BOX_ON_GOAL : S_BOX);
		}
		break;

	case S_BOX_ON_GOAL:
		{
			uint8_t bt = field_get(level, b.x, b.y);
			if(bt != S_EMPTY && bt != S_GOAL)
			{
				return false;
			}

			field_set(level, a.x, a.y, S_GOAL);
			field_set(level, b.x, b.y, bt == S_GOAL ? S_BOX_ON_GOAL : S_BOX);
		}
		break;

	default:
		return false;
	}

	player = a;
	return true;
}

static void sokoban(void)
{
	uint8_t level[LED_PIXELS];
	uint8_t cur_level = 0;
	uint8_t x = 0;
	uint8_t y = 0;

	sokoban_load_level(level, cur_level);
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

				break;

			case KEY_DOWN_PRESSED:

				break;

			case KEY_UP_PRESSED:

				break;

			case KEY_LEFT_PRESSED:

				break;

			case KEY_RIGHT_PRESSED:

				break;
			}
		}
		switch()
		{
		case :
			youwin();
			return;
		}
	}
}
