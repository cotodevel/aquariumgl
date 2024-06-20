#if defined(_MSC_VER)
//disable _CRT_SECURE_NO_WARNINGS message to build this in VC++
#pragma warning(disable:4996)
#endif

#include "Scene.h"
#ifdef ARM9
#include "grass_tex.h"
#include "fish_tex.h"
#include "GXPayload.h" //required to flush the GX<->DMA<->FIFO circuit on real hardware
#endif

#ifndef _MSC_VER
					// //
#define ARM9 1		// Enable only if not real GCC + NDS environment
#undef _MSC_VER		// //
#undef WIN32		// //
#endif


#ifdef _MSC_VER
GLint texturesRenderable[10];
#endif

int widthScene;	/// the width of the window
int heightScene;	/// the height of the window

// light 0 colours
#ifdef WIN32
GLfloat ambient0Scene[4]	= {0.1f, 0.1f, 0.1f, 1.0f}; //WIN32
GLfloat diffuse0Scene[4]	= {0.4f, 0.4f, 0.4f, 1.01f}; //WIN32
GLfloat specular0Scene[4]	= {0.2f, 0.2f, 0.2f, 1.0f}; //WIN32
GLfloat position0Scene[4]	= {0.0f, -1.0f, 0.0f, 0.0f}; //WIN32
#endif
#ifdef ARM9
GLfloat ambient0Scene[]  = { 0.0f, 0.0f, 0.0f, 1.0f }; //NDS
GLfloat diffuse0Scene[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; //NDS
GLfloat specular0Scene[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //NDS
GLfloat position0Scene[] = { 2.0f, 5.0f, 5.0f, 0.0f }; //NDS
#endif

// light 1 colours
GLfloat ambient1Scene[4]	= {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat diffuse1Scene[4]	= {0.45f, 0.45f, 0.45f, 1.0f};
GLfloat specular1Scene[4]	= {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat position1Scene[4]	= {-2.0f, -5.0f, -5.0f, -1.0f};
GLfloat direction1Scene[4]	= {0.0f, 0.0f, -1.0f};

//Separate Cameras for upper/bottom screen: A global camera directs the scene, then each camera is customized per screen
static struct Camera upperScreenCamera;
static struct Camera bottomScreenCamera;

/// Resets the camera position to default position and tilt
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void initializeCamera(struct Camera * Inst){
	TWLPrintf("-- Creating camera\n");
	Inst->distance = -25.0f;
	Inst->verticalTilt = -30.0f;
	Inst->horizontalAngle = 90.0f;
	Inst->verticalAngle = 1.0f;
}

/// Positions the camera at the required place and rotation
/// Zoom and spin is done by translate/rotate
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void position(struct Camera * Inst){
	glTranslatef(0.0f, 0.0f, Inst->distance);
	glRotatef(Inst->horizontalAngle, Inst->verticalTilt, 1.0f, 0.0f);

//DS GX: Set extra camera parameters
#ifdef ARM9	
	//any floating point gl call is being converted to fixed prior to being implemented
	gluPerspective(-45, 256.0 / 192.0, 0.1, 250);

	gluLookAt(	1.0, -Inst->distance, -45.0f + Inst->horizontalAngle,		//camera possition 
				1.0, Inst->verticalAngle, 1.0,		//look at
				1.0, 1.0, 45.0 		//up
	);		
	glRotateX(-25.0f);
#endif
}

/// Decrements the distance to origin (zoom in)
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void dec(struct Camera * Inst){
	Inst->distance--;
}

/// Incrementes the distance to origin (zoom out)
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void inc(struct Camera * Inst){
	Inst->distance++;
}

/// Adjusts the camera rotation around the Y axis
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void clockwise(struct Camera * Inst){
	Inst->horizontalAngle++;
}

/// Adjusts the camera rotation around the Y axis
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void anticlockwise(struct Camera * Inst){
	Inst->horizontalAngle--;
}

/// Adjusts the camera rotation around the X axis
/// the angle is locked if it gets above 0 degrees
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void tiltup(struct Camera * Inst){
	if (Inst->verticalTilt < 0)
		Inst->verticalTilt++;
}

/// Adjusts the camera rotation around the X axis
/// The angle is locked if it gets greate than 90 degrees
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void tiltdown(struct Camera * Inst){
	if (Inst->verticalTilt > -90)
		Inst->verticalTilt--;
}

/// Default Constructor. Initialises defaults.
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void initializeScene(struct Scene * Inst){
	TWLPrintf("-- Creating scene\n");

	// set up our directional overhead lights
	Inst->light0On = false;
	Inst->light1On = false;
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0Scene);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0Scene);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0Scene);
	glLightfv(GL_LIGHT0, GL_POSITION, position0Scene);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1Scene);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1Scene);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1Scene);
	glLightfv(GL_LIGHT1, GL_POSITION, position1Scene);
	
	Inst->fogMode = false;
	Inst->wireMode = false;		/// wireframe mode on / off
	Inst->flatShading = false;	/// flat shading on / off
	initializeCamera(&Inst->camera); //construct camera
}

