#include <SFML/Graphics.hpp>
#include <sim3ds/sim/Simulator.h>

namespace cpp3ds {

	void display(){
		_simulator->screen->display();
		_simulator->screen->renderWindow.clear();
		while (_simulator->isPaused)
			sf:sleep(sf::milliseconds(100));
	}

}
