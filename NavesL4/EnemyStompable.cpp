#include "EnemyStompable.h"
EnemyStompable::EnemyStompable(float x, float y, Game* game)
	: Enemy("res/enemigo_pisable.png", x, y, game) {
	stompable = true;
	aDying = new Animation("res/enemigo_pisable_morir.png", width, height,
		280, 40, 6, 8, false, game);
	aMoving = new Animation("res/enemigo_pisable_movimiento.png", width, height,
		108, 40, 6, 3, true, game);
	animation = aMoving;
}