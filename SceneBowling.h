#ifndef SCENE_BOWLING_H
#define SCENE_BOWLING_H

#include "BaseScene.h"
#include "Mesh.h"
#include "FPCamera.h"
#include "MatrixStack.h"
#include "Light.h"


//GameObjects
#include "Ground.h"
#include "bowlingBalls.h"
#include "bowlingPins.h"
#include "MapProps.h"
#include "MapProps2.h"
#include "Sign.h"
#include "TornPage2.h"
#include "Inventory.h"



class SceneBowling : public BaseScene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_SPHERE,
		GEO_CUBE,
		GEO_PLANE,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_TEXT,
		GEO_TEST,
		GEO_TEST2,
		GEO_PINS,
		GEO_BALL,
		GEO_FENCE,
		GEO_TORCH,
		GEO_TREE,
		GEO_BALLBOX,
		GEO_FLOOR,
		GEO_QUAD,
		GEO_ROAD,
		GEO_SIGN,
		GEO_PAGE,
		NUM_GEOMETRY,
	};

	SceneBowling() {};
	~SceneBowling();

	void Init();
	void Update(double dt);
	void Render();
	void Exit();
	void InitLights();

private:
	//Render functions
	void HandleKeyPress();
	void RenderSkybox();
	void RenderFloor();
	void SetUpPins();
	void SetUpPins2();
	void lightDisabler();
	bool inRange();

	bool ball2Pin2Pin();
	void RenderBox();
	bool holdBall();
	void Die();
	int grabbed;
	int Used = 0;
	void RenderBlack();
	bool Won();
	void RenderInventory();

	bool Pin2Pin;
	int Round = 0;
	void RenderTextOnScreen2(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y);
	int PinCount;
	int PinCounter();
	void goalChecker();
	
	//meshlist
	Mesh* meshList[NUM_GEOMETRY];

	//Light settings
	static const int NUM_LIGHTS = 4;
	Light light[NUM_LIGHTS];
	bool enableLight;

	//Game objects
	bowlingBalls* balls[1];
	bowlingPins* pins[10];
	Player* player;
	MapProps* props[33];//From No.31  Onwards will just be tree
	MapProps2* props2[80];
	float ballCD;

	Sign* sign;

	static const int NUM_IOBJS = 1;
	InteractableObject* interactableObjs[NUM_IOBJS];
	float interactionCD;
	TornPage2* page;
	bool pickedUpPage;
};

#endif