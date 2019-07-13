#include "ts.h"
#include "bmp.h"
#include <pthread.h>
#include "audio.h"
#include "scorenum.h"
int white[12], black[11];
char *FB;
struct fb_var_screeninfo vinfo;
pthread_t tid;
struct coordinate coor;
bool released = false;
int ts;
char *init_lcd(struct fb_var_screeninfo *vinfo)
{
	int lcd = open("/dev/fb0", O_RDWR);

	bzero(vinfo, sizeof(struct fb_var_screeninfo));
	ioctl(lcd, FBIOGET_VSCREENINFO, vinfo);

	char *FB = mmap(NULL, vinfo->xres * vinfo->yres * 4,
					PROT_READ | PROT_WRITE,
					MAP_SHARED, lcd, 0);
	return FB;
}
void init_frame()
{
	// 显示背景
	bmp2lcd(BACKGROUND, FB, &vinfo, 0, 0);
	// 显示标题栏
	bmp2lcd(BAR, FB, &vinfo, 0, 0);
	// 显示琴键
	int i;
	for (i = 0; i < 12; i++)
	{
		white[i] = 65 * i + 10;
		bmp2lcd(KEYOFF, FB, &vinfo, white[i], 47);
		if (i > 0)
		{
			black[i - 1] = 65 * (i - 1) + 52;
			bmp2lcd(KEYBLACKOFF, FB, &vinfo, black[i - 1], 47);
		}
	}
	// 显示按键
	bmp2lcd(KEY1, FB, &vinfo, 10, 430);
	bmp2lcd(KEY2, FB, &vinfo, 100, 430);
	bmp2lcd(KEY3, FB, &vinfo, 190, 430);
	bmp2lcd(KEYSTOP, FB, &vinfo, 280, 430);
}

void delay(int time)
{
	usleep(time * 1000);
}

// 点击了在琴键之外区域
bool out_of_range(struct coordinate *coor)
{
	if (coor->x < 10 || coor->x > 790)
	{
		return true;
	}
	return false;
}
void usage(const char *prog)
{
	printf("\nwelcome you you you\nUsage: %s volume[0-3]\n", prog);
}
//白色单个按键处理
void key_white(bool key, int pos)
{
	if (pos > 0 && pos < 11)
	{
		if (key)
		{
			bmp2lcd(KEYONC, FB, &vinfo, white[pos], 47);
		}
		else
		{
			bmp2lcd(KEYOFFC, FB, &vinfo, white[pos], 47);
		}
		bmp2lcd(KEYBLACKOFF, FB, &vinfo, black[pos], 47);
	}
	else
	{
		if (key)
		{
			bmp2lcd(KEYON, FB, &vinfo, white[pos], 47);
		}
		else
		{
			bmp2lcd(KEYOFF, FB, &vinfo, white[pos], 47);
		}
		if (pos == 0)
			bmp2lcd(KEYBLACKOFF, FB, &vinfo, black[pos], 47);
		if (pos == 11)
			bmp2lcd(KEYBLACKOFF, FB, &vinfo, black[pos - 1], 47);
	}
}

//选择黑或白键
void key_white_black(bool keyon, bool is_white, int pos)
{
	if (is_white)
	{
		key_white(keyon, pos);
	}
	else
	{
		if (keyon)
		{
			bmp2lcd(KEYBLACKON, FB, &vinfo, black[pos], 47);
		}
		else
		{
			bmp2lcd(KEYBLACKOFF, FB, &vinfo, black[pos], 47);
		}
	}
}

