#include <cpp3ds/Emulator.hpp>
#include <cpp3ds/Window/Keyboard.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace cpp3ds {

float Keyboard::m_slider3d     = 0;
float Keyboard::m_sliderVolume = 0;

////////////////////////////////////////////////////////////
bool Keyboard::isKeyDown(Key key) {
	return isKeyPressed(key);
}

////////////////////////////////////////////////////////////
bool Keyboard::isKeyPressed(Key key) {
	return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
}

////////////////////////////////////////////////////////////
float Keyboard::getSlider3D() {
	return m_slider3d;
}

////////////////////////////////////////////////////////////
float Keyboard::getSliderVolume() {
	return m_sliderVolume;
}

////////////////////////////////////////////////////////////
void Keyboard::update() {
	m_slider3d = _emulator->get_slider3d();
	m_sliderVolume = 0;
}

}
