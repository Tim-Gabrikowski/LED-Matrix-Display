#define ADC_CHANNEL_NOISE 0

static void random_init(void)
{
	srand(adc_read(ADC_CHANNEL_NOISE));
}
