//
//  main.cpp
//  testxxx
//
//  Created by Rhody Lugo on 8/27/14.
//  Copyright (c) 2014 Rhody Lugo. All rights reserved.
//

#include "SPACEGOOLevelCurve.h"
/*
 <script type='text/javascript' src='js/main.js'></script>
 */

#include <iostream>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include "utils.h"

void idle() {
	glutPostRedisplay();
}

void display() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 1, 1);

	glBegin(GL_POLYGON);
	glVertex2f(0.25, 0.25);
	glVertex2f(0.75, 0.25);
	glVertex2f(0.75, 0.75);
	glVertex2f(0.25, 0.75);
	glEnd();

	glColor3f(0, 0, 0);
	glRasterPos2d(0.32,0.48);
	drawStr("HELLO WORLD!", GLUT_BITMAP_TIMES_ROMAN_24);

	glColor3f(1, 1, 1);
	glRasterPos2d(0.0,0.97);
	drawFPS();

	glutSwapBuffers();
}

void keyboard( unsigned char key, int x, int y ) {
	switch (key) {
		case 27:
			exit(0);
			break;
			
		default:
			break;
	}
}

void mouse ( int button, int state, int x, int y ) {
}

void motion ( int x, int y ) {
}

int main(int argc, char * argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GL_DOUBLE);
	glutInitWindowPosition ( 100, 100 );
	glutInitWindowSize ( 600, 600 );
	glutCreateWindow(argv[0]);

	CGLContextObj context = CGLGetCurrentContext();
	const GLint SYNC_TO_REFRESH = 0;
	CGLSetParameter(context, kCGLCPSwapInterval, &SYNC_TO_REFRESH);

	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc (motion);
	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

