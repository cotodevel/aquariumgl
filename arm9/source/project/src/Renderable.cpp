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

// create the static parts of all objects
unsigned int texturesRenderable[2];

GLint DLSOLIDCUBE0_06F=-1;

/// Default Constructor. Initialises the position to zero.
/// The rotation around the Y axis is picked randomly to allow random
/// spinning of objects. Display lists are off by default
Renderable::Renderable()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	
	this->rx = 0.0f;
	this->ry = getRand(0.0f, 360.0f);
	this->rz = 0.0f;

	this->sx = 1.0f;
	this->sy = 1.0f;
	this->sz = 1.0f;

	isList = false;
}


/// Default Destructor. Does nothing.
Renderable::~Renderable()
{
	// Nothing needs to be done here
}


/// Builds a display list of this object.
void Renderable::build(GLuint &dlist)
{
	dlist = glGenLists(1
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	if (!glIsList(dlist
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	)) {
		isList = false;
		return;
	}
	isList = true;

	glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glNewList(dlist, GL_COMPILE
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
		switch(callerType){
				case(RENDERABLE_CRAB):{
					draw_objectCrab fn = (draw_objectCrab)buildDL;
					Crab thisCrab = (*(Crab *)callerArg0);
					fn(thisCrab);
				}break;

				case(RENDERABLE_PLANT):{
					draw_objectPlant fn = (draw_objectPlant)buildDL;
					Plant thisPlant = (*(Plant *)callerArg0);
					fn(thisPlant);
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
void Renderable::move(GLfloat x, GLfloat y, GLfloat z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


/// Rotates an object
/*
* This method rotates the object coordinates a specified number of degrees
* around each of the x, y and z axes. All of the rotations are
* performed around the point (1.0f, 1.0f, 1.0f) and happen in
* the following manner: x-rot, y-rot, z-rot
*/
void Renderable::rotate(GLfloat x, GLfloat y, GLfloat z)
{
	this->rx = x;
	this->ry = y;
	this->rz = z;
}


/// Scales the object
void Renderable::scale(GLfloat x, GLfloat y, GLfloat z)
{
	this->sx = x;
	this->sy = y;
	this->sz = z;
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
void Renderable::draw(void)
{
	glPushMatrix(
#ifdef ARM9
		USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	glTranslatef(this->x, this->y, this->z
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	glRotatef(this->rx, 1.0f, 0.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glRotatef(this->ry, 0.0f, 1.0f, 0.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glRotatef(this->rz, 0.0f, 0.0f, 1.0f
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	glScalef(sx, sy, sz
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// if the object is flagged as a display list object, then call the
	// display list drawing function of the object, otherwise just call
	// the normal draw function of the object
	switch(callerType){
		case(RENDERABLE_STARFISH):{
			_drawStarFish((StarFish *)callerArg0);
		}break;

		case(RENDERABLE_FISH):{
			_drawFish((Fish *)callerArg0);
		}break;

		case(RENDERABLE_CRAB):{
			if (isList){
				Crab thisCrab = (*(Crab *)callerArg0);
				thisCrab._draw_dlist();
			}
			else{
				_drawCrab((Crab *)callerArg0);
			}
		}break;

		case(RENDERABLE_OCTOPUS):{
			_drawOctopus((Octopus *)callerArg0);
		}break;

		case(RENDERABLE_QUAD):{
			_drawQuad((Quad *)callerArg0);
		}break;

		case(RENDERABLE_PLANT):{
			if (isList){
				Plant thisPlant = (*(Plant *)callerArg0);
				thisPlant._draw_dlist();
			}
			else{
				_drawPlant((Plant *)callerArg0);
			}
		}break;
	}

	glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
}


/// Generates a random value to use
GLfloat Renderable::getRand(GLfloat minimum, GLfloat range)
{
	return (((GLfloat)rand() / (GLfloat)RAND_MAX) * range) + minimum;
}

//glutSolidSphere(radius, 16, 16);  -> NDS GX Replacement
void drawSphere(float r, int lats, int longs) {
	#if !defined(M_PI) 
	#define M_PI (3.14159265358979323846)
	#endif
	int i, j;
	for (i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = M_PI * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		#ifdef _MSC_VER
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
		#endif
		#ifdef ARM9
		glBegin(GL_QUAD_STRIP, USERSPACE_TGDS_OGL_DL_POINTER);
		for (j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0, USERSPACE_TGDS_OGL_DL_POINTER);
			glVertex3f(r * x * zr0, r * y * zr0, r * z0, USERSPACE_TGDS_OGL_DL_POINTER);
			glNormal3f(x * zr1, y * zr1, z1, USERSPACE_TGDS_OGL_DL_POINTER);
			glVertex3f(r * x * zr1, r * y * zr1, r * z1, USERSPACE_TGDS_OGL_DL_POINTER);
		}
		glEnd(USERSPACE_TGDS_OGL_DL_POINTER);
		#endif
		
	}
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
	#ifdef AMR9
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