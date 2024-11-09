#define LED_WIDTH            16
#define LED_HEIGHT           16
#define LED_PIXELS             (LED_WIDTH * LED_HEIGHT)
#define LED_BYTES              (3 * LED_PIXELS)

static uint8_t led_color[3];
static uint8_t led_pixels[LED_BYTES];

static uint8_t *led_pixel_ptr(uint8_t x, uint8_t y)
{
	y = (LED_HEIGHT - 1) - y;
	uint16_t idx = ((y & 1) ? ((LED_WIDTH * y) + ((LED_WIDTH - 1) - x)) : (LED_WIDTH * y + x));
	return led_pixels + 3 * idx;
}

static void led_put_pixel(uint8_t x, uint8_t y, uint8_t *led_color)
{
	uint8_t *pixel = led_pixel_ptr(x, y);
	pixel[0] = led_color[0];
	pixel[1] = led_color[1];
	pixel[2] = led_color[2];
}

static void led_set_pixel(uint8_t x, uint8_t y)
{
	led_put_pixel(x, y, led_color);
}

static void led_shift_left(void)
{
	uint8_t black[3] = { 0, 0, 0 };
	for(uint8_t y = 0; y < LED_HEIGHT; ++y)
	{
		for(uint8_t x = 0; x < LED_WIDTH - 1; ++x)
		{
			led_put_pixel(x, y, led_pixel_ptr(x + 1, y));
		}

		led_put_pixel(LED_WIDTH - 1, y, black);
	}
}

static void led_clear_black(void)
{
	memset(led_pixels, 0, LED_BYTES);
}

static void led_set_color(uint8_t r, uint8_t g, uint8_t b)
{
	led_color[0] = g;
	led_color[1] = r;
	led_color[2] = b;
}

static void led_clear(void)
{
	uint8_t n = 0;
	uint8_t *p = led_pixels;
	for(uint8_t i = 0; i < LED_PIXELS; ++i)
	{
		*p++ = led_color[n];
		if(++n == 3)
		{
			n = 0;
		}
	}
}

static void led_update(void)
{
	ws2812(led_pixels, LED_BYTES);
}

static void led_clear_instant(void)
{
	led_clear_black();
	led_update();
}
