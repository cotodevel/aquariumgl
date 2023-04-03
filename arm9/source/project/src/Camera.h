/*
* Camera class. This class is responsible for positioninig the
* camera at the right place. It also takes care of adjusting the
* camera position/angle.
*
* Igor Kromin 40125374
*/

#ifndef __CAMERA_3201
#define __CAMERA_3201

#ifdef _MSC_VER
#include <GL/glut.h>
#endif

#ifdef ARM9
#include "VideoGL.h"
#endif

#ifdef __cplusplus
#include <iostream>
#include <math.h>

#define PI 3.14159

class Camera
{
private:
	GLfloat verticalTilt;
	GLfloat horizontalAngle;
	GLfloat distance;

public:
	Camera();	/// default constructor
	virtual ~Camera();	/// default destructor

	void reset(void);	/// resets the camera to default position
	void position(void);	/// sets up camera at current position
	void dec(void);	/// decreases distance to origin
	void inc(void);	/// increases distance to origin
	void clockwise(void);	/// rotate scene clockwise
	void anticlockwise(void);	/// rotate scene anticlockwise
	void tiltup(void);	/// tilts camera upwards
	void tiltdown(void);	/// tilts camera downwards
};
#endif

#endif
