#ifndef BOTTOM_SCREEN_H
#define BOTTOM_SCREEN_H

#include <sim3ds/sim/Simulator.h>
#include <cpp3ds/Screen.h>

#define BOTTOM_X (400-320)/2
#define BOTTOM_Y 240
#define BOTTOM_WIDTH 320
#define BOTTOM_HEIGHT 240

namespace cpp3ds {
	class BottomScreen: public Screen {
	public:
		BottomScreen(): Screen(BOTTOM_X, BOTTOM_Y, BOTTOM_WIDTH, BOTTOM_HEIGHT) {}
	};
}

#endif
