#ifndef CPP3DS_DISPLAY_H
#define CPP3DS_DISPLAY_H

#include <SFML/Graphics.hpp>
#include <cpp3ds/Screen.h>
#include <cpp3ds/TopScreen.h>

namespace cpp3ds {

	template<class... T>
	void display(Screen& screen, T&... rest){
		screen._display();
		display(rest...);
	}

	// template<class... T>
	// void display(TopScreen& screen, T&... rest){
	// 	screen._display();
	// 	display(rest...);
	// }

	void display();

}

#endif
