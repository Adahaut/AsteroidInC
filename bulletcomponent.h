#ifndef _H3_COMPONENTS_BULLET_H_
#define _H3_COMPONENTS_BULLET_H_

#include <h3.h>

#define BULLET_TYPEID 0x00001001

H3_CAPI_BEGIN_BLOCK
void BulletInit(SH3Transform* transform, void* properties);
void BulletTerminate(void* properties);
void BulletUpdate(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void BulletCollision(H3Handle col1, SH3Collision col2);
void* Bullet_CreateProperties();

H3_CAPI_END_BLOCK

#define BULLET_CREATE()												 \
	(SH3Component) {																	 \
		.Init               = BulletInit,                                                \
		.Terminate          = BulletTerminate,											 \
		.Update             = BulletUpdate,                                              \
		.OnCollisionEnter	= BulletCollision,											 \
		.isInitialized      = false,                                                     \
		.componentType      = BULLET_TYPEID,                                             \
		.properties         = Bullet_CreateProperties()				 \
	}

#endif /* _H3_COMPONENTS_Bullet_H_ */
#pragma once
