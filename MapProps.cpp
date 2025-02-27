#include "MapProps.h"

void MapProps::translateObject(Vector3 translation)
{
	translatePosition(translation);
	hitbox.translate(translation);
}

void MapProps::translateObject(glm::vec3 translation)
{
	translatePosition(translation);
	hitbox.translate(translation);
}

void MapProps::setObjectPosition(Vector3 Position)
{
	setPosition(Position);
	hitbox.setPosition(Position);
}

BoxCollider& MapProps::getHitbox()
{
	return hitbox;
}



MapProps::MapProps(Vector3 Position, float Gravity, float HalfLength, float HalfBreath, float HalfHeight) : PhysicsObject(Position, Gravity), hitbox(Position, HalfLength, HalfBreath, HalfHeight)
{
	setBounciness(0);
	setMass(0);
}
MapProps::~MapProps()
{
}
