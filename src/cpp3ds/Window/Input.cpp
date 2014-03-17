#include <cpp3ds/Window/Input.hpp>
#include <cpp3ds/System/utils.hpp>

namespace cpp3ds {

	float Input::slider = 0;

	int Input::getHID(){
		last_hid = read_word(HID);
		return last_hid;
	}

	bool Input::isDown(Button button) {
		return (~read_word(HID) & button);
	}

	void Input::update(float deltaTime){
		Input::slider = (float)(read_word(SLIDERSTATE) & 0xFF) / 255;
	}

}
