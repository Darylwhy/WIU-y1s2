#include "bowlingBalls.h"

void bowlingBalls::translateObject(Vector3 translation)
{
	translatePosition(translation);
	hitbox.translate(translation);
}

void bowlingBalls::translateObject(glm::vec3 translation)
{
	translatePosition(translation);
	hitbox.translate(translation);
}

void bowlingBalls::setObjectPosition(Vector3 Position)
{
	setPosition(Position);
	hitbox.setOrigin(Position);
}

CircleCollider& bowlingBalls::getHitbox()
{
	return hitbox;
}



bowlingBalls::bowlingBalls(Vector3 Position, float Radius, float Gravity) : PhysicsObject(Position, Gravity), hitbox(Position, Radius)
{
	setMass(10);
}

bowlingBalls::~bowlingBalls()
{
}