/*
    ssystem 1.6
    Copyright (C) 1997-1998  Raul Alonso <amil@las.es>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#ifndef PLANET
#include "cfgparse.tab.h"
#endif


#define NUMSTARS 3141 /* 3141 for full star catalog, decrease number for better
			performance, first N brighter stars are used */

#define MAXBODIES 50

#ifndef PI
#define PI 3.14159265358979323846
#define PIl 3.14159265358979323846L
#endif

#define DATEOFELEMENTS 2450680.5 /* In Julian days */

/* Macros used to scale down distances/radii so we can reduce the
  jerkiness effect in outter bodies */
#define DFACTOR 1000.0
#define RADIUSSCALE(x) ((x)*0.1/DFACTOR)
#define DISTCORRECTION(x) ((x)*149.0/DFACTOR)

/* Need no comment ;-) */
#define DEG2RAD(x) ((x)*PI/180.0)
#define RAD2DEG(x) ((x)*180.0/PI)
#define DISTANCE(x,y,z) sqrt((x)*(x)+(y)*(y)+(z)*(z))
#define RANGE(n,l,h) ((n)>(h)?(h):((n)<(l)?(l):(n)))

#ifndef WIN32
#define error(s) fprintf(stderr,s)
#define scmp strcasecmp
#else
#define error(s) MessageBox(NULL,s,"ERROR", MB_OK); 
#define scmp _stricmp
#endif

extern GLfloat LightPos[4];
extern GLfloat ambient[4];
extern GLfloat White[4];
extern GLfloat Black[4];
extern int texture,smodel,lighting,drawstars;
extern int Stars,Ecliptic,red,polaris;
extern int ImgWidth, ImgHeight, width, height;
extern double days,timefactor;
extern char texturepath[100];
extern int slices,stacks,NUMBODIES;
extern double camx, camy, camz, eyex, eyey, eyez, upx, upy, upz;
extern float fov,fps,sec,star_mag, speed,accel,oldaccel,d,zoom,aratio;
extern int frames,paused,cmode,currsource,currtarget;
extern int demomode,bench,help,plabels,slabels,init,openbitmap;
extern int fullscreen,info,realtime,trail,jpeg_quality,atmospheres;
extern char sbuf[100],game_string[20];
extern int joyavailable,joyactive;



typedef struct {
	char Name[20];
	int Type,TrailEnabled;
	int Sat,CustomFunction;
	GLUquadricObj *Object;
	GLubyte *Image;
	GLuint Sphere,Trail,*textures,texnum,texwidth,texheight;
	float DeltaRotation,Radius,xflat,yflat,zflat,Rotation,Degrees,OrbitalPeriod;
	float Inclination,AscendingNode,Perihelion,MeanDistance,DailyMotion;
	float Eccentricity,MeanLongitude,Color[4];
	double posx,posy,posz;
} planetdata;

typedef struct {
	char Name[20];
	double posx,posy,posz;
	float magnitude,color[3];
	char type;
} stardata;

extern planetdata planets[MAXBODIES];
extern stardata stars[NUMSTARS];	

				/* cmdline.c */	
#ifdef WIN32
void ParseCmdLineWIN32(char *);
#else
void ParseCmdLine(int, char **);
#endif
				/* init.c */
void Init(void);
void InitStars(float, int);
				/* positions.c */
void UpdatePositions(double, int);
				/* joystick.c */
void joystick(void);
void joydetect(void);
				/* cfgparse */
void ReadConfigFile(void);
				/* vsop87.c */
void EarthPos(double, double *, double *, double *);
void JupiterPos(double, double *, double *, double *);
void MarsPos(double, double *, double *, double *);
void MercuryPos(double, double *, double *, double *);
void NeptunePos(double, double *, double *, double *);
void SaturnPos(double, double *, double *, double *);
void UranusPos(double, double *, double *, double *);
void VenusPos(double, double *, double *, double *);
void PlutoPos(double, double *, double *, double *);
void MoonPos(double, double *, double *, double *);
				/* keyboard.c */
void Key( unsigned char, int, int);
void Special(int, int, int);
				/* mouse.c */
void Mouse(int, int, int, int);
void Motion(int, int);
				/* sshot.c */
void ScreenShot(char *);
				/* sun.c */
void SunBillBoard( void );
				/* timer.c */
void TimerDemo(int);
				/* util.c */
void printstring(void *, char *);
float gettime(void);
void Rotation(double, double, double, double, double *, double *, double *);
				/* jpeg.c */
GLubyte *read_JPEG_file (char *, int *, int *, int *);
				/* stars.c */
void InitStars(float, int);
extern float star_size;