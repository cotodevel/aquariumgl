/*
* Octopus class. Draws a Octopus.
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

static bool bottomReach;
void _drawOctopus(struct MarineObject * marineObj)
{
	// work out how much to advance the object by relative to its orientation
	GLfloat yInc = cos(marineObj->rx * (3.14156 ) / 180) / 10.0f;
	
	// the floor is 70.0 x 70.0, but i want to keep the object inside a
	// 65.0 x 65.0 area, so work out the circular boundaries if the object goes
	// outside of this area

#ifdef WIN32
	if (marineObj->y < -20) {
		marineObj->y = -20;
		bottomReach = true;
	}
	
	if (marineObj->y > 4){ 
		marineObj->y = 4;
		bottomReach = false;
	}
#endif

#ifdef ARM9
	if (marineObj->y < 0) {
		marineObj->y = 0;
		bottomReach = false;
	}
	
	if (marineObj->y > 26){ 
		marineObj->y = 26;
		bottomReach = true;
	}
#endif

	if(bottomReach == true){
		marineObj->y += yInc;
	}
	else{
		marineObj->y -= yInc;
	}

	
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
	
#ifdef WIN32
	// turn of colour material tracking
	glDisable(GL_COLOR_MATERIAL);
#endif
}