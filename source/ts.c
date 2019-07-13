#include "ts.h"

void wait4touch(int ts, struct coordinate *coor, bool *released)
{
	struct input_event buf;

	bool x_ready = false;
	bool y_ready = false;

	while (1)
	{
		bzero(&buf, sizeof(buf));
		read(ts, &buf, sizeof(buf));
		if (buf.type == EV_KEY &&
			buf.code == BTN_TOUCH &&
			buf.value == 0)
		{
			*released = true;
			break;
		}
		if (buf.type == EV_ABS &&
			buf.code == ABS_X)
		{
			coor->x = buf.value;
			x_ready = true;
		}

		if (buf.type == EV_ABS &&
			buf.code == ABS_Y)
		{
			coor->y = buf.value;
			y_ready = true;
		}

		if (x_ready && y_ready)
			break;
	}
}
