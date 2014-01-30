#ifndef STAGE_H
#define STAGE_H

#include <cpp3ds/Screen.h>
#include <cpp3ds/actors/Actor.h>

namespace cpp3ds {

	struct ActorNode{
		Actors::Actor* actor;
		ActorNode* prev;
		ActorNode* next;
	};

	class Stage: public Actors::Actor {
	protected:
		int width, height;
		ActorNode *actorHead, *actorTail;
	public:
		Stage();
		void addActor(Actor& actor);
		void removeActor(Actor& actor);
		virtual void draw(Screen& screen, int x = 0, int y = 0);
	};
}

#endif
