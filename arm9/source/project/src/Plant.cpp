/*
* Plant class. Draws a plant.
*
* Igor Kromin 40125374
*/

#include "Renderable.h"

using namespace std;

// setup the static variables
GLfloat Plant::material1[4] = {0.1f, 0.3f, 0.15f, 1.f};
GLfloat Plant::material2[4] = {0.6f, 1.f, 0.8f, 1.f};
GLfloat Plant::shininess = 100.f;


/// Default Constructor. Initialises defaults.
Plant::Plant(void * drawPlantFn) : Renderable()
{
	TWLPrintf("-- Creating Plant\n");
	buildDL = drawPlantFn;
	callerArg0 = this; //cast this object for later usage
	callerType = RENDERABLE_PLANT;
	build(dlist);
}


/// Default destructor.
Plant::~Plant()
{
	TWLPrintf("++ Destructing Plant\n");
}


/// Draws the Plant
void _drawPlant(Plant * plantObj)
{
	/*
	* The materials are set in _draw_dlist() since that function
	* is called eacht time to draw the display list version of the
	* plant, this functin only gets used to build the display list
	*/

	// in the start we generate a plant with AT LEAST one branch
	// so that we never get a situation where a plant is created
	// with 0 branches
	plantObj->generate(0, Renderable::getRand(1, 6));
}


// Generates the plan. This really draws it, but the draw commands
// are saved into the display list
void Plant::generate(int level, int number)
{
	// if we reached 5 levels of recursion, exit
	// remember we start at level 0
	if (level == 5) return;

	// each branch is abuout 3/4 of the size of the previous
	GLfloat height = 3.0f / (0.75 * (level + 1.f));	
	GLfloat bottom = 0.75f / pow(2, level);
	GLfloat top = 0.75f / pow(2, (GLfloat)level + 1.f);
	
	for (int i = 0; i < number; i++)
	{
		
		GLfloat horzAngle = Renderable::getRand(0, 180);
		GLfloat vertAngle = Renderable::getRand(0, 180);
		int numChildren = Renderable::getRand(0, 6);

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
		generate(level + 1, numChildren);
		glPopMatrix(
#ifdef ARM9
		1, USERSPACE_TGDS_OGL_DL_POINTER
#endif		
		);
	}
}


/// Draws the display list for the plant object
void Plant::_draw_dlist(void)
{
	// set up the material properties (only front needs to be set)
	glMaterialfv(GL_FRONT, GL_AMBIENT, material1
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material1
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material2
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
#ifdef WIN32
	glColor4fv(material1);
#endif
	glCallList(this->dlist
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
}