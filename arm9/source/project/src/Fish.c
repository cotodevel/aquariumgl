/*
* Fish class. Draws a fish.
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

struct MarineObject BuildFish(
	void * drawFishFn, 
	GLfloat materialIn[4], GLfloat shininessIn,
	GLfloat * vertexIn,
	GLfloat * normalIn,
	GLfloat * texelsIn,
	GLfloat * coloursIn
	) 
{
	struct MarineObject obj = MarineObjectInit1(RENDERABLE_FISH, materialIn, materialIn /*unused*/, shininessIn, vertexIn, normalIn, texelsIn, coloursIn);
	// angles and cut offs for tail animation
	obj.tailAngle = 0.0f;
	obj.tailAngleCutOff = 20.0f;
	obj.tailAngleInc = 1.0f;
	TWLPrintf("-- Creating fish\n");
	return obj;
}

/// Draws the full fish
void _drawFish(struct MarineObject * marineObj)
{
	// work out how much to advance the fish by relative to its orientation
	GLfloat xInc = cos(marineObj->ry * (3.14156 ) / 180) / 10.0f;
	GLfloat zInc = sin(marineObj->ry * (3.14156 ) / 180) / 10.0f;
	GLfloat pt = 0;

	// the floor is 70.0 x 70.0, but i want to keep the fish inside a
	// 65.0 x 65.0 area, so work out the circular boundaries if the fish goes
	// outside of this area
	if (marineObj->x < -35) marineObj->x += 65.f;
	if (marineObj->x > 35) marineObj->x -= 65.f;
	if (marineObj->z < -35) marineObj->z += 65.f;
	if (marineObj->z > 35) marineObj->z -= 65.f;

	// increment the fish position
	marineObj->x -= xInc;
	marineObj->z += zInc;

	// set up the material properties (only front needs to be set)
	glMaterialfv(GL_FRONT, GL_AMBIENT, marineObj->material1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, marineObj->material1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, marineObj->material1);
	glMaterialf(GL_FRONT, GL_SHININESS, marineObj->shininess);

	// enable texturing
	glEnable(GL_TEXTURE_2D);

	glBindTexture(
	#ifdef ARM9
		0, 
	#endif
	#ifdef WIN32
		GL_TEXTURE_2D,
	#endif
		FISH_TEXTURE+1 
	);
	// set up texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// set up vertex arrays
	glVertexPointer(3, GL_FLOAT, 0, marineObj->vertex);
	glNormalPointer(GL_FLOAT, 0, marineObj->normal);
	glTexCoordPointer(2, GL_FLOAT, 0, marineObj->texels);
	glColorPointer(3, GL_FLOAT, 0, marineObj->colours);

	// enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// draw first side of the fish
	drawSideFish();
	
	// draw second side of the fish
	glScalef(1.0f, 1.0f, -1.0f);
	drawSideFish();

	// work out new fish tail position
	pt = sin(marineObj->tailAngle * 3.14159 / 180);
	marineObj->tailAngle += marineObj->tailAngleInc;
	if (marineObj->tailAngle < -marineObj->tailAngleCutOff || marineObj->tailAngle > marineObj->tailAngleCutOff)
		marineObj->tailAngleInc *= -1;
	
	// draw one side of flexible part of the tail
	marineObj->vertex[143] = marineObj->vertex[152] = marineObj->vertex[149] = marineObj->vertex[158] = marineObj->vertex[167] = pt;
	glDrawArrays(GL_TRIANGLES, 6 + (4 * 6) + (3 * 5), 3 * 4);
	glScalef(1.0f, 1.0f, -1.0f);

	// draw second side of flexible part of the tail
	marineObj->vertex[143] = marineObj->vertex[152] = marineObj->vertex[149] = marineObj->vertex[158] = marineObj->vertex[167] = -pt;
	glDrawArrays(GL_TRIANGLES, 6 + (4 * 6) + (3 * 5), 3 * 4);
	
	// disable all vertex arrays and texturing
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisable(GL_TEXTURE_2D);
}


/// Draws a side of the fish
void drawSideFish()
{
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2); 
    glDrawArrays(GL_QUADS, 6, 4 * 7);
    glDrawArrays(GL_TRIANGLES, 6 + 4 * 6, 3 * 5); 
}


/*
* ========================================
* ====== VERTEX ARRAYS FOLLOW BELOW ======
* ========================================
*
* Fish is divided into sections like this:
*        _________
*      /   |    | \
*    /|    | 6  |   \     /|
*  / 2| 4  |    | 8  \-- / |
* -------------------- 9 | 10|
*  \ 1| 3  |    | 7  /-- \ |
*    \|    | 5  |  /      \|
*      \---------/
* 
* 9:
*      ___-------\
*     /___d_------\
*     |          . | 
*    ` |   a   .   |
*   `   |    .     |
*  `     | .       |
* `       |    b   | --> 10 joins here
*  `     | .       |
*   `   |    .     |
*    ` |   c   .   |
*     |_____     . |
*         e -------/    e is on the bottom
* 
* 10:
*                . // 
*              - / .|
*            /H/   /| 
*          -|/    / |
*         | |    /  |
*         | | F /   |
*    -->| |  /    |
*         | | /     |
*          -|/  G   |
*            \      |
*              \    |
*  	            I\  |
*                  \|  I is on the bottom
* 
*/
//GLfloat * vertexIn,
GLfloat vertexFish[] =
{
	// 2				|					|					|
	0.0f, 0.0f, 0.0f,	0.5f, -0.5f, 0.0f,	0.5f, 0.0f, 0.45f,
	// 1				|					|					|
    0.0f, 0.0f, 0.0f,	0.5f, 0.0f, 0.45f,	0.5f, 0.5f, 0.0f,
	// 3				|					|					|
    0.5f, 0.0f, 0.45f,	0.5f, -0.5f, 0.0f,	1.5f, -1.0f, 0.0f,	1.5f, 0.0f, 0.45f,
    // 4				|					|					|
    0.5f, 0.5f, 0.0f,	0.5f, 0.0f, 0.45f,	1.5f, 0.0f, 0.45f,	1.5f, 1.0f, 0.0f,
    // 6				|					|					|
    1.5f, 0.0f, 0.45f,	1.5f, -1.0f, 0.0f,	2.5f, -1.0f, 0.0f,	2.5f, 0.0f, 0.45f,
    // 5				|					|					|
    1.5f, 1.0f, 0.0f,	1.5f, 0.0f, 0.45f,	2.5f, 0.0f, 0.45f,	2.5f, 1.0f, 0.0f,
    // 8				|					|					|
    2.5f, 0.0f, 0.45f,	2.5f, -1.0f, 0.0f,	3.5f, -0.5f, 0.0f,	3.5f, 0.0f, 0.2f,
    // 7				|					|					|
    2.5f, 1.0f, 0.0f,	2.5f, 0.0f, 0.45f,	3.5f, 0.0f, 0.2f,	3.5f, 0.5f, 0.0f,
    
	// 9
	// c				|					|					|
    3.5f, 0.5f, 0.0f,	3.5f, 0.0f, 0.2f,	3.75f, 0.4f, 0.1f,
    // b				|					|					|
	3.5f, 0.0f, 0.2f,	3.75f, -0.4f, 0.1f,	3.75f, 0.4f, 0.1f,
	// a				|					|					|
    3.5f, 0.0f, 0.2f,	3.5f, -0.5f, 0.0f,	3.75f, -0.4f, 0.1f,
	// e				|					|					|
    3.5f, 0.5f, 0.0f,	3.75f, 0.4f, 0.1f,	3.75f, 0.4f, -0.1f,
	// d				|					|					|
    3.5f, -0.5f, 0.0f,	3.75f, -0.4f, -0.1f,3.75f, -0.4f, 0.1f,
	
	// 10
	// F				|					|					|
	3.75f, 0.4f, 0.1f,	3.75f, -0.4f, 0.1f,	4.3f, -0.8f, 0.0f,
	// G				|					|					|
	3.75f, 0.4f, 0.1f,	4.3f, -0.8f, 0.0f,	4.3f, 0.8f, 0.0f,
	// I				|					|					|
	3.75f, 0.4f, 0.1f,	4.3f, 0.8f, 0.0f,	3.75f, 0.4f, -0.1f,
	// H				|					|					|
	3.75f, -0.4f, -0.1f,4.3f, -0.8f, 0.0f,	3.75f, -0.4f, 0.1f,

	//NDS requires this one otherwise segfaults on drawArrays();
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f

};

