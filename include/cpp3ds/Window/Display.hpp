#ifndef CPP3DS_DISPLAY_HPP
#define CPP3DS_DISPLAY_HPP

// #include <SFML/Graphics.hpp>
#include <cpp3ds/Window/Screen.hpp>
#include <cpp3ds/Window/TopScreen.hpp>

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
