#include "SceneBowling.h"
#include "GL\glew.h"

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>
// Audio

//Include GLFW
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "KeyboardController.h"
#include "LoadTGA.h"
#include "SceneManager.h"
#include "CollisionDetection.h"
#include <MouseController.h>
#include "Inventory.h"



SceneBowling::~SceneBowling()
{
}

void SceneBowling::Init()
{
	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	/*glEnable(GL_CULL_FACE);*/


	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Initialis elights

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shader programs
	//m_programID = LoadShaders("Shader//Texture.vertexshader",
		//"Shader//Texture.fragmentshader");
	m_programID = LoadShaders("Shader//Texture.vertexshader",
		"Shader//Text.fragmentshader");
	glUseProgram(m_programID);
	


	// Initialise camera properties
	//Initialise player
	player = new Player(Vector3(0, 1, -20));
	// Initialise camera properties
	camera.Init((player->getPosition() + Vector3{ 1.f, 3.f, 0.f }).convert2glm(),
		(player->getPosition() + Vector3{ 2.f, 0.f, 0.f }).convert2glm());
	InitLights();
	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", glm::vec3(1.f, 1.f, 1.f), 1.f, 16, 16);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", glm::vec3(0.5f, 0.5f, 0.5f), 1.f);


	//Generate skybox
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 200.f);
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 200.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//nightsky_lf.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 200.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//nightsky_rt.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 200.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//nightsky_up.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 200.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//nightsky_dn.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 200.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//nightsky_bk.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 200.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Images//nightsky_ft.tga");
	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("bowlingfloor", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Images//bowlingfloor.tga");
	meshList[GEO_PLANE] = MeshBuilder::GenerateQuad("drygrass", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_PLANE]->textureID = LoadTGA("Images//drygrass.tga");
	meshList[GEO_BALL] = MeshBuilder::GenerateOBJ("ball", "Models//bowlingball.obj");
	meshList[GEO_BALL]->textureID = LoadTGA("Images//bowlingball.tga");
	meshList[GEO_PINS] = MeshBuilder::GenerateOBJ("pins", "Models//pins.obj");
	meshList[GEO_PINS]->textureID = LoadTGA("Images//pins.tga");
	meshList[GEO_FENCE] = MeshBuilder::GenerateOBJ("fence", "Models//oldfence.obj");
	meshList[GEO_FENCE]->textureID = LoadTGA("Images//oldfence.tga");
	meshList[GEO_TORCH] = MeshBuilder::GenerateOBJ("Plane", "Models//torch.obj");
	meshList[GEO_TORCH]->textureID = LoadTGA("Images//torch.tga");
	meshList[GEO_BALLBOX] = MeshBuilder::GenerateOBJ("box", "Models//box.obj");
	meshList[GEO_BALLBOX]->textureID = LoadTGA("Images//box.tga");
	meshList[GEO_TREE] = MeshBuilder::GenerateOBJ("Tree", "Models//PineTree.obj");
	meshList[GEO_TREE]->textureID = LoadTGA("Images//tree.tga");
	meshList[GEO_ROAD] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 200.f);
	meshList[GEO_ROAD]->textureID = LoadTGA("Images//path.tga");
	meshList[GEO_SIGN] = MeshBuilder::GenerateOBJ("Tree", "Models//sign.obj");
	meshList[GEO_SIGN]->textureID = LoadTGA("Images//sign.tga");
	meshList[GEO_PAGE] = MeshBuilder::GenerateQuad("paper", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_PAGE]->textureID = LoadTGA("Images//tornpaper.tga");
	// For models with mtl files, put models below the following line
	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR],	m_parameters[U_MATERIAL_SHININESS]);

	// 16 x 16 is the number of columns and rows for the text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Images//calibri.tga");

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);




	enableLight = true;


	for (int i = 0; i < 1; i++) {
		balls[i] = nullptr;
	}
	ballCD = 0;
	for (int i = 0; i < 10; i++) {
		pins[i] = nullptr;
	}
	
	SetUpPins();

	//candles
	props[0] = new MapProps(Vector3(8, 0, -4), 0, 12.5, 12.5, 1);
	props[1] = new MapProps(Vector3(-8, 0, -4), 0, 12.5, 12.5, 1);
	props[2] = new MapProps(Vector3(8, 0, 25), 0, 12.5, 12.5, 1);
	props[3] = new MapProps(Vector3(-8, 0, 25), 0, 12.5, 12.5, 1);
	//box
	props[21] = new MapProps(Vector3(-5, .1, -8), 0, 1.25, 1.25, 1.5);

	//Fence
	props[16] = new MapProps(Vector3(8, 0, -12.5), 0, 6, 2.5, 1);
	props[17] = new MapProps(Vector3(-8, 0, -12.5), 0, 6, 2.5, 1);
	for (int i = 18; i < 21; i++) {
		int x = 8;
		int Multiply = i - 19;
		props[i] = new MapProps(Vector3(x * Multiply, 0, 37.5), 0, 6, 2.5, 1);
	}
	for (int i = 4; i < 10; i++) 
	{
		int z = 8;
		int Multiply = i - 5 ;
		props[i] = new MapProps(Vector3(12.5, 0, z * Multiply), 0, 1, 2.5, 6);
	}
	for (int i = 10; i < 16; i++)
	{
		int z = 8;
		int Multiply = i - 11;
		props[i] = new MapProps(Vector3(-12.5, 0, z * Multiply), 0, 1, 2.5, 6);
	}

	props[23] = new MapProps(Vector3(-2.8, 0, -18), 0, 1, 2.5, 5);
	props[24] = new MapProps(Vector3(-2.8, 0, -27), 0, 1, 2.5, 5);

	props[25] = new MapProps(Vector3(2.8, 0, -18), 0, 1, 2.5, 5);
	props[26] = new MapProps(Vector3(2.8, 0, -27), 0, 1, 2.5, 5);

	props[27] = new MapProps(Vector3(-2.8, 0, 43), 0, 1, 2.5, 5);
	props[28] = new MapProps(Vector3(-2.8, 0, 52), 0, 1, 2.5, 5);

	props[29] = new MapProps(Vector3(2.8, 0, 43), 0, 1, 2.5, 5);
	props[30] = new MapProps(Vector3(2.8, 0, 52), 0, 1, 2.5, 5);

	props[31] = new MapProps(Vector3(-6, 0, 70), 0, 1, 2.5, 5);
	props[32] = new MapProps(Vector3(6,  0, 70), 0, 1, 2.5, 5);
	//BlockPlayerfromPassing
	props[22] = new MapProps(Vector3(0, 0, 0), 0, 11, 1, 2);

	//Trees
	for (int i = 0; i < 5; i++)
	{
		int z = -12.5;
		int v = i * 4;
		props2[i] =  new MapProps2(Vector3(-15, 0, z-v));
	}
	for (int i = 5; i < 10; i++)
	{
		int z = -12.5;
		int v = (i - 4) * 4;
		props2[i] = new MapProps2(Vector3(15, 0, z - v));
	}
	for (int i = 10; i < 25;i++) {
		int z = -32;
		int v = (i - 9) * 7.5;
		props2[i] = new MapProps2(Vector3(30, 0, z+v));
	}
	for (int i = 25; i < 40; i++) {
		int z = -32;
		int v = (i - 24) * 7.5;
		props2[i] = new MapProps2(Vector3(-30, 0, z + v));
	}
	for (int i = 40; i < 55; i++) {
		int z = -36;
		int v = (i - 39) * 7.5;
		props2[i] = new MapProps2(Vector3(-25, 0, z + v));
	}
	for (int i = 55; i < 70; i++) {
		int z = -36;
		int v = (i - 54) * 7.5;
		props2[i] = new MapProps2(Vector3(25, 0, z + v));
	}

	for (int i = 70; i < 75; i++)
	{
		int z = 57;
		int v = (i- 69) * 4;
		props2[i] = new MapProps2(Vector3(-15, 0, z + v));
	}
	for (int i = 75; i < 80; i++)
	{
		int z = 57;
		int v = (i-74) * 4;
		props2[i] = new MapProps2(Vector3(15, 0, z + v));
	}
	 sign = new Sign(Vector3(4,0,-8), "Sign2");
	 interactionCD = 0;
	 page = new TornPage2(Vector3(10,.1,50), "Torn record page 2");
}

