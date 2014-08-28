#include "levelCurve.h"

#include <iostream>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include "utils.h"

// iso-surfaces tests
float surface(Vec2 p) {
	float x= (p.x-300);
	float y = (p.y-300);
	return cos(x/50)*x*x + y*y - 10000;
}

float surface2(Vec2 p) {
	float x= (p.x-300);
	float y = (p.y-300);
	return 5*x*x + y*y - 10000;
}

static float t = 0;

float surface3(Vec2 p) {
	float x= (p.x-300);
	float y = (p.y-300);
	return cos(x/100+y/150+t);
}

// GLUT functions
void idle() {
	glutPostRedisplay();
}

void display() {
	t += 1.0e-3;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5, 605, -5, 605, -1, 1);
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	levelCurve(surface, 0, 0, 600, 600);
	
	glColor3f(1, 1, 1);
	glRasterPos2d(600*0.01,600*0.97);
	drawFPS();
	
	glutSwapBuffers();
}

void keyboard( unsigned char key, int x, int y ) {
	switch (key) {
		case 27:
			exit(0);
			break;
			
		case 'd':
		case 'D':
			LevelCurveAPI::getInstance().debug = !LevelCurveAPI::getInstance().debug;
			break;
			
		case '+':
			LevelCurveAPI::getInstance().addProf();
			break;
			
		case '-':
			LevelCurveAPI::getInstance().subProf();
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

// main
int main(int argc, char * argv[]) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GL_DOUBLE);
	glutInitWindowPosition ( 100, 100 );
	glutInitWindowSize ( 600, 600 );
	glutCreateWindow(argv[0]);
	
#if 0
	CGLContextObj context = CGLGetCurrentContext();
	const GLint SYNC_TO_REFRESH = 0;
	CGLSetParameter(context, kCGLCPSwapInterval, &SYNC_TO_REFRESH);
#endif
	
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc (motion);
	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
