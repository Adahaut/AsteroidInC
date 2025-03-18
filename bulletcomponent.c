#include <stdlib.h>
#include <stdio.h>
#include "bulletcomponent.h"

typedef struct
{
	
	float posX;
	float posY;

	SH3Transform* GlobalTransform;

} Bullet_Properties;


void BulletInit(SH3Transform* transform, void* properties) {
	
}

void BulletTerminate(void* properties) {
	free(properties);
}

void BulletUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {

	Bullet_Properties* props = (Bullet_Properties*)properties;

	H3_Transform_GetPosition(H3_Object_GetTransform(object), &props->posX, &props->posY);

	if (props->posX > 1000 || props->posX < -100 || props->posY > 1000 || props->posY < -100) {
		
		H3_Object_Destroy(&object, true);

	}


}

void BulletCollision(H3Handle col1, SH3Collision col2) {
	

}

void* Bullet_CreateProperties(H3Handle scene, H3Handle Bullet, float x, float y, bool* gameOver)
{
	Bullet_Properties* properties = malloc(sizeof(Bullet_Properties));

	

	return properties;
}

