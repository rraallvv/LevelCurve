#include "levelCurve.h"

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
	glOrtho(-5, 605, -5, 605, -1, 1);
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(1, 1, 1);
	
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(600, 0);
	glVertex2f(600, 600);
	glVertex2f(0, 600);
	glEnd();
	
	glColor3f(0, 0, 0);
	glRasterPos2d(600*0.01,600*0.97);
	drawFPS();
	
	levelCurve(surface, 0, 0, 600, 600, 5, true);
	
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

Quadtree *gCurve;

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