void render3DUpperScreen(){
	//Get Camera
	upperScreenCamera = scene.camera;
	
	//Adjust Camera
	upperScreenCamera.verticalAngle = -330.0f; 
	upperScreenCamera.horizontalAngle = upperScreenCamera.horizontalAngle / 2;
}

void render3DBottomScreen(){
	//Get Camera
	bottomScreenCamera = scene.camera;
}


/// Renders a single frame of the scene
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void drawScene(){
	struct Scene * Inst = &scene;

	#ifdef ARM9
	//NDS: Dual 3D Render implementation. Must be called right before a new 3D scene is drawn
	if(Inst->TGDSProjectDual3DEnabled == true){
		TGDS_ProcessDual(render3DUpperScreen, render3DBottomScreen);
	}
	#endif

	#ifdef WIN32
	// clear scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	#endif

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//position camera
	if(NE_Screen == 0){
		position(&upperScreenCamera); 
	}
	else{
		position(&bottomScreenCamera);
	}
	
	// draw all elements in the scene
	{
		int i = 0;
		for (i = 0; i < Inst->curElementAlloced; i++){
			#ifdef ARM9
				glMatrixMode(GL_TEXTURE); //GX 3D hardware needs this to enable texturing on a frame basis
				glLoadIdentity();	
				glMatrixMode(GL_MODELVIEW);
				
				glMaterialShinnyness();
				updateGXLights(); //Update GX 3D light scene!
				glColor3f(1.0, 1.0, 1.0); //clear last scene color/light vectors
			#endif
			draw(&Inst->elementsStart[i]);
		}
	}
	
	#ifdef WIN32
	drawHUD(Inst);
	glutSwapBuffers();
	#endif

	#ifdef ARM9
    glFlush();
	handleARM9SVC();	/* Do not remove, handles TGDS services */
    IRQVBlankWait();
    #endif
}

#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void glut2SolidCube0_06f() {
#ifdef ARM9
	updateGXLights(); //Update GX 3D light scene!
#endif
	glCallList(DLSOLIDCUBE0_06F);
}