void SceneBowling::Update(double dt)
{

	HandleKeyPress();
	lightDisabler();

	ballCD -= dt;
	interactionCD -= dt;

	if (interactionCD <= 0.f) {
		if (KeyboardController::GetInstance()->IsKeyPressed('E')) {
			if (page != nullptr) {
				if ((page->getPosition() - player->getPosition()).Length() < page->getRadius()) {
					page->interact(*player);
					currInteraction = page;
					currGameState = DIALOGUE;
					interactionCD = 1.5f;
					return;
				}
			}
			if (sign != nullptr) {
				if ((sign->getPosition() - player->getPosition()).Length() < sign->getRadius()) {
					sign->interact(*player);
					currInteraction = sign;
					currGameState = DIALOGUE;
					interactionCD = 1.5f;
					return;
				}
			}
		}
	}

	//BallThrow
	if (Used < 4) {
		if (holdBall()) {
			if (MouseController::GetInstance()->IsButtonPressed(0)) {
				if (balls[0] == nullptr) {
					balls[0] = new bowlingBalls{ player->getPosition(), 1.5f, 100.f };
					balls[0]->AddImpulse(Vector3(normalize(camera.target - camera.position)) * 300.f);
					balls[0]->setBounciness(0);
					grabbed = 0;
				}
			}
		}
	}

	goalChecker();	
	//cheatcode - modify however you need (INSTANT WIN :money_face:)
	if (KeyboardController::GetInstance()->IsKeyPressed('Z')) {
		for (int i = 0; i < 10; i++) {
			pins[i] = nullptr;
		}

	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                  BowlingBall collisions                                            //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Collision Between ball and ground
	if (balls[0] != nullptr) {
			for (int j = 0; j < 2; j++) {
				OverlapSphere2Ground(*balls[0], balls[0]->getHitbox().getRadius(), 0);
			}
			balls[0]->UpdatePhysics(dt);
			if ((balls[0]->getPosition().x > 50) || (balls[0]->getPosition().x < -50) ||
				(balls[0]->getPosition().y > 50) || (balls[0]->getPosition().y < -50) ||
				(balls[0]->getPosition().z > 50) || (balls[0]->getPosition().z < -50)) {
				delete balls[0];
				balls[0] = nullptr;
		}
	}
	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                                                  Pins collisions                                                   //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Collision bewteen Pins to ground - Ball to Pins - Pins to Pins
	for (int i = 0; i < 10; i++) {
		if (pins[i] != nullptr) {
			CollisionData cd;
			for (int o = 0; o < 2; o++) {
				 (OverlapCube2Ground(*pins[i], pins[i]->getHitbox(), 0));
			}
			for (int j = 0; j < 1; j++) {
				if (balls[j] != nullptr) {
					if (OverlapSphere2Cube(*balls[j], balls[j]->getHitbox(), *pins[i], pins[i]->getHitbox(), cd))
					{
						ResolveCollision(cd);
					}
				}
				}
			
			for (int j = i + 1; j < 10; j++) {
				if (pins[j] != nullptr) {
					CollisionData cd;
					if (OverlapCube2Cube(*pins[i], pins[i]->getHitbox(), *pins[j], pins[j]->getHitbox(), cd))
						ResolveCollision(cd);
				}
			}
			pins[i]->UpdatePhysics(dt);
			if ((pins[i]->getPosition().x > 50) || (pins[i]->getPosition().x < -50) ||
				(pins[i]->getPosition().y > 50) || (pins[i]->getPosition().y < -50) ||
				(pins[i]->getPosition().z > 50) || (pins[i]->getPosition().z < -50)) {
				delete pins[i];
				pins[i] = nullptr;
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                  Player collisions                                                 //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Player to Fences
	for (int i = 4; i < 19; i++) {
		if (props[i] != nullptr) {
			CollisionData cd;
			if (OverlapCube2Cube(*player, player->getHitbox(), *props[i], props[i]->getHitbox(), cd))
				ResolveCollision(cd);
		}
	}//Make way for player if they win
	if (!Won()) {
		CollisionData cd;
		if (props[19] != nullptr) {
			if (OverlapCube2Cube(*player, player->getHitbox(), *props[19], props[19]->getHitbox(), cd))
				ResolveCollision(cd);
		}
		//Blockway because bowling is bowling
		if (props[22] != nullptr) {	
			if (OverlapCube2Cube(*player, player->getHitbox(), *props[22], props[22]->getHitbox(), cd))
				ResolveCollision(cd);
		}
	}
	for (int i = 20; i < 22; i++) {
		if (props[i] != nullptr) {
			CollisionData cd;
			if (OverlapCube2Cube(*player, player->getHitbox(), *props[i], props[i]->getHitbox(), cd))
				ResolveCollision(cd);
		}
		for (int i = 23; i < 33; i++) {
			if (props[i] != nullptr) {
				CollisionData cd;
				if (OverlapCube2Cube(*player, player->getHitbox(), *props[i], props[i]->getHitbox(), cd))
					ResolveCollision(cd);
			}
		}
	}
	for (int i = 0; i < 10; i++) {
		if (pins[i] != nullptr) {
			CollisionData cd;
			for (int o = 4; o < 22; o++) {
				if (OverlapCube2Cube(*pins[i], pins[i]->getHitbox(), *props[o], props[o]->getHitbox(), cd)) {
					ResolveCollision(cd);
				}
			}
		}
	}
	for (int i = 0; i < 1; i++) {
		if (balls[i] != nullptr) {
			CollisionData cd;
			for (int o = 4; o < 22; o++) {
				if (OverlapSphere2Cube(*balls[i], balls[i]->getHitbox(), *props[o], props[o]->getHitbox(), cd)) {
					ResolveCollision(cd);
				}
			}
		}
	}
	
	if ((player->getPosition().x <= 25 && player->getPosition().x >= -25) && (player->getPosition().z >= -30 && (player->getPosition().z <= -25))) {
		if (KeyboardController::GetInstance()->IsKeyPressed('L')) {
			SceneManager::GetInstance().LoadScene(SCENE_START);
			return;
		}
	}
	if (Won()) {
		Used = 0;
		if ((player->getPosition().x <= 8 && player->getPosition().x >= -8) && (player->getPosition().z >= 65 && (player->getPosition().z <= 70))) {
			if (KeyboardController::GetInstance()->IsKeyPressed('L')) {
				SceneManager::GetInstance().LoadScene(SCENE_START);
				return;
			}
		}
	}
	if ((player->getPosition().z <= -30)) {
		player->setObjectPosition(Vector3(player->getPosition().x, player->getPosition().y, -30));
	}
	if ((player->getPosition().z >= 69)) {
		player->setObjectPosition(Vector3(player->getPosition().x, player->getPosition().y, 69));
	}

	if (player->getPosition().y >= 1.5) {
		player->setObjectPosition(Vector3(player->getPosition().x, 1.5, player->getPosition().z));
	}
	if (player->getPosition().x <= -15) {
		player->setObjectPosition(Vector3(-15, player->getPosition().y, player->getPosition().z));
	}
	if (player->getPosition().x >= 15) {
		player->setObjectPosition(Vector3(15 ,player->getPosition().y, player->getPosition().z));
	}

	player->movePlayer(dt, camera);
	player->UpdatePhysics(dt, camera);
	camera.Update(dt); 

	if (Used == 4) {
		if (currGameState == RUNNING) {
			if (KeyboardController::GetInstance()->IsKeyPressed('E')) {
				Die();
			}
		}
	}
}

void SceneBowling::Render()
{

	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));

	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		glm::vec3 lightPosition_cameraspace1 = viewStack.Top() * glm::vec4(light[0].position, 1);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightPosition_cameraspace1));
		glm::vec3 spotDirection_cameraspace1 = viewStack.Top() * glm::vec4(light[0].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace1));


	}
	else {
		//Light point
		glm::vec3 propPosition1(props[0]->getPosition().x, props[0]->getPosition().y, props[0]->getPosition().z);
		glm::vec3 lightPosition1 = viewStack.Top() * glm::vec4(propPosition1, 1);

		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightPosition1));

	}

	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{

		glm::vec3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		glm::vec3 lightDirection_cameraspace1 = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, glm::value_ptr(lightDirection_cameraspace1));


	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{

		glm::vec3 lightPosition_cameraspace2 = viewStack.Top() * glm::vec4(light[1].position, 1);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, glm::value_ptr(lightPosition_cameraspace2));
		glm::vec3 spotDirection_cameraspace2 = viewStack.Top() * glm::vec4(light[1].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace2));


	}
	else {
		//Light point
		glm::vec3 propPosition2(props[1]->getPosition().x, props[1]->getPosition().y, props[1]->getPosition().z);
		glm::vec3 lightPosition2 = viewStack.Top() * glm::vec4(propPosition2, 1);

		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, glm::value_ptr(lightPosition2));
	
	}

	if (light[2].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
		glm::vec3 lightDirection_cameraspace2 = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, glm::value_ptr(lightDirection_cameraspace2));

	}
	else if (light[2].type == Light::LIGHT_SPOT)
	{


		glm::vec3 lightPosition_cameraspace3 = viewStack.Top() * glm::vec4(light[2].position, 1);
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, glm::value_ptr(lightPosition_cameraspace3));
		glm::vec3 spotDirection_cameraspace3 = viewStack.Top() * glm::vec4(light[2].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace3));

	}
	else {
		//Light point

		glm::vec3 propPosition3(props[2]->getPosition().x, props[2]->getPosition().y, props[2]->getPosition().z);
		glm::vec3 lightPosition3 = viewStack.Top() * glm::vec4(propPosition3, 1);	
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, glm::value_ptr(lightPosition3));

	}

	if (light[3].type == Light::LIGHT_DIRECTIONAL)
	{

		glm::vec3 lightDir(light[3].position.x, light[3].position.y, light[3].position.z);
		glm::vec3 lightDirection_cameraspace3 = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, glm::value_ptr(lightDirection_cameraspace3));
	}
	else if (light[3].type == Light::LIGHT_SPOT)
	{

		glm::vec3 lightPosition_cameraspace4 = viewStack.Top() * glm::vec4(light[3].position, 1);
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, glm::value_ptr(lightPosition_cameraspace4));
		glm::vec3 spotDirection_cameraspace4 = viewStack.Top() * glm::vec4(light[3].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace4));
	}
	else {
		//Light point
		glm::vec3 propPosition4(props[3]->getPosition().x, props[3]->getPosition().y, props[3]->getPosition().z);
		glm::vec3 lightPosition4 = viewStack.Top() * glm::vec4(propPosition4, 1);

		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, glm::value_ptr(lightPosition4));
	}
	// Clear color buffer every frames
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load view matrix stack and set it with camera position, target position and up direction
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);

	// Load identity matrix into the model stack
	modelStack.LoadIdentity();

	
	//Skybox first so it is not affected by stack errors
	RenderSkybox();

	// Render objects

	modelStack.PushMatrix();
	// Render light
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_SPHERE], false);
	modelStack.PopMatrix();
	//ball render

	if (balls[0] != nullptr) {
		modelStack.PushMatrix();
		modelStack.Translate(balls[0]->getPosition().x, balls[0]->getPosition().y, balls[0]->getPosition().z);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_BALL], true);
		modelStack.PopMatrix();
	}
	//pin render
	for (int i = 0; i < 10; i++) {
		
		if (pins[i] != nullptr) {
			modelStack.PushMatrix();
			modelStack.Translate(pins[i]->getPosition().x, pins[i]->getPosition().y , pins[i]->getPosition().z);
			modelStack.Rotate(17 * i,0,1,0);
			modelStack.Scale(1, 1, 1);
			meshList[GEO_PINS]->material.kAmbient = glm::vec3(1,1, 1);
			meshList[GEO_PINS]->material.kDiffuse = glm::vec3(1, 1, 1);
			meshList[GEO_PINS]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
			meshList[GEO_PINS]->material.kShininess = 1.0f;
			RenderMesh(meshList[GEO_PINS], true);
			modelStack.PopMatrix();
		}
		
	}

	

	//Fence render
	for (int i = 4; i < 16; i++)
	{
	modelStack.PushMatrix();		
		modelStack.Translate(props[i]->getPosition().x, props[i]->getPosition().y, props[i]->getPosition().z);
		modelStack.Rotate(90, 0, 90, 1);
		modelStack.Scale(22, 20, 25);
		meshList[GEO_FENCE]->material.kAmbient = glm::vec3(.1, .1, .1);
		meshList[GEO_FENCE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_FENCE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_FENCE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_FENCE], true);
	modelStack.PopMatrix();

	}
	for (int i = 16; i < 19; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(props[i]->getPosition().x, props[i]->getPosition().y, props[i]->getPosition().z);
		modelStack.Scale(25, 20, 25);
		meshList[GEO_FENCE]->material.kAmbient = glm::vec3(.1, .1, .1);
		meshList[GEO_FENCE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_FENCE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_FENCE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();
	}
	if (!Won()) {
		modelStack.PushMatrix();
		modelStack.Translate(props[19]->getPosition().x, props[19]->getPosition().y, props[19]->getPosition().z);
		modelStack.Scale(25, 20, 25);
		meshList[GEO_FENCE]->material.kAmbient = glm::vec3(.1, .1, .1);
		meshList[GEO_FENCE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_FENCE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_FENCE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(props[22]->getPosition().x, 0.1, props[22]->getPosition().z);
		modelStack.Rotate(90, -1, 0, 0);
		modelStack.Scale(25, 1, 1.5);
		meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.1, 0.1, 0.1);
		meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kShininess = 5.0f;
		RenderMesh(meshList[GEO_PLANE], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(props[20]->getPosition().x, props[20]->getPosition().y, props[20]->getPosition().z);
	modelStack.Scale(25, 20, 25);
	meshList[GEO_FENCE]->material.kAmbient = glm::vec3(.1, .1, .1);
	meshList[GEO_FENCE]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_FENCE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_FENCE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_FENCE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(player->getPosition().x, player->getPosition().y, player->getPosition().z);
	modelStack.Scale(player->getHitbox().getHalfDimensions().x * 2, player->getHitbox().getHalfDimensions().y * 2, player->getHitbox().getHalfDimensions().z * 2);
	meshList[GEO_CUBE]->material.kAmbient = glm::vec3(.1, .1, .1);
	meshList[GEO_CUBE]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_CUBE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_CUBE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_CUBE], true);
	modelStack.PopMatrix();
	//box render
	RenderBox();
	RenderBlack();
	//TorchRender
	for (int i = 0; i < 4; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(props[i]->getPosition().x, props[i]->getPosition().y, props[i]->getPosition().z);
		modelStack.Scale(1, 1, 1);
		meshList[GEO_FENCE]->material.kAmbient = glm::vec3(.1, .1, .1);
		meshList[GEO_TORCH]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_TORCH]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_TORCH]->material.kShininess = 5.0f;
		RenderMesh(meshList[GEO_TORCH], true);
		modelStack.PopMatrix();
		}

	for (int i = 23; i < 33; i++) {
		if (props[i] != nullptr) {
			modelStack.PushMatrix();
			modelStack.Translate(props[i]->getPosition().x, props[i]->getPosition().y, props[i]->getPosition().z);
			modelStack.Rotate(90, 0, 90, 1);
			modelStack.Scale(25, 20, 25);
			meshList[GEO_FENCE]->material.kAmbient = glm::vec3(.1, .1, .1);
			meshList[GEO_FENCE]->material.kDiffuse = glm::vec3(1, 1, 1);
			meshList[GEO_FENCE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
			meshList[GEO_FENCE]->material.kShininess = 5.0f;
			RenderMesh(meshList[GEO_FENCE], true);
			modelStack.PopMatrix();
		}
	}


	//GeoTree
	for (int i = 0; i < 80; i++) {
		if (props2[i] != nullptr) {
			modelStack.PushMatrix();
			modelStack.Translate(props2[i]->getPosition().x, props2[i]->getPosition().y, props2[i]->getPosition().z);
			modelStack.Scale(12, 12, 12);
			meshList[GEO_TREE]->material.kAmbient = glm::vec3(0.1, 0.1, 0.1);
			meshList[GEO_TREE]->material.kDiffuse = glm::vec3(1, 1, 1);
			meshList[GEO_TREE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
			meshList[GEO_TREE]->material.kShininess = 5.0f;
			RenderMesh(meshList[GEO_TREE], true);
			modelStack.PopMatrix();
		}
	}

	modelStack.PushMatrix();
	modelStack.Translate(sign->getPosition().x, sign->getPosition().y, sign->getPosition().z);
	modelStack.Scale(1.5, 1.5, 1.5);
	modelStack.Rotate(225, 0, 1, 0 );
	meshList[GEO_SIGN]->material.kAmbient = glm::vec3(.5, .5, .5);
	meshList[GEO_SIGN]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_SIGN]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_SIGN]->material.kShininess = 5.0f;
	RenderMesh(meshList[GEO_SIGN], true);
	modelStack.PopMatrix();


	//grab Ball from the box
	if (!Won()) {
		if (inRange()) {
			if (KeyboardController::GetInstance()->IsKeyPressed('E')) {
				Used++;
				grabbed = 1;
				delete balls[0];
				balls[0] = nullptr;
				SetUpPins2();
				}
			
		}
	}
	
	
	RenderFloor();	

	PinCounter();
	{
		if (holdBall()) {
			RenderTextOnScreen2(meshList[GEO_TEXT], "Balls: 1/1", glm::vec3(0, 1, 0), 20, 0, 0);
		}
		else {
			RenderTextOnScreen2(meshList[GEO_TEXT], "Balls: 0/1", glm::vec3(0, 1, 0), 20, 0, 0);
		}
	}
	if ((player->getPosition().x <= 25 && player->getPosition().x >= -25) && (player->getPosition().z >= -30 && (player->getPosition().z <= -25))) {
		RenderTextOnScreen2(meshList[GEO_TEXT], "Previous Area? Press L to Leave", glm::vec3(0, 1, 0), 20, 100, 300);
	}
	if ((player->getPosition().x <= 25 && player->getPosition().x >= -25) && (player->getPosition().z >= 65 && (player->getPosition().z <= 70))) {
		RenderTextOnScreen2(meshList[GEO_TEXT], "Next Area? Press L to Leave", glm::vec3(0, 1, 0), 20, 100, 300);
	}

	if (currGameState == INVENTORY) {
		RenderMeshOnScreen(meshList[GEO_RIGHT], 400, 150, 700, 200);
	}

	 if (currGameState == DIALOGUE) {
		if (interactionCD <= 0.f) {
			if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE) ||
				KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_X) ||
				KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_LEFT_SHIFT) ||
				KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E) ||
				MouseController::GetInstance()->IsButtonPressed(0) ||
				MouseController::GetInstance()->IsButtonPressed(1)) {
				currInteraction->nextDialogue();
				interactionCD = 1.5f;
				if (currInteraction->getCurrDialogue() == nullptr) {
					currGameState = RUNNING;
					pickedUpPage = true;
					return;
				}
			}
		}
	}
	 if (currGameState == INVENTORY) {
		if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_RIGHT) ||
			KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_D)) {
			Inventory::GetInstance().nextItem();
		}
		else if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_LEFT) ||
			KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_A)) {
			Inventory::GetInstance().prevItem();
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                                                For Inventory Render                                                //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (currGameState == INVENTORY) {
		RenderInventory();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                                                For Dialogue Render                                                 //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (currGameState == DIALOGUE) {
		RenderMeshOnScreen(meshList[GEO_QUAD], 400, 150, 700, 200);
		RenderTextOnScreen(meshList[GEO_TEXT], currInteraction->getCurrDialogue()->text, glm::vec3(1, 1, 1), 0.65f, 16, 100, 175);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //                                                For Page Render                                                     //
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	modelStack.PushMatrix();
	modelStack.Translate(page->getPosition().x, page->getPosition().y, page->getPosition().z);
	modelStack.Scale(1.5, 1.5, 1.5);
	modelStack.Rotate(90, 1, 0, 0);
	meshList[GEO_PAGE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PAGE]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_PAGE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PAGE]->material.kShininess = 5.0f;
	RenderMesh(meshList[GEO_PAGE], true);
	modelStack.PopMatrix();


}