//黑白键处理
bool piano_change(bool is_white, bool released, int new_pos, int old_pos, bool touch)
{
	printf("new_pos is %d\nold_pos is %d \nreleased is %d\n touch is %d", new_pos, old_pos, released, touch);
	if (released)
	{
		key_white_black(false, is_white, new_pos);
		touch = true;
		return touch;
	}
	if (new_pos != old_pos || touch)
	{
		//更新琴键状态
		key_white_black(true, is_white, new_pos);
		if (!touch)
			key_white_black(false, is_white, old_pos);
		else
			touch = false;
		// 使用madplay播放琴音
		if (is_white)
		{
			pthread_create(&tid, NULL, play_note, (void *)(new_pos + 1));
		}
		else
		{
			pthread_create(&tid, NULL, play_note, (void *)(new_pos + 12));
		}
	}
	return touch;
}

//歌曲播放
void music_score(int m[])
{
	int i;
	int len = m[0];
	printf("len is %d\n", len);
	for (i = 1; i < len; i++)
	{ //歌曲结尾退出
		if (m[i] == 0)
		{
			break;
		}
		printf("m is %d\n", m[i]);
		//歌曲分段
		if (m[i] == -1)
		{
			delay(500);
			continue;
		}
		key_white(true, m[i] - 1);
		pthread_create(&tid, NULL, play_note, (void *)(m[i]));
		delay(300);
		key_white(false, m[i] - 1);
		delay(200);
	}
}
int main(int argc, char **argv)
{
	usage(argv[0]);
	if (argc == 2)
	{
		switch (atoi(argv[1]))
		{
		case 0:
			vol = -105;
			break;
		case 1:
			vol = -45;
			break;
		case 2:
			vol = -15;
			break;
		case 3:
			vol = 10;
			break;
		}
	}
	else
	{
		vol = -45;
	}

	// 准备LCD
	FB = init_lcd(&vinfo);
	bzero(FB, vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8);
	//初始化界面
	init_frame();
	// 准备触摸屏
	ts = open("/dev/input/event0", O_RDONLY);
	int wnew_pos = 0, wold_pos = 0, bnew_pos = 0, bold_pos = 0;
	bool w_touch = true, b_touch = true;
	int len;
	while (1)
	{
		// 等待手指触碰琴键
		released = false;
		wait4touch(ts, &coor, &released);
		if (out_of_range(&coor))
			continue;
		//白色键部分
		if (coor.y > 200 && coor.y < 320)
		{
			wold_pos = wnew_pos;
			wnew_pos = (coor.x - 10) / 65;
			printf("white x=%d  y=%d wnew_pos=%d\n", coor.x, coor.y, wnew_pos);
			if (wnew_pos > 11)
			{
				wnew_pos = 11;
			}
			w_touch = piano_change(true, released, wnew_pos, wold_pos, w_touch);
		}
		//黑色键部分
		if (coor.y > 20 && coor.y < 200)
		{
			bold_pos = bnew_pos;
			bnew_pos = (coor.x - 50) / 65;
			printf("black x=%d  y=%d wnew_pos=%d\n", coor.x, coor.y, bnew_pos);
			if (bnew_pos > 10)
			{
				bnew_pos = 10;
			}
			b_touch = piano_change(false, released, bnew_pos, bold_pos, b_touch);
		}
		//歌曲部分
		if (coor.y > 430 && coor.y < 480 && released)
		{
			printf("music x=%d  y=%d \n", coor.x, coor.y);
			if (coor.x > 10 && coor.x < 90)
			{
				len = sizeof(music[0]) / sizeof(music[0][0]);
				music[0][0] = len;
				// pthread_create(&scoreid, NULL, music_score, (music[0]));
				music_score(music[0]);
			}
			else if (coor.x > 100 && coor.x < 180)
			{
				len = sizeof(music[1]) / sizeof(music[1][0]);
				music[1][0] = len;
				// pthread_create(&scoreid, NULL, music_score, (music[1]));
				music_score(music[1]);
			}
			else if (coor.x > 190 && coor.x < 270)
			{
				len = sizeof(music[2]) / sizeof(music[2][0]);
				music[2][0] = len;
				music_score(music[2]);
				// pthread_create(&scoreid, NULL, music_score, (music[2]));
			}
		}
	}
	pthread_exit(NULL);
	return 0;
}
