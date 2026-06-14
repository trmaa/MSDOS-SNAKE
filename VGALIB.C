/*
 * VGALIB.C -- VGA palette management & color matching library
 * (Turbo C 2.01) (8086 real mode, mode 13h)
 *
 * Converts 0xRRGGBB ints to the nearest palette index 0x00-0xFF
 * using Euclidean distance in 6-bit RGB space.
 *
 * Copyright (C) 2026
 * Author: Pablo Trik Marín (trmaa) <mail@pablotrik.online>
 * License: GPL 3.0
 */

#include <dos.h>
#include "vgalib.h"

static u8 palette[PAL_SIZE][3];	/* software copy of VGA DAC (6 bits/channel) */

void
vga_init(void)
{
	union REGS rf;
	int i;

	/* enter mode 13h (320x200, 256 colours) */
	rf.x.ax = MODE_VGA;
	int86(0x10, &rf, &rf);

	/* read the current VGA palette into RAM */
	for (i = 0; i < PAL_SIZE; i++) {
		outp(DAC_RDIDX, i);		/* DAC read index */
		palette[i][0] = inp(DAC_DATA);
		palette[i][1] = inp(DAC_DATA);
		palette[i][2] = inp(DAC_DATA);
	}
}

void
vga_close(void)
{
	union REGS rf;

	rf.x.ax = MODE_TXT;
	int86(0x10, &rf, &rf);
}

void
vga_setpal(int idx, int r, int g, int b)
{
	u8 r6, g6, b6;

	r6 = r >> 2;
	g6 = g >> 2;
	b6 = b >> 2;

	outp(DAC_WRIDX, idx);		/* DAC write index */
	outp(DAC_DATA, r6);
	outp(DAC_DATA, g6);
	outp(DAC_DATA, b6);

	palette[idx][0] = r6;
	palette[idx][1] = g6;
	palette[idx][2] = b6;
}

int
vga_match(int r, int g, int b)
{
	int best = 0;
	int best_dist = 999999;
	int i;
	int vr, vg, vb;
	int dr, dg, db, dist;

	vr = r >> 2;
	vg = g >> 2;
	vb = b >> 2;

	for (i = 0; i < PAL_SIZE; i++) {
		dr = palette[i][0] - vr;
		dg = palette[i][1] - vg;
		db = palette[i][2] - vb;
		dist = dr * dr + dg * dg + db * db;

		if (dist < best_dist) {
			best_dist = dist;
			best = i;
			if (dist == 0)
				break;
		}
	}
	return best;
}

void
vga_init_cube(void)
{
	int ri, gi, bi;
	int idx;
	int r, g, b;

	idx = 0;
	for (ri = 0; ri < CUBE_R; ri++) {
		r = (ri * 255) / 7;
		for (gi = 0; gi < CUBE_G; gi++) {
			g = (gi * 255) / 7;
			for (bi = 0; bi < CUBE_B; bi++) {
				b = (bi * 255) / 3;
				vga_setpal(idx, r, g, b);
				idx++;
			}
		}
	}
}
