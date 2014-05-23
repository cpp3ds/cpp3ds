#include <cpp3ds/Window/Keyboard.hpp>
#include <cpp3ds/System/utils.hpp>

namespace cpp3ds {

float Keyboard::m_slider = 0;

////////////////////////////////////////////////////////////
bool Keyboard::isKeyPressed(Key key) {
	return (~read_word(HID) & static_cast<int>(key));
}


////////////////////////////////////////////////////////////
float Keyboard::get3DSlider() {
	return m_slider;
}

void Keyboard::update() {
	m_slider = (float)(read_word(SLIDERSTATE) & 0xFF) / 255;
}

}
