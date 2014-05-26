#ifndef CPP3DS_BOTTOMSCREEN_HPP
#define CPP3DS_BOTTOMSCREEN_HPP

#include <cpp3ds/Emulator/Emulator.hpp>
#include <cpp3ds/Window/Screen.hpp>

#define BOTTOM_X ((400-320)/2)
#define BOTTOM_Y (240 + EMU_OUTLINE_THICKNESS*2)
#define BOTTOM_WIDTH 320
#define BOTTOM_HEIGHT 240

namespace cpp3ds {
	class BottomScreen: public Screen {
	public:
		BottomScreen(): Screen(BOTTOM_X, BOTTOM_Y, BOTTOM_WIDTH, BOTTOM_HEIGHT) {}
	};
}

#endif
