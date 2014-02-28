/*
 * This is not a perfect clone of Gateway's launcher.
 * The font size is different.
 */

#include <string.h>
#include <cpp3ds/BottomScreen.h>
#include <cpp3ds/TopScreen.h>
#include <cpp3ds/Stage.h>
#include <cpp3ds/actors/Text.h>
#include <cpp3ds/Color.h>

using namespace cpp3ds;

extern "C" int main() {
	BottomScreen bottom_screen;
	TopScreen top_screen;
	Stage stage;

	Color cBlue  = {0, 0, 255},
	      cWhite = {255, 255, 255};

	Actors::Text text1("BOOTROM 8046\nERRCODE:", cWhite, 10, 10);
	Actors::Text text2("00F800EF\nFFFFFFFF FFFFFFFF\n00000008 00000000", cWhite, 80, 18);

	stage.addActor(text1);
	stage.addActor(text2);

	bottom_screen.clear(cBlue);
	top_screen.clear(cBlue);
	bottom_screen.draw(stage);
	top_screen.draw(stage);

	bottom_screen.display();
	top_screen.display();

	return 0;
}
