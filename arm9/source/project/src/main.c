/*

			Copyright (C) 2017  Coto
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
USA

*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef ARM9
#include <ctype.h>
#include <stdlib.h>
#include <_ansi.h>
#include <reent.h>
#include "videoTGDS.h"
#include "posixHandleTGDS.h"
#include "consoleTGDS.h"
#include "debugNocash.h"
#include "main.h"
#include "spitscTGDS.h"
#include "timerTGDS.h"
#include "keypadTGDS.h"
#include "biosTGDS.h"
#include "InterruptsARMCores_h.h"
#include "interrupts.h"
#include "ipcfifoTGDSUser.h"

#include "grass_tex.h"
#include "fish_tex.h"
#include <ctype.h>

#ifdef __cplusplus
extern "C"{
#endif

extern int vsnprintf(char *str, size_t size, const char *format, va_list ap);

#ifdef __cplusplus
}
#endif

#endif

#include "Scene.h"
#include "Renderable.h"

#ifndef _MSC_VER
					// //
#define ARM9 1		// Enable only if not real GCC + NDS environment
#undef _MSC_VER		// //
#undef WIN32		// //
#endif

struct Scene scene;	/// the scene we render

#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Os")))
#endif

#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
int main(int argc, char *argv[])
{
	#ifdef _MSC_VER
	startTGDSProject(argc, argv);
	#endif

	#ifdef ARM9
	/*			TGDS 1.6 Standard ARM9 Init code start	*/
	bool isTGDSCustomConsole = true;	//set default console or custom console: custom console 
	GUI_init(isTGDSCustomConsole);
	GUI_clear();
	
	//xmalloc init removes args, so save them
	int i = 0;
	for(i = 0; i < argc; i++){
		argvs[i] = argv[i];
	}

	bool isCustomTGDSMalloc = true;
	setTGDSMemoryAllocator(getProjectSpecificMemoryAllocatorSetup(TGDS_ARM7_MALLOCSTART, TGDS_ARM7_MALLOCSIZE, isCustomTGDSMalloc, TGDSDLDI_ARM7_ADDRESS));
	sint32 fwlanguage = (sint32)getLanguage();
	
	//argv destroyed here because of xmalloc init, thus restore them
	for(i = 0; i < argc; i++){
		argv[i] = argvs[i];
	}

	int ret=FS_init();
	if (ret == 0)
	{
		printf("FS Init ok.");
	}
	else{
		printf("FS Init error: %d", ret);
	}
	
	asm("mcr	p15, 0, r0, c7, c10, 4");
	flush_icache_all();
	flush_dcache_all();	
	/*			TGDS 1.6 Standard ARM9 Init code end	*/
	
	/////////////////////////////////////////////////////////Reload TGDS Proj///////////////////////////////////////////////////////////
	char tmpName[256];
	char ext[256];
	if(__dsimode == true){
		char TGDSProj[256];
		char curChosenBrowseFile[256];
		strcpy(TGDSProj,"0:/");
		strcat(TGDSProj, "ToolchainGenericDS-multiboot");
		if(__dsimode == true){
			strcat(TGDSProj, ".srl");
		}
		else{
			strcat(TGDSProj, ".nds");
		}
		//Force ARM7 reload once 
		if( 
			(argc < 3) 
			&& 
			(strncmp(argv[1], TGDSProj, strlen(TGDSProj)) != 0) 	
		){
			REG_IME = 0;
			MPUSet();
			REG_IME = 1;
			char startPath[MAX_TGDSFILENAME_LENGTH+1];
			strcpy(startPath,"/");
			strcpy(curChosenBrowseFile, TGDSProj);
			
			char thisTGDSProject[MAX_TGDSFILENAME_LENGTH+1];
			strcpy(thisTGDSProject, "0:/");
			strcat(thisTGDSProject, TGDSPROJECTNAME);
			if(__dsimode == true){
				strcat(thisTGDSProject, ".srl");
			}
			else{
				strcat(thisTGDSProject, ".nds");
			}
			
			//Boot .NDS file! (homebrew only)
			strcpy(tmpName, curChosenBrowseFile);
			separateExtension(tmpName, ext);
			strlwr(ext);
			
			//pass incoming launcher's ARGV0
			char arg0[256];
			int newArgc = 3;
			if (argc > 2) {
				printf(" ---- test");
				printf(" ---- test");
				printf(" ---- test");
				printf(" ---- test");
				printf(" ---- test");
				printf(" ---- test");
				printf(" ---- test");
				printf(" ---- test");
				
				//arg 0: original NDS caller
				//arg 1: this NDS binary
				//arg 2: this NDS binary's ARG0: filepath
				strcpy(arg0, (const char *)argv[2]);
				newArgc++;
			}
			//or else stub out an incoming arg0 for relaunched TGDS binary
			else {
				strcpy(arg0, (const char *)"0:/incomingCommand.bin");
				newArgc++;
			}
			//debug end
			
			char thisArgv[4][MAX_TGDSFILENAME_LENGTH];
			memset(thisArgv, 0, sizeof(thisArgv));
			strcpy(&thisArgv[0][0], thisTGDSProject);	//Arg0:	This Binary loaded
			strcpy(&thisArgv[1][0], curChosenBrowseFile);	//Arg1:	Chainload caller: TGDS-MB
			strcpy(&thisArgv[2][0], thisTGDSProject);	//Arg2:	NDS Binary reloaded through ChainLoad
			strcpy(&thisArgv[3][0], (char*)&arg0[0]);//Arg3: NDS Binary reloaded through ChainLoad's ARG0
			addARGV(newArgc, (char*)&thisArgv);				
			if(TGDSMultibootRunNDSPayload(curChosenBrowseFile) == false){ //should never reach here, nor even return true. Should fail it returns false
				
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	REG_IME = 0;
	MPUSet();
	//TGDS-Projects -> legacy NTR TSC compatibility
	if(__dsimode == true){
		TWLSetTouchscreenTWLMode();
	}
	REG_IME = 1;
	
	/*
	#ifdef NO_VIDEO_PLAYBACK
	argv[2] = (char*)0x02000000; //debug, if enabled, disables video intro
	#endif
	
	//Play game intro if coldboot
	if(argv[2] == NULL){
		char tmpName[256];
		strcpy(tmpName, videoIntro);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		char startPath[MAX_TGDSFILENAME_LENGTH+1];
		strcpy(startPath,"/");
		if(__dsimode == true){
			strcpy(curChosenBrowseFile, "0:/ToolchainGenericDS-videoplayer.srl");
		}
		else{
			strcpy(curChosenBrowseFile, "0:/ToolchainGenericDS-videoplayer.nds");
		}
		//Send args
		printf("[Booting %s]", curChosenBrowseFile);
		printf("Want to send argument?");
		printf("(A) Yes: (Start) Choose arg.");
		printf("(B) No. ");
		
		int argcCount = 0;
		argcCount++;
		printf("[Booting... Please wait] >%d", TGDSPrintfColor_Red);
		
		char thisArgv[3][MAX_TGDSFILENAME_LENGTH];
		memset(thisArgv, 0, sizeof(thisArgv));
		strcpy(&thisArgv[0][0], TGDSPROJECTNAME);	//Arg0:	This Binary loaded
		strcpy(&thisArgv[1][0], curChosenBrowseFile);	//Arg1:	NDS Binary reloaded
		strcpy(&thisArgv[2][0], tmpName);					//Arg2: NDS Binary ARG0
		addARGV(3, (char*)&thisArgv);				
		if(TGDSMultibootRunNDSPayload(curChosenBrowseFile) == false){ //should never reach here, nor even return true. Should fail it returns false
			printf("Invalid NDS/TWL Binary >%d", TGDSPrintfColor_Yellow);
			printf("or you are in NTR mode trying to load a TWL binary. >%d", TGDSPrintfColor_Yellow);
			printf("or you are missing the TGDS-multiboot payload in root path. >%d", TGDSPrintfColor_Yellow);
			printf("Press (A) to continue. >%d", TGDSPrintfColor_Yellow);
			while(1==1){
				scanKeys();
				if(keysDown()&KEY_A){
					scanKeys();
					while(keysDown() & KEY_A){
						scanKeys();
					}
					break;
				}
			}
			menuShow();
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	else if(strncmp(argv[2], videoIntro, strlen(videoIntro)) == 0){
		//video intro already played, skip.
	}
	else if(strncmp(argv[2], videoTest, strlen(videoTest)) == 0){
		//play videoTest maybe?
	}
	*/
	
	clrscr();
	printf("---");
	printf("---");
	printf("---");
	printf("starting TGDS Project");

	ret = startTGDSProject(argc, argv);

	clrscr();
	printf("---");
	printf("---");
	printf("---");
	printf("ending TGDS Project. Halt");
	while(1==1){

	}
	#endif
	
	return 0;
}

void TGDSAPPExit(u32 fn_address){
#ifdef ARM9
	clrscr();
	printf("----");
	printf("----");
	printf("----");
	printf("TGDSAPP:Exit(); From Addr: %x", fn_address);
	while(1==1){
		swiDelay(1);
	}
#endif
}

/// Animates the scene. This function just renders the scene every
/// 25 milliseconds. A timer is used to give smooth animation at the
/// same rate on differnt computers. idle function draws the scenes
/// at way too different speeds on different computers
void animateScene(int type)
{
#ifdef _MSC_VER
	glutPostRedisplay();
	glutTimerFunc(25, animateScene, 0);
#endif
}

/// Handles keyboard input for normal keys
void keyboardInput(unsigned char key, int x, int y)
{
	switch(key) {
	
		case 27:	// ESC key (Quits)
			exit(0);
			break;

		case ' ':	// SPACE key (Toggle flat/smooth shading)
			scene.flatShading = !scene.flatShading;
			if (scene.flatShading) glShadeModel(GL_FLAT);
			else glShadeModel(GL_SMOOTH);
			break;

		case 'A':
		case 'a':
			tiltdown(&scene.camera);
			break;

		case 'Z':
		case 'z':
			tiltup(&scene.camera);
			break;

		case 'W':
		case 'w':	// toggles wireframe mode on/off
			scene.wireMode = !scene.wireMode;
			if (!scene.wireMode) {
				glDisable(GL_BLEND);
			} else {
				glEnable(GL_BLEND);
			}
			break;

		case 'P':
		case 'p':	// toglles between perspective/orthographic projections
			scene.perspectiveMode = !scene.perspectiveMode;
			setupViewVolume();
			break;

		case 'f':
		case 'F':{	// toggles fog on/off
			
		#ifdef _MSC_VER
			if (scene.fogMode) glEnable(GL_FOG);
			else glDisable(GL_FOG);
		#endif
		}break;

		case 'l':
		case 'L':{	// toggles lighting calculations on/off
			scene.lightMode = !scene.lightMode;
			if (scene.lightMode) {
				glEnable(GL_LIGHTING);
			}
			else {
				glDisable(GL_LIGHTING);
			}
		}break;

		case '1':{	// toggles light 0 on / off
			scene.light0On = !scene.light0On;
			if (scene.light0On){
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
			}
			else {
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);
			}
		}break;

		case '2':{	// toggles light 1 on / off
			scene.light1On = !scene.light1On;
			if (scene.light1On){
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT1);
			}
			else {
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT1);
			}
		}break;
	}
}


