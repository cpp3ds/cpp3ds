#include <SFML/Graphics.hpp>
#include <cpp3ds/Input.h>
#include <cpp3ds/utils.h>

namespace cpp3ds {

	float Input::slider = 0;

	int Input::getHID(){
		// last_hid = read_word(HID);
		last_hid = 0;
		return last_hid;
	}

	bool Input::isDown(Button button) {
		// return (~read_word(HID) & button);
		return sf::Keyboard::isKeyPressed(button);
	}

	void Input::update3DSlider(){
		Input::slider = 1.0f;
	}

}
