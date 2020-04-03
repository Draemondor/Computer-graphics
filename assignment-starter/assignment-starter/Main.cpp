
#include <windows.h>
#include <GL/glut.h>
#include "location.h"
#include "RGBColor.h"
#include "Ball.h"
#include "Checkerboard.h"
#include "Camera.h"
#include <cmath>
#include <iostream>
#include <random>

using namespace std;

/*************************************************************************
Class:		Checkboard class implementation
Purpose:	Provide a Red/White 8x8 checkerboard
Author      Cooper, using https://cs.lmu.edu/~ray/notes/openglexamples/ as an inital base
Date:	    3/2/2020

Note:       Implementation uses existing Location, RGBColor classes rather than primitives
************************************************************************/
//  globals

Ball balls[7];          //  seven balls in the example
Checkerboard board;
Camera camera;

//  set up colors.  Unfortunately a lot of the openGL calls use GLfloat so the RGBColor and Location information has to include a toGLFloat() function
RGBColor red(1, 0, 0);
RGBColor green(0, 1, 0);
RGBColor blue(0, 0, 1);
RGBColor cyan(0, 1, 1);
RGBColor magenta(1, 0, 1);
RGBColor yellow(1, 1, 0);
RGBColor white(1, 1, 1);
GLfloat WHITE[3], RED[3], GREEN[3], BLUE[3], MAGENTA[3], YELLOW[3], CYAN[3];
RGBColor colors[] = { red,green,blue,magenta,cyan,yellow, white };



/*************************************
Function:   init()
Purpose:    set up 3-D lighting
Author:      https://cs.lmu.edu/~ray/notes/openglexamples/
Date:   3/2/2020
*************************************/
void init() {
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
	glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	board.create();
}

/*************************************
Function:   initBalls()
Purpose:    set up 7 bouncing balls
Author:      Cooper
Date:       3/2/2020
*************************************/
void initBalls() {
	default_random_engine generator;
	uniform_int_distribution<int> distribution(1, 7);
	Location location(0, 0, 0);
	red.toGLfloatArray(RED);
	green.toGLfloatArray(GREEN);
	blue.toGLfloatArray(BLUE);
	magenta.toGLfloatArray(MAGENTA);
	yellow.toGLfloatArray(YELLOW);
	cyan.toGLfloatArray(CYAN);
	white.toGLfloatArray(WHITE);
	for (int i = 0; i < sizeof balls / sizeof(Ball); i++) {
		balls[i].setColor(colors[i]);
		balls[i].setRadius(0.5);
		balls[i].setDirection(1);
		float x = distribution(generator);
		float z = distribution(generator);
		float y = distribution(generator);
		location.setLocation(x, y, z);
		balls[i].setLocation(location);
	}

}

/*************************************
Function:   display()
Purpose:    set up display and update
Author:      https://cs.lmu.edu/~ray/notes/openglexamples/
Date:       3/2/2020
*************************************/
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
		board.centerx(), 0.0, board.centerz(),
		0.0, 1.0, 0.0);
	board.draw();
	for (int i = 0; i < sizeof balls / sizeof(Ball); i++) {
		balls[i].update();
	}
	glFlush();
	glutSwapBuffers();
}

/*************************************
Function:   reshape()
Purpose:    manage changes to the window aspect ratio
Author:      https://cs.lmu.edu/~ray/notes/openglexamples/
Date:       3/2/2020
*************************************/
void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}

/*************************************
Function:   timer()
Purpose:    manage update speed
Author:      https://cs.lmu.edu/~ray/notes/openglexamples/
Date:       3/2/2020
*************************************/
void timer(int v) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, v);
}

/*************************************
Function:   special()
Purpose:    manage special keys, Arrow keys
Author:      https://cs.lmu.edu/~ray/notes/openglexamples/
Date:       3/2/2020
*************************************/
void special(int key, int, int) {
	switch (key) {
	case GLUT_KEY_LEFT: camera.moveLeft(); break;
	case GLUT_KEY_RIGHT: camera.moveRight(); break;
	case GLUT_KEY_UP: camera.moveUp(); break;
	case GLUT_KEY_DOWN: camera.moveDown(); break;
	}
	glutPostRedisplay();
}

// Initializes GLUT and enters the main loop.
int main(int argc, char** argv) {
	initBalls();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Bouncing Balls");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutTimerFunc(100, timer, 0);

	init();
	glutMainLoop();
}