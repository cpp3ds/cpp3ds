#include <SFML/Graphics.hpp>
#include <cpp3ds/Simulator/Simulator.hpp>

namespace cpp3ds {

	void display(){
		_simulator->screen->display();
		if (_simulator->getState() == SIM_PAUSED){
			_simulator->screen->renderWindow.setActive(false);
			_simulator->updatePausedFrame();
			while (_simulator->getState() == SIM_PAUSED)
				sf:sleep(sf::milliseconds(100));
		}
		_simulator->screen->renderWindow.clear();
	}

}
