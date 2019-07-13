#ifndef __TS_H_
#define __TS_H_

#include <stdio.h>
#include <stdbool.h>
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

struct coordinate
{
	int x;
	int y;
};
void Init_Ts(void);
void wait4touch(struct coordinate *coor, bool *released);

#endif
