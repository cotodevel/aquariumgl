/*
* Quad class. Draws a yellowish colour quad.
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

/// Default Constructor. Initialises defaults.
struct MarineObject BuildQuad(
		void * drawQuadFn, GLfloat materialIn[4], GLfloat shininessIn,
		GLfloat * vertexIn, GLfloat * normalIn, GLfloat * texelsIn, GLfloat * coloursIn
	)
{
	TWLPrintf("-- Creating quad\n");
	return MarineObjectInit1(RENDERABLE_QUAD, materialIn, materialIn /*unused*/, shininessIn, vertexIn, normalIn, texelsIn, coloursIn);
}

/// Draws the quad
void _drawQuad(struct MarineObject * marineObj)
{
	// set up the material properties (only front needs to be set)
	glMaterialfv(GL_FRONT, GL_AMBIENT, marineObj->material1
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, marineObj->material1
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
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

	// enable texturing
	glEnable(GL_TEXTURE_2D);
	
#ifdef WIN32
	glBindTexture(GL_TEXTURE_2D, FLOOR_TEXTURE+1);
	// set up texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif

#ifdef ARM9
	glBindTexture(0, FLOOR_TEXTURE+1, USERSPACE_TGDS_OGL_DL_POINTER); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT, USERSPACE_TGDS_OGL_DL_POINTER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT, USERSPACE_TGDS_OGL_DL_POINTER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR, USERSPACE_TGDS_OGL_DL_POINTER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR, USERSPACE_TGDS_OGL_DL_POINTER);
#endif

	glColor3f(0.9f, 0.6f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glBegin(GL_QUADS
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
		glNormal3f(0.f, 0.f, 1.f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		glTexCoord2f(0.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);	
		glVertex2f(-0.5f, -0.5f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		
		glTexCoord2f(1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);	
		glVertex2f(0.5f,  -0.5f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		glTexCoord2f(1.0f, 1.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);	
		glVertex2f(0.5f, 0.5f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		glTexCoord2f(0.0f, 1.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);	
		glVertex2f(-0.5f, 0.5f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
	glEnd(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	glDisable(GL_TEXTURE_2D);
}
