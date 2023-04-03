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
#include <list>
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

class Scene
{
private:
	GLenum error;	/// current error
	GLenum polygonModel;	/// polygon mode wire/solid

	std::list<Renderable*> *elements;	/// list of elements
	std::list<Renderable*>::iterator iter;	/// iterator for the elements

	/// light 0 data
	static GLfloat ambient0[4];
	static GLfloat diffuse0[4];
	static GLfloat specular0[4];
	static GLfloat position0[4];

	/// light 1 data
	static GLfloat ambient1[4];
	static GLfloat diffuse1[4];
	static GLfloat specular1[4];
	static GLfloat position1[4];
	static GLfloat direction1[4];

	static GLfloat spotAngle;
		
public:
	Camera camera;	/// camera (duh)
	int objects[5];	/// counter for objects
	bool perspectiveMode;	/// perspective on / off
	bool showMenu;	/// show menu on / off
	bool light0On;	/// light 0 on / off
	bool light1On;	/// light 1 on / off
	bool fogMode;	/// fog on / off
	bool lightMode;	/// lighting on / off
	
	static int width;	/// the width of the window
	static int height;	/// the height of the window

public:
	Scene();	/// default constructor
	virtual ~Scene();	/// default destructor

	bool render(void);	/// renders a frame
	void add(Renderable *object);	/// add object to rendering queue

private:
	void clear(void);	/// clears the scene for drawing
	void printGL(GLfloat x, GLfloat y, GLfloat z, const char *str, int count);
	void printGL(GLfloat x, GLfloat y, GLfloat z, const char *str);
	void printMenu(void);	/// display the menu
	void drawHUD(void);	/// draw the heads up display
	void distort(void);
	void eval(int i, int j, GLfloat *ix, GLfloat *iy);
};

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
