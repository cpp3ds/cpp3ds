#include <stddef.h>
#include <cpp3ds/actors/Actor.h>
#include <cpp3ds/Stage.h>

namespace cpp3ds {
	namespace Actors {

		Actor::Actor(float x, float y, float depth3d) {
			stageHead = NULL;
			stageTail = NULL;
			this->x = x;
			this->y = y;
			this->depth3d = depth3d;
		}
		
		void Actor::addStage(Stage& stage){
			StageNode *node = new StageNode();
			node->stage = &stage;
			node->next = NULL;
			if (stageHead == NULL)
				stageHead = node;
			if (stageTail != NULL)
				stageTail->next = node;
			stageTail = node;
		}

		void Actor::detach(){
			// Remove this actor from all stages
			StageNode* cur = stageHead;
			while (cur != NULL) {
				cur->stage->removeActor(*this);
				cur = cur->next;
			}
		}

		Actor::~Actor(){
			detach();
		}

	}
}
