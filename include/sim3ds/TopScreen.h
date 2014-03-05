#ifndef TOP_SCREEN_H
#define TOP_SCREEN_H

#include <sim3ds/sim/Simulator.h>
#include <cpp3ds/Screen.h>

#define TOP_ASPECT_X 0x5
#define TOP_ASPECT_Y 0x3
#define TOP_HEIGHT 240
#define TOP_WIDTH 400
#define TOP_LEFT_X 0
#define TOP_LEFT_Y 0
#define TOP_LEFT_FRAME1 0x20184E60
#define TOP_LEFT_FRAME2 0x201CB370
#define TOP_RIGHT_X 400 + SIM_OUTLINE_THICKNESS*2
#define TOP_RIGHT_Y 0
#define TOP_RIGHT_FRAME1 0x20282160  
#define TOP_RIGHT_FRAME2 0x202C8670
#define SLIDER_STATE 0x10144000

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
