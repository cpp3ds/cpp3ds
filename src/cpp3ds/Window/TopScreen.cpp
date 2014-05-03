#include <cpp3ds/Window/TopScreen.hpp>
#include <cpp3ds/Window/Keyboard.hpp>
#include <cpp3ds/Graphics/Color.hpp>
#include <cpp3ds/Graphics/Drawable.hpp>

namespace cpp3ds {

	void TopScreen::clear(Color color){
		left_screen.clear(color);
		right_screen.clear(color);
	}

	void TopScreen::draw(Drawable& obj, float x, float y, bool use3D) {
		if (use3D) {
			float slider = Keyboard::get3DSlider() * obj.depth3d;
			obj.draw(left_screen, x - slider, y, use3D, true);
			obj.draw(right_screen, x + slider, y, use3D, false);
		} else {
			obj.draw(left_screen, x, y, use3D, true);
			obj.draw(right_screen, x, y, use3D, false);
		}
	}

}
