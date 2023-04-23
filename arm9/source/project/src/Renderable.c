/*
* Renderable Class. This is an abstract class that all of the objects that
* are attached to the scene inherit from. It provides basic functionality
* for moving/rotating/scaling objects as well as defining a pure virtual
* function '_draw()' that must be difined by its children classes which 
* actually does the drawing of any specific object.
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

#ifdef ARM9
#include "VideoGL.h"
#include "Sphere008.h" //comment out when running on NDSDisplayListUtils
#endif

// create the static parts of all objects
unsigned int texturesRenderable[2];

GLint DLSOLIDCUBE0_06F=-1;

GLint currentOGLDisplayListObject=-1;

struct MarineObject MarineObjectInit1(
	int callerTypeIn,
	GLfloat materialIn1[4], GLfloat materialIn2[4], GLfloat shininessIn,
	GLfloat * vertexIn,
	GLfloat * normalIn,
	GLfloat * texelsIn,
	GLfloat * coloursIn
){
	struct MarineObject marineObj;
	marineObj.vertex = vertexIn;
	marineObj.normal = normalIn;
	marineObj.texels = texelsIn;
	marineObj.colours = coloursIn;

	marineObj.material1[0] = materialIn1[0];
	marineObj.material1[1] = materialIn1[1];
	marineObj.material1[2] = materialIn1[2];
	marineObj.material1[3] = materialIn1[3];

	marineObj.material2[0] = materialIn2[0];
	marineObj.material2[1] = materialIn2[1];
	marineObj.material2[2] = materialIn2[2];
	marineObj.material2[3] = materialIn2[3];
	marineObj.shininess = shininessIn;
	marineObj.dlist = -1;

	marineObj.tailAngle = 0;
	marineObj.tailAngleCutOff = 0;
	marineObj.tailAngleInc = 0;
	marineObj.legAngle = 0;
	marineObj.legAngleCutOff = 0;
	marineObj.legAngleInc = 0;

	marineObj.x = 0.0f;
	marineObj.y = 0.0f;
	marineObj.z = 0.0f;
	
	marineObj.rx = 0.0f;
	marineObj.ry = getRand(0.0f, 360.0f);
	marineObj.rz = 0.0f;

	marineObj.sx = 1.0f;
	marineObj.sy = 1.0f;
	marineObj.sz = 1.0f;

	marineObj.isList = false;
	marineObj.callerType = callerTypeIn;
	return marineObj;
}

/// Builds a display list of this object.
void build(struct MarineObject * marineObjRef, GLuint *dlist){
	*dlist = currentOGLDisplayListObject;
	if (!glIsList(*dlist
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	)) {
		marineObjRef->isList = false;
		return;
	}
	marineObjRef->isList = true;
	currentOGLDisplayListObject++; //point to next list if generation OK
	glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glNewList(*dlist, GL_COMPILE
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
		//Build display lists once for these objects
		switch(marineObjRef->callerType){
				case(RENDERABLE_CRAB):{
					_drawCrab(marineObjRef);
				}break;

				case(RENDERABLE_PLANT):{
					_drawPlant(marineObjRef);
				}break;
		}
	glEndList(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
}


/// Moves the object
/*
* This method moves the object coordinates to the specified position along
* the x, y and z axes.
*/
void move(struct MarineObject * marineObjRef, GLfloat x, GLfloat y, GLfloat z){
	marineObjRef->x = x;
	marineObjRef->y = y;
	marineObjRef->z = z;
}


/// Rotates an object
/*
* This method rotates the object coordinates a specified number of degrees
* around each of the x, y and z axes. All of the rotations are
* performed around the point (1.0f, 1.0f, 1.0f) and happen in
* the following manner: x-rot, y-rot, z-rot
*/
void rotate(struct MarineObject * marineObjRef, GLfloat x, GLfloat y, GLfloat z){
	marineObjRef->rx = x;
	marineObjRef->ry = y;
	marineObjRef->rz = z;
}


/// Scales the object
void scale(struct MarineObject * marineObjRef, GLfloat x, GLfloat y, GLfloat z){
	marineObjRef->sx = x;
	marineObjRef->sy = y;
	marineObjRef->sz = z;
}


