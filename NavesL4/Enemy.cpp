#include "Enemy.h"

Enemy::~Enemy() {
	if (aDying != nullptr) {
		delete aDying;
		aDying = nullptr;
	}
	if (aMoving != nullptr) {
		delete aMoving;
		aMoving = nullptr;
	}
}

Enemy::Enemy(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, 36, 40, game) {

	stompable = false;
	state = game->stateMoving;
	aDying = nullptr;
	aMoving = nullptr;
	animation = nullptr;

	vx = 1;
	vxIntelligence = -1;
	vx = vxIntelligence;
}

Enemy::Enemy(float x, float y, Game* game)
	: Enemy("res/enemigo.png", x, y, game) {

	aDying = new Animation("res/enemigo_morir.png", width, height,
		280, 40, 6, 8, false, game);

	aMoving = new Animation("res/enemigo_movimiento.png", width, height,
		108, 40, 6, 3, true, game);
	animation = aMoving;
}

void Enemy::update() {
	// Actualizar la animacion
	bool endAnimation = animation->update();

	// Acabo la animacion, no sabemos cual
	if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateDying) {
			state = game->stateDead;
		}
	}

	if (state == game->stateMoving) {
		animation = aMoving;
	}
	if (state == game->stateDying) {
		animation = aDying;
	}

	// Establecer velocidad
	if (state != game->stateDying) {
		// no esta muerto y se ha quedado parado
		if (vx == 0) {
			vxIntelligence = vxIntelligence * -1;
			vx = vxIntelligence;
		}
		if (outRight) {
			// mover hacia la izquierda vx tiene que ser negativa
			if (vxIntelligence > 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
		}
		if (outLeft) {
			// mover hacia la derecha vx tiene que ser positiva
			if (vxIntelligence < 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
		}
	}
	else {
		vx = 0;
	}
}

void Enemy::impacted() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}

void Enemy::draw(float scrollX) {
	if (animation != nullptr) {
		animation->draw(x - scrollX, y);
	}
}