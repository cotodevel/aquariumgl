/*
* Quad class. Draws a yellowish colour quad.
*
* Igor Kromin 40125374
*/

#include "Renderable.h"

using namespace std;

// setup the static variables
GLfloat Quad::material[4] = {1.f, 1.f, 1.f, 1.f};
GLfloat Quad::shininess = 120.f;


/// Default Constructor. Initialises defaults.
Quad::Quad() : Renderable()
{
	TWLPrintf("-- Creating quad\n");
}


/// Default destructor.
Quad::~Quad()
{
	TWLPrintf("++ Destructing quad\n");
}


/// Draws the quad
void Quad::_draw(void)
{
	// set up the material properties (only front needs to be set)
	glMaterialfv(GL_FRONT, GL_AMBIENT, material
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);

	// enable texturing
	glEnable(GL_TEXTURE_2D);
	
#ifdef WIN32
	glBindTexture(GL_TEXTURE_2D, FLOOR_TEXTURE);
	// set up texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#endif

#ifdef ARM9
	//Todo
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
