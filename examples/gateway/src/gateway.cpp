/*
 * This is not a perfect clone of Gateway's launcher.
 * The font size is different.
 */

#include <string.h>
#include <cpp3ds/Graphics.hpp>

using namespace cpp3ds;

void cpp3ds_main() {
	BottomScreen bottom_screen;
	TopScreen top_screen;
	Stage stage;

	Color cBlue  = {0, 0, 255},
	      cWhite = {255, 255, 255};

	Text text1("BOOTROM 8046\nERRCODE:", cWhite, 10, 10);
	Text text2("00F800EF\nFFFFFFFF FFFFFFFF\n00000008 00000000", cWhite, 80, 18);

	stage.addActor(text1);
	stage.addActor(text2);

	bottom_screen.clear(cBlue);
	top_screen.clear(cBlue);
	bottom_screen.draw(stage);
	top_screen.draw(stage);

	display(top_screen, bottom_screen);
}
