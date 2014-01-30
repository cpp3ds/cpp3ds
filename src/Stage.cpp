#include <stddef.h>
#include <cpp3ds/Stage.h>
#include <cpp3ds/utils.h>
#include <cpp3ds/actors/Actor.h>

namespace cpp3ds {

	Stage::Stage(){
		actorHead = NULL;
		actorTail = NULL;
	}

	void Stage::addActor(Actors::Actor& actor){
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

	void Stage::removeActor(Actors::Actor& actor){
		ActorNode* cur = actorHead;
		while (cur != NULL) {
			if (cur->actor == &actor) {
				cur->prev->next = cur->next;
				cur->next->prev = cur->prev;
				delete cur;
				break;
			}
			cur = cur->next;
		}
	}

	void Stage::draw(Screen& screen, int x, int y) {
		// Loop through and draw all actors on the stage
		ActorNode* cur = actorTail;
		while (cur != NULL) {
			cur->actor->draw(screen, this->x + x, this->y + y);
			cur = cur->prev;
		}
	}

}
