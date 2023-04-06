/*
* StarFish class. Draws a yellowish colour StarFish.
*
* Igor Kromin 40125374
*/

#include "Renderable.h"

/// Default Constructor. Initialises defaults.
struct MarineObject BuildStarfish(
		void * drawStarFishFn,
		GLfloat materialIn[4], GLfloat shininessIn,
		GLfloat * vertexIn,
		GLfloat * normalIn,
		GLfloat * texelsIn,
		GLfloat * coloursIn
	)
{
	TWLPrintf("-- Creating StarFish\n");
	return MarineObjectInit1(RENDERABLE_STARFISH, materialIn, materialIn /*unused*/, shininessIn, vertexIn, normalIn, texelsIn, coloursIn);
}

/// Draws the StarFish
void _drawStarFish(struct MarineObject * marineObj){
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

	// set up vertex arrays
	glVertexPointer(3, GL_FLOAT, 0, marineObj->vertex);
	glNormalPointer(GL_FLOAT, 0, marineObj->normal);
	glColorPointer(3, GL_FLOAT, 0, marineObj->colours);

	// enable vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	{
		// draw all legs
		GLfloat step = 360.0f / 5;
		int i = 0;
		for (i = 0; i < 5;  i++){
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
			glDrawArrays(GL_QUADS, 0, 4 * 5);
			glPopMatrix(
	#ifdef ARM9
			1, USERSPACE_TGDS_OGL_DL_POINTER
	#endif
			);
		}
	}
#ifdef WIN32
	// disable vertex arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	// turn of colour material tracking
	glDisable(GL_COLOR_MATERIAL);
#endif
}


/*
* ========================================
* ====== VERTEX ARRAYS FOLLOW BELOW ======
* ========================================
*
* StarFish is divided into sections like this (only one leg is defined):
*
* Leg Top:			Leg Bottom:
*  _				 _
* |  - _			|  - _
* |      -			|      -
* |  1     -		|  3     -
* 0---------|X		0---------|X
* |  2     -		|  4     -
* |     _ -			|     _ -
* | _ -				| _ - 
*
* X: This is the end point of a leg
*      /\
*    /    \
*    \    /
*      \/
*/

GLfloat vertexStarFish[] =
{
	//3					|					|					|
    0.0f, 0.2f, 0.0f,	0.0f, 0.0f, 0.6f,	1.5f, 0.0f, 0.3f,	1.5f, 0.2f, 0.0f,
    //4					|					|					|
	0.0f, 0.0f, -0.6f,	0.0f, 0.2f, 0.0f,	1.5f, 0.2f, 0.0f,	1.5f, 0.0f, -0.3f,
    //1					|					|					|
	0.0f, -0.2f, 0.0f,	1.5f, -0.2f, 0.0f,	1.5f, 0.0f, 0.3f,	0.0f, 0.0f, 0.6f,		
	//2					|					|					|
    0.0f, 0.0f, -0.6f,	1.5f, 0.0f, -0.3f,	1.5f, -0.2f, 0.0f,	0.0f, -0.2f, 0.0f,		
	//X					|					|					|
	1.5f, 0.2f, 0.0f,	1.5f, 0.0f, 0.3f,	1.5f, -0.2f, 0.0f,	1.5f, 0.0f, -0.3f
};

GLfloat normalStarFish[] =
{
    //3					|					|					|
	0.f, 0.71f, -0.71f,	0.f, 1.f, 0.0f,		0.f, 1.0f, 0.f,		0.f, 0.71f, -0.71f,
    //4					|					|					|
	0.f, 1.0f, 0.f,		0.0f, 0.71f, 0.71f,	0.f, 0.71f, 0.71f,	0.0f, 1.f, 0.f,
	//1					|					|					|
	0.f, -0.71f, -0.71f,0.f, -1.f, 0.0f,	0.f, -1.0f, 0.f,	0.f, -0.71f, -0.71f,
	//2					|					|					|
    0.f, -1.0f, 0.f,	0.0f, -0.71f, 0.71f,0.f, -0.71f, 0.71f,	0.0f, -1.f, 0.f,
	//X					|					|					|
	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f

};

GLfloat coloursStarFish[] =
{
    //3					|					|					|
	0.1f, 0.0f, 0.0f,	0.9f, 0.4f, 0.0f,	0.9f, 0.4f, 0.0f,	0.1f, 0.0f, 0.0f,
    //4					|					|					|
	0.9f, 0.4f, 0.0f,	0.1f, 0.0f, 0.0f,	0.1f, 0.0f, 0.0f,	0.9f, 0.4f, 0.0f,
    //1					|					|					|
	0.85f, 0.5f, 0.0f,	0.1f, 0.0f, 0.0f,	0.1f, 0.0f, 0.0f,	0.85f, 0.5f, 0.0f,
    //2					|					|					|
	0.9f, 0.4f, 0.0f,	0.1f, 0.0f, 0.0f,	0.1f, 0.0f, 0.0f,	0.9f, 0.4f, 0.0f,
	//X					|					|					|
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f

};