//GLfloat * normalIn 
GLfloat normalFish[] =
{
	// 2				|					|					|
	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 1.0f,
    // 1				|					|					|
    -1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,
    // 4				|					|					|
    0.0f, 0.0f, 1.0f,	0.0f, 0.0f, -1.0f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f, 1.0f,
    // 3				|					|					|
    0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,
    // 6				|					|					|
    0.0f, 0.0f, 1.0f,	0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f, 1.0f,
    // 5				|					|					|
    0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,
    // 8				|					|					|
    0.0f, 0.0f, 1.0f,	0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f, 1.0f,
    // 7				|					|					|
    0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,

	// 9
	// c				|					|					|
    0.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.71f, 0.71f,
    // b				|					|					|
	0.0f, -0.71f, 0.71f,0.0f, 0.0f, 1.0f,	0.0f, -0.71f, 0.71f,
	// a				|					|					|
	0.0f, 0.0f, 1.0f,	0.0f, -1.0f, 0.0f,	0.0f, -0.71f, 0.71f,
	// e				|					|					|
	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,    
	// d				|					|					|
	0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f,
	
	// 10
	// F				|					|					|
	0.0f, -0.71f, 0.71f,0.71f, -0.71f, 0.0f,0.71f, -0.71f, 0.0f,
	// G				|					|					|
	0.0f, -0.71f, 0.71f,0.71f, -0.71f, 0.0f,0.71f, 0.71f, 0.0f,
	
	// I				|					|					|
	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
	// H				|					|					|
	0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f,	0.0f, -1.0f, 0.0f,

	//NDS requires this one otherwise segfaults on drawArrays();
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f

};

//GLfloat * texelsIn
GLfloat texelsFish[] =
{
	// 2					|						|						|
	126.f/128.f,68.f/128.0f,119.f/128.f,47.f/128.f,	112.f/128.f, 68.f/128.f,
    // 1					|						|						|
    126.f/128.f,68.f/128.0f,112.f/128.f,68.f/128.f,	117.f/128.f, 84.f/128.f,
    // 4					|						|						|
    112.f/128.f, 68.f/128.f,119.f/128.f,47.f/128.f,	86.f/128.f, 8.f/128.f,	84.f/128.f, 68.f/128.f,
    // 3					|						|						|
    117.f/128.f, 84.f/128.f,112.f/128.f,68.f/128.f,	84.f/128.f, 68.f/128.f,	83.f/128.f, 120.f/128.f,
    // 6					|						|						|
    84.f/128.f, 68.f/128.f,	86.f/128.f, 8.f/128.f,	52.f/128.f, 15.f/128.f,	53.f/128.f, 68.f/128.f,
    // 5					|						|						|
    83.f/128.f, 120.f/128.f,84.f/128.f, 68.f/128.f,	53.f/128.f, 68.f/128.f,	54.f/128.f, 110.f/128.f,
    // 8					|						|						|
    53.f/128.f, 68.f/128.f,	52.f/128.f, 15.f/128.f,	26.f/128.f, 54.f/128.f,	25.f/128.f, 69.f/128.f,
    // 7					|						|						|
    54.f/128.f, 110.f/128.f,53.f/128.f, 68.f/128.f,	25.f/128.f, 69.f/128.f,	28.f/128.f, 76.f/128.f,

	// 9
	// c					|						|						|
    28.f/128.f, 74.f/128.f,	25.f/128.f, 69.f/128.f,	20.f/128.f, 69.f/128.f,
    // b					|						|						|
	25.f/128.f, 69.f/128.f,	20.f/128.f, 69.f/128.f,	20.f/128.f, 69.f/128.f,
	// a					|						|						|
    25.f/128.f, 69.f/128.f,	26.f/128.f, 54.f/128.f,	20.f/128.f, 69.f/128.f,
	// e					|						|						|
    26.f/128.f, 69.f/128.f,	20.f/128.f, 69.f/128.f,	20.f/128.f, 69.f/128.f,
	// d					|						|						|
    26.f/128.f, 54.f/128.f,	20.f/128.f, 69.f/128.f,	20.f/128.f, 69.f/128.f,
	
	// 10
	// F					|						|						|
	20.f/128.f, 69.f/128.f,	20.f/128.f, 69.f/128.f,	5.f/128.f, 69.f/128.f,
	// G					|						|						|
	20.f/128.f, 69.f/128.f,	5.f/128.f, 69.f/128.f,	5.f/128.f, 69.f/128.f,
	// I					|						|						|
	20.f/128.f, 69.f/128.f,	5.f/128.f, 69.f/128.f,	20.f/128.f, 69.f/128.f,
	// H					|						|						|
	20.f/128.f, 69.f/128.f,	5.f/128.f, 69.f/128.f,	20.f/128.f, 69.f/128.f,

	//NDS requires this one otherwise segfaults on drawArrays();
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f
};

