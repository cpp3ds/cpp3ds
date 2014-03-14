#ifndef CPP3DS_ACTOR_HPP
#define CPP3DS_ACTOR_HPP

#include <stddef.h>
#include <cpp3ds/Graphics/Drawable.hpp>
#include <TweenEngine/Tweenable.h>

namespace cpp3ds {

	class Stage; // Forward-declaration for cyclic reference

	struct StageNode{
		Stage* stage;
		StageNode* next;
	};

	class Actor: public Drawable, public TweenEngine::Tweenable {
	protected:
		float x, y;
		StageNode *stageHead, *stageTail;
	public:
		Actor(float x, float y, float depth3d):
			Drawable(depth3d), x(x), y(y), stageHead(NULL), stageTail(NULL) {}
		void setX(float x) { this->x = x; }
		void setY(float y) { this->y = y; }
		float getX() { return x; }
		float getY() { return y; }
		void move(float offsetX, float offsetY){
			this->x += offsetX;
			this->y += offsetY;
		}
		void setPosition(float x, float y, float duration = 0);
		void addStage(Stage& stage);
		void detach();
		~Actor();

		static const int POSITION_X  = 1;
		static const int POSITION_Y  = 2;
		static const int POSITION_XY = 3;

		virtual int getValues(int tweenType, float *returnValues);
		virtual void setValues(int tweenType, float *newValues);
	};

}

#endif
