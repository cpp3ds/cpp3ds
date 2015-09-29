#include <cpp3ds/Window/Keyboard.hpp>
#include <3ds.h>

namespace cpp3ds {

u32 Keyboard::m_keysHeld = 0;
u32 Keyboard::m_keysPressed = 0;
u32 Keyboard::m_keysReleased = 0;
float Keyboard::m_slider3d = 0;
float Keyboard::m_sliderVolume = 0;

////////////////////////////////////////////////////////////
bool Keyboard::isKeyDown(Key key) {
	return (m_keysHeld & static_cast<u32>(key));
}

////////////////////////////////////////////////////////////
bool Keyboard::isKeyPressed(Key key) {
	return (m_keysPressed& static_cast<u32>(key));
}

////////////////////////////////////////////////////////////
bool Keyboard::isKeyReleased(Key key) {
	return (m_keysReleased & static_cast<u32>(key));
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
	hidScanInput();
	m_keysHeld = hidKeysHeld();
	m_keysPressed = hidKeysDown();
	m_keysReleased = hidKeysUp();

	u8 volume;
	HIDUSER_GetSoundVolume(&volume);
	m_sliderVolume = static_cast<float>(volume) / 63;

	m_slider3d = *(float*)0x1FF81080;
}

}
