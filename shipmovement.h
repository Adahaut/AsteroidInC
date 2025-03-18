#ifndef _H3_COMPONENTS_SHIPMOVEMENT_H_
#define _H3_COMPONENTS_SHIPMOVEMENT_H_

#include <h3.h>

#define SHIPCOMPONENT_TYPEID 0x00000101

H3_CAPI_BEGIN_BLOCK
void ShipInit(void* properties);
void ShipTerminate(void* properties);
void ShipUpdate(H3Handle h3, SH3Transform* transform, void* properties);
void ShipCollision(H3Handle col1, SH3Collision col2);

void* ShipProperties(H3Handle ship, H3Handle scene, bool* gameOver, int* life);

H3_CAPI_END_BLOCK

#define SHIPCOMPONENT_CREATE(SHIP, SCENE, GO, LIFE)                                          \
	(SH3Component) {                                                            \
		.Init               = ShipInit,                                                      \
		.Terminate          = ShipTerminate,                                 \
		.Update             = ShipUpdate,                                         \
		.OnCollisionEnter	= ShipCollision,											 \
		.isInitialized      = false,                                                     \
		.componentType      = SHIPCOMPONENT_TYPEID,                                    \
		.properties         = ShipProperties(SHIP, SCENE, GO, LIFE)									\
	}

#endif /* _H3_COMPONENTS_SPRITECOMPONENT_H_ */
