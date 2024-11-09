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

#define BTN_TICKS    50
#define BTN_COUNT     6
#define BTN_REPEAT  300

static uint16_t btn_ticks[BTN_COUNT];
static uint8_t events[BTN_COUNT];
static int8_t event_count = -1;

static const uint8_t btn_pins[] PROGMEM =
{
	1 << 2,
	1 << 3,
	1 << 4,
	1 << 5,
	1 << 6,
	1 << 7
};

static void buttons_init(void)
{
	/* Enable pullups */
	PORTD = 0xFC;
}

static uint8_t key_get(void)
{
	/* First invocation */
	if(event_count < 0)
	{
		/* Scan buttons */
		event_count = 0;
		uint8_t port = PIND;
		for(uint8_t i = 0; i < BTN_COUNT; ++i)
		{
			uint8_t mask = pgm_read_byte(btn_pins + i);
			if(port & mask)
			{
				/* 1 = Button released */
				if(btn_ticks[i])
				{
					if(btn_ticks[i] > BTN_TICKS)
					{
						btn_ticks[i] = BTN_TICKS;
					}

					if(--btn_ticks[i] == 0)
					{
						events[event_count++] = 1 + i;
					}
				}
			}
			else
			{
				/* 0 = Button pressed */
				if(!btn_ticks[i])
				{
					btn_ticks[i] = BTN_TICKS;
					events[event_count++] = 1 + BTN_COUNT + i;
				}
				else if(btn_ticks[i] == BTN_REPEAT)
				{
					btn_ticks[i] = BTN_TICKS;
					events[event_count++] = 1 + BTN_COUNT + i;
				}
				else
				{
					++btn_ticks[i];
				}
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
