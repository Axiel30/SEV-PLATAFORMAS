#pragma once
#include "Actor.h"
#include "Animation.h" 
class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	Enemy(string filename, float x, float y, Game* game);
	virtual ~Enemy();
	virtual void draw(float scrollX = 0) override; // Va a sobrescribir
	virtual void update();
	virtual void impacted(); // Recibe impacto y pone animacion de morir
	virtual bool isStompable() const { return stompable; }
	bool stompable = false;
	float vxIntelligence;
	int state;
	Animation* aDying;
	Animation* aMoving;
	Animation* animation; // Referencia a la animacion mostrada
};