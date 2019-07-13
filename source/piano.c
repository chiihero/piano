#include "ts.h"
#include "bmp.h"
#include <pthread.h>
#include "audio.h"
#include "scorenum.h"
#define ON 1
#define OFF 0

int white[2][12], black[2][11], line[12]; //1.按键范围，2.按键是否被按
char *FB;
struct fb_var_screeninfo vinfo;
pthread_t tid;
pthread_t touchid;
pthread_t gameid;
pthread_t playid;

struct coordinate coor, oldcoor;
bool released = false;
bool play = true;

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
	// bmp2lcd(BAR, FB, &vinfo, 0, 0);
	// 显示琴键
	int i,j;
	for (i = 0; i < 12; i++)
	{
		line[i] = 65 * i + 42;
		bmp2lcd(GAMELINE, FB, &vinfo, line[i], 0);

		white[0][i] = 65 * i + 10;
		white[1][i] = OFF;
		bmp2lcd(KEYOFF, FB, &vinfo, white[0][i], 150);
		if (i > 0)
		{

			j = i-1;
			if (j==2||j==5||j==9)
			{
				continue;
			}
			black[0][j] = 65 * j + 53;
			black[1][j] = OFF;
			bmp2lcd(KEYBLACKOFF, FB, &vinfo, black[0][i - 1], 150);
		}
	}
	// 显示按键
	bmp2lcd(KEY1, FB, &vinfo, 10, 430);
	bmp2lcd(KEY2, FB, &vinfo, 100, 430);
	bmp2lcd(KEY3, FB, &vinfo, 190, 430);
	bmp2lcd(KEY4, FB, &vinfo, 280, 430);
	bmp2lcd(KEYSTOP, FB, &vinfo, 720, 430);
}
void delay(float time)
{
	usleep(time * 1000);
}


// 点击了在琴键之外区域
bool out_of_range(struct coordinate *coor, struct coordinate *oldcoor)
{
	if (oldcoor->x == coor->x && oldcoor->y == coor->y && released == false)
	{
		// coor->x = 0;
		// coor->y = 0;
		return true;
	}
	if (coor->x < 10 || coor->x > 790)
	{
		return true;
	}
	return false;
}

bool in_of_range(int x1, int x2, int y1, int y2)
{
	if (coor.y > y1 && coor.y < y2 && coor.x > x1 && coor.x < x2)
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
			printf("key is %d\n pos is %d \n", key, pos);
	if (pos ==0 ||pos ==3 ||pos ==6 ||pos ==10)
	{
		if (key)
		{
			bmp2lcd(KEYONC, FB, &vinfo, white[0][pos], 150);
		}
		else
		{
			bmp2lcd(KEYOFFC, FB, &vinfo, white[0][pos], 150);
		}
		bmp2lcd(KEYBLACKOFF, FB, &vinfo, black[0][pos], 150);
	}
	else if (pos ==2 ||pos ==5 ||pos ==9 ||pos ==11)
	{	if (key)
		{
			bmp2lcd(KEYON, FB, &vinfo, white[0][pos], 150);
		}
		else
		{
			bmp2lcd(KEYOFF, FB, &vinfo, white[0][pos], 150);
		}

		bmp2lcd(KEYBLACKOFF, FB, &vinfo, black[0][pos - 1], 150);
	}
	else
	{
		if (key)
		{
			bmp2lcd(KEYONC, FB, &vinfo, white[0][pos], 150);
		}
		else
		{
			bmp2lcd(KEYOFFC, FB, &vinfo, white[0][pos], 150);
		}
		bmp2lcd(KEYBLACKOFF, FB, &vinfo, black[0][pos], 150);

	}
}

//选择黑或白键
void key_white_black(bool keyon, bool is_white, int pos)
{
		printf("keyon is %d\n is_white is %d\n pos is %d \n", keyon, is_white, pos);

	if (is_white)
	{
		key_white(keyon, pos);
	}
	else
	{
		if (keyon)
		{
			if (pos==2||pos==5||pos==9)
			{
				return;
			}
			bmp2lcd(KEYBLACKON, FB, &vinfo, black[0][pos], 150);
		}
		else
		{
			if (pos==2||pos==5||pos==9)
			{
				return;
			}
			bmp2lcd(KEYBLACKOFF, FB, &vinfo, black[0][pos], 150);
		}
	}
}

//黑白键处理
bool piano_change(bool is_white, int new_pos, int old_pos, bool touch)
{
	int num=0;
	printf("is_white is %d\nnew_pos is %d\nold_pos is %d \nreleased is %d\n touch is %d\n", is_white, new_pos, old_pos, released, touch);
	if (released)
	{
		key_white_black(false, is_white, new_pos);
		touch = true;
		released = false;
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
			pthread_create(&tid, NULL, play_note, (void *)(new_pos));
		}
		else
		{
			if (new_pos==2||new_pos==5||new_pos==9)
			{
				return touch;
			}
			if (new_pos>2)
				num++;
			if (new_pos>5)
				num++;	
			if (new_pos>9)
				num++;	
			printf("playnotes is %d\n",new_pos - num+12);
			pthread_create(&tid, NULL, play_note, (void *)(new_pos - num+20));
		}
		
	}
	return touch;
}
void *play_line(void *n)
{
	int i;
	int num = (int)n - 1;
	printf("play_line ===========%d\n", num);

	for (i = 0; i < 145; i += 2)
	{
		bmp2lcd(GAMEBLOCK, FB, &vinfo, line[num] - 35, i);
		bmp2lcd(GAMEUNBLOCK, FB, &vinfo, line[num] - 35, i - 5);
		delay(13.7);
	}
	bmp2lcd(GAMEUNBLOCK, FB, &vinfo, line[num] - 35, 144);
}

