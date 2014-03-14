#include <stddef.h>
#include <cpp3ds/Graphics/Stage.hpp>
#include <cpp3ds/Graphics/Actor.hpp>
#include <cpp3ds/Window/Input.hpp>
#include <cpp3ds/System/utils.hpp>

namespace cpp3ds {

	void Stage::addActor(Actor& actor){
		if (this == &actor)
			return; // Avoid infinite recursion
		removeActor(actor); // Remove a potential duplicate
		ActorNode *node = new ActorNode();
		node->actor = &actor;
		node->prev = actorTail;
		node->next = NULL;
		if (actorHead == NULL)
			actorHead = node;
		if (actorTail != NULL)
			actorTail->next = node;
		actorTail = node;
		actor.addStage(*this);
	}

	void Stage::removeActor(Actor& actor){
		ActorNode* cur = actorHead;
		while (cur != NULL) {
			if (cur->actor == &actor) {
				if (cur == actorHead)
					actorHead = cur->next;
				if (cur == actorTail)
					actorTail = cur->prev;
				if (cur->prev != NULL)
					cur->prev->next = cur->next;
				if (cur->next != NULL)
					cur->next->prev = cur->prev;
				delete cur;
				break;
			}
			cur = cur->next;
		}
	}

	void Stage::draw(Screen& screen, float x, float y, bool use3D, bool isLeftside) {
		// Loop through and draw all actors on the stage
		ActorNode* cur = actorTail;
		x += this->x;
		y += this->y;
		while (cur != NULL) {
			Actor& actor = *cur->actor;
			if (use3D) {
				float slider = Input::get3DSlider() * actor.depth3d;
				actor.draw(screen, (isLeftside) ? x-slider : x+slider, y, use3D, isLeftside);
			} else {
				actor.draw(screen, x, y, use3D, isLeftside);
			}
			cur = cur->prev;
		}
	}

}
