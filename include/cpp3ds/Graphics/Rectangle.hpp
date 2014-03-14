#ifndef CPP3DS_RECTANGLE_HPP
#define CPP3DS_RECTANGLE_HPP

#include <cpp3ds/Graphics/Actor.hpp>
#include <cpp3ds/Graphics/Color.hpp>
#include <cpp3ds/Window/Screen.hpp>

namespace cpp3ds {

	class Rectangle: public Actor {
	protected:
		int width, height;
	public:
		Color color;
		Rectangle(int w, int h, Color color, float x = 0, float y = 0, float depth3d = 0):
		        Actor(x, y, depth3d), width(w), height(h), color(color) {}
		virtual void draw(Screen& screen, float x = 0, float y = 0);
	};

}

#endif
