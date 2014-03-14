#include <SFML/Graphics.hpp>
#include <cpp3ds/Simulator/Simulator.hpp>

namespace cpp3ds {

	void display(){
		_simulator->screen->display();
		_simulator->screen->renderWindow.clear();
		while (_simulator->isPaused)
			sf:sleep(sf::milliseconds(100));
	}

}
