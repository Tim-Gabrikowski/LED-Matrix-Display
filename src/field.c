static void field_clear(uint8_t *field)
{
	memset(field, 0, LED_PIXELS);
}

static uint8_t field_get(const uint8_t *field, uint8_t x, uint8_t y)
{
	return field[y * LED_WIDTH + x];
}

static void field_set(uint8_t *field, uint8_t x, uint8_t y, uint8_t v)
{
	field[y * LED_WIDTH + x] = v;
}
