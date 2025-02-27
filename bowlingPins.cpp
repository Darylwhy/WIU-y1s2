#include "bowlingPins.h"

//void bowlingPins::translateObject(Vector3 translation)
//{
//	translatePosition(translation);
//	hitbox.translate(translation);
//}
//
//void bowlingPins::translateObject(glm::vec3 translation)
//{
//	translatePosition(translation);
//	hitbox.translate(translation);
//}
//
//void bowlingPins::setObjectPosition(Vector3 Position)
//{
//	setPosition(Position);
//	hitbox.setOrigin(Position);
//}
//
//CircleCollider& bowlingPins::getHitbox()
//{
//	return hitbox;
//}
//
//void bowlingPins::setHit()
//{
//	hit = false;
//}
//
//int bowlingPins::getHit()
//{
//	return hit;
//}
//
//
//
//bowlingPins::bowlingPins(Vector3 Position, float Radius, float Gravity) : PhysicsObject(Position, Gravity), hitbox(Position, Radius)
//{
//	hit = hit;
//}
//
//bowlingPins::~bowlingPins()
//{
//}

void bowlingPins::translateObject(Vector3 translation)
{
	translatePosition(translation);
	hitbox.translate(translation);
}

void bowlingPins::translateObject(glm::vec3 translation)
{
	translatePosition(translation);
	hitbox.translate(translation);
}

void bowlingPins::setObjectPosition(Vector3 Position)
{
	setPosition(Position);
	hitbox.setPosition(Position);
}

BoxCollider& bowlingPins::getHitbox()
{
	return hitbox;
}

bool bowlingPins::HitCheck()
{
	return hit;  	
}


void bowlingPins::setHit(int check)
{
	if (check == 1)
		hit = true;
	
	else
		hit = false;
}

bowlingPins::bowlingPins(Vector3 Position, float Gravity, float HalfLength, float HalfBreath, float HalfHeight) : PhysicsObject(Position, Gravity), hitbox(Position, HalfLength, HalfBreath, HalfHeight)
{
	setMass(1);
}

bowlingPins::~bowlingPins()
{
}

//3 Rounds
//2 Chances
// 1 Shot = 1 Candle
//Basic Condition for winning - Hit one pin
//Condition to keep light - Do 1 Safe
//Strike Loses 0 Lights