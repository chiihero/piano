#include "audio.h"

int vol = 0;

void *play_note(void *n)
{
	pthread_detach(pthread_self());

	char *cmd = calloc(1, 64);
	bzero(cmd, 64);

	// 拼凑恰当的mp3文件的名称
	sprintf(cmd, "./bin/madplay ./res/notes/d%d.mp3 -a %d", (int)n, vol);

	// 运行madplay来播放指定的mp3
	char *kill_the_current_note = "killall madplay";

	system(kill_the_current_note);
	system(cmd);
	free(cmd);
}
