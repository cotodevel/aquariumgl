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

	static unsigned int textures[2];	/// texture id array
public:
	Renderable();	/// default constructor
	virtual ~Renderable();	/// default destructor

	void build(GLuint &dlist);	/// builds a display list of this object
	void move(GLfloat x, GLfloat y, GLfloat z);	/// moves the object
	void rotate(GLfloat x, GLfloat y, GLfloat z);	/// rotates the object
	void scale(GLfloat x, GLfloat y, GLfloat z);	/// scales the object
	void draw(void);

	static GLfloat getRand(GLfloat minimum, GLfloat range);	/// generates a random value in max range

	//specific generic draw members
	void * buildDL; //a.k.a _draw()
	void * _draw_dlist; 
	void * callerArg0;	//arg0 of above caller (either one)
	int callerType;		//caller type

};
#endif


/*
* StarFish class. Draws a yellowish colour StarFish.
*
* Igor Kromin 40125374
*/
#ifndef __STARFISH_3201
#define __STARFISH_3201
class StarFish : public Renderable
{
public:
	StarFish(void * drawStarFishFn);	/// default constructor
	virtual ~StarFish();	/// default destructor
	static GLfloat vertex[];	/// vertex array data
	static GLfloat normal[];	/// normals for each vertex
	static GLfloat colours[];	/// colour array data
	static GLfloat material[4];
	static GLfloat shininess;
};
#endif


/*
* Fish class. Draws a Fish.
*
* Igor Kromin 40125374
*/
#ifndef __FISH_3201
#define __FISH_3201
class Fish : public Renderable
{
public:
	Fish(void * drawFishFn);	/// default constructor
	virtual ~Fish();	/// default destructor

	GLfloat tailAngle;
	GLfloat tailAngleCutOff;
	GLfloat tailAngleInc;
	static GLfloat vertex[];	/// vertex array data
	static GLfloat normal[];	/// normals for each vertex
	static GLfloat texels[]; /// texture coords for each vertex
	static GLfloat colours[];	/// colour array data
	static GLfloat material[4];
	static GLfloat shininess;
	void drawSide(void);	/// draws a side of the fish
};
#endif


/*
* Crab class. Draws a pinky coloured crab.
*
* Igor Kromin 40125374
*/
#ifndef __CRAB_3201
#define __CRAB_3201
class Crab : public Renderable
{
public:
	Crab(void * drawCrabFn);	/// default constructor
	virtual ~Crab();	/// default destructor
	static void drawLeg(void);	/// draws one leg
	static void drawLeg(GLfloat jointAngle, GLfloat jointOffset);	/// draw leg with an angle specified
	static void drawLegs(void);	/// draws complete set of legs
	GLuint dlist;	/// display list
	static GLfloat material[4];
	static GLfloat shininess;
	
	void _draw_dlist(void);	/// draws the crab's display list
	void generate(int level, int number);	/// generates the branches

friend class Octopus;	/// so the octopus can use the drawLeg method
};
#endif

/*
* Octopus class. Draws a Octopus.
*
* Igor Kromin 40125374
*/
#ifndef __OCTOPUS_3201
#define __OCTOPUS_3201
class Octopus : public Renderable
{
public:
	Octopus(void * drawOctopusFn);	/// default constructor
	virtual ~Octopus();	/// default destructor
	GLfloat legAngle;	/// angle to spin the legs at
	GLfloat legAngleCutOff;	/// cut of angle for spinning
	GLfloat legAngleInc;	/// angle spin increment
	static GLfloat material[4];
	static GLfloat shininess;
};
#endif


/*
* Quad class. Draws a yellowish colour shaded quad.
*
* Igor Kromin 40125374
*/
#ifndef __QUAD_3201
#define __QUAD_3201
class Quad : public Renderable
{
public:
	Quad(void * drawQuadFn);	/// default constructor
	virtual ~Quad();	/// default destructor
	static GLfloat material[4];
	static GLfloat shininess;
};
#endif


/*
* Plant class. Draws a plant.
*
* Igor Kromin 40125374
*/
#ifndef __PLANT_3201
#define __PLANT_3201
#include <list>
class Plant : public Renderable
{
public:
	Plant(void * drawPlantFn);	/// default constructor
	virtual ~Plant();	/// default destructor
	GLuint dlist;	/// display list
	static GLfloat material1[4];
	static GLfloat material2[4];
	static GLfloat shininess;
	void generate(int level, int number);	/// generates the branches
	void _draw_dlist(void);	/// draws the display list of this object
};

typedef void (*draw_objectStarFish)(StarFish & starFishObj);
typedef void (*draw_objectFish)(Fish & fishObj);
typedef void (*draw_objectCrab)(Crab & crabObj);
typedef void (*draw_objectOctopus)(Octopus & octopusObj);

typedef void (*draw_objectQuad)(Quad & quadObj);

typedef void (*draw_objectPlant)(Plant & plantObj);

#endif

extern void drawSphere(float r, int lats, int longs);
extern void drawCircle(GLfloat x, GLfloat y, GLfloat r, GLfloat BALL_RADIUS);
extern void drawCylinder(int numMajor, int numMinor, float height, float radius);
extern GLint DLSOLIDCUBE0_06F;
extern void glut2SolidCube0_06f();

extern void _drawQuad(Quad * quadObj); /// draws the Quad
extern void _drawStarFish(StarFish * starFishObj); /// draws the StarFish
extern void _drawCrab(Crab * crabObj);  /// draws the crab
extern void _drawFish(Fish * fishObj);		/// draws the Fish
extern void _drawOctopus(Octopus * octopusObj);	/// draws the Octopus
extern void _drawPlant(Plant * plantObj);	/// draws the plant

#ifdef __cplusplus
extern "C"{
#endif

extern int TWLPrintf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif
