#define WS2812_OUT         PORTC
#define WS2812_DIR         DDRC
#define WS2812_PIN         0

#define W_ZERO_PULSE    350
#define W_ONE_PULSE     900
#define W_TOTAL_PERIOD 1250
#define W_FIXED_LOW       2
#define W_FIXED_HIGH      4
#define W_FIXED_TOTAL     8

#define W_ZERO_CYCLES      (((F_CPU / 1000) * W_ZERO_PULSE) / 1000000)
#define W_ONE_CYCLES       (((F_CPU / 1000) * W_ONE_PULSE + 500000) / 1000000)
#define W_TOTAL_CYCLES     (((F_CPU / 1000) * W_TOTAL_PERIOD + 500000) / 1000000)

#define W_1                (W_ZERO_CYCLES - W_FIXED_LOW)
#define W_2                (W_ONE_CYCLES - w_fixedhigh - W_1)
#define W_3                (W_TOTAL_CYCLES - W_FIXED_TOTAL - W_1 - W_2)

#if (W_1 > 0)
#define W_1_NOPS W_1
#else
#define W_1_NOPS  0
#endif

#define W_LOWTIME (((W_1_NOPS + W_FIXED_LOW) * 1000000) / (F_CPU / 1000))

#if (W_LOWTIME > 550)
#error "F_CPU to low"
#elif (W_LOWTIME > 450)
#warning "Critical timing"
#endif

#if (W_2 > 0)
#define W_2_NOPS W_2
#else
#define W_2_NOPS 0
#endif

#if (W_3 > 0)
#define W_3_NOPS W_3
#else
#define W_3_NOPS 0
#endif

#define W_NOP_1  "nop      \n\t"
#define W_NOP_2  "rjmp .+0 \n\t"
#define W_NOP_4  W_NOP_2 W_NOP_2
#define W_NOP_8  W_NOP_4 W_NOP_4
#define W_NOP_16 W_NOP_8 W_NOP_8

static void ws2812(uint8_t *pixels, uint16_t count)
{
	uint8_t b, c, h, l, s;
	h = (1 << WS2812_PIN);
	WS2812_DIR |= h;
	l = ~h & WS2812_OUT;
	h |= WS2812_OUT;
	s = SREG;
	asm volatile ("cli");
	while(count--)
	{
		b = *pixels++;
		asm volatile
		(
			"       ldi   %0,8  \n\t"
			"loop%=:            \n\t"
			"       out   %2,%3 \n\t"
#if (W_1_NOPS & 1)
	W_NOP_1
#endif
#if (W_1_NOPS & 2)
	W_NOP_2
#endif
#if (W_1_NOPS & 4)
	W_NOP_4
#endif
#if (W_1_NOPS & 8)
	W_NOP_8
#endif
#if (W_1_NOPS & 16)
	W_NOP_16
#endif
			"       sbrs  %1,7  \n\t"
			"       out   %2,%4 \n\t"
			"       lsl   %1    \n\t"
#if (W_2_NOPS & 1)
	W_NOP_1
#endif
#if (W_2_NOPS & 2)
	W_NOP_2
#endif
#if (W_2_NOPS & 4)
	W_NOP_4
#endif
#if (W_2_NOPS & 8)
	W_NOP_8
#endif
#if (W_2_NOPS & 16)
	W_NOP_16
#endif
			"       out   %2,%4 \n\t"
#if (W_3_NOPS & 1)
	W_NOP_1
#endif
#if (W_3_NOPS & 2)
	W_NOP_2
#endif
#if (W_3_NOPS & 4)
	W_NOP_4
#endif
#if (W_3_NOPS & 8)
	W_NOP_8
#endif
#if (W_3_NOPS & 16)
	W_NOP_16
#endif
			"       dec   %0    \n\t"
			"       brne  loop%=\n\t"
			: "=&d" (c)
			: "r" (b),
			"I" (_SFR_IO_ADDR(WS2812_OUT)),
			"r" (h),
			"r" (l)
		);
	}

	SREG = s;
}
