#include <h3.h>
#include <stdio.h>
#include <components/spritecomponent.h>
#include <components/rectanglecollidercomponent.h>
#include <components/textcomponent.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "playercomponent.h"
#include "shipmovement.h"

#ifndef NDEBUG
# pragma comment(lib, "h3-s-d.lib")
#else // !NDEBUG
# pragma comment(lib, "h3-s.lib")
#endif // !NDEBUG

int main(int argc, char** argv)
{

	H3Handle h3 = H3_Init((SH3InitParams) {
		.width = 900,
		.height = 700,
		.fullscreen = false,
		.windowTitle = "BRADAFRAMANADAMADA"
	});
	srand(time(NULL));

	H3Handle scene = H3_Scene_Create(h3, true);


	bool gameOver = false;
	int score = 0;
	int life = 3;


	H3Handle player = H3_Object_Create(scene, "player", NULL);
	H3_Object_SetTranslation(player, 450, 350);
	H3_Object_EnablePhysics(player, H3_BOX_COLLIDER(CDT_Dynamic, 30, 30, A_Center | A_Middle, false));
	H3_Object_SetRenderOrder(player, 10);
	H3_Object_AddComponent(player, SPRITECOMPONENT_CREATE("assets/ship.png", 0x22));
	H3_Object_AddComponent(player, SHIPCOMPONENT_CREATE(player, scene, &gameOver, &life));
	H3_Object_Translate(player, 450, 350);


	H3Handle font = H3_Font_Load("assets/Comfortaa-Regular.ttf");
	H3Handle scoreText = H3_Object_Create(scene, "scoreText", NULL);
	H3Handle lifeText = H3_Object_Create(scene, "lifeText", NULL);
	H3_Object_SetTranslation(lifeText, 10, 10);
	H3_Object_SetTranslation(scoreText, 890, 10);

	SH3TextProperties textProps1 = {
		.font = font,
		.size = 30,
		.fillColor = {255,0,0,255},
		.hasOutline = false,
		.outlineColor = {0,255,0,0},
		.outlineThickness = 5,
		.anchor = A_Left | A_Top,
		.isBold = false,
		.isItalic = false,
		.isUnderlined = false,
		.isViewLocal = false
	};

	SH3TextProperties textProps2 = {
		.font = font,
		.size = 30,
		.fillColor = {255,0,0,255},
		.hasOutline = false,
		.outlineColor = {0,255,0,0},
		.outlineThickness = 5,
		.anchor = A_Right | A_Top,
		.isBold = false,
		.isItalic = false,
		.isUnderlined = false,
		.isViewLocal = false
	};
	char scoreBuffer[256];
	char lifeBuffer[256];
	
	H3_Object_AddComponent(scoreText, TEXTCOMPONENT_CREATE("", textProps2));
	H3_Object_AddComponent(lifeText, TEXTCOMPONENT_CREATE("", textProps1));

	
	


	typedef struct
	{
		const char* Text;
		SH3TextProperties Props;
	} TextComponent_Properties;



	clock_t start, stop;
	start = clock();
	



	float asteroidsDelay = 2000.0f;
	float asteroPosX;
	float asteroPosY;
	float asteroVelX;
	float asteroVelY;

	

	char nameAstero[256];
	int asteroidsIndex = 1;
	int nexAsteroidsIndex = 1;

	bool moving;


	do{

		snprintf(scoreBuffer, 256, "score : %d", score);
		SH3Component* compoText = H3_Object_GetComponent(scoreText, SPRITECOMPONENT_TYPEID);
		TextComponent_Properties* textProps = (TextComponent_Properties*)(compoText->properties);
		textProps->Text = scoreBuffer;
		snprintf(lifeBuffer, 256, "life : %d", life);
		SH3Component* compoText2 = H3_Object_GetComponent(lifeText, SPRITECOMPONENT_TYPEID);
		TextComponent_Properties* textProps2 = (TextComponent_Properties*)(compoText2->properties);
		textProps2->Text = lifeBuffer;

		if (!gameOver) {
			asteroidsDelay = 2000 / (1 + score / 2000);

			stop = clock();

			if (stop - start > asteroidsDelay) {
				start = clock();
				int tempTime = rand() % 4;
				if (tempTime == 0) {
					asteroPosX = rand() % 940;
					asteroPosY = 740 * (rand() % 2);
				}
				else {
					asteroPosY = rand() % 740;
					asteroPosX = 940 * (rand() % 2);
				}
				snprintf(nameAstero, 256, "asteroid_%d", asteroidsIndex++);
				H3Handle astero = H3_Object_Create(scene, nameAstero, NULL);

				char tempName;

				int randAstero = rand() % 6;
				if (randAstero == 0) {
					H3_Object_EnablePhysics(astero, H3_CIRCLE_COLLIDER(CDT_Dynamic, 40, false));
					H3_Object_SetRenderOrder(astero, 13);
					H3_Object_AddComponent(astero, SPRITECOMPONENT_CREATE("assets/asteroid2.png", 0x22));
					tempName = 'B';
				}
				else if (randAstero == 1 || randAstero == 2) {
					H3_Object_EnablePhysics(astero, H3_CIRCLE_COLLIDER(CDT_Dynamic, 20, false));
					H3_Object_SetRenderOrder(astero, 12);
					H3_Object_AddComponent(astero, SPRITECOMPONENT_CREATE("assets/asteroid1.png", 0x22));
					tempName = 'M';
				}
				else {
					H3_Object_EnablePhysics(astero, H3_CIRCLE_COLLIDER(CDT_Dynamic, 10, false));
					H3_Object_SetRenderOrder(astero, 11);
					H3_Object_AddComponent(astero, SPRITECOMPONENT_CREATE("assets/asteroid3.png", 0x22));
					tempName = 'S';
				}


				H3_Object_AddComponent(astero, PLAYER_CREATE(scene, astero, asteroPosX - 20, asteroPosY - 20, &nexAsteroidsIndex, tempName, &score));
				H3_Object_SetRotation(astero, rand() % 360);
				H3_Object_Translate(astero, asteroPosX - 20, asteroPosY - 20);

				asteroVelX = (450 - (asteroPosX - 20));
				asteroVelY = (350 - (asteroPosY - 20));
				float multiple = ((float)(rand() % 30) + 30) / 100;
				asteroVelX *= multiple;
				asteroVelY *= multiple;

				H3_Object_AddVelocity(astero, asteroVelX, asteroVelY);
			}		

			
		}
		if (gameOver) {
			printf("\nENDED");
		}
	} while (H3_DoFrame(h3,scene));
	
	H3_Scene_Destroy(&scene);

	return 0;
}
