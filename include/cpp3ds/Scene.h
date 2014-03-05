#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>
#include <cpp3ds/BottomScreen.h>
#include <cpp3ds/TopScreen.h>
#include <cpp3ds/Input.h>

namespace cpp3ds {

	/**
	 * Handles the main game loop for you.
	 */
	class Scene {
	protected:
		uint64_t lastTime;
	public:
		BottomScreen bottomScreen;
		TopScreen topScreen;
		Input input;
		void display();
		virtual int update(float deltaTime) = 0;
		virtual void render() = 0;
		int run(bool useDisplay = true);
	};

}

#endif
