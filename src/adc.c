static uint16_t adc_read(uint8_t channel)
{
	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC)) {}
	return ADCW;
}

static void adc_init(void)
{
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN);
}
