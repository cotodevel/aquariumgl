/*
* Octopus class. Draws a Octopus.
*
* Igor Kromin 40125374
*/

#include "Renderable.h"

using namespace std;

// setup the static variables
GLfloat Octopus::material[4] = {0.0f, 0.0f, 2.0f, 1.f};
GLfloat Octopus::shininess = 50.f;


Octopus::Octopus(void * drawOctopusFn)
{
	TWLPrintf("-- Creating octopus\n");
	buildDL = drawOctopusFn;
	callerArg0 = this; //cast this object for later usage
	callerType = RENDERABLE_OCTOPUS;
	// leg rotation angles
	legAngle = 0.0f;
	legAngleCutOff = 30.0f;
	legAngleInc = 1.0f;
}


Octopus::~Octopus()
{
	TWLPrintf("++ Destructing octopus\n");
}


void _drawOctopus(Octopus * octopusObj)
{
	// select our colour
	glColor3f(1.0f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// set up the material properties (only front needs to be set)
	glMaterialfv(GL_FRONT, GL_SPECULAR, octopusObj->material
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glMaterialf(GL_FRONT, GL_SHININESS, octopusObj->shininess
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	
#ifdef WIN32
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
#endif

	// draw octopus body (stretched along Y axis)
	glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif		
	);
	glScalef(1.0f, 3.0f, 1.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif		
	);
	drawSphere(0.3f, 16, 8);
	glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif		
	);

	octopusObj->legAngle += octopusObj->legAngleInc;
	if (octopusObj->legAngle < -octopusObj->legAngleCutOff || octopusObj->legAngle > octopusObj->legAngleCutOff) octopusObj->legAngleInc *= -1;

	glRotatef(octopusObj->legAngle, 0.0f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif		
	);

	GLfloat step = 360.0f / 8;
	for (int i = 0; i < 8;  i++)
	{
		glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif	
		);

		glRotatef(i * step, 0.0f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		glTranslatef(0.1f, 0.5f + (octopusObj->legAngle / octopusObj->legAngleCutOff) / 7.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		Crab::drawLeg();

		glTranslatef(0.2f, 0.725f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		glRotatef(120.0f, 0.0f, 0.0f, 1.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		
		Crab::drawLeg();

		glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
	}

	// turn of colour material tracking
	glDisable(GL_COLOR_MATERIAL);
}