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

#ifdef _MSC_VER
#include <GL/glut.h>
#endif

#ifdef ARM9
#include "VideoGL.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>

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

extern int vsnprintf( char* buffer, size_t buf_size, const char* format, va_list vlist );
#endif

#include "Scene.h"
#include "Renderable.h"
#include "Textures.h"
#include "Camera.h"

#define OBJ_CRAB 0
#define OBJ_OCTOPUS 1
#define OBJ_STARFISH 2
#define OBJ_FISH 3
#define OBJ_PLANT 4

Scene scene;	/// the scene we render
bool wireMode = false;	/// wireframe mode on / off
bool flatShading = false;	/// flat shading on / off

using namespace std;

#ifdef WIN32
extern int startAquarium(int argc, char *argv[]);

/// Program starts here
int main(int argc, char *argv[])
{
	startAquarium(argc, argv);
	return 0;
}
#endif

int startAquarium(int argc, char *argv[])
{
	TWLPrintf("-- Program starting\n");

	srand(time(NULL));
	init(argc, argv);

	// register our call-back functions
	TWLPrintf("-- Registering callbacks\n");
	
#ifdef WIN32
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(keyboardInput);
	glutSpecialFunc(keyboardInputSpecial);
#endif

	// generate/load textures
	TWLPrintf("-- Generating/Loading Textures\n");

	getTextures();

	// create the scene and set perspective projection as default
	scene ;
	SceneInit1(&scene);
	scene.perspectiveMode = true;

	// create all quads for the floor of the aquarium
	for (GLfloat i = -9.5; i <= 9.5; i++)
	{
		for (GLfloat j = -9.5; j <= 9.5; j++)
		{
			GLfloat mat1[] = {1.f, 1.f, 1.f, 1.f};
			struct MarineObject quad = BuildQuad((void*)&_drawQuad, mat1, 120.f, NULL, NULL, NULL, NULL);
			quad.ry = 0.0f;	// we don't want random rotation
			quad.rx = 90.0f;
			quad.x = 3.5f * i;
			quad.z = 3.5f * j;
			scale(&quad, 3.5f, 3.5f, 1.0f);
			add(&scene, &quad);
		}
	}

	// 'fake' keys being pressed to enable the state to
	// setup lighting and fog
	keyboardInput((unsigned char)'L', 0, 0);
	keyboardInput((unsigned char)'0', 0, 0);
	keyboardInput((unsigned char)'1', 0, 0);
	keyboardInput((unsigned char)'F', 0, 0);

	// add some stuff to the scene
	for (int o = 0; o < 3; o++)
	{
		addObject(OBJ_CRAB);
		addObject(OBJ_STARFISH);
		addObject(OBJ_FISH);
		addObject(OBJ_FISH);
		addObject(OBJ_FISH);
		addObject(OBJ_OCTOPUS);
		addObject(OBJ_PLANT);
	}

	// start the timer and enter the mail GLUT loop
#ifdef WIN32
	glutTimerFunc(50, animator, 0);
	glutMainLoop();
#endif

#if defined(ARM9)
    startTimerCounter(tUnitsMilliseconds, 1);

	while(1==1){
		
		//Handle Input & game logic
		scanKeys();
		
		if(runGameTick == true){
			//next(1);
			runGameTick = false;
		}
		
		struct touchPosition touch;
		XYReadScrPosUser(&touch);
		//mouse(0, 0, touch.px, touch.py); 	//OnTSCTouch todo
		keyboardInput(keysDown(), touch.px, touch.py); //Keys
		
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
#ifdef WIN32
	glutPostRedisplay();
	glutTimerFunc(25, animator, 0);
#endif
}


#ifdef WIN32
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
	
#ifdef WIN32
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
	case 'F':	// toggles fog on/off
		scene.fogMode = !scene.fogMode;
		if (scene.fogMode) glEnable(GL_FOG);
		else glDisable(GL_FOG);
		break;

	case 'l':
	case 'L':	// toggles lighting calculations on/off
		scene.lightMode = !scene.lightMode;
		if (scene.lightMode) glEnable(GL_LIGHTING);
		else glDisable(GL_LIGHTING);
		break;

	case '0':	// toggles light 0 on / off
		scene.light0On = ! scene.light0On;
		if (scene.light0On) glEnable(GL_LIGHT0);
		else glDisable(GL_LIGHT0);
		break;

	case '1':	// toggles light 1 on / off
		scene.light1On = ! scene.light1On;
		if (scene.light1On) glEnable(GL_LIGHT1);
		else glDisable(GL_LIGHT1);
		break;
#endif

	}
}


