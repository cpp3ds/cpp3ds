#ifndef CPP3DS_TEXT_HPP
#define CPP3DS_TEXT_HPP

#include <stddef.h>
#include <cpp3ds/Graphics/Actor.hpp>
#include <cpp3ds/Graphics/Color.hpp>
#include <cpp3ds/Window/Screen.hpp>

namespace cpp3ds {

	class Text: public Actor {
	protected:
		void drawChar(Screen& screen, char letter, int x, int y);
	public:
		char* text;
		Color color;
		Text(char* text, Color color = {0,0,0}, float x = 0, float y = 0, float depth3d = 0):
			Actor(x, y, depth3d), text(text), color(color) {}
		virtual void draw(Screen& screen, float x = 0, float y = 0, bool use3D = true, bool isLeftside = true);
	};

}

#endif
