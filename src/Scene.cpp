#include <stdint.h>
#include <cpp3ds/Scene.h>
#include <cpp3ds/utils.h>

namespace cpp3ds {
	
	int Scene::run(){
		int ret = 0;
		float deltaTime;
		lastTime = GetSystemTick();
		while (ret == 0) {
			render();
			deltaTime = (float)(GetSystemTick() - lastTime) / TICKS_PER_SEC;
			lastTime = GetSystemTick();
			ret = update(deltaTime);
		}
		return ret;
	}

}
