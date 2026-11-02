#pragma once
#include "Enemy.h"
class EnemyStompable : public Enemy
{
public:
	EnemyStompable(float x, float y, Game* game);
	bool isStompable() const override { return true; }
};
