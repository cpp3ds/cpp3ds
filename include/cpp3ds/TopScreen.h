#ifndef TOP_SCREEN_H
#define TOP_SCREEN_H

#include <cpp3ds/Screen.h>

#define TOP_ASPECT_X 0x5
#define TOP_ASPECT_Y 0x3
#define TOP_HEIGHT 240
#define TOP_WIDTH 400
#define TOP_LEFT_FRAME1 0x20184E60
#define TOP_LEFT_FRAME2 0x201CB370
#define TOP_RIGHT_FRAME1 0x20282160  
#define TOP_RIGHT_FRAME2 0x202C8670

namespace cpp3ds {
	class TopScreen: public Screen {
	protected:
		Screen right_screen;
		int width, height;
	public:
		TopScreen():
			Screen(TOP_LEFT_FRAME1, TOP_LEFT_FRAME2, TOP_WIDTH, TOP_HEIGHT),
			right_screen(TOP_RIGHT_FRAME1, TOP_RIGHT_FRAME2, TOP_WIDTH, TOP_HEIGHT)
			{};
		void clear(Color color);
		void draw(Drawable& obj, int x = 0, int y = 0);
	};
}

#endif
