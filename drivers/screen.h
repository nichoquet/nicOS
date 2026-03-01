#ifndef SCREEN_H
#define SCREEN_H

#include "../cpu/type.h"
#include "../libc/string.h"
#include "../libc/boolean.h"

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* VGA color codes (foreground/background) */
#define BLACK          0x0
#define BLUE           0x1
#define GREEN          0x2
#define CYAN           0x3
#define RED            0x4
#define MAGENTA        0x5
#define BROWN          0x6
#define LIGHT_GREY     0x7
#define DARK_GREY      0x8
#define LIGHT_BLUE     0x9
#define LIGHT_GREEN    0xA
#define LIGHT_CYAN     0xB
#define LIGHT_RED      0xC
#define LIGHT_MAGENTA  0xD
#define LIGHT_BROWN    0xE
#define WHITE          0xF

/* backward-compatible names */
#define YELLOW         LIGHT_BROWN

/* Helper macro to build an attribute byte from fg/bg colors */
#define MAKE_COLOR(fg, bg) ((fg) | ((bg) << 4))

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

/* Public kernel API */
void clear_screen();
void kprint_at(string message, int col, int row);
void kprint(string message);
void kprint(int message);
void kprintHexAdress(int message);
void kprint_backspace();

/* Drawing helpers for colored shapes */
void draw_char(int col, int row, char c, char attr);
void draw_hline(int col, int row, int length, char attr);
void draw_vline(int col, int row, int length, char attr);
void draw_box(int col, int row, int width, int height, char attr);

#endif