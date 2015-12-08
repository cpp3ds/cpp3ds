#ifndef CPP3DS_KEYBOARD_HPP
#define CPP3DS_KEYBOARD_HPP

#ifndef EMULATION
#include <3ds.h>
#endif

namespace cpp3ds {

////////////////////////////////////////////////////////////
/// \brief Give access to the real-time state of the keyboard
///
////////////////////////////////////////////////////////////
class Keyboard {
public :

	////////////////////////////////////////////////////////////
	/// \brief Key codes
	///
	////////////////////////////////////////////////////////////
	enum Key {
		A           = 1,                     ///< The A button
		B           = 1 << 1,                ///< The B button
		Select      = 1 << 2,                ///< The Select button
		Start       = 1 << 3,                ///< The Start button
		DPadRight   = 1 << 4,
		DPadLeft    = 1 << 5,
		DPadUp      = 1 << 6,
		DPadDown    = 1 << 7,
		R           = 1 << 8,                ///< The R button
		L           = 1 << 9,                ///< The L button
		X           = 1 << 10,               ///< The X button
		Y           = 1 << 11,               ///< The Y button
		ZL          = 1 << 14,               ///< The ZL button (New 3DS only)
		ZR          = 1 << 15,               ///< The ZR button (New 3DS only)
		Touchpad    = 1 << 20,
		CStickRight = 1 << 24,
		CStickLeft  = 1 << 25,
		CStickUp    = 1 << 26,
		CStickDown  = 1 << 27,
		CPadRight   = 1 << 28,
		CPadLeft    = 1 << 29,
		CPadUp      = 1 << 30,
		CPadDown    = 1 << 31,
		Up          = DPadUp    | CPadUp,    ///< Either DPadUp or CPadUp
		Down        = DPadDown  | CPadDown,  ///< Either DPadDown or CPadDown
		Left        = DPadLeft  | CPadLeft,  ///< Either DPadLeft or CPadLeft
		Right       = DPadRight | CPadRight, ///< Either DPadRight or CPadRight
	};

	////////////////////////////////////////////////////////////
	/// \brief Check if a key is still being pressed
	///
	/// \param key Key to check
	///
	/// \return True if the key is pressed, false otherwise
	///
	////////////////////////////////////////////////////////////
	static bool isKeyDown(Key key);

	////////////////////////////////////////////////////////////
	/// \brief Check if a key is now being pressed
	///
	/// \param key Key to check
	///
	/// \return True if the key is pressed, false otherwise
	///
	////////////////////////////////////////////////////////////
	static bool isKeyPressed(Key key);

	////////////////////////////////////////////////////////////
	/// \brief Check if a key is now being released
	///
	/// \param key Key to check
	///
	/// \return True if the key is pressed, false otherwise
	///
	////////////////////////////////////////////////////////////
	static bool isKeyReleased(Key key);

	static float getSlider3D();

	static float getSliderVolume();

	////////////////////////////////////////////////////////////
	/// \brief Update cache values for input
	////////////////////////////////////////////////////////////
	static void update();

private:

	static float m_slider3d;
	static float m_sliderVolume;

	#ifndef EMULATION
	static u32 m_keysHeld;
	static u32 m_keysPressed;
	static u32 m_keysReleased;
	#endif
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
/// if (cpp3ds::Keyboard::isKeyPressed(cpp3ds::Keyboard::Left))
/// {
///     // move left...
/// }
/// else if (cpp3ds::Keyboard::isKeyPressed(cpp3ds::Keyboard::Right))
/// {
///     // move right...
/// }
/// \endcode
///
/// \see cpp3ds::Joystick, cpp3ds::Touch
///
////////////////////////////////////////////////////////////