void SceneBowling::RenderInventory()
{
	RenderMeshOnScreen(meshList[GEO_QUAD], 400, 300, 700, 400);
	RenderTextOnScreen(meshList[GEO_TEXT], "Inventory", glm::vec3(1, 1, 1), 0.65f, 32, 300, 450);
	if (Inventory::GetInstance().getInventory().size() < 1) {
		RenderTextOnScreen(meshList[GEO_TEXT], "Empty", glm::vec3(1, 1, 1), 0.65f, 16, 300, 350);
	}
	else {
		RenderTextOnScreen(meshList[GEO_TEXT], Inventory::GetInstance().getItem().name, glm::vec3(1, 1, 1), 0.65f, 16, 125, 400);
		if (Inventory::GetInstance().moreThan1Item())
			RenderTextOnScreen(meshList[GEO_TEXT], std::string(" (" + std::to_string(Inventory::GetInstance().getItemQuantity()) + ')'),
				glm::vec3(1, 1, 1), 0.65f, 16, 200, 400);
		RenderTextOnScreen(meshList[GEO_TEXT], Inventory::GetInstance().getItem().description, glm::vec3(1, 1, 1), 0.65f, 16, 100, 350);
		RenderTextOnScreen(meshList[GEO_TEXT], std::string('<' + std::to_string(Inventory::GetInstance().getCurrItemIndex() + 1) +
			'/' + std::to_string(Inventory::GetInstance().getNumUniqueItems()) + '>'), glm::vec3(1, 1, 1), 0.65f, 16, 300, 150);
	}
}

