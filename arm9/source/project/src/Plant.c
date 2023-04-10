/*
* Plant class. Draws a plant.
*
* Igor Kromin 40125374
*/

#include "Renderable.h"
#include <math.h>

#ifndef _MSC_VER
					// //
#define ARM9 1		// Enable only if not real GCC + NDS environment
#undef _MSC_VER		// //
#undef WIN32		// //
#endif

/// Default Constructor. Initialises defaults.
struct MarineObject BuildPlant(
		void * drawPlantFn, void * displayListFn,
		GLfloat materialIn1[4], GLfloat materialIn2[4], GLfloat shininessIn,
		GLfloat * vertexIn,
		GLfloat * normalIn,
		GLfloat * texelsIn,
		GLfloat * coloursIn
	)
{
	struct MarineObject obj = MarineObjectInit1(RENDERABLE_PLANT, materialIn1, materialIn2, shininessIn, vertexIn, normalIn, texelsIn, coloursIn);
	build(&obj, &obj.dlist);
	TWLPrintf("-- Creating Plant\n");
	return obj;
}

/// Draws the Plant
void _drawPlant(struct MarineObject * marineObj)
{
	/*
	* The materials are set in _draw_dlist() since that function
	* is called eacht time to draw the display list version of the
	* plant, this functin only gets used to build the display list
	*/

	// in the start we generate a plant with AT LEAST one branch
	// so that we never get a situation where a plant is created
	// with 0 branches
	generateBranches(0, getRand(1, 6));
}


// Generates the plan. This really draws it, but the draw commands
// are saved into the display list
void generateBranches(int level, int number){
	GLfloat height = 0;
	GLfloat bottom = 0;
	GLfloat top = 0;
	int i = 0;
	
	// if we reached 5 levels of recursion, exit
	// remember we start at level 0
	if (level == 5) return;

	// each branch is abuout 3/4 of the size of the previous
	height = 3.0f / (0.75 * (level + 1.f));	
	bottom = 0.75f / pow(2, level);
	top = 0.75f / pow(2, (GLfloat)level + 1.f);
	for (i = 0; i < number; i++){
		GLfloat horzAngle = getRand(0, 180);
		GLfloat vertAngle = getRand(0, 180);
		int numChildren = getRand(0, 6);

		glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		glRotatef(horzAngle, 0.0f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		glRotatef(vertAngle, 1.0f, 0.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);

		// cap the bottom
		drawCircle(0.5f, bottom, 8 - level, 3.0f);

		// cap top
		glTranslatef(0.0f, 0.0f, height
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		drawCircle(0.5f, top, 8 - level, 3.0f);
		
		// call this function again to generate more branches
		// rotate back to 'normal' position first i.e. facing up
		// this avoids branches looking down and going through
		// the floor
		glRotatef(-vertAngle, 1.0f, 0.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
		);
		generateBranches(level + 1, numChildren);
		glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif		
		);
	}
}


/// Draws the display list for the plant object
void _draw_dlistPlant(struct MarineObject * marineObj)
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
	glMaterialfv(GL_FRONT, GL_SPECULAR, marineObj->material2
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glMaterialf(GL_FRONT, GL_SHININESS, marineObj->shininess
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glColor4fv(marineObj->material1
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glCallList(marineObj->dlist
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
}