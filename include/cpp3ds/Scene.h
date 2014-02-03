#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>
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
		virtual int update(float deltaTime) = 0;
		virtual void render() = 0;
		int run();
	};

}

#endif
