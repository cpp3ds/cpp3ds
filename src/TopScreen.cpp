#include <cpp3ds/TopScreen.h>
#include <cpp3ds/Color.h>
#include <cpp3ds/Drawable.h>

namespace cpp3ds {

	void TopScreen::clear(Color color){
		Screen::clear(color);
		right_screen.clear(color);
	}

	void TopScreen::draw(Drawable& obj, float x, float y) {
		// TODO: Get real 3D Slider State
		float slider = 1.0f;
		obj.draw(*this, x - slider * obj.depth3d, y);
		obj.draw(right_screen, x + slider * obj.depth3d, y);
	}

}