void SceneBowling::HandleKeyPress()
{
	if (KeyboardController::GetInstance()->IsKeyPressed(0x31))
	{
		// Key press to enable culling
		glEnable(GL_CULL_FACE);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x32))
	{
		// Key press to disable culling
		glDisable(GL_CULL_FACE);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x33))
	{
		// Key press to enable fill mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x34))
	{
		// Key press to enable wireframe mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe modessd
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
	{
		// Change to black background
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_ENTER))
	{
		// Change camera type
		if (camera.cameraState == FIRST_PERSON) {
			camera.target = player->getPosition().convert2glm();
			camera.cameraState = THIRD_PERSON;
		}
		else if (camera.cameraState == THIRD_PERSON) {
			//Vector3 cameraOffset{ player->getPosition().Normalize().x, 0, player->getPosition().Normalize().z };
			camera.position = (player->getPosition()).convert2glm();
			camera.target = (player->getPosition() + Vector3{ 0.f, 0.f, 1.f }).convert2glm();
			camera.cameraState = FIRST_PERSON;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_I))
	{
		// Load the inventory
		if (currGameState == RUNNING) {
			currGameState = INVENTORY;
		}
		else if (currGameState == INVENTORY) {
			currGameState = RUNNING;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_TAB))
	{
		if (light[0].type == Light::LIGHT_POINT) {
			light[0].type = Light::LIGHT_DIRECTIONAL;
		}
		else if (light[0].type == Light::LIGHT_DIRECTIONAL) {
			light[0].type = Light::LIGHT_SPOT;
		}
		else {
			light[0].type = Light::LIGHT_POINT;
		}

		if (light[2].type == Light::LIGHT_POINT) {
			light[2].type = Light::LIGHT_DIRECTIONAL;
		}
		else if (light[2].type == Light::LIGHT_DIRECTIONAL) {
			light[2].type = Light::LIGHT_SPOT;
		}
		else {
			light[2].type = Light::LIGHT_POINT;
		}

		if (light[1].type == Light::LIGHT_POINT) {
			light[1].type = Light::LIGHT_DIRECTIONAL;
		}
		else if (light[1].type == Light::LIGHT_DIRECTIONAL) {
			light[1].type = Light::LIGHT_SPOT;
		}
		else {
			light[1].type = Light::LIGHT_POINT;
		}

		if (light[3].type == Light::LIGHT_POINT) {
			light[3].type = Light::LIGHT_DIRECTIONAL;
		}
		else if (light[3].type == Light::LIGHT_DIRECTIONAL) {
			light[3].type = Light::LIGHT_SPOT;
		}
		else {
			light[3].type = Light::LIGHT_POINT;
		}
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[1].type);
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[2].type);
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[3].type);
	}
}





