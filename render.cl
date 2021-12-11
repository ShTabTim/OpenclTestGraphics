typedef unsigned char byte;
typedef struct immer {
	float d, i;
}immer;
immer im(float d, float i) {
	immer z;
	z.d = d;
	z.i = i;
	return z;
}
float* fSeed;
unsigned long int f;

immer zz(immer z, immer h) {
	return im(z.d * z.d - z.i * z.i + h.d, 2 * z.d * z.i + h.i);
}

byte get_gray_color(immer z0) {

	immer z = z0;

	for (int gray = 0; gray < 256; gray++)
		if (z.i * z.i + z.d * z.d > 4)
			return gray;
		else
			z = zz(z, z0);

	return 256;
}

__kernel void render(__global byte* drawBuf) {
	size_t gid = get_global_id(0);
	float a = 7;
	immer e = im(sin(a), cos(a));
	immer z = im(gid % 512, gid / 512);
	z.d -= 256+128; z.i -= 256;
	z.d /= 256; z.i /= 256;
	byte f = get_gray_color(z);
	drawBuf[gid * 3 + 0] = (f<<2);
	drawBuf[gid * 3 + 1] = (f>>5);
	drawBuf[gid * 3 + 2] = ((f>>3)<<2);
}