/// Processes special keyboard keys like F1, F2, etc
void keyboardInputSpecial(int key, int x, int y){
	switch (key){
	
		//WIN32 ONLY
		#ifdef _MSC_VER
		case GLUT_KEY_F1:{
			scene.showMenu = !scene.showMenu;
		}break;
		case GLUT_KEY_F2:{
			addObject(OBJ_CRAB);
		}break;
		case GLUT_KEY_F3:{
			addObject(OBJ_OCTOPUS);
		}break;
		case GLUT_KEY_F4:{
			addObject(OBJ_STARFISH);
		}break;
		case GLUT_KEY_F5:{
			addObject(OBJ_FISH);
		}break;
		case GLUT_KEY_F6:{
			addObject(OBJ_PLANT);
		}break;
		#endif


		//WIN32 & NDS Shared
		#ifdef _MSC_VER
		case GLUT_KEY_LEFT:
		#endif
		#ifdef ARM9
		case KEY_UP:
		#endif
		{
			anticlockwise(&scene.camera);
		}break;

		#ifdef _MSC_VER
		case GLUT_KEY_RIGHT:
		#endif
		#ifdef ARM9
		case KEY_DOWN:
		#endif
		{
			clockwise(&scene.camera);
		}break;
		#ifdef _MSC_VER
		case GLUT_KEY_UP:
		#endif
		#ifdef ARM9
		case KEY_LEFT:
		#endif
		{
			inc(&scene.camera);
		}break;

		#ifdef _MSC_VER
		case GLUT_KEY_DOWN:
		#endif
		#ifdef ARM9
		case KEY_RIGHT:
		#endif
		{
			dec(&scene.camera);
		}break;

		//NDS only
		#ifdef ARM9
		case KEY_L:{
			keyboardInput('L', 0, 0); // toggles lighting calculations on/off
		}break;

		case KEY_A:{
			BgMusic(); //turn on bg music
		}break;
		
		case KEY_B:{
			BgMusicOff(); //turn off bg music
		}break;
		#endif
	}
}

