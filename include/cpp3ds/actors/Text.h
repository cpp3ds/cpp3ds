#ifndef TEXT_ACTOR_H
#define TEXT_ACTOR_H

#include <cpp3ds/actors/Actor.h>
#include <cpp3ds/Color.h>
#include <cpp3ds/Screen.h>

namespace cpp3ds {
	namespace Actors {

		class Text: public Actor {
		protected:
			void drawChar(Screen& screen, char letter, int x, int y);
		public:
			char* text;
			Color color;
			Text(char* text, Color color, int x = 0, int y = 0, int depth3d = 0):
			        text(text), color(color), Actor(x, y, depth3d) {}
			virtual void draw(Screen& screen, int x = 0, int y = 0);
		};

	}
}

#endif
