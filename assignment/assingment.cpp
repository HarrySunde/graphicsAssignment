//  ========================================================================
//  COSC363: Computer Graphics Aaaignment  (2025); 
//  FILE NAME: assingment.cpp
//  author: Harry Sunde 63872199
//  
//  ========================================================================

#include <iostream>
#include <cmath> 
#include <GL/freeglut.h>
#include "loadTGA.h"
using namespace std;

/*lab references
lab 2: transformations: useful for conveyer/ item models
lab 3: illumination, texture mapping, auto texturing ,  https://polyhaven.com/hdris
lab 4: surface of revolution, quad strips, wireframe mode, geometry
lab 5: smoke/fire. spark emmision 
*/

/*
mim specifications 

1) A model of a conveyor belt with item(s) moving on it: This need not be made of
highly complex models. It could be constructed using a set of GLUT objects. The
item(s) should move along the conveyor belt, and there should always be at least
one item on the conveyor belt.

2) Animated machinery: Machine models may also be constructed using GLUT
objects. They do not need to be complicated models, but each piece of machinery
should perform some action such as oscillating, hammering, or spinning.

3) Lighting: The scene should have proper lighting so that the models and animations
are clearly visible.

4) Camera: The user should be able to move the camera through the scene using a set
of directional keys: E.g. Up arrow: move camera forward in the current direction,
Down arrow: move camera backward in the current direction, Left arrow: Turn left
by 5 degs, Right arrow: Turn right by 5 degs.

5) Textures: At least three different textures must be used in the scene. It is not
necessary to texture-map all surfaces. Please do not use very large images (>10MB)
as textures. A skybox texture set, if used, will be counted as a single texture.

6) Wireframe mode: The user should be able to toggle a wireframe mode on/off by
pressing a key (e.g. ‘q’). When wireframe mode is on, polygon filling and lighting
must be disabled

*/

// set globals here ==============================================
//camera angles:
float angle=0., look_x=0., look_z=0., eye_x=0., eye_z=10.;	//View parameters
bool wireframeMode = false; 
//----------------------------------------------------------------------

//load textures: SPEC At least three different textures must be used in the scen

void loadTexture() {
	glGenTextures(5, txId); // will def need more than 4 lol
	// 0- 3 are just format, need to replace textures
	glBindTexture(GL_TEXTURE_2D, txId[0]);
    loadTGA("Brick_Texture.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glBindTexture(GL_TEXTURE_2D, txId[1]);
    loadTGA("Floor_Texture.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glBindTexture(GL_TEXTURE_2D, txId[2]);
	loadTGA("Stone_Texture.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[3]);
	loadTGA("Tree_Texture.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// texture for inside of sphere
	glBindTexture(GL_TEXTURE_2D, txId[4]);
	loadTGA("boiler_room_for_skybox.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}

// initialise gl features =============================
void initialise() { 
	glEnable(GL_TEXTURE_2D);
	loadTexture(); // need to build out our skybox 
	// including gl enable stuff
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	glClearColor(0., 0., 0., 0.);  //Background colour 	 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_PROJECTION); // Applies subsequent matrix operations to the texture matrix stack. 
	glLoadIdentity(); // replaces the current matrix with the identity matrix
	gluPerspective(50., 1., 1., 100.);
}


// models ===================================================================================
/* order of business: 1)floor 2) industrial skybox, 3)conveyer with objects, 
will need to model a conveyer belt, have objects moving on it, two cylinders inside also

modularise after models complete
*/
void floor() {
	glBindTexture(GL_TEXTURE_2D,  txId[1]);
	glColor3f(1, 1, 1);
	glNormal3f(0, 1, 0);
	glBegin(GL_QUADS);		//A single quad
		glTexCoord2f(0, 0);
		glVertex3f(-16, -0.1, 16);
		glTexCoord2f(1, 0);
		glVertex3f(16, -0.1, 16);
		glTexCoord2f(1, 1);
		glVertex3f(16, -0.1, -16);
		glTexCoord2f(0, 1);
		glVertex3f(-16, -0.1, -16);
	glEnd();
}

void skybox() {
	/*
	When using skybox textures, specify the texture wrap parameters as follows:
	#define GL_CLAMP_TO_EDGE 0x812F //Include this line only if GL_CLAMP_TO_EDGE is undefined.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	trying sphere first: 
	plan:
	1) generate sphere, make it very large, so everything can be internal
	glutSphere(radius, slices, stacks);

	2) map boileroom texture to inside of sphere;
	*/

	//1 from lec 4 as ref, pg 29
	GLUquadricObj*q;
	q = gluNewQuadric();
	gluQuadraticTexture(q, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, txId[4]);

	//depth stuff need testing and comprehension
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);

	//may put matrix in display func, not sure
	glPushMatrix(); //very big sphere lol
		glRotatef(-90., 1.0, 0., 0.0);
		gluSphere (q, 500.0, 64, 32);
	glPopMatrix();



}

//display function =================================================




//initialise
//-- Initialise OpenGL parameters, load textures ---------------------
void initialise() { 
	glEnable(GL_TEXTURE_2D); // Enable 2D Texture
	loadTexture();
	// including gl enable stuff
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	glClearColor(0., 0., 0., 0.);  //Background colour 	 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50., 1., 1., 100.);
}


// special function, permits movement of camera via key bindings, specs state view angle change should be by 5 degrees-----------------
void special(int key, int x, int y) {
    /*Wireframe mode: The user should be able to toggle a wireframe mode on/off by
    pressing a key (e.g. ‘q’). When wireframe mode is on, polygon filling and lighting
    must be disabled */

    if (key == GLUT_KEY)

	// maipulate camera t move with keypress
	if		(key == GLUT_KEY_LEFT)  angle -= 0.1;	
	else if (key == GLUT_KEY_RIGHT) angle += 0.1;
	else if (key == GLUT_KEY_DOWN)  {
		eye_x -= 0.1*sin(angle);
		eye_z += 0.1*cos(angle);
	}
	else if (key == GLUT_KEY_UP)	{	
		eye_x += 0.1*sin(angle);
		eye_z -= 0.1*cos(angle);
	}

	look_x = eye_x + 10*sin(angle);
	look_z = eye_z - 10*cos(angle);
	glutPostRedisplay();
}

// wireframe View enambled via normal key press
void normal(int key, int x, int y) {
    /*Wireframe mode: The user should be able to toggle a wireframe mode on/off by
    pressing a key (e.g. ‘q’). When wireframe mode is on, polygon filling and lighting
    must be disabled */
    // reference lec o5 object modeling pg 32
    // need to adjust display settings also create if statement checking q
    if (key == 'q') wireframeMode = true;
    glutPostRedisplay();
}


// generic timer func from ToyTrain lab three
void myTimer(int value)
{
    /*
    for timers:
    Use only a single timer event sequence
    • Use only one timer callback function
    • Define a global int frame variable, incremented each time inside the timer
    callback function
    • Define start and end points of animation sequences based on the values of the
    frame variable.
    */

	//angle ++; commented out due to toytrain having different function
	glutPostRedisplay();
	glutTimerFunc(50, myTimer, 0);
}

// display function =======================================================
void display(void) {
	float light[] = {0., 50., 0., 1.};	//Light above the origin
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	


}



// set main here
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800); 
	glutInitWindowPosition(10, 10);
	glutCreateWindow("assignment"); // name
	initialise();
    // incorporate timer func here
	glutDisplayFunc(display); 
	glutSpecialFunc(special);
    glutKeyboardFunc(normal);
	glutMainLoop();
	return 0;
}