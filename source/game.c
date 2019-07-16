#ifdef DEBUG
//音节下降
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
//歌曲演示
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
		//歌曲分段
		if (m[i] <= 0)
		{
			continue;
			delay(m[i + 1]);
		}
		printf("play is %d\n", m[i]);
		pthread_create(&playid, NULL, play_line, (void *)m[i]);
		delay(m[i + 1]);
		if (in_of_range(720, 800, 430, 480))
		{
			printf("exit\n");
			pthread_exit(NULL);
		}
	}
}
#endif