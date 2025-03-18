#ifndef _H3_COMPONENTS_PLAYER_H_
#define _H3_COMPONENTS_PLAYER_H_

#include <h3.h>

#define PLAYER_TYPEID 0x00000011

H3_CAPI_BEGIN_BLOCK
void PlayerInit(SH3Transform* transform, void* properties);
void PlayerTerminate(void* properties);
void PlayerUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void PlayerCollision(H3Handle col1, SH3Collision col2);
void* Player_CreateProperties(H3Handle scene, H3Handle player, float x, float y, int* index, char* type, int* score);

H3_CAPI_END_BLOCK

#define PLAYER_CREATE(SCENE, PLAYER, X, Y, INDEX, TYPE, SCORE)												 \
	(SH3Component) {																	 \
		.Init               = PlayerInit,                                                \
		.Terminate          = PlayerTerminate,											 \
		.Update             = PlayerUpdate,                                              \
		.OnCollisionEnter	= PlayerCollision,											 \
		.isInitialized      = false,                                                     \
		.componentType      = PLAYER_TYPEID,                                             \
		.properties         = Player_CreateProperties(SCENE, PLAYER, X, Y, INDEX, TYPE, SCORE)				 \
	}

#endif /* _H3_COMPONENTS_PLAYER_H_ */
