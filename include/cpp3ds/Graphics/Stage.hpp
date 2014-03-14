#ifndef CPP3DS_STAGE_HPP
#define CPP3DS_STAGE_HPP

#include <cpp3ds/Window/Screen.hpp>
#include <cpp3ds/Graphics/Actor.hpp>

namespace cpp3ds {

	struct ActorNode{
		Actor* actor;
		ActorNode* prev;
		ActorNode* next;
	};

	class Stage: public Actor {
	protected:
		float width, height;
		ActorNode *actorHead, *actorTail;
	public:
		Stage(): Actor(0,0,0), actorHead(NULL), actorTail(NULL) {}
		void addActor(Actor& actor);
		void removeActor(Actor& actor);
		virtual void draw(Screen& screen, float x = 0, float y = 0, bool use3D = true, bool isLeftside = true);
	};
}

#endif
