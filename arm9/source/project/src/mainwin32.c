/*
* Main file that contains the main() method. Takes care of
* instantiating the Scene class.
*
* This file has all of the callback functions that GLUT calls.
*
* The Initialiser class is defined here too. It had to be inlined
* here to save on the number of files to upload.
*
* Igor Kromin 40125374
*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

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
#include "grass_tex.h"
#include "fish_tex.h"

extern int vsnprintf( char* buffer, size_t buf_size, const char* format, va_list vlist );
#endif

#include "Scene.h"
#include "Renderable.h"
#include "Camera.h"

#ifndef _MSC_VER
					// //
#define ARM9 1		// Enable only if not real GCC + NDS environment
#undef _MSC_VER		// //
#undef WIN32		// //
#endif

#define OBJ_CRAB 0
#define OBJ_OCTOPUS 1
#define OBJ_STARFISH 2
#define OBJ_FISH 3
#define OBJ_PLANT 4

struct Scene scene;	/// the scene we render
bool wireMode;	/// wireframe mode on / off
bool flatShading;	/// flat shading on / off

#ifdef _MSC_VER
extern int startAquarium(int argc, char *argv[]);

/// Program starts here
int main(int argc, char *argv[])
{
	startAquarium(argc, argv);
	return 0;
}
#endif

#ifdef ARM9

#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__((optnone))
#endif
#endif
int InitGL()
{
	glInit(); //NDSDLUtils: Initializes a new videoGL context
	
	glClearColor(255,255,255);		// White Background
	glClearDepth(0x7FFF);		// Depth Buffer Setup
	glEnable(GL_ANTIALIAS|GL_TEXTURE_2D|GL_BLEND); // Enable Texture Mapping + light #0 enabled per scene
	glDisable(GL_LIGHT0|GL_LIGHT1);
	glEnable(GL_LIGHT0|GL_LIGHT1); //light #1 & #2 enabled per scene
	
	setupDLEnableDisable2DTextures();
	return 0;
}

#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__((optnone))
#endif
#endif
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}
#endif

int startAquarium(int argc, char *argv[])
{
	time_t time1 = time(NULL);
	TWLPrintf("-- Program starting: %d\n", (unsigned int)time1);
	srand(time1);
	init(argc, argv);

	// register our call-back functions
	TWLPrintf("-- Registering callbacks\n");
	
#ifdef _MSC_VER
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(keyboardInput);
	glutSpecialFunc(keyboardInputSpecial);
#endif

	// generate/load textures
	getTextures();

	// create the scene and set perspective projection as default
	SceneInit1(&scene);
	scene.perspectiveMode = true;

	// create a single quad for the floor of the aquarium (reduces 420~ objects into mere 22 which makes the engine runnable on NintendoDS at good framerate)
	{

		GLfloat mat_ambient[]    = { 
#ifdef _MSC_VER
			1.0f, 1.0f, 1.0f, 1.0f		//WIN32
#endif
#ifdef ARM9
			30.0f, 30.0f, 30.0f, 30.0f	//NDS
#endif
		}; 

		GLfloat mat_diffuse_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
		GLfloat high_shininess[] = { 
#ifdef _MSC_VER
			120.0f
#endif
#ifdef ARM9
			128.0f //NDS
#endif
		}; 
		
		struct MarineObject quad = BuildQuad((void*)&_drawQuad, mat_ambient, mat_diffuse_specular, high_shininess[0], NULL, NULL, NULL, NULL);
		quad.ry = 0.0f;	// we don't want random rotation
		quad.rx = 90.0f;
		quad.x = 1.0f * 1; //X axis relative to polygon origin
		quad.z = 1.0f * 1; //Z axis relative to polygon origin
		scale(&quad, 95.5f, 95.5f, 3.0f);
		add(&scene, &quad);
	}
	// 'fake' keys being pressed to enable the state to
	// setup lighting and fog
	keyboardInput((unsigned char)'L', 0, 0);
	keyboardInput((unsigned char)'0', 0, 0);
	keyboardInput((unsigned char)'1', 0, 0);
	keyboardInput((unsigned char)'F', 0, 0);

	// add some stuff to the scene
	{
		int o = 0;
		for (o = 0; o < 4; o++){
			addObject(OBJ_CRAB);
			addObject(OBJ_STARFISH);
			//addObject(OBJ_FISH);
			//addObject(OBJ_FISH);
			//addObject(OBJ_FISH);
			addObject(OBJ_OCTOPUS);
			addObject(OBJ_PLANT);
		}
	}
	// start the timer and enter the mail GLUT loop
#ifdef _MSC_VER
	glutTimerFunc(50, animator, 0);
	glutMainLoop();
#endif

#if defined(ARM9)
	BgMusic();
	glReset(); //Depend on GX stack to render scene
	glClearColor(0,35,195);		// blue green background colour
	while(1==1){
		//Handle Input & game logic
		scanKeys();
		keyboardInputSpecial((int)keysHeld(), 0, 0);
		
		//sound
		switch(pendPlay){
			case(1):{
				internalCodecType = playSoundStream(curChosenBrowseFile, _FileHandleVideo, _FileHandleAudio);
				if(internalCodecType == SRC_NONE){
					//stop right now
					pendPlay = 2;
				}
				else{
					pendPlay = 3;
				}
			}
			break;
			case(2):{
				stopSoundStreamUser();
			}
			break;
		}
		
		//Audio track ended? Repeat
		if((pendPlay == 3) && (cutOff == true)){ 
			//Let decoder close context so we can start again
			closeSound();
			
			updateStream();	
			updateStream();
			updateStream();
			updateStream();
			
			updateStream();	
			updateStream();
			updateStream();
			updateStream();
			
			updateStream();	
			updateStream();
			updateStream();
			updateStream();
			
			updateStream();	
			updateStream();
			updateStream();
			updateStream();
			pendPlay = 1;
		}
		
		//Render
		drawScene();
	}
#endif
	return 0;
}


/// Animates the aquarium. This function just renders the scene every
/// 25 milliseconds. A timer is used to give smooth animation at the
/// same rate on differnt computers. idle function draws the scenes
/// at way too different speeds on different computers
void animator(int type)
{
#ifdef _MSC_VER
	glutPostRedisplay();
	glutTimerFunc(25, animator, 0);
#endif
}


#ifdef _MSC_VER
/// Resizes the current viewport to the full window size
void resizeWindow(int w, int h)
{
	glViewport(0, 0, w, h);

	// update the width and height we are using
	widthScene = w;
	heightScene = h;

	setupViewVolume();
}
#endif

/// Handles keyboard input for normal keys
void keyboardInput(unsigned char key, int x, int y)
{
	switch(key) {
	
		case 27:	// ESC key (Quits)
			exit(0);
			break;

		case ' ':	// SPACE key (Toggle flat/smooth shading)
			flatShading = !flatShading;
			if (flatShading) glShadeModel(GL_FLAT);
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
			wireMode = !wireMode;
			if (!wireMode) {
				glDisable(GL_BLEND);
				//glDisable(GL_LINE_SMOOTH);
			} else {
				glEnable(GL_BLEND);
				//glEnable(GL_LINE_SMOOTH);
			}
			break;

		case 'P':
		case 'p':	// toglles between perspective/orthographic projections
			scene.perspectiveMode = !scene.perspectiveMode;
			setupViewVolume();
			break;

		case 'f':
		case 'F':{	// toggles fog on/off
			scene.fogMode = !scene.fogMode;
			
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
			scene.light0On = ! scene.light0On;
			if (scene.light0On){ 
				glEnable(GL_LIGHT0);
			}
			else {
				glDisable(GL_LIGHT0);
			}
		}break;

		case '2':{	// toggles light 1 on / off
			scene.light1On = ! scene.light1On;
			if (scene.light1On) {
				glEnable(GL_LIGHT1);
			}
			else {
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

#ifdef _MSC_VER
void load_image(const char* filename)
{
    int width, height;
    unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
}
#endif

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

/// Init GLUT and OpenGL
/*
* Creates a window that is half the size of the screen
* and then calls setupGL() to setup the initial OpenGL
* environment.
*/
bool init(int argc, char *argv[])
{
	// initialise glut
	TWLPrintf("-- Initialising GLUT\n");
	wireMode = false;	/// wireframe mode on / off
	flatShading = false;	/// flat shading on / off

#ifdef _MSC_VER
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
#endif
	// setup our widnow now

	TWLPrintf("-- Creating window\n");

#ifdef _MSC_VER
	glutCreateWindow("Aquarium Scene 3D");
	glutFullScreen();
#endif

	// initialise opengl initial state
	setupGL();

	return true;
}


