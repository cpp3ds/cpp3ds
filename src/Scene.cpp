#include <stdint.h>
#include <cpp3ds/Scene.h>
#include <cpp3ds/utils.h>

namespace cpp3ds {
	
	int Scene::run(){
		int ret = 0;
		float deltaTime;
		lastTime = GetSystemTick();
		while (ret == 0) {
			deltaTime = (GetSystemTick() - lastTime) / TICKS_PER_SEC;
			lastTime = GetSystemTick();
			ret = render(deltaTime);
		}
		return ret;
	}

}
