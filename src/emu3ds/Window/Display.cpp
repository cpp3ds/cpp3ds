#include <SFML/Graphics.hpp>
#include <cpp3ds/Emulator.hpp>

namespace cpp3ds {

	void display(){
		_emulator->screen->display();
		if (_emulator->getState() == EMU_PAUSED){
			_emulator->screen->setActive(false);
			_emulator->updatePausedFrame();
			while (_emulator->getState() == EMU_PAUSED)
				sf:sleep(sf::milliseconds(100));
		}
		// _simulator->screen->renderWindow.clear();
	}

}
