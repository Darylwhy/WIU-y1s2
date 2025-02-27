#ifndef BOWLINGBALL_H
#define BOWLINGBALL_H

#include "Vector3.h"
#include "PhysicsObject.h"
#include "CircleCollider.h"

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

class bowlingBalls : public PhysicsObject
{
private:
	CircleCollider hitbox;

public:
	//Note: Overload a game object's set and translate functions if the object has a hitbox so that the hibox gets moved
	void translateObject(Vector3 translation);
	void translateObject(glm::vec3 translation);
	void setObjectPosition(Vector3 Position);
	CircleCollider& getHitbox();

	bowlingBalls(Vector3 Position, float Radius, float Gravity);
	~bowlingBalls();
};

#endif