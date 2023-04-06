#if defined(_MSC_VER)
//disable _CRT_SECURE_NO_WARNINGS message to build this in VC++
#pragma warning(disable:4996)
#endif

/*
* Camera class. This class is responsible for positioninig the
* camera at the right place. It also takes care of adjusting the
* camera position/angle.
*
* Igor Kromin 40125374
*/

#include "Camera.h"

/// Default Constructor. Initialises defaults.
void CameraInit1(struct Camera * Inst){
	TWLPrintf("-- Creating camera\n");
	reset(Inst);
}

/// Resets the camera position to default position and tilt
void reset(struct Camera * Inst){
	Inst->distance = -50.0f;
	Inst->verticalTilt = -30.0f;
	Inst->horizontalAngle = 90.0f;
}


/// Positions the camera at the required place and rotation
/// Zoom and spin is done by translate/rotate
void position(struct Camera * Inst){
	#ifdef _MSC_VER
	glTranslatef(0.0f, 0.0f, Inst->distance);
	glRotatef(Inst->verticalTilt, 1.0f, 0.0f, 0.0f);
	glRotatef(Inst->horizontalAngle, 0.0f, 1.0f, 0.0f); 
	#endif

	#ifdef ARM9
	glTranslatef(0.0f, 0.0f, Inst->distance, USERSPACE_TGDS_OGL_DL_POINTER);
	glRotatef(Inst->verticalTilt, 1.0f, 0.0f, 0.0f, USERSPACE_TGDS_OGL_DL_POINTER);
	glRotatef(Inst->horizontalAngle, 0.0f, 1.0f, 0.0f, USERSPACE_TGDS_OGL_DL_POINTER);
	#endif
}


/// Decrements the distance to origin (zoom in)
void dec(struct Camera * Inst){
		Inst->distance--;
}


/// Incrementes the distance to origin (zoom out)
void inc(struct Camera * Inst){
	Inst->distance++;
}


/// Adjusts the camera rotation around the Y axis
void clockwise(struct Camera * Inst){
	Inst->horizontalAngle++;
}


/// Adjusts the camera rotation around the Y axis
void anticlockwise(struct Camera * Inst){
	Inst->horizontalAngle--;
}


/// Adjusts the camera rotation around the X axis
/// the angle is locked if it gets above 0 degrees
void tiltup(struct Camera * Inst){
	if (Inst->verticalTilt < 0)
		Inst->verticalTilt++;
}


/// Adjusts the camera rotation around the X axis
/// The angle is locked if it gets greate than 90 degrees
void tiltdown(struct Camera * Inst){
	if (Inst->verticalTilt > -90)
		Inst->verticalTilt--;
}
