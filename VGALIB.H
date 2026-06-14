#ifndef VGALIB_H
#define VGALIB_H

typedef unsigned char u8;

#define RGB(r, g, b)  ((unsigned long)(r) << 16 | (unsigned long)(g) << 8 | (unsigned long)(b))

/* display constants */
#define SCREEN_W    320
#define SCREEN_H    200
#define PAL_SIZE    256
#define CUBE_R      8
#define CUBE_G      8
#define CUBE_B      4
#define MODE_VGA    0x13
#define MODE_TXT    0x03
#define DAC_RDIDX   0x3C7
#define DAC_WRIDX   0x3C8
#define DAC_DATA    0x3C9

/* framebuffer access macros */
#define VGA_MEM          ((char far *)0xA0000000L)
#define GET_IDX(x, y)    ((unsigned)(y) * 320u + (unsigned)(x))
#define SET_PX(x, y, c)  (VGA_MEM[GET_IDX(x, y)] = (c))
#define GET_PX(x, y)     (VGA_MEM[GET_IDX(x, y)])

/* inline cube palette index */
#define VGA_RGB(r, g, b) (((r) >> 5) * 32 + ((g) >> 5) * 4 + ((b) >> 6))

void vga_init(void);
void vga_close(void);
void vga_setpal(int idx, int r, int g, int b);
int  vga_match(int r, int g, int b);
void vga_init_cube(void);

#endif
