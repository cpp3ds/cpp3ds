#ifndef SCENE_H
#define SCENE_H

#include <cpp3ds/BottomScreen.h>
#include <cpp3ds/TopScreen.h>
#include <cpp3ds/Input.h>

namespace cpp3ds {

	class Scene {
	protected:
		uint64_t lastTime;
	public:
		BottomScreen bottomScreen;
		TopScreen topScreen;
		Input input;
		virtual int render(float deltaTime) = 0;
		int run();
	};

}

#endif