void *game_play(int *m)
{
	int i;
	int len = m[0];
	printf("len is %d\n", len);

	for (i = 1; i < len; i += 2)
	{ //歌曲结尾退出

		if (m[i] == 0)
		{
			pthread_exit(NULL);
		}
		printf("m is %d\n", m[i]);
		//歌曲分段
		if (m[i] <= 0)
		{
			continue;
			delay(m[i + 1]);
		}
		pthread_create(&playid, NULL, play_line, (void*)m[i]);
		delay(m[i + 1]);
		if (in_of_range(720, 800, 430, 480))
		{
			printf("exit\n");
			pthread_exit(NULL);
		}
	}

}
//歌曲播放

void music_score(int m[])
{
	int i;
	int len = m[0];//歌曲长度
	printf("len is %d\n", len);

	for (i = 1; i < len; i += 2)
	{ //歌曲结尾退出

		if (m[i] == 0)
		{
			break;
		}
		printf("m is %d\n", m[i]);
		//歌曲分段
		if (m[i] <= 0)
		{
			delay(m[i + 1]);
			continue;
		}

		key_white(true, m[i] - 1);
		pthread_create(&tid, NULL, play_note, (void *)(m[i]-1));
		
		delay(m[i + 1] * 0.9);
		key_white(false, m[i] - 1);
		delay(m[i + 1] * 0.1);
		if (in_of_range(720, 800, 430, 480))
		{
			printf("exit\n");
			break;
		}
	}
}
void *get_touch_xy(void *n)
{
	while (1)
	{
		wait4touch(&coor, &released);
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
	Init_Ts();
	int wnew_pos = 0, wold_pos = 0, bnew_pos = 0, bold_pos = 0;
	bool w_touch = true, b_touch = true;
	int len;
	bool use_touch = false;
	pthread_create(&touchid, NULL, get_touch_xy, NULL);
	while (1)
	{
		// 等待手指触碰琴键
		delay(10);//防止太快导致coor没写入就读取
		// printf("x=%d  y=%d \n", coor.x, coor.y);
		if (out_of_range(&coor, &oldcoor))
			continue;
		oldcoor.x = coor.x;
		oldcoor.y = coor.y;
		//黑色键部分
		if (coor.y > 150 && coor.y < 300)
		{
			bold_pos = bnew_pos;
			bnew_pos = (coor.x - 50) / 65;
			printf("black x=%d  y=%d wnew_pos=%d\n", coor.x, coor.y, bnew_pos);
			if (bnew_pos > 10)
			{
				continue;
			}

			b_touch = piano_change(false, bnew_pos, bold_pos, b_touch);
			use_touch = true;
			continue;
		}
		//白色键部分
		if (coor.y > 300 && coor.y < 430)
		{
			wold_pos = wnew_pos;
			wnew_pos = (coor.x - 10) / 65;
			printf("white x=%d  y=%d wnew_pos=%d\n", coor.x, coor.y, wnew_pos);
			if (wnew_pos > 11)
			{
				wnew_pos = 11;
			}
		
			w_touch = piano_change(true, wnew_pos, wold_pos, w_touch);
			use_touch = true;
			continue;
		}
		//歌曲部分
		if (coor.y > 430 && coor.y < 480)
		{
			printf("music x=%d  y=%d \n", coor.x, coor.y);
			if (coor.x > 10 && coor.x < 90)
			{
				len = sizeof(musicnum[0]) / sizeof(musicnum[0][0]);
				musicnum[0][0] = len;
				// music_score(musicnum[0]);
					music_score(musicnum[0]);

			}
			else if (coor.x > 100 && coor.x < 180)
			{
				len = sizeof(musicnum[1]) / sizeof(musicnum[1][0]);
				musicnum[1][0] = len;
				// music_score(musicnum[1]);
								music_score(musicnum[1]);

			}
			else if (coor.x > 190 && coor.x < 270)
			{
				len = sizeof(musicnum[2]) / sizeof(musicnum[2][0]);
				musicnum[2][0] = len;
				// music_score(musicnum[2]);
				music_score(musicnum[2]);

			}
			else if (coor.x > 280 && coor.x < 360)
			{
				len = sizeof(musicnum[3]) / sizeof(musicnum[3][0]);
				musicnum[3][0] = len;
				music_score(musicnum[3]);
				if (play) music_score2(musicnum[0]);
				else pthread_create(&gameid, NULL, game_play, musicnum[0]);
			
			}
			
			else if (coor.x > 640 && coor.x < 720)
			{
				if (play)
				{
					bmp2lcd(KEY4, FB, &vinfo, 640, 430);
					play = false;
					
				}else
				{
					bmp2lcd(KEY3, FB, &vinfo, 640, 430);
					play = true;
				}

			}
			coor.x =0;
			coor.y =0;
			released = false;
			continue;
		}
		//黑白键超出范围重置按键
		if (coor.y > 0 && coor.y < 150&&use_touch)
			bmp2lcd(KEYBLACKOFF, FB, &vinfo, black[0][bold_pos], 150);
		if (coor.y > 320 && coor.y < 430&&use_touch)
			key_white(false,wold_pos);
		use_touch = false;		
	}
	pthread_join(tid, NULL);
	// pthread_exit(NULL);
	return 0;
}
