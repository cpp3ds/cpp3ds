#ifndef DRAWABLE_H
#define DRAWABLE_H

namespace cpp3ds {

	class Screen; // Forward-declaration for cyclic reference

	class  Drawable {
	public:
		float depth3d;
		Drawable(){ depth3d = 0; }
		virtual void draw(Screen& screen, float x, float y) = 0;
	};
}

#endif