/// Sets up the OpenGL state machine environment
/// All hints, culling, fog, light models, depth testing, polygon model
/// and blending are set up here
#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
#endif
int InitGL(int argc, char *argv[]){
	TWLPrintf("-- Setting up OpenGL context\n");
#ifdef _MSC_VER
	// initialise glut
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("TGDS Project through OpenGL (GLUT)");
	glutFullScreen();
#endif

#ifdef WIN32
	// blue green background colour
	glClearColor(0.0, 0.5, 0.55
#ifdef _MSC_VER
		, 1.0
#endif	
	);
	glShadeModel(GL_SMOOTH);

	// depth testing used on with less than testing
#ifdef _MSC_VER
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
#endif

	// setup  fog, but disable for now
#ifdef _MSC_VER
	glDisable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	{
		GLfloat fogColor[4] = {0.0f, 0.5f, 0.55f, 1.0f};
		glFogfv(GL_FOG_COLOR, fogColor);
	}
	glFogf(GL_FOG_DENSITY, 0.0075);
	
	// enable normalising of normals after scaling
	glEnable(GL_NORMALIZE);
#endif
	// setup lighting, but disable for now
	glDisable(GL_LIGHTING);
#ifdef _MSC_VER
	{
		GLfloat ambient[] = {0.1f, 0.1f, 0.1f, 1.0};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	}
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	/*
	* The actual lights are defined in the Scene class
	*/

	// set up line antialiasing
	glLineWidth(1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
	// setup backface culling
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
#endif

#ifdef ARM9
	int TGDSOpenGLDisplayListWorkBufferSize = (256*1024);
	struct Scene * Inst = &scene;
	glInit(TGDSOpenGLDisplayListWorkBufferSize); //NDSDLUtils: Initializes a new videoGL context
	glClearDepth(0x7FFF);		// Depth Buffer Setup
	glEnable(GL_ANTIALIAS|GL_TEXTURE_2D|GL_BLEND); // Enable Texture Mapping

	glDisable(GL_LIGHT0|GL_LIGHT1|GL_LIGHT2|GL_LIGHT3);

	if(Inst->TGDSProjectDual3DEnabled == false){
		setTGDSARM9PrintfCallback((printfARM9LibUtils_fn)&TGDSDefaultPrintf2DConsole); //Redirect to default TGDS printf Console implementation
		menuShow();
	}
	setTGDSARM9PrintfCallback((printfARM9LibUtils_fn)&TGDSDefaultPrintf2DConsole); //Redirect to default TGDS printf Console implementation
	//REG_IE = REG_IE & ~(IRQ_VBLANK); //Handled later in BG Music
	//REG_IE |= IRQ_VCOUNT;
	
	glReset(); //Depend on GX stack to render scene
	glClearColor(0,35,195);		// blue green background colour

	setOrientation(ORIENTATION_0, true);
	//set mode 0, enable BG0 and set it to 3D
	SETDISPCNT_MAIN(MODE_0_3D);
	
	/* TGDS 1.65 OpenGL 1.1 Initialization */
	ReSizeGLScene(255, 191);
	glMaterialShinnyness();
	//glGenTextures(1, &textureSizePixelCoords[Texture_CubeID].textureIndex);
	//glBindTexture(0, textureSizePixelCoords[Texture_CubeID].textureIndex);
	//glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)_6bppBitmap);		
	
	//Multiple 64x64 textures as PCX/BMP
	//Load 2 textures and map each one to a texture slot
	u32 arrayOfTextures[2];
	arrayOfTextures[FLOOR_TEXTURE] = (u32)&grass_tex; //0: grass_tex.bmp
	arrayOfTextures[FISH_TEXTURE] = (u32)&fish_tex; //1: fish_tex.bmp  
	int texturesInSlot = LoadLotsOfGLTextures((u32*)&arrayOfTextures, (sizeof(arrayOfTextures)/sizeof(u32)) ); //Implements both glBindTexture and glTexImage2D 
	int i = 0;
	for(i = 0; i < texturesInSlot; i++){
		printf("Tex. index: %d: Tex. name[%d]", i, getTextureNameFromIndex(i));
	}
	printf("Free Mem: %d KB", ((int)TGDSARM9MallocFreeMemory()/1024));
	glCallListGX((u32*)&GXPayload); //Run this payload once to force cache flushes on DMA GXFIFO
#endif
	
	glEnable(GL_COLOR_MATERIAL);	//allow to mix both glColor3f + light sources when lighting is enabled (glVertex + glNormal3f)

	glDisable(GL_CULL_FACE); 
	glCullFace (GL_NONE);

	//setupGLUTObjects();
	setupTGDSProjectOpenGLDisplayLists();
	return 0;
}


#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__((optnone))
#endif
#endif
void setupTGDSProjectOpenGLDisplayLists(){
	DLSOLIDCUBE0_06F = (GLint)glGenLists(1);

	//glut2SolidCube(); -> NDS GX Implementation
	glNewList(DLSOLIDCUBE0_06F, GL_COMPILE);
	{
		float size = 0.06f;
		GLfloat n[6][3] =
		{
			{-1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, -1.0f}
		};
		GLint faces[6][4] =
		{
			{0, 1, 2, 3},
			{3, 2, 6, 7},
			{7, 6, 5, 4},
			{4, 5, 1, 0},
			{5, 6, 2, 1},
			{7, 4, 0, 3}
		};
		GLfloat v[8][3];
		GLint i;

		v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
		v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
		v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
		v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
		v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
		v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

		for (i = 5; i >= 0; i--)
		{
			glBegin(GL_QUADS);
			glNormal3fv(&n[i][0]);
			glTexCoord2f(0, 0);
			glVertex3fv(&v[faces[i][0]][0]);
			glTexCoord2f(1, 0);
			glVertex3fv(&v[faces[i][1]][0]);
			glTexCoord2f(1, 1);
			glVertex3fv(&v[faces[i][2]][0]);
			glTexCoord2f(0, 1);
			glVertex3fv(&v[faces[i][3]][0]);
			glEnd();
		}
	}
	glEndList();
	
	
}

#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
#endif
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// resizes the window (GLUT & TGDS GL)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

#ifdef _MSC_VER
/// Resizes the current viewport to the full window size
void resizeWindow(int w, int h)
{
	glViewport(0, 0, w, h);

	// update the width and height we are using
	widthScene = w;
	heightScene = h;

	setupViewVolume();
}
#endif

#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
int startTGDSProject(int argc, char *argv[])
{
	time_t time1 = time(NULL);
	TWLPrintf("-- Program starting: %d\n", (unsigned int)time1);
	srand(time1);
	InitGL(argc, argv);

	// register our call-back functions
	TWLPrintf("-- Registering callbacks\n");
	
#ifdef _MSC_VER
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(keyboardInput);
	glutSpecialFunc(keyboardInputSpecial);
#endif

	// generate/load textures
	getTextures();

	// create the scene and set perspective projection as default
	initializeScene(&scene);
	scene.perspectiveMode = true;

	// create a single quad for the floor of the aquarium (reduces 420~ objects into mere 22 which makes the engine runnable on NintendoDS at good framerate)
	{

		GLfloat mat_ambient[]    = { 
#ifdef _MSC_VER
			1.0f, 1.0f, 1.0f, 1.0f		//WIN32
#endif
#ifdef ARM9
			30.0f, 30.0f, 30.0f, 30.0f	//NDS
#endif
		}; 

		GLfloat mat_diffuse_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
		GLfloat high_shininess[] = { 
#ifdef _MSC_VER
			120.0f
#endif
#ifdef ARM9
			128.0f //NDS
#endif
		}; 
		
		struct MarineObject quad = BuildQuad((void*)&_drawQuad, mat_ambient, mat_diffuse_specular, high_shininess[0], NULL, NULL, NULL, NULL);
		quad.ry = 0.0f;	// we don't want random rotation
		quad.rx = 90.0f;
		quad.x = 1.0f * 1; //X axis relative to polygon origin
		quad.z = 1.0f * 1; //Z axis relative to polygon origin
		scale(&quad, 95.5f, 95.5f, 3.0f);
		add(&scene, &quad);
	}

	// 'fake' keys being pressed to enable the state to
	// setup lighting and fog
	keyboardInput((unsigned char)'L', 0, 0);
	keyboardInput((unsigned char)'0', 0, 0);
	keyboardInput((unsigned char)'1', 0, 0);
	keyboardInput((unsigned char)'2', 0, 0);
	keyboardInput((unsigned char)'F', 0, 0);

	// add some stuff to the scene
	{
		int o = 0;
		for (o = 0; o < 4; o++){
			addObject(OBJ_CRAB);
			addObject(OBJ_STARFISH);
			//addObject(OBJ_FISH);
			//addObject(OBJ_FISH);
			//addObject(OBJ_FISH);
			addObject(OBJ_OCTOPUS);
			addObject(OBJ_PLANT);
		}
	}

	// start the timer and enter the mail GLUT loop
#ifdef _MSC_VER
	glutTimerFunc(50, animateScene, 0);
	glutMainLoop();
#endif

#if defined(ARM9)
	BgMusicOff();
	BgMusic("0:/tank.ima");
	//startTimerCounter(tUnitsMilliseconds, 1);
    glMaterialShinnyness();
	glReset(); //Depend on GX stack to render scene
	glClearColor(0,35,195);		// blue green background colour
	while(1==1){
		//Handle Input & game logic
		scanKeys();
		keyboardInputSpecial((int)keysHeld(), 0, 0);
		
		//sound (ARM7)
		
		//Render
		drawScene();
	}
#endif
	return 0;
}




/// Adds an object to the rendering queue
/*
* This method simply adds on the passed object to the end
* of the rendering queue.
*/
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
bool add(struct Scene * Inst, struct MarineObject *object){
	if(Inst->curElementAlloced < renderableElementsTotal){
		memcpy(&Inst->elementsStart[Inst->curElementAlloced], object, sizeof(struct MarineObject));
		Inst->curElementAlloced++;
		return true;
	}
	return false;
}


/// Draws the heads up display
/*
* This method draws all of the required information about the
* current scene in the window. Orthographic projection is used
* here to position the text easier. both the PROJECTION and the
* MODELVIEW matrices are restored to their original states when
* this method returns.
*/
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void drawHUD(struct Scene * Inst){
	GLboolean lightsOn;
	
	// disable depth testing to HUD is drawn on top of everything
	glDisable(GL_DEPTH_TEST);

	// disable lightint so we see everything OK
	glGetBooleanv(GL_LIGHTING, &lightsOn);
	glDisable(GL_LIGHTING);

	// grab and save the current projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// setup new projection
	glOrtho(-0.1f, 10.0f, 10.0f, -0.4f, -1.0f, 10.0f);

	// grab and save current view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);

	// print all of the stats
	printGL1(Inst, 0.0f, 0.0f, 0.0f, "Crabs:    ", Inst->objects[OBJ_CRAB]);
	printGL1(Inst, 1.5f, 0.0f, 0.0f, "Starfish: ", Inst->objects[OBJ_STARFISH]);
	printGL1(Inst, 3.0f, 0.0f, 0.0f, "Octopi:   ", Inst->objects[OBJ_OCTOPUS]);
	printGL1(Inst, 4.5f, 0.0f, 0.0f, "Fish:     ", Inst->objects[OBJ_FISH]);
	printGL1(Inst, 6.0f, 0.0f, 0.0f, "Plants:   ", Inst->objects[OBJ_PLANT]);

	/*
	* the printGL() function expects the string to be 30 characters in length
	* so we have to pad each string with white space so that weird characters
	* do not appear
	*/
	
	// print lighting mode calculations
	if (Inst->lightMode) printGL2(Inst, 0.0f, 0.3f, -1.0f, "Lighting Calculations: On     ");
	else printGL2(Inst, 0.0f, 0.3f, -1.0f, "Lighting Calculations: Off    ");

	// print light 0 status
	if (Inst->light0On) printGL2(Inst, 0.0f, 0.6f, -1.0f, "Overhead Light: On            ");
	else printGL2(Inst, 0.0f, 0.6f, -1.0f, "Overhead Light: Off           ");

	// print light 1 status
	if (Inst->light1On) printGL2(Inst, 0.0f, 0.9f, -1.0f, "Miner's Hat Light: On         ");
	else printGL2(Inst, 0.0f, 0.9f, -1.0f, "Miner's Hat Light: Off        ");

	// print fog status
	if (Inst->fogMode) printGL2(Inst, 0.0f, 1.2f, -1.0f, "Fog: On                       ");
	else printGL2(Inst, 0.0f, 1.2f, -1.0f, "Fog: Off                      ");

	// print the menu if needed
	if (Inst->showMenu)
		printMenu(Inst);

	// restore old view
	glPopMatrix(
#ifdef ARM9
		1
#endif	
	);

	// grab and restore old projection
	glMatrixMode(GL_PROJECTION);
	glPopMatrix(
#ifdef ARM9
		1
#endif
	);

	// return to view mode
	glMatrixMode(GL_MODELVIEW);

	// enable depth testing again
	glEnable(GL_DEPTH_TEST);

	// enable lighting if needed
	if (lightsOn){
		glEnable(GL_LIGHTING);
	}
}


/// Prints a string with a count attached
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void printGL1(struct Scene * Inst, GLfloat x, GLfloat y, GLfloat z, const char *str, int count){
	char buffer[30];
	int i = 0;
	for (i = 0; i < 30; ++i){
		buffer[i] = ' ';
	}
	sprintf(buffer,"%s %i", str, count);
	printGL2(Inst, x, y, z, buffer);
}


/// Prints a string in the window
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void printGL2(struct Scene * Inst, GLfloat x, GLfloat y, GLfloat z, const char *str){
#ifdef WIN32
	int j = 0;
	glRasterPos3f(x, y, z);
	// print character one by one
	for (j = 0; j < 30; j++) {
		int c = str[j];
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
	}
#endif
}


/// Displays the menu screen
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void printMenu(struct Scene * Inst){
	glColor3f(0.0f, 0.0f, 0.0f);
	printGL2(Inst, 0.0f, 1.7f, -1.0f, "---[ Help Screen ]------------");

	glColor3f(1.0f, 1.0f, 0.0f);
	printGL2(Inst, 0.0f, 2.0f, -1.0f, "F1    - Toggle Help Screen      ");
	printGL2(Inst, 0.0f, 2.3f, -1.0f, "F2    - Add a crab              ");
	printGL2(Inst, 0.0f, 2.6f, -1.0f, "F3    - Add an octopus          ");
	printGL2(Inst, 0.0f, 2.9f, -1.0f, "F4    - Add a starfish          ");
	printGL2(Inst, 0.0f, 3.2f, -1.0f, "F5    - Add a fish              ");
	printGL2(Inst, 0.0f, 3.5f, -1.0f, "F6    - Add a plant             ");

	glColor3f(0.5f, 1.0f, 0.0f);
	printGL2(Inst, 0.0f, 3.8f, -1.0f, "F/f   - Toggle fog on/off       ");
	printGL2(Inst, 0.0f, 4.1f, -1.0f, "W/w   - Toggle wireframe on/off ");
	printGL2(Inst, 0.0f, 4.4f, -1.0f, "L/l   - Toggle lighting on/off  ");
	printGL2(Inst, 0.0f, 4.7f, -1.0f, "1/2   - Toggle light 1/0 on/off ");

	glColor3f(0.0f, 1.0f, 0.0f);
	printGL2(Inst, 0.0f, 5.0f, -1.0f, "A/Z   - Inc/Dec elevation angl  ");
	printGL2(Inst, 0.0f, 5.3f, -1.0f, "UP    - Dec distance to origin  ");
	printGL2(Inst, 0.0f, 5.6f, -1.0f, "DOWN  - Inc distance to origin  ");
	printGL2(Inst, 0.0f, 5.9f, -1.0f, "LEFT  - Rotate clockwise        ");
	printGL2(Inst, 0.0f, 6.2f, -1.0f, "RIGHT - Rotate anticlockwise    ");

	glColor3f(0.0f, 0.0f, 1.0f);
	printGL2(Inst, 0.0f, 6.5f, -1.0f, "ESC   - Exit                    ");

	glColor3f(0.0f, 0.0f, 0.0f);
	printGL2(Inst, 0.0f, 6.8f, -1.0f, "---[Igor Kromin 40125374 ]----");
}

#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("Ofast")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void drawSphereCustom(float r, int lats, int longs){

#ifdef WIN32
	int i, j;
	for (i = 0; i <= lats; i++) {
		float lat0 = PI * (-0.5 + (float)(i - 1) / lats);
		float z0 = sin((float)lat0);
		float zr0 = cos((float)lat0);

		float lat1 = PI * (-0.5 + (float)i / lats);
		float z1 = sin((float)lat1);
		float zr1 = cos((float)lat1);
		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= longs; j++) {
			float lng = 2 * PI * (float)(j - 1) / longs;
			float x = cos(lng);
			float y = sin(lng);
			glNormal3f(x * zr0, y * zr0, z0); //lights are off
			glVertex3f(r * x * zr0, r * y * zr0, r * z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(r * x * zr1, r * y * zr1, r * z1);
		}
		glEnd();
	}
#endif

	#ifdef ARM9
	#include "Sphere008.h"
	glScalef(r, r, r);
	glCallListGX((u32*)&Sphere008); //comment out when running on NDSDisplayListUtils
	#endif
}