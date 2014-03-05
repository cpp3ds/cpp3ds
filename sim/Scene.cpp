#include <stdint.h>
#include <cpp3ds/Screen.h>
#include <cpp3ds/Scene.h>
#include <cpp3ds/Display.h>
#include <cpp3ds/utils.h>
#include <SFML/Graphics.hpp>

namespace cpp3ds {
	
	int Scene::run(bool useDisplay){
		int ret = 0;
		sf::Clock clock;
		sf::Time deltaTime;
		while (ret == 0) {
			render();
			if (useDisplay)
				display();
			deltaTime = clock.restart();
			ret = update(deltaTime.asSeconds());
		}
		return ret;
	}

	void Scene::display(){
		cpp3ds::display(bottomScreen, topScreen);
	}

}
