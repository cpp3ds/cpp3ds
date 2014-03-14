#include <stddef.h>
#include <cpp3ds/Graphics/Actor.hpp>
#include <cpp3ds/Graphics/Stage.hpp>

namespace cpp3ds {
		
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

	int Actor::getValues(int tweenType, float *returnValues){
		switch (tweenType) {
			case POSITION_X: returnValues[0] = getX(); return 1;
			case POSITION_Y: returnValues[0] = getY(); return 1;
			case POSITION_XY:
				returnValues[0] = getX();
				returnValues[1] = getY();
				return 2;
			default: return -1;
		}
	}

	void Actor::setValues(int tweenType, float *newValues){
		switch (tweenType) {
			case POSITION_X: setX(newValues[0]); break;
			case POSITION_Y: setY(newValues[0]); break;
			case POSITION_XY:
				setX(newValues[0]);
				setY(newValues[1]);
				break;
			default: break;
		}
	}

}
