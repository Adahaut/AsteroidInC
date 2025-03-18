#include <stdlib.h>
#include <stdio.h>
#include <components/spritecomponent.h>
#include "playercomponent.h"
#include "bulletcomponent.h"

typedef struct
{
	H3Handle scene;
	H3Handle player;
	float posX;
	float posY;
	float velX;
	float velY;
	bool* gameOver;
	SH3Transform* GlobalTransform;

	char asteroType;

	char asteroName[256];
	int* asteroIndex;

	int colType;
	bool colision;

	int* score;

} Player_Properties;


void PlayerInit(SH3Transform* transform, void* properties) {
	Player_Properties* props = (Player_Properties*)properties;
}

void PlayerTerminate(void* properties) {
	free(properties);
}

void PlayerUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {

	Player_Properties* props = (Player_Properties*)properties;
	
	H3_Transform_GetPosition(H3_Object_GetTransform(object), &props->posX, &props->posY);

	if (props->posX > 1000 || props->posX < -100 || props->posY > 1000 || props->posY < -100) {

		H3_Object_Destroy(&object, true);

	}

	if (props->colision)
	{
		//*props->asteroIndex++;

		int ind = (int)(t * 1000);

		if (props->colType == 1) {
			snprintf(props->asteroName, 256, "newAsteroid_%d", ind);
			H3Handle astero = H3_Object_Create(props->scene, props->asteroName, NULL);
			//H3_Object_SetTranslation(astero, (props->posX + (props->velX * 1.5 * 10)) * 1, (props->posY + (props->velY * 0.5 * 10)) * 1);
			H3_Object_EnablePhysics(astero, H3_CIRCLE_COLLIDER(CDT_Dynamic, 20, false));
			//H3_Object_SetRenderOrder(astero, 12);
			H3_Object_AddComponent(astero, SPRITECOMPONENT_CREATE("assets/asteroid1.png", 0x22));
			H3_Object_AddComponent(astero, PLAYER_CREATE(props->scene, astero, props->posX, props->posY, props->asteroIndex, 'M', props->score));
			H3_Object_SetTranslation(astero, props->posX, props->posY);
			H3_Object_Translate(astero, props->velX * 1.5 / 100, props->velY * 0.5 / 100);
			H3_Object_SetVelocity(astero, props->velX * 1.5, props->velY * 0.5);

			snprintf(props->asteroName, 256, "newAsteroid_%d", ind+1);
			H3Handle astero2 = H3_Object_Create(props->scene, props->asteroName, NULL);
			//H3_Object_SetTranslation(astero2, (props->posX + (props->velX * 0.5 * 10)) * 1, (props->posY + (props->velY * 1.5 * 10)) * 1);
			H3_Object_EnablePhysics(astero2, H3_CIRCLE_COLLIDER(CDT_Dynamic, 20, false));
			//H3_Object_SetRenderOrder(astero2, 12);
			H3_Object_AddComponent(astero2, SPRITECOMPONENT_CREATE("assets/asteroid1.png", 0x22));
			H3_Object_AddComponent(astero2, PLAYER_CREATE(props->scene, astero2, props->posX, props->posY, props->asteroIndex, 'M', props->score));
			H3_Object_SetTranslation(astero2, props->posX, props->posY);
			H3_Object_Translate(astero2, props->velX * 0.5 / 100, props->velY * 1.5 / 100);
			H3_Object_SetVelocity(astero2, props->velX * 0.5, props->velY * 1.5);
		}
		else {
			snprintf(props->asteroName, 256, "newAsteroid_%d", ind);
			H3Handle astero = H3_Object_Create(props->scene, props->asteroName, NULL);
			H3_Object_SetTranslation(astero, (props->posX + (props->velX * 1.5 * 10)) * 1, (props->posY + (props->velY * 0.5 * 10)) * 1);
			H3_Object_EnablePhysics(astero, H3_CIRCLE_COLLIDER(CDT_Dynamic, 10, false));
			//H3_Object_SetRenderOrder(astero, 12);
			H3_Object_AddComponent(astero, SPRITECOMPONENT_CREATE("assets/asteroid3.png", 0x22));
			H3_Object_AddComponent(astero, PLAYER_CREATE(props->scene, astero, props->posX, props->posY, props->asteroIndex, 'S', props->score));
			H3_Object_SetTranslation(astero, props->posX, props->posY);
			H3_Object_Translate(astero, props->velX * 1.5 / 100, props->velY * 0.5 / 100);
			H3_Object_SetVelocity(astero, props->velX * 1.5, props->velY * 0.5);

			snprintf(props->asteroName, 256, "newAsteroid_%d", ind+1);
			H3Handle astero2 = H3_Object_Create(props->scene, props->asteroName, NULL);
			H3_Object_SetTranslation(astero2, (props->posX + (props->velX * 0.5 * 10)) * 1, (props->posY + (props->velY * 1.5 * 10)) * 1);
			H3_Object_EnablePhysics(astero2, H3_CIRCLE_COLLIDER(CDT_Dynamic, 10, false));
			//H3_Object_SetRenderOrder(astero2, 12);
			H3_Object_AddComponent(astero2, SPRITECOMPONENT_CREATE("assets/asteroid3.png", 0x22));
			H3_Object_AddComponent(astero2, PLAYER_CREATE(props->scene, astero2, props->posX, props->posY, props->asteroIndex, 'S', props->score));
			H3_Object_SetTranslation(astero2, props->posX, props->posY);
			H3_Object_Translate(astero2, props->velX * 0.5 / 100, props->velY * 1.5 / 100);
			H3_Object_SetVelocity(astero2, props->velX * 0.5, props->velY * 1.5);
		}

		
	}
	

}

void PlayerCollision(H3Handle col1, SH3Collision col2) {

	SH3Component* component = H3_Object_GetComponent(col1, PLAYER_TYPEID);
	Player_Properties* props = (Player_Properties*)(component->properties);

	H3_Object_GetVelocity(col1, &props->velX, &props->velY);

	if (H3_Object_HasComponent(col2.other, BULLET_TYPEID)) {
		if (props->asteroType == 'B') {
			props->colision = true;
			props->colType = 1;
			*props->score += 10;
		}else if (props->asteroType == 'M') {
			props->colision = true;
			props->colType = 2;
			*props->score += 20;
		}else {
			*props->score += 40;
		}
		
		H3_Object_Destroy(&col1, false);
		H3_Object_Destroy(&col2, true);
		
		
	}
	else if (H3_Object_GetRenderOrder(col2.other) == 10) {
		/*if (props->asteroType == 'B') {
			props->colision = true;
			props->colType = 1;
		}
		else if (props->asteroType == 'M') {
			props->colision = true;
			props->colType = 2;
		}*/
		H3_Object_Destroy(&col1, false);
	}
	
}

void* Player_CreateProperties(H3Handle scene, H3Handle player, float x, float y, int* index, char* type, int* score)
{
	Player_Properties* properties = malloc(sizeof(Player_Properties));

	properties->player = player;
	properties->scene = scene;
	properties->posX = x;
	properties->posY = y;
	properties->asteroIndex = index;
	properties->colision = false;
	properties->asteroType = type;
	properties->score = score;

	return properties;
}