//Only if the ball hits a Pin
bool SceneBowling::ball2Pin2Pin() {
	return Pin2Pin;
}


//Checks for goals
void SceneBowling::goalChecker()
{
	//checker
	for (int i = 0; i < 10; i++) {
		if (pins[i] != nullptr) {
			CollisionData cd;
			for (int j = 0; j < 1; j++) {
				if (balls[j] != nullptr) {
					if (OverlapSphere2Cube(*balls[j], balls[j]->getHitbox(), *pins[i], pins[i]->getHitbox(), cd))
					{		
						pins[i]->setHit(1);
						Pin2Pin = true;
						
					}
				}
			}

			if(ball2Pin2Pin()){
			for (int o = i + 1; o < 10; o++) {
				if (pins[o] != nullptr) {
					if (OverlapCube2Cube(*pins[i], pins[i]->getHitbox(), *pins[o], pins[o]->getHitbox(), cd)) {
						pins[o]->setHit(1);
					}
				}
			}
			}
		}
	}
}





//Check if Player is inRange to grab the Ball from the Box 
bool SceneBowling::inRange()
{
	if (abs(player->getPosition().x - props[21]->getPosition().x) <= 3 && abs(player->getPosition().z - props[21]->getPosition().z) <= 3) {
		return true;
	}
	return false;
}

