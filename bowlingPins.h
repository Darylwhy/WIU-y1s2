#ifndef BOWLINGPINS_H
#define BOWLINGPINS_H

#include "Vector3.h"
#include "PhysicsObject.h"
#include "CircleCollider.h"
#include "BoxCollider.h"

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>
//
//class bowlingPins : public PhysicsObject
//{
//private:
//	CircleCollider hitbox;
//	char hit;
//
//public:
//	//Note: Overload a game object's set and translate functions if the object has a hitbox so that the hibox gets moved
//	void translateObject(Vector3 translation);
//	void translateObject(glm::vec3 translation);
//	void setObjectPosition(Vector3 Position);
//	CircleCollider& getHitbox();
//
//	void setHit();
//	int getHit();
//	bowlingPins(Vector3 Position, float Radius, float Gravity);
//	~bowlingPins();
//};
//
//#endif

class bowlingPins : public PhysicsObject
{
private:
	BoxCollider hitbox;
	char hit;

public:
	//Note: Overload a game object's set and translate functions if the object has a hitbox so that the hibox gets moved
	void translateObject(Vector3 translation);
	void translateObject(glm::vec3 translation);
	void setObjectPosition(Vector3 Position);

	BoxCollider& getHitbox();
	bool HitCheck();
	void setHit(int check);
	bowlingPins(Vector3 Position, float Gravity, float HalfLength, float HalfBreath, float HalfHeight);
	~bowlingPins();
};

#endif