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
			int x, y;
			StageNode *stageHead, *stageTail;
		public:
			Actor(int x = 0, int y = 0, int depth3d = 0);
			void addStage(Stage& stage);
			void detach();
			~Actor();
		};

	}
}

#endif
