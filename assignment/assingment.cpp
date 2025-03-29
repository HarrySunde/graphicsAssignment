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

// set globals here ==============================================
//camera angles:
float angle=0., look_x=0., look_z=0., eye_x=0., eye_z=10.;	//View parameters
//----------------------------------------------------------------------

//load textures: SPEC At least three different textures must be used in the scen

void loadTexture() {
	glGenTextures(4, txId); // will def need more than 4 lol

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

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}


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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50., 1., 1., 100.);
}


//illumination


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
    
}



// set main here
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800); 
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Factory"); // name
	initialise();
    // incorporate timer func here
	glutDisplayFunc(display); 
	glutSpecialFunc(special);
	glutMainLoop();
	return 0;
}