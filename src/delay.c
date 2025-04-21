static void delay1(void)
{
	_delay_ms(1);
}

static void delayn(uint8_t i)
{
	do
	{
		delay1();
	}
	while(--i);
}

static void delay10n(uint8_t i)
{
	do
	{
		delayn(10);
	}
	while(--i);
}
