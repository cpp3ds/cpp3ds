#include <cpp3ds/TopScreen.h>
#include <cpp3ds/Color.h>
#include <cpp3ds/Drawable.h>

namespace cpp3ds {

	void TopScreen::clear(Color color){
		Screen::clear(color);
		right_screen.clear(color);
	}

	void TopScreen::draw(Drawable& obj, int x, int y) {
		// TODO: Get real 3D Slider State
		int slider = 1;
		obj.draw(*this, x - slider * obj.depth3d, y);
		obj.draw(right_screen, x + slider * obj.depth3d, y);
	}

}
