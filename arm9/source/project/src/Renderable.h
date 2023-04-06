/*
*
* THIS FILE HAS ALL OF THE HEADER INFORMATION FOR THE RENDERABLE OBJECT
* AS WELL AS THE CRAB, FISH, OCTOPUS, QUAD AND STARFISH.
*
* THE REASON FOR PUTTING IT ALL HERE IS BECAUSE THE SUBMIT SYSTEM ON THE
* ITEE SERVERS ONLY ALLOWS TO SUBMIT 15 FILES AND IF I HAD SEPARATE FILES
* FOR EACH OBJECT, THEN IT GET TO ABOUT 20 FILES.
*
* --Igor Kromin 40125374
*
*/

/*
* Renderable Class. This is an abstract class that all of the objects that
* are attached to the scene inherit from. It provides basic functionality
* for moving/rotating/scaling objects as well as defining a pure virtual
* function '_draw()' that must be difined by its children classes which 
* actually does the drawing of any specific object.
*
* Igor Kromin 40125374
*/
#ifndef __RENDERABLE_3201
#define __RENDERABLE_3201

#ifdef _MSC_VER
#include <GL/glut.h>
#endif

#ifdef ARM9
#include "VideoGL.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <math.h>

#ifndef TEXTURE_IDS
#define TEXTURE_IDS
#define FLOOR_TEXTURE 1
#define FISH_TEXTURE 2
#endif

typedef void (*draw_object)();

#define RENDERABLE_STARFISH ((int)1)
#define RENDERABLE_FISH ((int)2)
#define RENDERABLE_CRAB ((int)3)
#define RENDERABLE_OCTOPUS ((int)4)
#define RENDERABLE_QUAD ((int)5)
#define RENDERABLE_PLANT ((int)6)
#define RENDERABLE_NONE ((int)7)

class Renderable
{
public:
	GLfloat x;	/// x position of object
	GLfloat y;	/// y position of object
	GLfloat z;	/// z position of object

	GLfloat rx;	/// x rotation angle of object
	GLfloat ry;	/// y rotation angle of object
	GLfloat rz;	/// z rotation angle of object

	GLfloat sx;	/// x scale of object
	GLfloat sy;	/// y scale of object
	GLfloat sz;	/// z scale of object

	bool isList;	/// is this a display list object?
	
	Renderable(void * buildDLIn, int callerTypeIn, void * _draw_dlistIn);	/// default constructor
	virtual ~Renderable();	/// default destructor

	
	
	//specific generic draw members
	void * buildDL; //a.k.a _draw()
	void * _draw_dlist; 
	int callerType;		//caller type
};
#endif

extern unsigned int texturesRenderable[2];	/// texture id array
extern GLfloat getRand(GLfloat minimum, GLfloat range);	/// generates a random value in max range

/*
* 
*
* Igor Kromin 40125374
*/
#ifndef __MARINEOBJ_3201
#define __MARINEOBJ_3201
class MarineObject : public Renderable
{
public:
	MarineObject(
		void * drawPlantFn, void * displayListFn, int callerType,
		GLfloat materialIn1[4], GLfloat materialIn2[4], GLfloat shininessIn,
		GLfloat * vertexIn,
		GLfloat * normalIn,
		GLfloat * texelsIn,
		GLfloat * coloursIn
	);	/// default constructor
	virtual ~MarineObject();	/// default destructor

	GLfloat * vertex;	/// vertex array data
	GLfloat * normal;	/// normals for each vertex
	GLfloat * texels; /// texture coords for each vertex
	GLfloat * colours;	/// colour array data
	GLfloat material1[4];
	GLfloat material2[4]; //only used by Plant
	GLfloat shininess;
	GLuint dlist;	/// display list
	
	//Fish
	void drawSideFish(void);	/// draws a side of the fish
	GLfloat tailAngle;
	GLfloat tailAngleCutOff;
	GLfloat tailAngleInc;

	//Crab
	//No ext. dependencies

	//Octopus
	GLfloat legAngle;	/// angle to spin the legs at
	GLfloat legAngleCutOff;	/// cut of angle for spinning
	GLfloat legAngleInc;	/// angle spin increment

