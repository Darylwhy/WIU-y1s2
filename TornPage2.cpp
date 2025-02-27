#include "TornPage2.h"

//Standard file reading libraries
#include <iostream>
#include <sstream>

#include "KeyboardController.h"
#include "MouseController.h"
#include "Inventory.h"

//Include GLFW
#include <GLFW/glfw3.h>

void TornPage2::interact(Player& Player)
{
	setCurrDialogue(0);
	Inventory::GetInstance().addItem("newspaper1");
	return;
}

float TornPage2::getRadius()
{
	return radius;
}

TornPage2::TornPage2(Vector3 Position, std::string Name) : InteractableObject(Name, Position)
{
	radius = 3.f;
}

TornPage2::~TornPage2()
{
}