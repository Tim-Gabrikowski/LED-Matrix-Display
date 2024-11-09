static void random_color(uint8_t *in, uint8_t *out)
{
	uint8_t r, g, b;
	uint8_t p = (in[3] + 2) % 6;
	uint8_t q = (p + (rand() % 3)) % 6;
	uint8_t v = rand() & 0xFF;
	switch(q)
	{
	default:
	case 0: r = 255; g =   v; b =   0; break;
	case 1: r =   v; g = 255; b =   0; break;
	case 2: r =   0; g = 255; b =   v; break;
	case 3: r =   0; g =   v; b = 255; break;
	case 4: r =   v; g =   0; b = 255; break;
	case 5: r = 255; g =   0; b =   v; break;
	}

	out[0] = g;
	out[1] = r;
	out[2] = b;
	out[3] = q;
}
