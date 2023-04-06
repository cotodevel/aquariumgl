/*
* Octopus class. Draws a Octopus.
*
* Igor Kromin 40125374
*/

#include "Renderable.h"

struct MarineObject BuildOctopus(
		void * drawOctopusFn, GLfloat materialIn[4], GLfloat shininessIn,
		GLfloat * vertexIn, GLfloat * normalIn, GLfloat * texelsIn, GLfloat * coloursIn
	){
	struct MarineObject obj = MarineObjectInit1(RENDERABLE_OCTOPUS, materialIn, materialIn /*unused*/, shininessIn, vertexIn, normalIn, texelsIn, coloursIn);
	// leg rotation angles
	obj.legAngle = 0.0f;
	obj.legAngleCutOff = 30.0f;
	obj.legAngleInc = 1.0f;
	TWLPrintf("-- Creating octopus\n");
	return obj;
}

void _drawOctopus(struct MarineObject * marineObj)
{
	// select our colour
	glColor3f(1.0f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// set up the material properties (only front needs to be set)
	glMaterialfv(GL_FRONT, GL_SPECULAR, marineObj->material1
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glMaterialf(GL_FRONT, GL_SHININESS, marineObj->shininess
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

	marineObj->legAngle += marineObj->legAngleInc;
	if (marineObj->legAngle < -marineObj->legAngleCutOff || marineObj->legAngle > marineObj->legAngleCutOff) marineObj->legAngleInc *= -1;

	glRotatef(marineObj->legAngle, 0.0f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif		
	);

	{
		GLfloat step = 360.0f / 8;
		int i = 0;
		for (i = 0; i < 8;  i++){
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
			glTranslatef(0.1f, 0.5f + (marineObj->legAngle / marineObj->legAngleCutOff) / 7.0f, 0.0f
	#ifdef ARM9
			, USERSPACE_TGDS_OGL_DL_POINTER
	#endif
			);
			draw1LegCrab();

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
		
			draw1LegCrab();

			glPopMatrix(
	#ifdef ARM9
			1, USERSPACE_TGDS_OGL_DL_POINTER
	#endif
			);
		}
	}
	// turn of colour material tracking
	glDisable(GL_COLOR_MATERIAL);
}