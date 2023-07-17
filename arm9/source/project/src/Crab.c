/*
* Crab class. Draws a pinky colour crab.
*
* Igor Kromin 40125374
*/
#include "Renderable.h"
#include "Scene.h"

#ifndef _MSC_VER
					// //
#define ARM9 1		// Enable only if not real GCC + NDS environment
#undef _MSC_VER		// //
#undef WIN32		// //
#endif

/// Default Constructor. Builds the display list for the crab.
//Crab class. Draws a pinky coloured crab.
struct MarineObject BuildCrab(
	void * drawCrabFn, void * displayListFn,
	GLfloat materialIn[4], GLfloat shininessIn,
	GLfloat * vertexIn,
	GLfloat * normalIn,
	GLfloat * texelsIn,
	GLfloat * coloursIn
	) 
{
	struct MarineObject obj = MarineObjectInit1(RENDERABLE_CRAB, materialIn, materialIn /*unused*/, shininessIn, vertexIn, normalIn, texelsIn, coloursIn);
	obj.sy = obj.sx = obj.sz = 2.f; // make crab twice as big
	build(&obj, &obj.dlist);
	TWLPrintf("-- Creating crab\n");
	return obj;
}


/// Draws the crab
void _drawCrab(struct MarineObject * marineObj)
{
	/*
	* The materials are set in _draw_dlist() since that function
	* is called eacht time to draw the display list version of the
	* crab, this functin only gets used to build the display list
	*/

	// get our pinky colour
	glColor3f(1.0f, 0.45f, 0.45f);

	// draw crab body (squashed along Y axis
	glPushMatrix();
	glScalef(1.0f, 0.5f, 1.0f);
	drawSphereCustom(0.3f, 16, 16);
	glPopMatrix(
#ifdef ARM9
		1
#endif
	);

	// draw all of the legs
	glPushMatrix();
	drawLegsCrab();
	glScalef(-1.f, 1.f, 1.f);
	drawLegsCrab();
	glPopMatrix(
#ifdef ARM9	
		1
#endif
	);

	// set to use our black colour
	glColor3f(0.0f, 0.0f, 0.0f);

	// draw left crab eye
	glTranslatef(-0.06f, 0.0f, 0.3f);
	drawSphereCustom(0.05f, 12, 8);

	// draw right crab eye
	glTranslatef(0.12f, 0.0f, 0.0f);
	drawSphereCustom(0.05f, 12, 8);
}


/// Draws the display list for the crab object
void _draw_dlistCrab(struct MarineObject * marineObj)
{
	// work out how much to advance the object by relative to its orientation
	GLfloat xInc = cos(marineObj->ry * (3.14156 ) / 180) / 10.0f;
	GLfloat zInc = sin(marineObj->ry * (3.14156 ) / 180) / 10.0f;
	GLfloat pt = 0;

	// the floor is 70.0 x 70.0, but i want to keep the object inside a
	// 65.0 x 65.0 area, so work out the circular boundaries if the object goes
	// outside of this area
	if (marineObj->x < -35) marineObj->x += 85.f;
	if (marineObj->x > 35) marineObj->x -= 85.f;
	if (marineObj->z < -35) marineObj->z += 85.f;
	if (marineObj->z > 35) marineObj->z -= 85.f;

	// increment the object position
	marineObj->x -= xInc;
	marineObj->z += zInc;

	// set up the material properties (only front needs to be set)
	glMaterialfv(GL_FRONT, GL_SPECULAR, marineObj->material1);
	glMaterialf(GL_FRONT, GL_SHININESS, marineObj->shininess);
	
#ifdef WIN32
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
#endif


	_drawCrab(marineObj);
#ifdef WIN32
	// turn of colour material tracking
	glDisable(GL_COLOR_MATERIAL);
#endif
}


/// Draws a leg using an angle between the two bones
void drawLegCrab(GLfloat jointAngle, GLfloat jointOffset){
	// draw first part of a leg
	glPushMatrix();
	glTranslatef(-0.38f, 0.0f, 0.0f);
	glScalef(3.0f, 1.0f, 1.0f);
	glut2SolidCube0_06f();
	glPopMatrix(
#ifdef ARM9
		1
#endif	
	);

	// draw second part of a leg
	glPushMatrix();
	glTranslatef(-0.53f, jointOffset, 0.0f);
	glRotatef(jointAngle, 0.0f, 0.0f, 1.0f);
	glScalef(4.0f, 1.0f, 1.0f);
	glut2SolidCube0_06f();
	glPopMatrix(
#ifdef ARM9
		1
#endif
	);
}


/// Draws one leg
void draw1LegCrab()
{
	drawLegCrab(-45.0f, 0.075f);
}


/// Draws complete set of legs (side legs)
void drawLegsCrab(){
	GLfloat i = 0;
	// set a darker pinky colour for legs
	glColor3f(1.0f, 0.55f, 0.55f);

	// draw three side legs
	for (i = -15.0f; i <= 15.0f; i += 15.0f)
	{
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -0.025f);
		glRotatef(i, 0.0f, 1.0f, 0.0f);
		draw1LegCrab();
		glPopMatrix(
#ifdef ARM9
		1
#endif
		);
	}

	// draw fourth leg (the straight and bent downwards leg)
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.00f);
	glRotatef(-65.0f, -0.2f, 1.0f, 0.0f);
	drawLegCrab(0.0f, 0.0f);
	glPopMatrix(
#ifdef ARM9
		1
#endif
	);

	// set a light pinky colour for front legs
	glColor3f(1.0f, 0.65f, 0.65f);

	// front leg (arm)
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(55.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	draw1LegCrab();
	glPopMatrix(
#ifdef ARM9
		1
#endif
	);

	// front clippers on the front leg (arm)
	glPushMatrix();
	glTranslatef(0.24f, 0.0f, 0.725f);
	glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	drawLegCrab(-60.0f, 0.1f);
	glPopMatrix(
#ifdef ARM9
		1
#endif
	);
}