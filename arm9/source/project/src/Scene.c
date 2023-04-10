#if defined(_MSC_VER)
//disable _CRT_SECURE_NO_WARNINGS message to build this in VC++
#pragma warning(disable:4996)
#endif

/*
* Scene class. This class takes care of drawing all of the contents
* of our virtual aquarium as well as camera movement.
*
* This is a partial scene graph implementation where there is a list
* of objects to draw, the class sets up the environment and calls the
* draw method on each object in the queue.
*
* Igor Kromin 40125374
*/

#include "Scene.h"

#ifndef _MSC_VER
					// //
#define ARM9 1		// Enable only if not real GCC + NDS environment
#undef _MSC_VER		// //
#undef WIN32		// //
#endif

int widthScene;	/// the width of the window
int heightScene;	/// the height of the window

// light 0 colours
GLfloat ambient0Scene[4]	= {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat diffuse0Scene[4]	= {0.4f, 0.4f, 0.4f, 1.01f};
GLfloat specular0Scene[4]	= {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat position0Scene[4]	= {0.0f, -1.0f, 0.0f, 0.0f};

// light 1 colours
GLfloat ambient1Scene[4]	= {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat diffuse1Scene[4]	= {0.45f, 0.45f, 0.45f, 1.0f};
GLfloat specular1Scene[4]	= {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat position1Scene[4]	= {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat direction1Scene[4]	= {0.0f, 0.0f, -1.0f};

// spotlight cut-off angle
GLfloat spotAngleScene	= 15.f;


/// Default Constructor. Initialises defaults.
void SceneInit1(struct Scene * Inst){
	TWLPrintf("-- Creating scene\n");

#ifdef WIN32
	Inst->error = GL_NO_ERROR;
#endif	
	Inst->polygonModel = GL_FILL;
	
	memset(&Inst->elementsStart[0], 0, (sizeof(struct MarineObject)*renderableElementsTotal));
	Inst->showMenu = true;	// menu is on
	Inst->light0On = false;	// light 0 is off
	Inst->light1On = false;	// light 1 is off
	Inst->fogMode = false;	// fog is off
	Inst->lightMode = false;	// lighting is off

	// set all counters to zero
	Inst->objects[OBJ_CRAB] = 0;
	Inst->objects[OBJ_STARFISH] = 0;
	Inst->objects[OBJ_OCTOPUS] = 0;
	Inst->objects[OBJ_FISH] = 0;
	Inst->objects[OBJ_PLANT] = 0;

	// set up light 0 colours
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0Scene
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0Scene
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0Scene
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// set up light 1 colours
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1Scene
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1Scene
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1Scene
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	CameraInit1(&Inst->camera); //construct camera
}

/// Renders a single frame of the scene
/*
* This function renders all of the objects that are attached
* to the rendering queue of the Scene.
*/
void drawScene(){
	struct Scene * Inst = &scene;
	// clear scene
	clearScene();
	
	// set up the miner's hat light before moving the camera
	glLightfv(GL_LIGHT1, GL_POSITION, position1Scene
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	
//Unsupported by NDS GX
#ifdef WIN32
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotAngleScene);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1Scene);
#endif

	//position camera
	position(&Inst->camera);

	// set up our directional overhead light
	glLightfv(GL_LIGHT0, GL_POSITION, position0Scene
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// check if there are any objects to draw
	{
		int i = 0;
		for (i = 0; i < Inst->curElementAlloced; i++){
			// draw all elements in the scene
			draw(&Inst->elementsStart[i]);
		}
	}

#ifdef WIN32
	drawHUD(Inst);
	glutSwapBuffers();
#endif

#ifdef ARM9
	glFlush(USERSPACE_TGDS_OGL_DL_POINTER);
	//HaltUntilIRQ(); //Save power until next Vblank
#endif

}


/// Clears the scene for drawing
/*
* This method clears the scene for drawing. Both the colour
* and the depth buffers are cleared. The cameara position is
* also reset here.
*/
void clearScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glLoadIdentity(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
}


/// Adds an object to the rendering queue
/*
* This method simply adds on the passed object to the end
* of the rendering queue.
*/
bool add(struct Scene * Inst, struct MarineObject *object){
	if(Inst->curElementAlloced < renderableElementsTotal){
		memcpy(&Inst->elementsStart[Inst->curElementAlloced], object, sizeof(struct MarineObject));
		Inst->curElementAlloced++;
		return true;
	}
	return false;
}


/// Draws the heads up display
/*
* This method draws all of the required information about the
* current scene in the window. Orthographic projection is used
* here to position the text easier. both the PROJECTION and the
* MODELVIEW matrices are restored to their original states when
* this method returns.
*/
void drawHUD(struct Scene * Inst){
	GLboolean lightsOn;
	
	// disable depth testing to HUD is drawn on top of everything
	glDisable(GL_DEPTH_TEST);

	// disable lightint so we see everything OK
	glGetBooleanv(GL_LIGHTING, &lightsOn);
	glDisable(GL_LIGHTING);

	// grab and save the current projection
	glMatrixMode(GL_PROJECTION
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glLoadIdentity(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// setup new projection
	glOrtho(-0.1f, 10.0f, 10.0f, -0.4f, -1.0f, 10.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// grab and save current view
	glMatrixMode(GL_MODELVIEW
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glLoadIdentity(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	glColor3f(1.0f, 1.0f, 1.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// print all of the stats
	printGL1(Inst, 0.0f, 0.0f, 0.0f, "Crabs:    ", Inst->objects[OBJ_CRAB]);
	printGL1(Inst, 1.5f, 0.0f, 0.0f, "Starfish: ", Inst->objects[OBJ_STARFISH]);
	printGL1(Inst, 3.0f, 0.0f, 0.0f, "Octopi:   ", Inst->objects[OBJ_OCTOPUS]);
	printGL1(Inst, 4.5f, 0.0f, 0.0f, "Fish:     ", Inst->objects[OBJ_FISH]);
	printGL1(Inst, 6.0f, 0.0f, 0.0f, "Plants:   ", Inst->objects[OBJ_PLANT]);

	/*
	* the printGL() function expects the string to be 30 characters in length
	* so we have to pad each string with white space so that weird characters
	* do not appear
	*/
	
	// print lighting mode calculations
	if (Inst->lightMode) printGL2(Inst, 0.0f, 0.3f, -1.0f, "Lighting Calculations: On     ");
	else printGL2(Inst, 0.0f, 0.3f, -1.0f, "Lighting Calculations: Off    ");

	// print light 0 status
	if (Inst->light0On) printGL2(Inst, 0.0f, 0.6f, -1.0f, "Overhead Light: On            ");
	else printGL2(Inst, 0.0f, 0.6f, -1.0f, "Overhead Light: Off           ");

	// print light 1 status
	if (Inst->light1On) printGL2(Inst, 0.0f, 0.9f, -1.0f, "Miner's Hat Light: On         ");
	else printGL2(Inst, 0.0f, 0.9f, -1.0f, "Miner's Hat Light: Off        ");

	// print fog status
	if (Inst->fogMode) printGL2(Inst, 0.0f, 1.2f, -1.0f, "Fog: On                       ");
	else printGL2(Inst, 0.0f, 1.2f, -1.0f, "Fog: Off                      ");

	// print the menu if needed
	if (Inst->showMenu)
		printMenu(Inst);

	// restore old view
	glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif	
	);

	// grab and restore old projection
	glMatrixMode(GL_PROJECTION
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// return to view mode
	glMatrixMode(GL_MODELVIEW
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// enable depth testing again
	glEnable(GL_DEPTH_TEST);

	// enable lighting if needed
	if (lightsOn) glEnable(GL_LIGHTING);
}


/// Prints a string with a count attached
void printGL1(struct Scene * Inst, GLfloat x, GLfloat y, GLfloat z, const char *str, int count){
	char buffer[30];
	int i = 0;
	for (i = 0; i < 30; ++i){
		buffer[i] = ' ';
	}
	sprintf(buffer,"%s %i", str, count);
	printGL2(Inst, x, y, z, buffer);
}


/// Prints a string in the window
void printGL2(struct Scene * Inst, GLfloat x, GLfloat y, GLfloat z, const char *str){
#ifdef WIN32
	int j = 0;
	glRasterPos3f(x, y, z);
	// print character one by one
	for (j = 0; j < 30; j++) {
		int c = str[j];
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
	}
#endif
}


/// Displays the menu screen
void printMenu(struct Scene * Inst){
	glColor3f(0.0f, 0.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	printGL2(Inst, 0.0f, 1.7f, -1.0f, "---[ Help Screen ]------------");

	glColor3f(1.0f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	printGL2(Inst, 0.0f, 2.0f, -1.0f, "F1    - Toggle Help Screen      ");
	printGL2(Inst, 0.0f, 2.3f, -1.0f, "F2    - Add a crab              ");
	printGL2(Inst, 0.0f, 2.6f, -1.0f, "F3    - Add an octopus          ");
	printGL2(Inst, 0.0f, 2.9f, -1.0f, "F4    - Add a starfish          ");
	printGL2(Inst, 0.0f, 3.2f, -1.0f, "F5    - Add a fish              ");
	printGL2(Inst, 0.0f, 3.5f, -1.0f, "F6    - Add a plant             ");

	glColor3f(0.5f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	printGL2(Inst, 0.0f, 3.8f, -1.0f, "F/f   - Toggle fog on/off       ");
	printGL2(Inst, 0.0f, 4.1f, -1.0f, "W/w   - Toggle wireframe on/off ");
	printGL2(Inst, 0.0f, 4.4f, -1.0f, "L/l   - Toggle lighting on/off  ");
	printGL2(Inst, 0.0f, 4.7f, -1.0f, "1/0   - Toggle light 1/0 on/off ");

	glColor3f(0.0f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	printGL2(Inst, 0.0f, 5.0f, -1.0f, "A/Z   - Inc/Dec elevation angl  ");
	printGL2(Inst, 0.0f, 5.3f, -1.0f, "UP    - Dec distance to origin  ");
	printGL2(Inst, 0.0f, 5.6f, -1.0f, "DOWN  - Inc distance to origin  ");
	printGL2(Inst, 0.0f, 5.9f, -1.0f, "LEFT  - Rotate clockwise        ");
	printGL2(Inst, 0.0f, 6.2f, -1.0f, "RIGHT - Rotate anticlockwise    ");

	glColor3f(0.0f, 0.0f, 1.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	printGL2(Inst, 0.0f, 6.5f, -1.0f, "ESC   - Exit                    ");

	glColor3f(0.0f, 0.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	printGL2(Inst, 0.0f, 6.8f, -1.0f, "---[Igor Kromin 40125374 ]----");
}