/// Processes special keyboard keys like F1, F2, etc
void keyboardInputSpecial(int key, int x, int y)
{
	switch (key){
	
#ifdef WIN32
	case GLUT_KEY_F1:
		scene.showMenu = !scene.showMenu;
		break;

	case GLUT_KEY_F2:
		addObject(OBJ_CRAB);
		break;

	case GLUT_KEY_F3:
		addObject(OBJ_OCTOPUS);
		break;

	case GLUT_KEY_F4:
		addObject(OBJ_STARFISH);
		break;

	case GLUT_KEY_F5:
		addObject(OBJ_FISH);
		break;

	case GLUT_KEY_F6:
		addObject(OBJ_PLANT);
		break;

	case GLUT_KEY_LEFT:
		anticlockwise(&scene.camera);
		break;

	case GLUT_KEY_RIGHT:
		clockwise(&scene.camera);
		break;

	case GLUT_KEY_UP:
		inc(&scene.camera);
		break;

	case GLUT_KEY_DOWN:
		dec(&scene.camera);
		break;
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
		GLfloat mat1[] = {0.3f, 0.3f, 0.3f, 1.f};
		object = BuildStarfish((void*)&_drawStarFish, mat1, 32.f, vertexStarFish, normalStarFish, NULL, coloursStarFish);
	}break;
	case OBJ_CRAB:{
		y = -0.4f;
		GLfloat mat1[] = {0.5f, 0.5f, 0.5f, 1.f};
		object = BuildCrab((void*)&_drawCrab, (void*)&_draw_dlistCrab, mat1, 50.f, NULL, NULL, NULL, NULL);
	}break;
	case OBJ_FISH:{
		y = getRand(-26.0f, 25.0f);
		GLfloat mat1[] = {1.f, 1.f, 1.f, 1.f};
		object = BuildFish((void*)&_drawFish, mat1, 120.f, vertexFish, normalFish, texelsFish, coloursFish);
	}break;
	case OBJ_OCTOPUS:{
		y = getRand(-27.0f, 25.0f);
		GLfloat mat1[] = {0.0f, 0.0f, 2.0f, 1.f};
		object = BuildOctopus((void*)&_drawOctopus, mat1, 50.f, NULL, NULL, NULL, NULL);
	}break;
	case OBJ_PLANT:{
		y = 0.0f;
		GLfloat mat1[] = {0.1f, 0.3f, 0.15f, 1.f};
		GLfloat mat2[] = {0.6f, 1.f, 0.8f, 1.f};
		object = BuildPlant((void*)&_draw_dlistPlant, NULL, mat1, mat2, 100.f, NULL, NULL, NULL, NULL);
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

	glMatrixMode(GL_PROJECTION
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glLoadIdentity(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// setup new viewing volume based on the aspect ratio and projection type
	if (scene.perspectiveMode == true){
		gluPerspective(-45.0f, aspect, 1.0f, 250.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
	}
	else {
		// orthographic mode correction depends on whether the ratio is greater
		// or less than 1.0 as the viewport must be scaled in different
		// directions to look right
		if (aspect >= 1.0f){
			glOrtho(-40.0f * aspect, 40.0f * aspect, -40.0f, 40.0f, 1.0f, 250.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
			);
		}
		else {
			glOrtho(-40.0f, 40.0f, -40.0f * iaspect, 40.0f * iaspect, 1.0f, 250.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
			);
		}
	}

	glMatrixMode(GL_MODELVIEW
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
}


/// Initiates all textures
void getTextures(void)
{
#ifdef WIN32
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(2, texturesRenderable);

	getSandTexture();
	getFishTexture();
#endif

#ifdef ARM9
//ARM9 Todo
#endif
}


/// Loads the sand texture
void getSandTexture(void)
{
#ifdef WIN32
	glBindTexture(GL_TEXTURE_2D, texturesRenderable[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 64, 64, GL_RGB, GL_UNSIGNED_BYTE, sand_image.pixel_data);
#endif
}


/// Loads the fish texture
void getFishTexture(void)
{
#ifdef WIN32
	glBindTexture(GL_TEXTURE_2D, texturesRenderable[1]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 128, 128, GL_RGB, GL_UNSIGNED_BYTE, fish_image.pixel_data);
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

#ifdef WIN32
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
#endif
	// setup our widnow now

	TWLPrintf("-- Creating window\n");

#ifdef WIN32
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
#ifdef WIN32
		, 1.0
#endif	
	);
	glShadeModel(GL_SMOOTH);

	// depth testing used on with less than testing
#ifdef WIN32
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
#endif

	// setup  fog, but disable for now
#ifdef WIN32
	glDisable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	GLfloat fogColor[4] = {0.0f, 0.5f, 0.55f, 1.0f};
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.0075);
	
	// enable normalising of normals after scaling
	glEnable(GL_NORMALIZE);
#endif
	// setup lighting, but disable for nwo
	glDisable(GL_LIGHTING);
#ifdef WIN32
	GLfloat ambient[] = {0.1f, 0.1f, 0.1f, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
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

	DLSOLIDCUBE0_06F = (GLint)glGenLists(1
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	//glut2SolidCube(); -> NDS GX Implementation
	#ifdef _MSC_VER
	glNewList(DLSOLIDCUBE0_06F, GL_COMPILE);
	#endif
	#ifdef ARM9
	glNewList(DLSOLIDCUBE0_06F, GL_COMPILE, USERSPACE_TGDS_OGL_DL_POINTER);
	#endif
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
#ifdef _MSC_VER
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
#endif
#ifdef ARM9
			glBegin(GL_QUADS, USERSPACE_TGDS_OGL_DL_POINTER);
			glNormal3fv(&n[i][0], USERSPACE_TGDS_OGL_DL_POINTER);
			glTexCoord2f(0, 0, USERSPACE_TGDS_OGL_DL_POINTER);
			glVertex3fv(&v[faces[i][0]][0], USERSPACE_TGDS_OGL_DL_POINTER);
			glTexCoord2f(1, 0, USERSPACE_TGDS_OGL_DL_POINTER);
			glVertex3fv(&v[faces[i][1]][0], USERSPACE_TGDS_OGL_DL_POINTER);
			glTexCoord2f(1, 1, USERSPACE_TGDS_OGL_DL_POINTER);
			glVertex3fv(&v[faces[i][2]][0], USERSPACE_TGDS_OGL_DL_POINTER);
			glTexCoord2f(0, 1, USERSPACE_TGDS_OGL_DL_POINTER);
			glVertex3fv(&v[faces[i][3]][0], USERSPACE_TGDS_OGL_DL_POINTER);
			glEnd(USERSPACE_TGDS_OGL_DL_POINTER);
#endif
		}
		#ifdef WIN32
		glEndList();
		#endif
		#ifdef ARM9
		glEndList(USERSPACE_TGDS_OGL_DL_POINTER);
		#endif
	}

	glDisable(GL_CULL_FACE); 
	glCullFace (GL_NONE);
}

//ARM9 version is in main.c
#ifdef WIN32
int TWLPrintf(const char *fmt, ...){
	return 0;
}
#endif
