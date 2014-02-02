#include <cpp3ds/Screen.h>
#include <cpp3ds/Color.h>
#include <cpp3ds/Drawable.h>
#include <cpp3ds/utils.h>
#include <string.h>

namespace cpp3ds {

	void Screen::setPixelAddress(int addr, Color color){
		write_byte(addr, color.b);
		write_byte(addr+1, color.g);
		write_byte(addr+2, color.r);
	}

	void Screen::setPixel(int x, int y, Color color){
		int addr = (720*x+720-(y*3)-3) + frame1;
		setPixelAddress(addr, color);
		addr = (720*x+720-(y*3)-3) + frame2;
		setPixelAddress(addr, color);
	}

	void Screen::clear(Color color){
		// Just clear to black
		// memset((char*)frame1, 0, width*height*3);
		// memset((char*)frame2, 0, width*height*3);
		// TODO: Too slow, maybe try memcpy?
		char* frameptr1 = (char*)frame1;
		char* frameptr2 = (char*)frame2;
		for (int i = 0; i < width*height; ++i){
			*frameptr1++ = *frameptr2++ = color.b;
			*frameptr1++ = *frameptr2++ = color.g;
			*frameptr1++ = *frameptr2++ = color.r;
		}
	}

	void Screen::draw(Drawable& obj, float x, float y) {
		obj.draw(*this, x, y);
	}

}
