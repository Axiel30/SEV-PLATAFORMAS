#include "MovingPlatform.h"
#include "GameLayer.h"

MovingPlatform::MovingPlatform(float x, float y, Game* game, float speed)
	: Actor("res/bloque_metal.png", x, y, 40, 32, game) {
	this->speed = speed;
	this->vx = speed;
	this->vy = 0;
	this->stopped = false;
}

bool MovingPlatform::isActorOnPlatform(Actor* actor) {
	if (actor == nullptr) {
		return false;
	}

	int topPlatform = y - height / 2;
	int leftPlatform = x - width / 2;
	int rightPlatform = x + width / 2;

	int bottomActor = actor->y + actor->height / 2;
	int leftActor = actor->x - actor->width / 2;
	int rightActor = actor->x + actor->width / 2;

	// Comprobar solapamiento horizontal entre el actor y la plataforma
	bool horizontalOverlap = (rightActor > leftPlatform) && (leftActor < rightPlatform);

	// Comprobar que los pies del actor esten en la superficie superior de la plataforma
	bool verticalTouch = abs(bottomActor - topPlatform) <= 3;

	return horizontalOverlap && verticalTouch;
}

void MovingPlatform::update() {
	GameLayer* layer = (GameLayer*)game->gameLayer;
	if (layer == nullptr) {
		return;
	}

	// 1. Detectar que actores dinamicos estan sobre la plataforma antes del movimiento
	bool playerOn = isActorOnPlatform(layer->player);

	list<Enemy*> enemiesOn;
	for (auto const& enemy : layer->enemies) {
		if (enemy->state != game->stateDying && enemy->state != game->stateDead) {
			if (isActorOnPlatform(enemy)) {
				enemiesOn.push_back(enemy);
			}
		}
	}

	// 2. Comprobar colision en el eje X antes de avanzar
	float nextX = x + vx;

	// Limites del mapa
	if (nextX - width / 2 <= 0) {
		vx = abs(speed);
	}
	else if (nextX + width / 2 >= layer->mapWidth) {
		vx = -abs(speed);
	}

	int topThis = y - height / 2;
	int bottomThis = y + height / 2;

	// Colision con tiles estaticos (bloques de tierra / paredes)
	for (auto const& tile : layer->tiles) {
		int topTile = tile->y - tile->height / 2;
		int bottomTile = tile->y + tile->height / 2;

		if (topThis < bottomTile && bottomThis > topTile) {
			int leftTile = tile->x - tile->width / 2;
			int rightTile = tile->x + tile->width / 2;

			int leftThis = nextX - width / 2;
			int rightThis = nextX + width / 2;

			if (rightThis > leftTile && leftThis < rightTile) {
				if (vx > 0) {
					vx = -abs(speed);
				}
				else if (vx < 0) {
					vx = abs(speed);
				}
				break;
			}
		}
	}

	// Colision con otras plataformas moviles
	for (auto const& other : layer->movingPlatforms) {
		if (other == this) continue;

		int topOther = other->y - other->height / 2;
		int bottomOther = other->y + other->height / 2;

		if (topThis < bottomOther && bottomThis > topOther) {
			int leftOther = other->x - other->width / 2;
			int rightOther = other->x + other->width / 2;

			int leftThis = nextX - width / 2;
			int rightThis = nextX + width / 2;

			if (rightThis > leftOther && leftThis < rightOther) {
				if (vx > 0) {
					vx = -abs(speed);
				}
				else if (vx < 0) {
					vx = abs(speed);
				}
				break;
			}
		}
	}

	// 3. Mover la plataforma
	x += vx;
	float deltaX = vx;

	// 4. Mover al jugador si esta sobre la plataforma (evitando empujarlo dentro de paredes)
	if (playerOn && layer->player != nullptr) {
		float nextPlayerX = layer->player->x + deltaX;
		bool playerHitsWall = false;

		int topPlayer = layer->player->y - layer->player->height / 2;
		int bottomPlayer = layer->player->y + layer->player->height / 2;

		for (auto const& tile : layer->tiles) {
			int topTile = tile->y - tile->height / 2;
			int bottomTile = tile->y + tile->height / 2;

			if (topPlayer < bottomTile && bottomPlayer > topTile) {
				int leftTile = tile->x - tile->width / 2;
				int rightTile = tile->x + tile->width / 2;
				int leftPlayer = nextPlayerX - layer->player->width / 2;
				int rightPlayer = nextPlayerX + layer->player->width / 2;

				if (rightPlayer > leftTile && leftPlayer < rightTile) {
					playerHitsWall = true;
					break;
				}
			}
		}

		if (!playerHitsWall) {
			layer->player->x += deltaX;
		}
	}

	// 5. Mover a los enemigos que esten sobre la plataforma
	for (auto const& enemy : enemiesOn) {
		float nextEnemyX = enemy->x + deltaX;
		bool enemyHitsWall = false;

		int topEnemy = enemy->y - enemy->height / 2;
		int bottomEnemy = enemy->y + enemy->height / 2;

		for (auto const& tile : layer->tiles) {
			int topTile = tile->y - tile->height / 2;
			int bottomTile = tile->y + tile->height / 2;

			if (topEnemy < bottomTile && bottomEnemy > topTile) {
				int leftTile = tile->x - tile->width / 2;
				int rightTile = tile->x + tile->width / 2;
				int leftEnemy = nextEnemyX - enemy->width / 2;
				int rightEnemy = nextEnemyX + enemy->width / 2;

				if (rightEnemy > leftTile && leftEnemy < rightTile) {
					enemyHitsWall = true;
					break;
				}
			}
		}

		if (!enemyHitsWall) {
			enemy->x += deltaX;
		}
	}
}
