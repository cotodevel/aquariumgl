/*
* StarFish class. Draws a yellowish colour StarFish.
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
#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
#endif
void _drawStarFish(struct MarineObject * marineObj){

	// work out how much to advance the object by relative to its orientation
	GLfloat xInc = cos(marineObj->ry * (3.14156 ) / 180) / 10.0f;
	GLfloat zInc = sin(marineObj->ry * (3.14156 ) / 180) / 10.0f;
	GLfloat pt = 0;

	// the floor is 70.0 x 70.0, but i want to keep the object inside a
	// 65.0 x 65.0 area, so work out the circular boundaries if the object goes
	// outside of this area
	if (marineObj->x < -35) marineObj->x += 55.f;
	if (marineObj->x > 35) marineObj->x -= 55.f;
	if (marineObj->z < -35) marineObj->z += 55.f;
	if (marineObj->z > 35) marineObj->z -= 55.f;

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
			glPushMatrix();
			glRotatef(i * step, 0.0f, 1.0f, 0.0f);
			glDrawArrays(GL_QUADS, 0, 4 * 6);
			glPopMatrix(
	#ifdef ARM9
			1
	#endif
			);
		}
	}

	// disable vertex arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

#ifdef WIN32
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
	1.5f, 0.2f, 0.0f,	1.5f, 0.0f, 0.3f,	1.5f, -0.2f, 0.0f,	1.5f, 0.0f, -0.3f,
	
	//NDS requires this one otherwise segfaults on drawArrays();
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f
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
	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
	
	//NDS requires this one otherwise segfaults on drawArrays();
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f
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
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,

	//NDS requires this one otherwise segfaults on drawArrays();
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f

};