//Renders the box with the balls inside it
void SceneBowling::RenderBox()
{
	modelStack.PushMatrix();
	modelStack.Translate(props[21]->getPosition().x, props[21]->getPosition().y, props[21]->getPosition().z);
	meshList[GEO_BALLBOX]->material.kAmbient = glm::vec3(.50, 0.50, 0.50);
	meshList[GEO_BALLBOX]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_BALLBOX]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_BALLBOX]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BALLBOX], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(props[21]->getPosition().x, props[21]->getPosition().y + 1, props[21]->getPosition().z);
	modelStack.Scale(10, 10, 10);
	meshList[GEO_BALL]->material.kAmbient = glm::vec3(.50, 0.50, 0.50);
	meshList[GEO_BALL]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_BALL]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_BALL]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BALL], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(props[21]->getPosition().x + .25f, props[21]->getPosition().y + 1, props[21]->getPosition().z - .25);
	modelStack.Scale(10, 10, 10);
	meshList[GEO_BALL]->material.kAmbient = glm::vec3(.50, 0.50, 0.50);
	meshList[GEO_BALL]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_BALL]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_BALL]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BALL], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(props[21]->getPosition().x - .25f, props[21]->getPosition().y + 1, props[21]->getPosition().z + .25);
	modelStack.Scale(10, 10, 10);
	meshList[GEO_BALL]->material.kAmbient = glm::vec3(.50, 0.50, 0.50);
	meshList[GEO_BALL]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_BALL]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_BALL]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_BALL], true);
	modelStack.PopMatrix();
}
//Check for your Balls
bool SceneBowling::holdBall()
{
	if (grabbed == 1) {
		return true;
	}
	else
		return false;
}

//Sends you to DeathScene
void SceneBowling::Die()
{
	SceneManager::GetInstance().LoadScene(SCENE_START);
	return;
}



