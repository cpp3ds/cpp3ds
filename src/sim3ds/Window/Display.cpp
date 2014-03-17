#include <SFML/Graphics.hpp>
#include <cpp3ds/Simulator/Simulator.hpp>

namespace cpp3ds {

	void display(){
		_simulator->screen->display();
		_simulator->screen->renderWindow.clear();
		while (_simulator->state == SIM_PAUSED)
			sf:sleep(sf::milliseconds(100));
	}

}
