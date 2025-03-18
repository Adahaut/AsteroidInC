#include <stdlib.h>
#include <stdio.h>
#include <components/spritecomponent.h>
#include <math.h>
#include "bulletcomponent.h"
#include "playercomponent.h"
#include "shipmovement.h"


typedef struct
{
	H3Handle player;
	H3Handle scene;
	H3Handle engine;
	float playerX;
	float playerY;
	float playerVX;
	float playerVY;
	float maxSpeed;
	float speed;
	float projSpeed;
	H3Handle laser;

	int* life;

	bool* gameOver;

	bool first;

	char nameProj[256];
	int projectilesIndex;

	int cursX;
	int cursY;

	SH3Transform* GlobalTransform;
} Player_Properties;




float Distance(float a, float b) {
	return sqrt(pow(a, 2) + pow(b, 2));
}




void ShipInit(SH3Transform* transform, void* properties) {
	Player_Properties* props = (Player_Properties*)properties;
	
}

void ShipTerminate(void* properties) {
	free(properties);
}

void ShipUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {

	Player_Properties* props = (Player_Properties*)properties;

	if (!*props->gameOver) {

		if (props->first) {


			H3Handle engine = H3_Object_Create(props->scene, "engine", NULL);
			H3_Object_AddComponent(engine, SPRITECOMPONENT_CREATE("assets/engine.png", 0x22));
			//H3_Object_SetRenderOrder(engine, 8);
			props->engine = engine;
			props->first = false;
		}

		H3_Input_GetMousePos(h3, &props->cursX, &props->cursY);
		H3_Transform_GetPosition(H3_Object_GetTransform(props->player), &props->playerX, &props->playerY);

		float rot = atan2((props->cursY - props->playerY), (props->cursX - props->playerX));

		H3_Object_SetRotation(props->player, (rot * (180 / 3.141593f)) - 90);
		H3_Object_GetVelocity(props->player, &props->playerVX, &props->playerVY);

		float hypo = Distance(props->cursX - props->playerX, props->cursY - props->playerY);

		if (H3_Input_IsMouseBtnDown(MB_Right) && sqrt(pow(props->playerVX + ((props->cursX - props->playerX) / hypo), 2) + pow(props->playerVY + ((props->cursY - props->playerY) / hypo), 2)) < props->maxSpeed) {
			H3_Object_AddVelocity(props->player, ((props->cursX - props->playerX) / hypo) * props->speed, ((props->cursY - props->playerY) / hypo) * props->speed);
			H3_Object_SetEnabled(props->engine, true);
		}
		else {
			H3_Object_SetVelocity(props->player, props->playerVX / 1.02, props->playerVY / 1.02);
			H3_Object_SetEnabled(props->engine, false);
		}

		if (H3_Input_IsMouseBtnPressed(MB_Left)) {

			H3_Sound_Play(props->laser, 100.0f, false);

			snprintf(props->nameProj, 256, "bullet_%d", props->projectilesIndex);
			H3Handle projectile = H3_Object_Create(props->scene, props->nameProj, props->player);
			props->projectilesIndex++;

			H3_Object_EnablePhysics(projectile, H3_BOX_COLLIDER(CDT_Dynamic, 8, 8, A_Center | A_Middle, true));
			//H3_Object_SetRenderOrder(projectile, 9);
			H3_Object_AddComponent(projectile, SPRITECOMPONENT_CREATE("assets/bullet.png", 0x22));
			H3_Object_AddComponent(projectile, BULLET_CREATE());
			H3_Object_Rotate(projectile, (rot * (180 / 3.141593f)) + 90);
			H3_Object_Translate(projectile, props->playerX, props->playerY);
			float hypoProj = Distance(props->cursX - props->playerX, props->cursY - props->playerY);
			H3_Object_SetVelocity(projectile, ((props->cursX - props->playerX) / hypoProj) * props->projSpeed, ((props->cursY - props->playerY) / hypoProj) * props->projSpeed);
		}

		H3_Object_SetRotation(props->engine, (rot * (180 / 3.141593f)) + 90);
		H3_Object_SetTranslation(props->engine, props->playerX, props->playerY);
		H3_Object_Translate(props->engine, 0, 15);
	}
}

void ShipCollision(H3Handle col1, SH3Collision col2) {

	SH3Component* component = H3_Object_GetComponent(col1, SHIPCOMPONENT_TYPEID);
	Player_Properties* props = (Player_Properties*)(component->properties);

	if (H3_Object_HasComponent(col2.other, PLAYER_TYPEID)) {
		if (*props->life-1 <= 0) {
			*props->gameOver = true;
			*props->life -= 1;
			H3_Object_SetEnabled(props->engine, false);
			H3_Object_Destroy(&col1, true);
		}else {
			*props->life -= 1;
		}
	}
}

void* ShipProperties(H3Handle ship, H3Handle scene, bool* gameOver, int* life)
{
	Player_Properties* properties = malloc(sizeof(Player_Properties));

	properties->player = ship;
	properties->scene = scene;
	properties->playerX = 450.0f;
	properties->playerY = 350.0f;
	properties->maxSpeed = 500.0f;
	properties->speed = 6.0f;
	properties->projSpeed = 1200.0f;
	properties->projectilesIndex = 1;
	properties->life = life;
	properties->first = true;
	properties->gameOver = gameOver;
	properties->laser = H3_Sound_Load("assets/laser.wav");

	return properties;
}

