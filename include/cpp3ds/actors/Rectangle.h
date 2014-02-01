#ifndef RECTANGLE_ACTOR_H
#define RECTANGLE_ACTOR_H

#include <cpp3ds/actors/Actor.h>
#include <cpp3ds/Color.h>
#include <cpp3ds/Screen.h>

namespace cpp3ds {
	namespace Actors {

		class Rectangle: public Actor {
		protected:
			int width, height;
		public:
			Color color;
			Rectangle(int w, int h, Color color, int x = 0, int y = 0, int depth3d = 0):
			        width(w), height(h), color(color), Actor(x, y, depth3d) {}
			virtual void draw(Screen& screen, int x = 0, int y = 0);
		};

	}
}

#endif