//GLfloat * coloursIn
GLfloat coloursFish[] =
{
	// 2				|					|					|
	0.0f, 0.0f, 0.2f,	0.0f, 0.0f, 0.2f,	0.0f, 0.2f, 0.8f,
	// 1				|					|					|
    0.0f, 0.0f, 0.2f,	0.0f, 0.2f, 0.8f,	0.0f, 0.0f, 0.2f,
	// 3				|					|					|
    0.0f, 0.2f, 0.8f,	0.0f, 0.0f, 0.2f,	0.0f, 0.0f, 0.2f,	0.0f, 0.2f, 0.8f,
    // 4				|					|					|
    0.0f, 0.0f, 0.2f,	0.0f, 0.2f, 0.8f,	0.0f, 0.2f, 0.8f,	0.0f, 0.0f, 0.2f,
    // 6				|					|					|
    0.0f, 0.2f, 0.8f,	0.0f, 0.0f, 0.2f,	0.0f, 0.0f, 0.2f,	0.0f, 0.2f, 0.8f,
    // 5				|					|					|
    0.0f, 0.0f, 0.2f,	0.0f, 0.2f, 0.8f,	0.0f, 0.2f, 0.8f,	0.0f, 0.0f, 0.2f,
    // 8				|					|					|
    0.0f, 0.2f, 0.8f,	0.0f, 0.0f, 0.2f,	0.0f, 0.0f, 0.2f,	0.0f, 0.2f, 0.8f,
    // 7				|					|					|
    0.0f, 0.0f, 0.2f,	0.0f, 0.2f, 0.8f,	0.0f, 0.2f, 0.8f,	0.0f, 0.0f, 0.2f,
    
	// 9
	// c				|					|					|
    0.0f, 0.0f, 0.2f,	0.0f, 0.2f, 0.8f,	0.0f, 0.0f, 0.2f,
    // b				|					|					|
	0.0f, 0.2f, 0.8f,	0.0f, 0.0f, 0.2f,	0.0f, 0.0f, 0.2f,
	// a				|					|					|
    0.0f, 0.2f, 0.8f,	0.0f, 0.0f, 0.2f,	0.0f, 0.0f, 0.2f,
	// e				|					|					|
    0.0f, 0.0f, 0.2f,	0.0f, 0.0f, 0.2f,	0.0f, 0.0f, 0.2f,
	// d				|					|					|
    0.0f, 0.0f, 0.2f,	0.0f, 0.0f, 0.2f,	0.0f, 0.0f, 0.2f,
	
	// 10
	// F				|					|					|
	0.0f, 0.0f, 0.2f,	0.0f, 0.0f, 0.2f,	0.0f, 0.2f, 0.8f,
	// G				|					|					|
	0.0f, 0.0f, 0.2f,	0.0f, 0.2f, 0.8f,	0.0f, 0.2f, 0.8f,
	// I				|					|					|
	0.0f, 0.0f, 0.2f,	0.0f, 0.2f, 0.8f,	0.0f, 0.0f, 0.2f,
	// H				|					|					|
	0.0f, 0.0f, 0.2f,	0.0f, 0.2f, 0.8f,	0.0f, 0.0f, 0.2f,

	//NDS requires this one otherwise segfaults on drawArrays();
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f
};