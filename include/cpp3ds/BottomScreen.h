#ifndef BOTTOM_SCREEN_H
#define BOTTOM_SCREEN_H

#include <cpp3ds/Screen.h>

#define BOTTOM_FRAME1 0x202118E0
#define BOTTOM_FRAME2 0X20249CF0
#define BOTTOM_WIDTH 320
#define BOTTOM_HEIGHT 240

namespace cpp3ds {
	class BottomScreen: public Screen {
	public:
		BottomScreen(): Screen(BOTTOM_FRAME1, BOTTOM_FRAME2, BOTTOM_WIDTH, BOTTOM_HEIGHT){}
	};
}

#endif
