/* Snake Game */

#define SNAKE_LEN_MAX        12
#define SNAKE_LEN_INITIAL     3

enum
{
	SNAKE_PAUSE,
	SNAKE_UP,
	SNAKE_DOWN,
	SNAKE_LEFT,
	SNAKE_RIGHT
};

typedef struct
{
	uint8_t x, y;
} SnakeBlock;

typedef struct
{
	uint8_t color[4];
	SnakeBlock pos;
} SnakeFood;

typedef struct
{
	uint8_t len;
	uint8_t color[4];
	SnakeBlock blocks[SNAKE_LEN_MAX];
} Snake;

typedef struct
{
	SnakeBlock food;
	Snake snake;
} SnakeGame;

static uint8_t snake_block_equals(SnakeBlock *a, SnakeBlock *b)
{
	return a->x == b->x && a->y == b->y;
}

static uint8_t snake_collides(Snake *snake, SnakeBlock *block)
{
	for(uint8_t i = 0; i < snake->len; ++i)
	{
		if(snake_block_equals(snake->blocks + i, block))
		{
			return 1;
		}
	}

	return 0;
}

static uint8_t snake_food_eaten(Snake *snake, SnakeBlock *food)
{
	return snake_block_equals(snake->blocks + snake->len - 1, food);
}

static void snake_food_random(Snake *snake, SnakeFood *food)
{
	random_color(snake->color, food->color);
	do
	{
		food->pos.x = rand() % LED_WIDTH;
		food->pos.y = rand() % LED_HEIGHT;
	}
	while(snake_collides(snake, &food->pos));
}

static void snake_block_put(SnakeBlock *block, uint8_t *color)
{
	led_put_pixel(block->x, block->y, color);
}

static void snake_food_draw(SnakeFood *food)
{
	snake_block_put(&food->pos, food->color);
}

static void snake_draw(Snake *snake)
{
	for(uint8_t i = 0; i < snake->len; ++i)
	{
		snake_block_put(snake->blocks + i, snake->color);
	}
}

static void snake_init(Snake *snake)
{
	snake->color[3] = 0;
	random_color(snake->color, snake->color);
	snake->len = SNAKE_LEN_INITIAL;
	for(uint8_t i = 0; i < SNAKE_LEN_INITIAL; ++i)
	{
		SnakeBlock *block = snake->blocks + i;
		block->x = 1 + i;
		block->y = 1;
	}
}

static SnakeBlock snake_move(Snake *snake, uint8_t dir)
{
	SnakeBlock last = snake->blocks[0];
	for(uint8_t i = 0; i < snake->len - 1; ++i)
	{
		snake->blocks[i] = snake->blocks[i + 1];
	}

	SnakeBlock *head = snake->blocks + snake->len - 1;
	switch(dir)
	{
	case SNAKE_UP:
		--head->y;
		break;

	case SNAKE_DOWN:
		++head->y;
		break;

	case SNAKE_LEFT:
		--head->x;
		break;

	case SNAKE_RIGHT:
		++head->x;
		break;
	}

	return last;
}

static void snake_render(Snake *snake, SnakeFood *food)
{
	led_clear_black();
	snake_draw(snake);
	snake_food_draw(food);
	led_update();
}

static void snake_grow(Snake *snake, SnakeBlock last)
{
	if(snake->len + 1 < SNAKE_LEN_MAX)
	{
		++snake->len;
		for(uint8_t i = snake->len; i > 0; --i)
		{
			snake->blocks[i] = snake->blocks[i - 1];
		}

		snake->blocks[0] = last;
	}
}

static uint8_t snake_check(Snake *snake)
{
	SnakeBlock *head = snake->blocks + snake->len - 1;
	if(head->x >= LED_WIDTH || head->y >= LED_HEIGHT)
	{
		return 1;
	}

	for(uint8_t i = 0; i < snake->len - 1; ++i)
	{
		if(snake_block_equals(snake->blocks + i, head))
		{
			return 1;
		}
	}

	return 0;
}

static void snake(void)
{
	uint8_t ticks_update = 220;
	uint8_t dir = SNAKE_PAUSE;
	uint8_t ticks = 0;
	uint16_t score = 0;
	Snake snake;
	SnakeFood food;

	snake_init(&snake);
	snake_food_random(&snake, &food);
	snake_render(&snake, &food);
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
				dir = SNAKE_PAUSE;
				break;

			case KEY_DOWN_PRESSED:
				dir = SNAKE_DOWN;
				break;

			case KEY_UP_PRESSED:
				dir = SNAKE_UP;
				break;

			case KEY_LEFT_PRESSED:
				dir = SNAKE_LEFT;
				break;

			case KEY_RIGHT_PRESSED:
				dir = SNAKE_RIGHT;
				break;
			}
		}

		if(dir)
		{
			if(++ticks >= ticks_update)
			{
				ticks = 0;
				SnakeBlock last = snake_move(&snake, dir);
				if(snake_check(&snake))
				{
					gameover_score(score);
					return;
				}

				if(snake_food_eaten(&snake, &food.pos))
				{
					if(ticks_update > 100)
					{
						ticks_update -= 10;
					}

					++score;
					memcpy(snake.color, food.color, 4);
					snake_grow(&snake, last);
					snake_food_random(&snake, &food);
				}

				snake_render(&snake, &food);
			}
		}

		delay1();
	}
}