/// Draws the object
/*
* This method sets up the object ready for drawing by a concrete
* class. Firstly all of the rotations are performed on the object,
* then the translation is done, finally the _draw() method is
* called to actually draw the object. Upon exit, the MODELVIEW
* matrix is restored.
*
* If a display list has been built for this object, then it is
* draw instead of re-drawing the object.
*/
void draw(struct MarineObject * marineObjRef){
	glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	glTranslatef(marineObjRef->x, marineObjRef->y, marineObjRef->z
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	glRotatef(marineObjRef->rx, 1.0f, 0.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glRotatef(marineObjRef->ry, 0.0f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glRotatef(marineObjRef->rz, 0.0f, 0.0f, 1.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	glScalef(marineObjRef->sx, marineObjRef->sy, marineObjRef->sz
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// if the object is flagged as a display list object, then call the
	// display list drawing function of the object, otherwise just call
	// the normal draw function of the object
	switch(marineObjRef->callerType){

		case(RENDERABLE_STARFISH):{
			_drawStarFish(marineObjRef);
		}break;

		case(RENDERABLE_QUAD):{
			_drawQuad(marineObjRef);
		}break;

		case(RENDERABLE_PLANT):{
			_draw_dlistPlant(marineObjRef);
		}break;

		//todo: when main camera is ported to NDS
		/*
		case(RENDERABLE_FISH):{
			_drawFish(marineObjRef);
		}break;

		case(RENDERABLE_CRAB):{
			_draw_dlistCrab(marineObjRef);
		}break;

		case(RENDERABLE_OCTOPUS):{
			_drawOctopus(marineObjRef);
		}break;
		*/

	}

	glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
}


/// Generates a random value to use
GLfloat getRand(GLfloat minimum, GLfloat range)
{
	return (((GLfloat)rand() / (GLfloat)RAND_MAX) * range) + minimum;
}

//glutSolidSphere(radius, 16, 16);  -> NDS GX Replacement
void drawSphere(float r, int lats, int longs) {
	#ifdef _MSC_VER
	int i, j;
	for (i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = M_PI * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);
		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(r * x * zr0, r * y * zr0, r * z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(r * x * zr1, r * y * zr1, r * z1);
		}
		glEnd();
	}
	#endif

	#ifdef ARM9
    // Execute the display list
    glCallListGX((u32*)&Sphere008); //comment out when running on NDSDisplayListUtils
	#endif
}


//gluDisk(qObj, 0.0, RADIUS, 16, 16); -> NDS GX Implementation
void drawCircle(GLfloat x, GLfloat y, GLfloat r, GLfloat BALL_RADIUS)
{
	#define SLICES_PER_CIRCLE ((int)16)
	float angle = 360.f / SLICES_PER_CIRCLE;
	float anglex = cos(angle);
	float angley = sin(angle);
	GLfloat lastX = 1;
	GLfloat lastY = 0;
	int c = 0; 
	#ifdef _MSC_VER
	glBegin(GL_TRIANGLE_STRIP);
	#endif
	#ifdef ARM9
	glBegin(GL_TRIANGLE_STRIP, USERSPACE_TGDS_OGL_DL_POINTER);
	#endif
	for (c = 1; c < SLICES_PER_CIRCLE; c++)
	{
		x = lastX * anglex - lastY * angley;
		y = lastX * angley + lastY * anglex;
		#ifdef _MSC_VER
		glVertex2f(x * BALL_RADIUS, y * BALL_RADIUS);
		#endif
		#ifdef ARM9
		glVertex2f(x * BALL_RADIUS, y * BALL_RADIUS, USERSPACE_TGDS_OGL_DL_POINTER);
		#endif
		lastX = x;
		lastY = y;
	}

	#ifdef _MSC_VER
	glEnd();
	#endif
	#ifdef ARM9
	glEnd(USERSPACE_TGDS_OGL_DL_POINTER);
	#endif
}


void drawCylinder(int numMajor, int numMinor, float height, float radius){
	double majorStep = height / numMajor;
	double minorStep = 2.0 * M_PI / numMinor;
	int i, j;

	for (i = 0; i < numMajor; ++i) {
		GLfloat z0 = 0.5 * height - i * majorStep;
		GLfloat z1 = z0 - majorStep;

		//glBegin(GL_TRIANGLE_STRIP);
		for (j = 0; j <= numMinor; ++j) {
			double a = j * minorStep;
			GLfloat x = radius * cos(a);
			GLfloat y = radius * sin(a);
			glNormal3f(x / radius, y / radius, 0.0
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
			);
			
			glTexCoord2f(j / (GLfloat) numMinor, i / (GLfloat) numMajor
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
			);
			glVertex3f(x, y, z0
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
			);

			glNormal3f(x / radius, y / radius, 0.0
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
			);
			glTexCoord2f(j / (GLfloat) numMinor, (i + 1) / (GLfloat) numMajor
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
			);
			glVertex3f(x, y, z1
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
			);
		}
		//glEnd();
	}
}

void glut2SolidCube0_06f() {
#ifdef ARM9
	updateGXLights(USERSPACE_TGDS_OGL_DL_POINTER); //Update GX 3D light scene!
	glCallList(DLSOLIDCUBE0_06F, USERSPACE_TGDS_OGL_DL_POINTER);
#endif
#ifdef _MSC_VER
	glCallList(DLSOLIDCUBE0_06F);
#endif
}