#ifndef __BMP_H_
#define __BMP_H_

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <pthread.h>

#define BACKGROUND	"./res/bmp/background.bmp"
#define BAR			"./res/bmp/bar.bmp"
#define LOGO		"./res/bmp/logo.bmp"
#define KEYON		"./res/bmp/key_on.bmp"
#define KEYONC		"./res/bmp/key_on_c.bmp"
#define KEYOFF		"./res/bmp/key_off.bmp"
#define KEYOFFC		"./res/bmp/key_off_c.bmp"
#define KEYBLACKON	"./res/bmp/key_black_on.bmp"
#define KEYBLACKOFF	"./res/bmp/key_black_off.bmp"
#define KEY1		"./res/bmp/key1.bmp"
#define KEY2		"./res/bmp/key2.bmp"
#define KEY3		"./res/bmp/key3.bmp"
#define KEY4		"./res/bmp/key4.bmp"
#define KEYSTOP		"./res/bmp/keystop.bmp"

#define GAMELINE	"./res/bmp/game_line.bmp"
#define GAMEBLOCK	"./res/bmp/game_block.bmp"
#define GAMEUNBLOCK	"./res/bmp/game_unblock.bmp"


struct bitmap_header
{
	int16_t type;
	int32_t size; // 图像文件大小
	int16_t reserved1;
	int16_t reserved2;
	int32_t offbits; // bmp图像数据偏移量
}__attribute__((packed));

struct bitmap_info
{
	int32_t size; // 本结构大小	
	int32_t width;
	int32_t height; // 总为零
	int16_t planes; // 总为零
	int16_t bit_count; // 色深
	int32_t compression;
	int32_t size_img; // bmp数据大小，必须是4的整数倍
	int32_t X_pel;
	int32_t Y_pel;
	int32_t clrused;
	int32_t clrImportant;
}__attribute__((packed));

struct rgb_quad
{
	int8_t blue;
	int8_t green;
	int8_t red;
	int8_t reserved;
}__attribute__((packed));


struct image_info
{
	int width;
	int height;
	int pixel_size;
};

void bmp2lcd(char *bmpfile, char *FB,
			 struct fb_var_screeninfo *vinfo,
			 int xoffset, int yoffset);
char *load_bmp (const char *bmpfile, struct image_info *imginfo);

#endif