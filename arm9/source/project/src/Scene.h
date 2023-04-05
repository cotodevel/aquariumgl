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

#ifndef __SCENE_3201
#define __SCENE_3201

#ifdef _MSC_VER
#include <GL/glut.h>
#endif

#ifdef ARM9
#include "VideoGL.h"
#endif
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "Renderable.h"
#include "Camera.h"

#define OBJ_CRAB 0
#define OBJ_OCTOPUS 1
#define OBJ_STARFISH 2
#define OBJ_FISH 3
#define OBJ_PLANT 4

#define SIGN(x) (x < 0 ? (-1) : 1)

struct Scene{
	GLenum error;	/// current error
	GLenum polygonModel;	/// polygon mode wire/solid

	std::vector<Renderable*> elements;	/// list of elements
	
	Camera camera;	/// camera (duh)
	int objects[5];	/// counter for objects
	bool perspectiveMode;	/// perspective on / off
	bool showMenu;	/// show menu on / off
	bool light0On;	/// light 0 on / off
	bool light1On;	/// light 1 on / off
	bool fogMode;	/// fog on / off
	bool lightMode;	/// lighting on / off

};

extern void clearScene();	/// clears the scene for drawing

extern void SceneInit1(struct Scene * Inst);	/// default constructor
extern bool render(struct Scene * Inst);	/// renders a frame
extern void drawHUD(struct Scene * Inst);	/// draw the heads up display
extern void add(struct Scene * Inst, Renderable *object);	/// add object to rendering queue

extern void printGL1(struct Scene * Inst, GLfloat x, GLfloat y, GLfloat z, const char *str, int count);
extern void printGL2(struct Scene * Inst, GLfloat x, GLfloat y, GLfloat z, const char *str);
extern void printMenu(struct Scene * Inst);	/// display the menu
	
// light 0 colours
extern GLfloat ambient0Scene[4];
extern GLfloat diffuse0Scene[4];
extern GLfloat specular0Scene[4];
extern GLfloat position0Scene[4];

// light 1 colours
extern GLfloat ambient1Scene[4];
extern GLfloat diffuse1Scene[4];
extern GLfloat specular1Scene[4];
extern GLfloat position1Scene[4];
extern GLfloat direction1Scene[4];

extern int widthScene;	/// the width of the window
extern int heightScene;	/// the height of the window

#endif

extern bool init(int argc, char *argv[]);	/// initialises the application
extern void setupGL(void);	/// initialises OpenGL
extern void animator(int type);	/// animates the aquarium
extern void resizeWindow(int w, int h);	/// resizes the window
extern void keyboardInput(unsigned char key, int x, int y);	/// handles keyboard input
extern void keyboardInputSpecial(int key, int x, int y);	/// handles keyboard input (special)
extern void drawScene(void);	/// draws the scene
extern void addObject(int type);	/// adds an object to the scene
extern void setupViewVolume(void);	/// sets up the viewing volume
extern void getTextures(void);	/// initiates all textures
extern void getSandTexture(void);	/// loads the sand texture
extern void getFishTexture(void);	/// loads the fish texture

extern Scene *scene;	/// the scene we render
extern bool wireMode;	/// wireframe mode on / off
extern bool flatShading;	/// flat shading on / off

extern GLfloat spotAngleScene;