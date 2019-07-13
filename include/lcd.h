#ifndef _LCD_H_  //防止重复包含功能
#define _LCD_H_




#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include <sys/mman.h>//mmap
#include <stdlib.h>//malloc
#include <linux/input.h>//输入子系统

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
#define KEYSTOP		"./res/bmp/keystop.bmp"
#define  FB_SIZE  800*480*4

//BMP文件信息头  14 BYTE
typedef struct BMPfileHeader { 
unsigned short bfType; 
unsigned int bfSize; 
unsigned short bfReserved1; 
unsigned short bfReserved2; 
unsigned int bfOffBits; 
}__attribute__ ((packed)) BMPFH, *pBMPFH;


//BMP位图信息头  40 BYTE
typedef struct BMPInfoHeader{ 
unsigned int biSize; 
unsigned long  biWidth; 
unsigned long  biHeight; 
unsigned short biPlanes; 
unsigned short biBitCount; 
unsigned int biCompression; 
unsigned int biSizeImage; 
unsigned long  biXPelsPerMeter; 
unsigned long  biYPelsPerMeter; 
unsigned int biClrUsed; 
unsigned int biClrImportant; 
}__attribute__ ((packed)) BMPIH, *pBMPIH;

struct image_info
{
	int width;
	int height;
	int pixel_size;
};

int init_lcd(void);//初始化LCD
int bmp2lcd(const char *bmpfile,int X, int Y);
int show_bmpxy(char *pathname,int X0,int Y0);//函数声明  想要使用该显示函数则直接包含本头文件即可
int show_charxy(int X0,int Y0,char ch,unsigned int color);
void show_stringxy(int X0,int Y0,char *str,unsigned int color);

#endif

