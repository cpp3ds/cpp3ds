#include <cpp3ds/Simulator/Simulator.hpp>
#include <cpp3ds/Window/Keyboard.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace cpp3ds {

float Keyboard::m_slider = 0;

////////////////////////////////////////////////////////////
bool Keyboard::isKeyPressed(Key key) {
	return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
}


////////////////////////////////////////////////////////////
float Keyboard::get3DSlider() {
	return m_slider;
}

void Keyboard::update() {
	m_slider = _simulator->get_slider3d();
}

}
