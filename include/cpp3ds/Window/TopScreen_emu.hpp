#ifndef CPP3DS_TOPSCREEN_HPP
#define CPP3DS_TOPSCREEN_HPP

#include <cpp3ds/Emulator/Emulator.hpp>
#include <cpp3ds/Window/Screen.hpp>

#define TOP_HEIGHT 240
#define TOP_WIDTH 400
#define TOP_LEFT_X 0
#define TOP_LEFT_Y 0
#define TOP_RIGHT_X 400 + EMU_OUTLINE_THICKNESS*2
#define TOP_RIGHT_Y 0

namespace cpp3ds {
	class TopScreen: public Screen {
	protected:
		Screen left_screen, right_screen;
	public:
		TopScreen():
			Screen(0, 0, TOP_WIDTH, TOP_HEIGHT),
			left_screen(TOP_LEFT_X, TOP_LEFT_Y, TOP_WIDTH, TOP_HEIGHT),
			right_screen(TOP_RIGHT_X, TOP_RIGHT_Y, TOP_WIDTH, TOP_HEIGHT)
			{};
		void clear(Color color = {0,0,0});
		void _display();
		virtual void draw(Drawable& obj, float x = 0, float y = 0, bool use3D = true);
	};
}

#endif