/// Sets up the OpenGL state machine environment
/// All hints, culling, fog, light models, depth testing, polygon model
/// and blending are set up here
void setupGL(void)
{
	TWLPrintf("-- Setting up OpenGL state\n");   

	// blue green background colour
	glClearColor(0.0, 0.5, 0.55
#ifdef _MSC_VER
		, 1.0
#endif	
	);
	glShadeModel(GL_SMOOTH);

	// depth testing used on with less than testing
#ifdef _MSC_VER
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
#endif

	// setup  fog, but disable for now
#ifdef _MSC_VER
	glDisable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	{
		GLfloat fogColor[4] = {0.0f, 0.5f, 0.55f, 1.0f};
		glFogfv(GL_FOG_COLOR, fogColor);
	}
	glFogf(GL_FOG_DENSITY, 0.0075);
	
	// enable normalising of normals after scaling
	glEnable(GL_NORMALIZE);
#endif
	// setup lighting, but disable for now
	glDisable(GL_LIGHTING);
#ifdef _MSC_VER
	{
		GLfloat ambient[] = {0.1f, 0.1f, 0.1f, 1.0};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	}
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	/*
	* The actual lights are defined in the Scene class
	*/

	// set up line antialiasing
	glLineWidth(1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
	// setup backface culling
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

#ifdef ARM9
	/* TGDS 1.65 OpenGL 1.1 Initialization */
	InitGL();
	ReSizeGLScene(255, 191);
	glMaterialShinnyness();
#endif

	DLSOLIDCUBE0_06F = (GLint)glGenLists(1);

	//glut2SolidCube(); -> NDS GX Implementation
	glNewList(DLSOLIDCUBE0_06F, GL_COMPILE);
	{
		float size = 0.06f;
		GLfloat n[6][3] =
		{
			{-1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, -1.0f}
		};
		GLint faces[6][4] =
		{
			{0, 1, 2, 3},
			{3, 2, 6, 7},
			{7, 6, 5, 4},
			{4, 5, 1, 0},
			{5, 6, 2, 1},
			{7, 4, 0, 3}
		};
		GLfloat v[8][3];
		GLint i;

		v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
		v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
		v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
		v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
		v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
		v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

		for (i = 5; i >= 0; i--)
		{
			glBegin(GL_QUADS);
			glNormal3fv(&n[i][0]);
			glTexCoord2f(0, 0);
			glVertex3fv(&v[faces[i][0]][0]);
			glTexCoord2f(1, 0);
			glVertex3fv(&v[faces[i][1]][0]);
			glTexCoord2f(1, 1);
			glVertex3fv(&v[faces[i][2]][0]);
			glTexCoord2f(0, 1);
			glVertex3fv(&v[faces[i][3]][0]);
			glEnd();
		}
		glEndList();
	}

	glDisable(GL_CULL_FACE); 
	glCullFace (GL_NONE);
}

//ARM9 version is in main.c
#ifdef _MSC_VER
int TWLPrintf(const char *fmt, ...){
	return 0;
}
#endif