	//Plant
	//No ext. dependencies
	
	//Quad
	//No ext. dependencies

	//Starfish
	//No ext. dependencies
};
#endif

extern void build(MarineObject * marineObjRef, GLuint *dlist);	/// builds a display list of this object
extern void move(MarineObject * marineObjRef, GLfloat x, GLfloat y, GLfloat z);	/// moves the object
extern void rotate(MarineObject * marineObjRef, GLfloat x, GLfloat y, GLfloat z);	/// rotates the object
extern void scale(MarineObject * marineObjRef, GLfloat x, GLfloat y, GLfloat z);	/// scales the object
extern void draw(MarineObject * marineObjRef);

//Builders
extern class MarineObject BuildFish(
	void * drawFishFn, GLfloat materialIn[4], GLfloat shininessIn,
	GLfloat * vertexIn, GLfloat * normalIn, GLfloat * texelsIn, GLfloat * coloursIn
);
extern class MarineObject BuildCrab(
	void * drawCrabFn, void * displayListFn, GLfloat materialIn[4], GLfloat shininessIn,
	GLfloat * vertexIn, GLfloat * normalIn, GLfloat * texelsIn, GLfloat * coloursIn
);
extern class MarineObject BuildOctopus(
	void * drawOctopusFn, GLfloat materialIn[4], GLfloat shininessIn,
	GLfloat * vertexIn, GLfloat * normalIn, GLfloat * texelsIn, GLfloat * coloursIn
);
extern class MarineObject BuildPlant(
	void * drawPlantFn, void * displayListFn,
	GLfloat materialIn1[4], GLfloat materialIn2[4], GLfloat shininessIn,
	GLfloat * vertexIn,
	GLfloat * normalIn,
	GLfloat * texelsIn,
	GLfloat * coloursIn
);
extern class MarineObject BuildQuad(
	void * drawQuadFn, GLfloat materialIn[4], GLfloat shininessIn,
	GLfloat * vertexIn, GLfloat * normalIn, GLfloat * texelsIn, GLfloat * coloursIn
);
extern class MarineObject BuildStarfish(
	void * drawStarFishFn,
	GLfloat materialIn[4], GLfloat shininessIn,
	GLfloat * vertexIn,
	GLfloat * normalIn,
	GLfloat * texelsIn,
	GLfloat * coloursIn
);
extern void drawSphere(float r, int lats, int longs);
extern void drawCircle(GLfloat x, GLfloat y, GLfloat r, GLfloat BALL_RADIUS);
extern void drawCylinder(int numMajor, int numMinor, float height, float radius);
extern GLint DLSOLIDCUBE0_06F;
extern void glut2SolidCube0_06f();


extern void _drawCrab(MarineObject * marineObj);  /// draws the crab
extern void draw1LegCrab();	/// draws one leg
extern void drawLegCrab(GLfloat jointAngle, GLfloat jointOffset);	/// draw leg with an angle specified
extern void drawLegsCrab();	/// draws complete set of legs
extern void _draw_dlistCrab(MarineObject * marineObj);	/// draws the crab's display list
extern void _draw_dlistPlant(MarineObject * marineObj);	/// draws the display list of this object
extern void generateBranches(int level, int number);	/// generates the branches for Plants and Crabs


extern void _drawFish(MarineObject * marineObj);		/// draws the Fish
extern void _drawOctopus(MarineObject * marineObj);	/// draws the Octopus
extern void _drawQuad(MarineObject * marineObj); /// draws the Quad
extern void _drawPlant(MarineObject * marineObj);	/// draws the plant
extern void _drawStarFish(MarineObject * marineObj); /// draws the StarFish

extern GLfloat vertexFish[];
extern GLfloat normalFish[];
extern GLfloat texelsFish[];
extern GLfloat coloursFish[];

extern GLfloat vertexStarFish[];
extern GLfloat normalStarFish[];
extern GLfloat coloursStarFish[];

#ifdef __cplusplus
extern "C"{
#endif

extern int TWLPrintf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif
