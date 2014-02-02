#include <stdint.h>
#include <cpp3ds/Scene.h>
#include <cpp3ds/utils.h>

namespace cpp3ds {
	
	int Scene::run(){
		int ret = 0;
		uint64_t deltaTime;
		lastTime = GetSystemTick();
		while (ret == 0) {
			deltaTime = GetSystemTick() - lastTime;
			lastTime = GetSystemTick();
			ret = render(deltaTime);
		}
		return ret;
	}

}