/// Adds an object to the scene
void addObject(int type)
{
	/*
	* we know our floor is drawn square at the origin, so we base
	* our random positions to lie within the the ocean floor. additionally
	* we extend the height of the volume by 10 units.
	*/

	// first pick the x and z locations
	GLfloat x = getRand(-25.0f, 50.0f);
	GLfloat z = getRand(-25.0f, 50.0f);

	// the height is a bit different, differnt objects need a different
	// offset above the sea floor
	GLfloat y;

	GLfloat mat1Init[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat mat2Init[] = {0.0f, 0.0f, 0.0f, 0.0f};
	struct MarineObject object;

	switch (type)
	{
	case OBJ_STARFISH:{
		y = -0.3f;
		{
			GLfloat mat1[] = {
		#ifdef _MSC_VER
				0.3f, 0.3f, 0.3f, 1.f
		#endif

		#ifdef ARM9
				100.0f, 200.4f, 220.0f, 1.f
		#endif
			};

			GLfloat high_shininess[] = { 
#ifdef _MSC_VER
				32.f
#endif
#ifdef ARM9
				128.0f //NDS
#endif
			};

			object = BuildStarfish((void*)&_drawStarFish, mat1, high_shininess[0], vertexStarFish, normalStarFish, NULL, coloursStarFish);
			//scale the starfish
			#ifdef ARM9
			scale(&object, 4.0f, 4.0f, 4.0f);
			#endif
		}
	}break;
	case OBJ_CRAB:{
		y = -0.4f;
		{
			GLfloat mat1[] = {0.5f, 0.5f, 0.5f, 1.f};
			object = BuildCrab((void*)&_drawCrab, (void*)&_draw_dlistCrab, mat1, 50.f, NULL, NULL, NULL, NULL);
			//scale the crab
			#ifdef ARM9
			scale(&object, 7.0f, 7.0f, 7.0f);
			#endif
		}
	}break;
	case OBJ_FISH:{
		y = getRand(-26.0f, 25.0f);
		{
			GLfloat mat1[] = {1.f, 1.f, 1.f, 1.f};
			object = BuildFish((void*)&_drawFish, mat1, 120.f, vertexFish, normalFish, texelsFish, coloursFish);
		}
	}break;
	case OBJ_OCTOPUS:{
#ifdef _MSC_VER
		y = getRand(-27.0f, 25.0f);
#endif
#ifdef ARM9
		y = getRand(10.0f, 26.0f);
#endif
		{
			GLfloat mat1[] = {
		#ifdef _MSC_VER
				0.0f, 0.0f, 2.0f, 1.f
		#endif

		#ifdef ARM9
				100.0f, 200.4f, 220.0f, 1.f
		#endif
			};

			GLfloat high_shininess[] = { 
#ifdef _MSC_VER
				50.f
#endif
#ifdef ARM9
				128.0f //NDS
#endif
			};

			object = BuildOctopus((void*)&_drawOctopus, mat1, high_shininess[0], NULL, NULL, NULL, NULL);
			//scale the octopus
			#ifdef ARM9
			object.ry = 0.0f;	// we don't want random rotation
			object.rx = 180.0f;
			scale(&object, 4.0f, 4.0f, 4.0f);
			#endif
		}
	}break;
	case OBJ_PLANT:{
		y = 0.0f;
		{
			GLfloat mat1[] = {
		#ifdef _MSC_VER
				0.1f, 0.3f, 0.15f, 1.f
		#endif

		#ifdef ARM9
				0.0f, 200.4f, 0.0f, 1.f
		#endif
			};
			GLfloat mat2[] = {
		#ifdef _MSC_VER
				0.6f, 1.f, 0.8f, 1.f
		#endif
		#ifdef ARM9
				1.0f, 1.0f, 1.0f, 1.0f
		#endif
			};
			GLfloat high_shininess[] = { 
#ifdef _MSC_VER
				100.f
#endif
#ifdef ARM9
				128.0f //NDS
#endif
			};
			object = BuildPlant((void*)&_draw_dlistPlant, NULL, mat1, mat2, high_shininess[0], NULL, NULL, NULL, NULL);
			//scale the DL plant
			#ifdef ARM9
			scale(&object, 9.0f, 9.0f, 9.0f);
			#endif
		}
		object.ry = 0.0f;
	}break;
	}

	if(object.callerType != RENDERABLE_NONE){
		move(&object, x, y, z);	// set objects new position
		if(add(&scene, &object) == true){	// adds object to rendering queue
			scene.objects[type]++;	// increments the count of this type of object
		}
	}
}


/// Sets up the viewing volume to be used
void setupViewVolume(void)
{
	// work out the aspect ratio for width and height
	GLfloat aspect = (GLfloat)widthScene / (GLfloat)heightScene;
	GLfloat iaspect = (GLfloat)heightScene / (GLfloat)widthScene;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// setup new viewing volume based on the aspect ratio and projection type
	if (scene.perspectiveMode == true){
		gluPerspective(-45.0f, aspect, 1.0f, 250.0f);
	}
	else {
		// orthographic mode correction depends on whether the ratio is greater
		// or less than 1.0 as the viewport must be scaled in different
		// directions to look right
		if (aspect >= 1.0f){
			glOrtho(-40.0f * aspect, 40.0f * aspect, -40.0f, 40.0f, 1.0f, 250.0f);
		}
		else {
			glOrtho(-40.0f, 40.0f, -40.0f * iaspect, 40.0f * iaspect, 1.0f, 250.0f);
		}
	}

	glMatrixMode(GL_MODELVIEW);
}

/// Initiates all textures
void getTextures(void)
{
	TWLPrintf("-- Generating/Loading Textures\n");

#ifdef _MSC_VER
	glGenTextures(2, texturesRenderable);
    glBindTexture(GL_TEXTURE_2D, texturesRenderable[FLOOR_TEXTURE]);
    load_image("../src/resources/grass.png");
	glBindTexture(GL_TEXTURE_2D, texturesRenderable[FISH_TEXTURE]);
    load_image("../src/resources/fish.png");
#endif

#ifdef ARM9
	//#1: Load a texture and map each one to a texture slot
	u32 arrayOfTextures[2];
	arrayOfTextures[FLOOR_TEXTURE] = (u32)&grass_tex; //0: grass_tex.bmp
	arrayOfTextures[FISH_TEXTURE] = (u32)&fish_tex; //1: fish_tex.bmp  
	int texturesInSlot = LoadLotsOfGLTextures((u32*)&arrayOfTextures, (int*)&texturesRenderable, 2); //Implements both glBindTexture and glTexImage2D 
	int i = 0;
	for(i = 0; i < texturesInSlot; i++){
		printf("Texture loaded: %d:textID[%d] Size: %d", i, texturesRenderable[i], getTextureBaseFromTextureSlot(activeTexture));
	}
	printf("Free Mem: %d KB", ((int)TGDSARM9MallocFreeMemory()/1024));
#endif
}

#ifdef _MSC_VER
void load_image(const char* filename)
{
    int width, height;
    unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
}
#endif

//ARM9 printf nocash debugger
int TWLPrintf(const char *fmt, ...){
#ifdef ARM9
	va_list args;
	va_start (args, fmt);
	vsnprintf ((sint8*)ConsolePrintfBuf, 64, fmt, args);
	va_end (args);
	
	nocashMessage((char*)ConsolePrintfBuf);
#endif
	return 0;
}

/////////////////////////////////////////////////////////// TGDS Project ARM9 specifics ///////////////////////////////////////
#ifdef ARM9

__attribute__((section(".dtcm")))
int pendPlay = 0;

char curChosenBrowseFile[MAX_TGDSFILENAME_LENGTH];

/////////////////////////////////////////////////////////////////////////////////////

//true: pen touch
//false: no tsc activity
#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__((optnone))
#endif
#endif
bool get_pen_delta( int *dx, int *dy ){
	static int prev_pen[2] = { 0x7FFFFFFF, 0x7FFFFFFF };
	
	// TSC Test.
	struct touchPosition touch;
	XYReadScrPosUser(&touch);
	
	if( (touch.px == 0) && (touch.py == 0) ){
		prev_pen[0] = prev_pen[1] = 0x7FFFFFFF;
		*dx = *dy = 0;
		return false;
	}
	else{
		if( prev_pen[0] != 0x7FFFFFFF ){
			*dx = (prev_pen[0] - touch.px);
			*dy = (prev_pen[1] - touch.py);
		}
		prev_pen[0] = touch.px;
		prev_pen[1] = touch.py;
	}
	return true;
}

void menuShow(){
	clrscr();
	printf(" ---- ");
	printf(" ---- ");
	printf(" ---- ");
	printf("[%s] running. >%d", TGDSPROJECTNAME, TGDSPrintfColor_Yellow);
	printf("Free Mem: %d KB >%d", ((int)TGDSARM9MallocFreeMemory()/1024), TGDSPrintfColor_Cyan);
}

//TGDS Soundstreaming API
int internalCodecType = SRC_NONE; //Returns current sound stream format: WAV, ADPCM or NONE
struct fd * _FileHandleVideo = NULL; 
struct fd * _FileHandleAudio = NULL;

#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif

#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
bool stopSoundStreamUser(){
	return stopSoundStream(_FileHandleVideo, _FileHandleAudio, &internalCodecType);
}

#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif

#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void closeSoundUser(){
	//Stubbed. Gets called when closing an audiostream of a custom audio decoder
}

char args[8][MAX_TGDSFILENAME_LENGTH];
char *argvs[8];

#endif