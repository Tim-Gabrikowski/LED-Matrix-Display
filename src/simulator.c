#include <SDL2/SDL.h>
#include <unistd.h>

#define SIZE 16
#define GFX_INITIAL_WIDTH     (SIZE * 32)
#define GFX_INITIAL_HEIGHT    (SIZE * 32)
#define WINDOW_TITLE             "Simulator"
#define COLOR_WHITE              0xFFFFFFFF

static SDL_Window *_window;
static SDL_Renderer *_renderer;

#define BTN_COUNT     6

#define PSTR(X) X
#define PROGMEM

#define pgm_read_byte(X) *(X)
#define pgm_read_word(X) *(X)

static uint8_t events[BTN_COUNT];
static int8_t event_count = -1;

enum
{
	EVENT_NONE,

	KEY_ESCAPE_RELEASED,
	KEY_UP_RELEASED,
	KEY_LEFT_RELEASED,
	KEY_CENTER_RELEASED,
	KEY_RIGHT_RELEASED,
	KEY_DOWN_RELEASED,

	KEY_ESCAPE_PRESSED,
	KEY_UP_PRESSED,
	KEY_LEFT_PRESSED,
	KEY_CENTER_PRESSED,
	KEY_RIGHT_PRESSED,
	KEY_DOWN_PRESSED
};

int key_get(void)
{
	/* First invocation */
	if(event_count < 0)
	{
		/* Scan buttons */
		event_count = 0;
		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
			case SDL_QUIT:
				SDL_DestroyRenderer(_renderer);
				SDL_DestroyWindow(_window);
				SDL_Quit();
				exit(1);

			case SDL_KEYUP:
				{
					uint32_t key = e.key.keysym.scancode;
					switch(key)
					{
					case SDL_SCANCODE_W: events[event_count++] = KEY_UP_RELEASED; break;
					case SDL_SCANCODE_A: events[event_count++] = KEY_LEFT_RELEASED; break;
					case SDL_SCANCODE_S: events[event_count++] = KEY_DOWN_RELEASED; break;
					case SDL_SCANCODE_D: events[event_count++] = KEY_RIGHT_RELEASED; break;
					case SDL_SCANCODE_SPACE: events[event_count++] = KEY_CENTER_RELEASED; break;
					case SDL_SCANCODE_ESCAPE: events[event_count++] = KEY_ESCAPE_RELEASED; break;
					}
				}
				break;

			case SDL_KEYDOWN:
				{
					uint32_t key = e.key.keysym.scancode;
					switch(key)
					{
					case SDL_SCANCODE_W: events[event_count++] = KEY_UP_PRESSED; break;
					case SDL_SCANCODE_A: events[event_count++] = KEY_LEFT_PRESSED; break;
					case SDL_SCANCODE_S: events[event_count++] = KEY_DOWN_PRESSED; break;
					case SDL_SCANCODE_D: events[event_count++] = KEY_RIGHT_PRESSED; break;
					case SDL_SCANCODE_SPACE: events[event_count++] = KEY_CENTER_PRESSED; break;
					case SDL_SCANCODE_ESCAPE: events[event_count++] = KEY_ESCAPE_PRESSED; break;
					}
				}
				break;
			}
		}
	}

	/* Pop events from stack */
	--event_count;
	if(event_count < 0)
	{
		/* No events left */
		return 0;
	}

	return events[event_count];
}

void _delay_ms(int ms)
{
	usleep(ms * 1000);
}

static void ws2812(uint8_t *pixels, uint16_t count)
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);
	for(int i = 0; i < count; ++i)
	{
		int n = i * 3;
		SDL_SetRenderDrawColor(_renderer, pixels[n + 1], pixels[n], pixels[n + 2], 255);

		int x = (i % SIZE);
		int y = (i / SIZE);

		y = (SIZE - 1) - y;
		if(!(y & 1))
		{
			x = (SIZE - 1) - x;
		}

		SDL_Rect r = { x*32, y*32, 32, 32 };
		SDL_RenderFillRect(_renderer, &r);
	}

	SDL_RenderPresent(_renderer);
}

#include "led.c"
#include "font.c"
#include "text.c"
#include "field.c"
#include "hsl.c"

#include "snake.c"
#include "stacker.c"
#include "minesweeper.c"
#include "tetris.c"
#include "game.c"

static void init(void)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Error initializing SDL; SDL_Init: %s\n", SDL_GetError());
		goto fail_sdl_init;
	}

	if(!(_window = SDL_CreateWindow(WINDOW_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		GFX_INITIAL_WIDTH, GFX_INITIAL_HEIGHT,
		SDL_WINDOW_RESIZABLE)))
	{
		printf("Error creating SDL_Window: %s\n", SDL_GetError());
		goto fail_create_window;
	}

	if(!(_renderer = SDL_CreateRenderer
		(_window, -1, SDL_RENDERER_ACCELERATED)))
	{
		printf("Error creating SDL_Renderer: %s\n", SDL_GetError());
		goto fail_create_renderer;
	}

	return;

fail_create_renderer:
	SDL_DestroyWindow(_window);
fail_create_window:
	SDL_Quit();
fail_sdl_init:
	exit(1);
}

int main(int argc, char *argv[])
{
	init();
	game();
	return 0;
}
