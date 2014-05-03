#ifndef CPP3DS_SCENE_HPP
#define CPP3DS_SCENE_HPP

#include <inttypes.h>
#include <cpp3ds/Window/BottomScreen.hpp>
#include <cpp3ds/Window/TopScreen.hpp>
//#include <cpp3ds/Window/Keyboard.hpp>
#include <cpp3ds/Window/EventManager.hpp>

namespace cpp3ds {

	/**
	 * Handles the main game loop for you.
	 */
	class Scene {
	protected:
		uint64_t lastTime;
	public:
		virtual ~Scene() {}
		BottomScreen bottomScreen;
		TopScreen topScreen;
//		Input input;
		EventManager events;
		void display();
		virtual int update(float deltaTime) = 0;
		virtual void render() = 0;
		int run(bool useDisplay = true);
	};

}

#endif
