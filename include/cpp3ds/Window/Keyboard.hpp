#ifdef EMULATION
#include <cpp3ds/Window/Keyboard_emu.hpp>
#endif

#ifndef CPP3DS_KEYBOARD_HPP
#define CPP3DS_KEYBOARD_HPP

namespace cpp3ds {

#define HID 0x10146000          ///< Address at which to get button states
#define SLIDERSTATE 0x10144000  ///< Address at which to access 3D slider

////////////////////////////////////////////////////////////
/// \brief Give access to the real-time state of the keyboard
///
////////////////////////////////////////////////////////////
class Keyboard {
protected:
	static float m_slider;
public :

	////////////////////////////////////////////////////////////
	/// \brief Key codes
	///
	////////////////////////////////////////////////////////////
	enum Key {
		A      = 1,     ///< The A button
		B      = 2,     ///< The B button
		X      = 1024,  ///< The X button
		Y      = 2048,  ///< The Y button
		Up     = 64,    ///< The Up button
		Down   = 128,   ///< The Down button
		Left   = 32,    ///< The Left button
		Right  = 16,    ///< The Right button
		L      = 512,   ///< The L button
		R      = 256,   ///< The R button
		Start  = 8,     ///< The Start button
		Select = 4,     ///< The Select button

		KeyCount = 12   ///< Keep last -- the total number of keyboard keys
	};

	////////////////////////////////////////////////////////////
	/// \brief Check if a key is pressed
	///
	/// \param key Key to check
	///
	/// \return True if the key is pressed, false otherwise
	///
	////////////////////////////////////////////////////////////
	static bool isKeyPressed(Key key);

	////////////////////////////////////////////////////////////
	/// \brief Get current value of 3D slider
	///
	/// \return float value from 0 to 1.0 (0 being completely off)
	///
	////////////////////////////////////////////////////////////
	static float get3DSlider();

	////////////////////////////////////////////////////////////
	/// \brief Update cache values for input
	////////////////////////////////////////////////////////////
	static void update();
};

}


#endif


////////////////////////////////////////////////////////////
/// \class cpp3ds::Keyboard
/// \ingroup window
///
/// cpp3ds::Keyboard provides an interface to the state of the
/// keyboard. It only contains static functions (a single
/// keyboard is assumed), so it's not meant to be instantiated.
///
/// This class allows users to query the keyboard state at any
/// time and directly, without having to deal with a window and
/// its events. Compared to the KeyPressed and KeyReleased events,
/// cpp3ds::Keyboard can retrieve the state of a key at any time
/// (you don't need to store and update a boolean on your side
/// in order to know if a key is pressed or released), and you
/// always get the real state of the keyboard, even if keys are
/// pressed or released when your window is out of focus and no
/// event is triggered.
///
/// Usage example:
/// \code
/// if (cpp3ds::Keyboard::isKeyPressed(csf::Mouse, pp3ds::Keyboard::Left))
/// {
///     // move left...
/// }
/// else if (cpp3ds::Keyboard::isKeyPressed(cpp3ds::Keyboard::Right))
/// {
///     // move right...
/// }
/// else if (cpp3ds::Keyboard::isKeyPressed(cpp3ds::Keyboard::Escape))
/// {
///     // quit...
/// }
/// \endcode
///
/// \see cpp3ds::Joystick, cpp3ds::Touch
///
////////////////////////////////////////////////////////////
