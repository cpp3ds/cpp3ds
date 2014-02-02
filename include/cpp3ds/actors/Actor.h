#ifndef ACTOR_H
#define ACTOR_H

#include <cpp3ds/Drawable.h>

namespace cpp3ds {

	class Stage; // Forward-declaration for cyclic reference

	namespace Actors {

		struct StageNode{
			Stage* stage;
			StageNode* next;
		};

		class Actor: public Drawable {
		protected:
			float x, y;
			StageNode *stageHead, *stageTail;
		public:
			Actor(float x = 0, float y = 0, float depth3d = 0);
			void addStage(Stage& stage);
			void detach();
			~Actor();
		};

	}
}

#endif
