/*
* Crab class. Draws a pinky colour crab.
*
* Igor Kromin 40125374
*/

#include "Renderable.h"

using namespace std;

// setup the static variables
GLfloat Crab::material[4] = {0.5f, 0.5f, 0.5f, 1.f};
GLfloat Crab::shininess = 50.f;


/// Default Constructor. Builds the display list for the crab.
Crab::Crab(void * drawCrabFn) : Renderable()
{
	TWLPrintf("-- Creating crab\n");
	buildDL = drawCrabFn;
	callerArg0 = this; //cast this object for later usage
	callerType = RENDERABLE_CRAB;
	sy = sx = sz = 2.f; // make crab twice as big
	build(dlist);
}


/// Default destructor.
Crab::~Crab()
{
	TWLPrintf("++ Destructing crab\n");
}


/// Draws the crab
void _drawCrab(Crab * crabObj)
{
	/*
	* The materials are set in _draw_dlist() since that function
	* is called eacht time to draw the display list version of the
	* crab, this functin only gets used to build the display list
	*/

	// get our pinky colour
	glColor3f(1.0f, 0.45f, 0.45f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif		
		);

	// draw crab body (squashed along Y axis
	glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glScalef(1.0f, 0.5f, 1.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	drawSphere(0.3f, 16, 16);
	glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// draw all of the legs
	glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
	crabObj->drawLegs();
	glScalef(-1.f, 1.f, 1.f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	crabObj->drawLegs();
	glPopMatrix(
#ifdef ARM9	
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// set to use our black colour
	glColor3f(0.0f, 0.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// draw left crab eye
	glTranslatef(-0.06f, 0.0f, 0.3f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	drawSphere(0.05f, 12, 8);

	// draw right crab eye
	glTranslatef(0.12f, 0.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	drawSphere(0.05f, 12, 8);
}


/// Draws the display list for the crab object
void Crab::_draw_dlist(void)
{
	// set up the material properties (only front needs to be set)
	glMaterialfv(GL_FRONT, GL_SPECULAR, material
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	
#ifdef WIN32
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
#endif

	glCallList(this->dlist
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

#ifdef WIN32
	// turn of colour material tracking
	glDisable(GL_COLOR_MATERIAL);
#endif
}


/// Draws a leg using an angle between the two bones
void Crab::drawLeg(GLfloat jointAngle, GLfloat jointOffset)
{
	// draw first part of a leg
	glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glTranslatef(-0.38f, 0.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glScalef(3.0f, 1.0f, 1.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glut2SolidCube0_06f();
	glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif	
	);

	// draw second part of a leg
	glPushMatrix(
#ifdef ARM9
	 USERSPACE_TGDS_OGL_DL_POINTER
#endif	
	);
	glTranslatef(-0.53f, jointOffset, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glRotatef(jointAngle, 0.0f, 0.0f, 1.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glScalef(4.0f, 1.0f, 1.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glut2SolidCube0_06f();
	glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
}


/// Draws one leg
void Crab::drawLeg()
{
	drawLeg(-45.0f, 0.075f);
}


/// Draws complete set of legs (side legs)
void Crab::drawLegs()
{
	// set a darker pinky colour for legs
	glColor3f(1.0f, 0.55f, 0.55f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// draw three side legs
	for (GLfloat i = -15.0f; i <= 15.0f; i += 15.0f)
	{
		glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		glTranslatef(0.0f, 0.0f, -0.025f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		glRotatef(i, 0.0f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		drawLeg();
		glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
	}

	// draw fourth leg (the straight and bent downwards leg)
	glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glTranslatef(0.0f, 0.0f, -0.00f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glRotatef(-65.0f, -0.2f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	drawLeg(0.0f, 0.0f);
	glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// set a light pinky colour for front legs
	glColor3f(1.0f, 0.65f, 0.65f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// front leg (arm)
	glPushMatrix(
#ifdef ARM9
	 USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glTranslatef(0.0f, 0.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glRotatef(55.0f, 0.0f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	drawLeg();
	glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// front clippers on the front leg (arm)
	glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glTranslatef(0.24f, 0.0f, 0.725f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glRotatef(-15.0f, 0.0f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	drawLeg(-60.0f, 0.1f);
	glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
}