void SceneBowling::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Scale(2, 2, 2);
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, -100.f);
	// Skybox should be rendered without light
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	// Offset in Z direction by 50 units
	modelStack.Translate(0.f, 0.f, 100.f);
	modelStack.Rotate(180, 0, 1, 0);
	// Skybox should be rendered without light
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(100.f, 0.f, 0.f);
	modelStack.Rotate(90, 0, -1, 0);
	// Skybox should be rendered without light
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	// Offset in X direction by 50 units
	modelStack.Translate(-100.f, 0.f, 0.f);
	modelStack.Rotate(90, 0, 1, 0);
	// Skybox should be rendered without light
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	// Offset in Y direction by -50 units
	modelStack.Translate(0.f, 100.f, 0.f);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 0, 3);
	// Skybox should be rendered without light
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PushMatrix();

	// Offset in Y direction by -50 units
	modelStack.Translate(0.f, -100.f, 0.f);
	modelStack.Rotate(90, -1, 0, 0);
	// Skybox should be rendered without light
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneBowling::RenderBlack()
{

	//vert
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, -35.f);
	modelStack.Scale(.5, .1, .1);
	meshList[GEO_FRONT]->material.kAmbient = glm::vec3(0, 0, 0);
	RenderMesh(meshList[GEO_QUAD], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, 75.f);
	modelStack.Scale(.5, .1, .1);
	meshList[GEO_FRONT]->material.kAmbient = glm::vec3(0, 0, 0);
	RenderMesh(meshList[GEO_QUAD], false);
	modelStack.PopMatrix();

	//Hori
	modelStack.PushMatrix();
	modelStack.Translate(50.f, 0.f, 15.f);
	modelStack.Scale(1., .1, 1);
	modelStack.Rotate(90, 0, -1, 0);
	meshList[GEO_FRONT]->material.kAmbient = glm::vec3(0, 0, 0);
	RenderMesh(meshList[GEO_QUAD], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50.f, 0.f, 15.f);
	modelStack.Scale(1, .1, .5);
	modelStack.Rotate(90, 0, -1, 0);
	meshList[GEO_FRONT]->material.kAmbient = glm::vec3(0, 0, 0);
	RenderMesh(meshList[GEO_QUAD], false);
	modelStack.PopMatrix();
}

void SceneBowling::RenderFloor()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(25,25,25);
	meshList[GEO_FLOOR]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_FLOOR]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_FLOOR]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_FLOOR]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_FLOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 25);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(25.f, 25.f, 25.f);
	meshList[GEO_FLOOR]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_FLOOR]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_FLOOR]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_FLOOR]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_FLOOR], true);
	modelStack.PopMatrix();



		modelStack.PushMatrix();
		modelStack.Translate(-25, 0, 25);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(25.f, 25.f, 25.f);
		meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_PLANE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 25);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(25.f, 25.f, 25.f);
		meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_PLANE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(25, 0, 25);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(25.f, 25.f, 25.f);
		meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_PLANE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-25, 0, -25);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(25.f, 25.f, 25.f);
		meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_PLANE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -25);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(25.f, 25.f, 25.f);
		meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_PLANE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(25, 0, -25);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(25.f, 25.f, 25.f);
		meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_PLANE], true);
		modelStack.PopMatrix();
	



	{
		modelStack.PushMatrix();
		modelStack.Translate(25, 0, 0);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(25.f, 25.f, 25.f);
		meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_PLANE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-25, 0, 0);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(25.f, 25.f, 25.f);
		meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_PLANE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(25, 0, 25);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(25.f, 25.f, 25.f);
		meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_PLANE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-25, 0, 25);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(25.f, 25.f, 25.f);
		meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
		meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
		meshList[GEO_PLANE]->material.kShininess = 1.0f;
		RenderMesh(meshList[GEO_PLANE], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(-25, 0, 50);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(25.f, 25.f, 25.f);
	meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PLANE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PLANE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 50);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(25.f, 25.f, 25.f);
	meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PLANE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PLANE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(25, 0, 50);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(25.f, 25.f, 25.f);
	meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PLANE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PLANE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-25, 0, 75);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(25.f, 25.f, 25.f);
	meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PLANE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PLANE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 75);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(25.f, 25.f, 25.f);
	meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PLANE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PLANE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(25, 0, 75);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(25.f, 25.f, 25.f);
	meshList[GEO_PLANE]->material.kAmbient = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PLANE]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_PLANE]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_PLANE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_PLANE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, .1,-32);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(.03, .03, .03);
	meshList[GEO_ROAD]->material.kAmbient = glm::vec3(.5, .5, .5);
	meshList[GEO_ROAD]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_ROAD]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_ROAD]->material.kShininess = 5.0f;
	RenderMesh(meshList[GEO_ROAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, .1, 70);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(.06, .06, .06);
	meshList[GEO_ROAD]->material.kAmbient = glm::vec3(.5, .5, .5);
	meshList[GEO_ROAD]->material.kDiffuse = glm::vec3(1, 1, 1);
	meshList[GEO_ROAD]->material.kSpecular = glm::vec3(0.5, 0.5, 0.5);
	meshList[GEO_ROAD]->material.kShininess = 5.0f;
	RenderMesh(meshList[GEO_ROAD], true);
	modelStack.PopMatrix();
}
//Position the Pins
void SceneBowling::SetUpPins()
{

	if (pins != nullptr) {
		pins[0] = new bowlingPins(Vector3(0, 2, 25), 20, 1, 0, 1);
		pins[1] = new bowlingPins(Vector3(1.5, 2, 26), 20, 1, 0, 1);
		pins[2] = new bowlingPins(Vector3(-1.5, 2, 26), 20, 1, 0, 1);
		pins[3] = new bowlingPins(Vector3(2, 2, 30), 20, 1, 0, 1);
		pins[4] = new bowlingPins(Vector3(0, 2, 30), 20, 1, 0, 1);
		pins[5] = new bowlingPins(Vector3(-2, 2, 30), 20, 1, 0, 1);
		pins[6] = new bowlingPins(Vector3(-3, 2, 32), 20, 1, 0, 1);
		pins[7] = new bowlingPins(Vector3(-1, 2, 32), 20, 1, 0, 1);
		pins[8] = new bowlingPins(Vector3(1, 2, 32), 20, 1, 0, 1);
		pins[9] = new bowlingPins(Vector3(3, 2, 32), 20, 1, 0, 1);
	}
	for (int i = 0; i < 10; i++) {
		pins[i]->setHit(0);
		pins[i]->setBounciness(.4);
	}

}
void SceneBowling::SetUpPins2()
{
	for (int i =0;i<10;i++){
		if(pins[i] != nullptr)
		if (pins[i]->HitCheck()) {
			delete pins[i];
			pins[i] = nullptr;
			if (pins[i] != nullptr) {
				SetUpPins();
			}
		}
	}
}

int SceneBowling::PinCounter() {
	PinCount = 0;
	for (int i = 0; i < 10; i++) {
		if (pins[i] != nullptr) {
			if (!pins[i]->HitCheck()) {
				PinCount += 1;
			}
		}
	}
	return PinCount;
}


