#ifndef CPP3DS_DRAWABLE_HPP
#define CPP3DS_DRAWABLE_HPP

namespace cpp3ds {

	class Screen; // Forward-declaration for cyclic reference

	class  Drawable {
	public:
		float depth3d;
		Drawable(float depth3d): depth3d(depth3d) {}
		virtual void draw(Screen& screen, float x, float y, bool use3D, bool isLeftside) = 0;
	};
}

#endif
