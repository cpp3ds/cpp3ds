#ifndef SCENE_H
#define SCENE_H

#include <cpp3ds/Scene.h>
#include <cpp3ds/BottomScreen.h>
#include <cpp3ds/TopScreen.h>
#include <cpp3ds/Input.h>

namespace cpp3ds {

	class Scene {
	protected:

	public:
		BottomScreen bottomScreen;
		TopScreen topScreen;
		Input input;
		virtual int run() = 0;
	};

}

#endif
