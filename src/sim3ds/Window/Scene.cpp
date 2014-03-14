#include <stdint.h>
#include <SFML/Graphics.hpp>
#include <cpp3ds/System/utils.hpp>
#include <cpp3ds/Window/Screen.hpp>
#include <cpp3ds/Window/Display.hpp>
#include <cpp3ds/Window/Scene.hpp>

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
