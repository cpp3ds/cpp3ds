#include <cpp3ds/Graphics/Text.hpp>
#include <cpp3ds/Graphics/Color.hpp>
#include <cpp3ds/Graphics/ascii64.h>
#include <cpp3ds/System/utils.hpp>
#include <cpp3ds/Window/Screen.hpp>

namespace cpp3ds {

	void Text::drawChar(Screen& screen, char letter, int x, int y) {
		int i, k;
		unsigned char mask;
		unsigned char l;

		for (i = 0; i < 8; i++){
			mask = 0b10000000;
			l = ascii_data[letter][i];
			for (k = 0; k < 8; k++){
				if ((mask >> k) & l){
					screen.setPixel(k+x, i+y, color);
				}
			}
		}
	}

	void Text::draw(Screen& screen, float x, float y, bool use3D, bool isLeftside) {
		float tmp_x = this->x + x;
		int i;
		int line = 0;

		int width = screen.getWidth();

		for (i = 0; i < strlen(text); ++i){
			if (tmp_x+8 > width || text[i] == '\n') {
				line++;
				tmp_x = this->x + x;
				if (text[i] == '\n')
					continue;
			}
			drawChar(screen, text[i], tmp_x, this->y+y+(line*8));
			tmp_x += 8;
		}
	}

}
