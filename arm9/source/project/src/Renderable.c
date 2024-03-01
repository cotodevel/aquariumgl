/*
* Renderable Class. This is an abstract class that all of the objects that
* are attached to the scene inherit from. It provides basic functionality
* for moving/rotating/scaling objects as well as defining a pure virtual
* function '_draw()' that must be difined by its children classes which 
* actually does the drawing of any specific object.
*
* Igor Kromin 40125374
*/

#include "Renderable.h"

#ifndef _MSC_VER
					// //
#define ARM9 1		// Enable only if not real GCC + NDS environment
#undef _MSC_VER		// //
#undef WIN32		// //
#endif

#ifdef ARM9
#include "videoGL.h"
#include "Sphere008.h" //comment out when running on NDSDisplayListUtils
#endif

// create the static parts of all objects
struct MarineObject MarineObjectInit1(
	int callerTypeIn,
	GLfloat materialIn1[4], GLfloat materialIn2[4], GLfloat shininessIn,
	GLfloat * vertexIn,
	GLfloat * normalIn,
	GLfloat * texelsIn,
	GLfloat * coloursIn
){
	struct MarineObject marineObj;
	marineObj.vertex = vertexIn;
	marineObj.normal = normalIn;
	marineObj.texels = texelsIn;
	marineObj.colours = coloursIn;

	marineObj.material1[0] = materialIn1[0];
	marineObj.material1[1] = materialIn1[1];
	marineObj.material1[2] = materialIn1[2];
	marineObj.material1[3] = materialIn1[3];

	marineObj.material2[0] = materialIn2[0];
	marineObj.material2[1] = materialIn2[1];
	marineObj.material2[2] = materialIn2[2];
	marineObj.material2[3] = materialIn2[3];
	marineObj.shininess = shininessIn;
	marineObj.dlist = -1;

	marineObj.tailAngle = 0;
	marineObj.tailAngleCutOff = 0;
	marineObj.tailAngleInc = 0;
	marineObj.legAngle = 0;
	marineObj.legAngleCutOff = 0;
	marineObj.legAngleInc = 0;

	marineObj.x = 0.0f;
	marineObj.y = 0.0f;
	marineObj.z = 0.0f;
	
	marineObj.rx = 0.0f;
	marineObj.ry = getRand(0.0f, 360.0f);
	marineObj.rz = 0.0f;

	marineObj.sx = 1.0f;
	marineObj.sy = 1.0f;
	marineObj.sz = 1.0f;

	marineObj.isList = false;
	marineObj.callerType = callerTypeIn;
	return marineObj;
}

/// Builds a display list of this object.
void build(struct MarineObject * marineObjRef, GLuint *dlist){
	*dlist = glGenLists(1);
	if (!glIsList(*dlist)) {
		printf("TGDS ERROR: glIsList() FALSE!: List (%d)", *dlist);
		while(1==1){}
		marineObjRef->isList = false;
		return;
	}
	marineObjRef->isList = true;
	glPushMatrix();
	glNewList(*dlist, GL_COMPILE);
		//Build display lists once for these objects
		switch(marineObjRef->callerType){
				case(RENDERABLE_CRAB):{
					_drawCrab(marineObjRef);
				}break;

				case(RENDERABLE_PLANT):{
					_drawPlant(marineObjRef);
				}break;
		}
	glEndList();
	glPopMatrix(
#ifdef ARM9
		1
#endif
	);
}


/// Moves the object
/*
* This method moves the object coordinates to the specified position along
* the x, y and z axes.
*/
void move(struct MarineObject * marineObjRef, GLfloat x, GLfloat y, GLfloat z){
	marineObjRef->x = x;
	marineObjRef->y = y;
	marineObjRef->z = z;
}


/// Rotates an object
/*
* This method rotates the object coordinates a specified number of degrees
* around each of the x, y and z axes. All of the rotations are
* performed around the point (1.0f, 1.0f, 1.0f) and happen in
* the following manner: x-rot, y-rot, z-rot
*/
void rotate(struct MarineObject * marineObjRef, GLfloat x, GLfloat y, GLfloat z){
	marineObjRef->rx = x;
	marineObjRef->ry = y;
	marineObjRef->rz = z;
}


/// Scales the object
void scale(struct MarineObject * marineObjRef, GLfloat x, GLfloat y, GLfloat z){
	marineObjRef->sx = x;
	marineObjRef->sy = y;
	marineObjRef->sz = z;
}


/// Draws the object
/*
* This method sets up the object ready for drawing by a concrete
* class. Firstly all of the rotations are performed on the object,
* then the translation is done, finally the _draw() method is
* called to actually draw the object. Upon exit, the MODELVIEW
* matrix is restored.
*
* If a display list has been built for this object, then it is
* draw instead of re-drawing the object.
*/
void draw(struct MarineObject * marineObjRef){
	glPushMatrix();

	glTranslatef(marineObjRef->x, marineObjRef->y, marineObjRef->z);

	glRotatef(marineObjRef->rx, 1.0f, 0.0f, 0.0f);
	glRotatef(marineObjRef->ry, 0.0f, 1.0f, 0.0f);
	glRotatef(marineObjRef->rz, 0.0f, 0.0f, 1.0f);

	glScalef(marineObjRef->sx, marineObjRef->sy, marineObjRef->sz);

	// if the object is flagged as a display list object, then call the
	// display list drawing function of the object, otherwise just call
	// the normal draw function of the object
	switch(marineObjRef->callerType){

		case(RENDERABLE_STARFISH):{
			_drawStarFish(marineObjRef);
		}break;

		case(RENDERABLE_QUAD):{
			_drawQuad(marineObjRef);
		}break;

		case(RENDERABLE_PLANT):{
			_draw_dlistPlant(marineObjRef);
		}break;

		case(RENDERABLE_OCTOPUS):{
			_drawOctopus(marineObjRef);
		}break;

		case(RENDERABLE_CRAB):{
			_draw_dlistCrab(marineObjRef);
		}break;

		//todo: Fish rendering on NDS
		/*
		case(RENDERABLE_FISH):{
			_drawFish(marineObjRef);
		}break;


		*/

	}

	glPopMatrix(
#ifdef ARM9
		1
#endif
	);
}


/// Generates a random value to use
GLfloat getRand(GLfloat minimum, GLfloat range)
{
	return (((GLfloat)rand() / (GLfloat)RAND_MAX) * range) + minimum;
}

