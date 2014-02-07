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
		float width, height;
		ActorNode *actorHead, *actorTail;
	public:
		Stage(): Actors::Actor(0,0,0), actorHead(NULL), actorTail(NULL) {}
		void addActor(Actors::Actor& actor);
		void removeActor(Actors::Actor& actor);
		virtual void draw(Screen& screen, float x = 0, float y = 0, bool use3D = true, bool isLeftside = true);
	};
}

#endif
