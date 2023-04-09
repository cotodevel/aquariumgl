/*
* Camera class. This class is responsible for positioninig the
* camera at the right place. It also takes care of adjusting the
* camera position/angle.
*
* Igor Kromin 40125374
*/

#ifndef __CAMERA_3201
#define __CAMERA_3201

#ifdef ARM9
#include "VideoGL.h"
#endif

#include "Renderable.h"
#include <math.h>

#define PI 3.14159

struct Camera{
	GLfloat verticalTilt;
	GLfloat horizontalAngle;
	GLfloat distance;
};

#endif

#ifdef __cplusplus
extern "C"{
#endif

extern void CameraInit1(struct Camera * Inst);	/// default init
extern void reset(struct Camera * Inst);	/// resets the camera to default position
extern void position(struct Camera * Inst);	/// sets up camera at current position
extern void dec(struct Camera * Inst);	/// decreases distance to origin
extern void inc(struct Camera * Inst);	/// increases distance to origin
extern void clockwise(struct Camera * Inst);	/// rotate scene clockwise
extern void anticlockwise(struct Camera * Inst);	/// rotate scene anticlockwise
extern void tiltup(struct Camera * Inst);	/// tilts camera upwards
extern void tiltdown(struct Camera * Inst);	/// tilts camera downwards

#ifdef __cplusplus
}
#endif
