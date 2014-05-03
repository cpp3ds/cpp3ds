#include <stdint.h>
#include <cpp3ds/Window/Screen.hpp>
#include <cpp3ds/Window/Scene.hpp>
#include <cpp3ds/Window/Display.hpp>
#include <cpp3ds/Window/Keyboard.hpp>
#include <cpp3ds/System/utils.hpp>

namespace cpp3ds {
	
	int Scene::run(bool useDisplay){
		int ret = 0;
		float deltaTime;
		lastTime = GetSystemTick();
		while (ret == 0) {
			render();
			if (useDisplay)
				display();
			deltaTime = (float)(GetSystemTick() - lastTime) / TICKS_PER_SEC;
			lastTime = GetSystemTick();
			Keyboard::update();
			ret = update(deltaTime);
		}
		return ret;
	}

	void Scene::display(){
		cpp3ds::display(bottomScreen, topScreen);
	}

}
