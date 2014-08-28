//
//  utils.h
//  testxxx
//
//  Created by Rhody Lugo on 8/27/14.
//  Copyright (c) 2014 Rhody Lugo. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <sys/time.h>

static double getSeconds() {
	struct timeval t;
	gettimeofday(&t, NULL);
	long msec = t.tv_sec*1e6 + t.tv_usec;
	return msec/1e6;
}

static double dumpTime() {
	static double psec = getSeconds();
	double csec = getSeconds();
	double sec = csec - psec;
	psec = csec;
	return sec;
}

void drawBitmapString( const char *string, void *font) {
	while (*string) glutBitmapCharacter(font, *string++);
}

void drawStr(const char* str, void *font, ...) {
	va_list args;
	char buffer[512];
	va_start(args,font);
	vsprintf(buffer,str,args);
	va_end(args);
	drawBitmapString(buffer,font);
}

void drawFPS() {
	drawStr("%.2f", GLUT_BITMAP_HELVETICA_18, 1.0f/dumpTime());
}