bool SceneBowling::Won()
{
	if (PinCount == 0) {
		return true;
	}
	else
	return false;
}
//Disable Lights when conditions are met
void SceneBowling::lightDisabler()
{
	if (Used == 1) {
		light[0].power = 0;	
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	}
	else if (Used == 2) {
		light[0].power = 0;
			light[1].power = 0;
			glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
			glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		
	}
	else if (Used == 3) {
		light[0].power = 0;
		light[1].power = 0;
		light[2].power = 0;
			glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
			glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
			glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	}
	else if (Used >= 4) {
		light[0].power = 0;
		light[1].power = 0;
		light[2].power = 0;
	    light[3].power = 0;
			glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
			glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
			glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
			glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
		
	}
	 if (Won()) {
		light[0].power = 0.2;
		light[1].power = 0.2;
		light[2].power = 0.2;
		light[3].power = 0.2;
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
		glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
	}
	
	
	
}
void SceneBowling::RenderTextOnScreen2(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glm::mat4 ortho = glm::ortho(-50.f, 800.f, 0.f, 600.f, -
		100.f, 100.f); // dimension of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	for (unsigned i = 0; i < text.length(); ++i)
	{
		glm::mat4 characterSpacing = glm::translate(
			glm::mat4(1.f),
			glm::vec3(0.2f + i * 1.0f, 0.5f, 0)
		);
		glm::mat4 MVP = projectionStack.Top() *
			viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			glm::value_ptr(MVP));
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}
void SceneBowling::Exit()
{
	//Game variables cleanup

		if (balls[0] != nullptr) {
			delete balls[0];
		}
		if (player != nullptr) {
			delete player;
		}
		if (sign != nullptr)
		{
			delete sign;
	    }
		if (page != nullptr) {
			delete page;
		}
	for (int i = 0; i < 10; i++) {
		if (pins[i] != nullptr) {
			delete pins[i];
		}
	}
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	for (int i = 0; i < NUM_IOBJS; i++) {
		if (interactableObjs[i] != nullptr) {
			delete interactableObjs[i];
		}
	}
	for (int i = 0; i < 80; i++) {
		if (props[i] != nullptr) {
			delete props[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneBowling::InitLights()
{
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	//Start of copy paste to add new lights
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	//how are you supposed to for loop this :pensive:
		m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
		m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
		m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
		m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
		m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
		m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
		m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
		m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
		m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
		m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
		m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

		m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
		m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
		m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
		m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
		m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
		m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
		m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
		m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
		m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
		m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
		m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

		m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
		m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
		m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
		m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
		m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
		m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
		m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
		m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
		m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
		m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
		m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");

	//End of copy paste, remeber to change the number in the enum and the string
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	//Do not touch this line of code
	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);
	//Light 


		light[0].color = glm::vec3(1, .5, 0);
		light[0].type = Light::LIGHT_POINT;
		light[0].power = .2f;
		light[0].kC = 1.f;
		light[0].kL = 0.01f;
		light[0].kQ = 0.001f;
		light[0].cosCutoff = 45.f;
		light[0].cosInner = 30.f;
		light[0].exponent = 3.f;
		light[0].position = glm::vec3(8, 0, -4);
		light[0].spotDirection = glm::vec3(0.f, 1.f, 0.f);

		light[1].color = glm::vec3(1, .5, 0);
		light[1].type = Light::LIGHT_POINT;
		light[1].power = .2f;
		light[1].kC = 1.f;
		light[1].kL = 0.01f;
		light[1].kQ = 0.001f;
		light[1].cosCutoff = 45.f;
		light[1].cosInner = 30.f;
		light[1].exponent = 3.f;
		light[1].position = glm::vec3(-8, 0, -4);
		light[1].spotDirection = glm::vec3(0.f, 1.f, 0.f);

		light[2].color = glm::vec3(1, .5, 0);
		light[2].type = Light::LIGHT_POINT;
		light[2].power = .2f;
		light[2].kC = 1.f;
		light[2].kL = 0.01f;
		light[2].kQ = 0.001f;
		light[2].cosCutoff = 45.f;
		light[2].cosInner = 30.f;
		light[2].exponent = 3.f;
		light[2].position = glm::vec3(-8, 0, 25);
		light[2].spotDirection = glm::vec3(0.f, 1.f, 0.f);

		light[3].color = glm::vec3(1, .5, 0);
		light[3].type = Light::LIGHT_POINT;
		light[3].power = .2f;
		light[3].kC = 1.f;
		light[3].kL = 0.01f;
		light[3].kQ = 0.001f;
		light[3].cosCutoff = 45.f;
		light[3].cosInner = 30.f;
		light[3].exponent = 3.f;
		light[3].position = glm::vec3(-8, 0, -25);
		light[3].spotDirection = glm::vec3(0.f, 1.f, 0.f);

		
			  
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], cosf(glm::radians<float>(light[0].cosCutoff)));
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], cosf(glm::radians<float>(light[0].cosInner)));
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
		glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
		glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
		glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], cosf(glm::radians<float>(light[1].cosCutoff)));
		glUniform1f(m_parameters[U_LIGHT1_COSINNER], cosf(glm::radians<float>(light[1].cosInner)));
		glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

		glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
		glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
		glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
		glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
		glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
		glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], cosf(glm::radians<float>(light[2].cosCutoff)));
		glUniform1f(m_parameters[U_LIGHT2_COSINNER], cosf(glm::radians<float>(light[2].cosInner)));
		glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

		glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &light[3].color.r);
		glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
		glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
		glUniform1f(m_parameters[U_LIGHT3_KC], light[3].kC);
		glUniform1f(m_parameters[U_LIGHT3_KL], light[3].kL);
		glUniform1f(m_parameters[U_LIGHT3_KQ], light[3].kQ);
		glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], cosf(glm::radians<float>(light[3].cosCutoff)));
		glUniform1f(m_parameters[U_LIGHT3_COSINNER], cosf(glm::radians<float>(light[3].cosInner)));
		glUniform1f(m_parameters[U_LIGHT3_EXPONENT], light[3].exponent);


}