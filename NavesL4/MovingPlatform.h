#pragma once

#include "Actor.h"

class MovingPlatform : public Actor
{
public:
	MovingPlatform(float x, float y, Game* game, float speed = 2.0f);
	void update();
	bool isActorOnPlatform(Actor* actor);

	float speed;
	bool stopped;
};
