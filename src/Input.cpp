#include <cpp3ds/Input.h>
#include <cpp3ds/utils.h>

namespace cpp3ds {

	int Input::getHID(){
		last_hid = read_word(HID);
		return last_hid;
	}

	bool Input::isDown(Button button) {
		return (!(read_word(HID) & button));
	}

}
