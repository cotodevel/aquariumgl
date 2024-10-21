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
#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
#endif
struct MarineObject BuildQuad(
		void * drawQuadFn, GLfloat materialIn1[4], GLfloat materialIn2[4], GLfloat shininessIn,
		GLfloat * vertexIn, GLfloat * normalIn, GLfloat * texelsIn, GLfloat * coloursIn
	)
{
	TWLPrintf("-- Creating quad\n");
	return MarineObjectInit1(RENDERABLE_QUAD, materialIn1, materialIn2 /*unused*/, shininessIn, vertexIn, normalIn, texelsIn, coloursIn);
}

/// Draws the quad
#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
#endif
void _drawQuad(struct MarineObject * marineObj)
{
	// set up the material properties (only front needs to be set)
	glMaterialfv(GL_FRONT, GL_AMBIENT, marineObj->material1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, marineObj->material2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, marineObj->material2);
	glMaterialf(GL_FRONT, GL_SHININESS, marineObj->shininess);

	// enable texturing
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(
#ifdef WIN32		
		GL_TEXTURE_2D, FLOOR_TEXTURE+1
#endif
#ifdef ARM9
	0, FLOOR_TEXTURE 
#endif	
	);

//DS doesn't support filtering.
#ifdef WIN32
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif

	glColor3f(0.9f, 0.6f, 0.0f);
	glBegin(GL_QUADS);
		glNormal3f(0.f, 0.f, 1.f);
		glTexCoord2f(0.0f, 0.0f);	
		glVertex2f(-0.5f, -0.5f);
		
		glTexCoord2f(1.0f, 0.0f);	
		glVertex2f(0.5f,  -0.5f);
		glTexCoord2f(1.0f, 1.0f);	
		glVertex2f(0.5f, 0.5f);
		glTexCoord2f(0.0f, 1.0f);	
		glVertex2f(-0.5f, 0.5f);
	glEnd();

#ifdef WIN32 //NDS actually disables texturing, so excluded
	glDisable(GL_TEXTURE_2D);
#endif
}
