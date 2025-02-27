#ifndef MAPROP2_H
#define MAPPROP2_H

#include "Vector3.h"
#include "GameObject.h"
#include "BoxCollider.h"

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

class MapProps2 : public GameObject
{
private:
	
public:
	//Note: Overload a game object's set and translate functions if the object has a hitbox so that the hibox gets moved


	MapProps2(Vector3 Position);
	~MapProps2();
};

#endif