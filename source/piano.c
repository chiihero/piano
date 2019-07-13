#include "ts.h"
#include "lcd.h"
#include <pthread.h>
#include "audio.h"
#include "scorenum.h"
#define ON 1
#define OFF 0

int white[2][12], black[2][11]; //1.按键范围，2.按键是否被按
char *FB;
struct fb_var_screeninfo vinfo;
pthread_t tid;
pthread_t touchid;
struct coordinate coor;
bool released = false;

void init_frame()
{
	// 显示背景
	show_bmpxy(BACKGROUND, 0, 0);
	// 显示标题栏
	show_bmpxy(BAR, 0, 0);
	// 显示琴键
	int i,j;
	for (i = 0; i < 12; i++)
	{
		white[i] = 65 * i + 10;
		show_bmpxy(KEYOFF, white[i], 47);
		if (i > 0)
		{
			black[i - 1] = 65 * (i - 1) + 52;
			show_bmpxy(KEYBLACKOFF, black[i - 1], 47);
		}
	}
	// 显示按键
	show_bmpxy(KEY1, 10, 430);
	show_bmpxy(KEY2, 100, 430);
	show_bmpxy(KEY3, 190, 430);
	show_bmpxy(KEYSTOP, 280, 430);
}


int main(int argc, char **argv)
{
	

	// 准备LCD
	//init_lcd();
	//初始化界面
	init_frame();
	// 准备触摸屏
	Init_Ts();

	while (1)
	{
		// 等待手指触碰琴键
		released = false;
		wait4touch(&coor, &released);
		printf("white x=%d  y=%d wnew_pos=%d\n", coor.x, coor.y);

	
	}
	return 0;
}
