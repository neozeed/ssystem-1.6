/*
    ssystem 1.6
    Copyright (C) 1997-1999  Raul Alonso <amil@las.es>

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
#include <string.h>
#include <math.h>
#ifndef __FreeBSD__
#include <malloc.h>
#endif
#include <time.h>
#include <setjmp.h>
#include "ssystem.h"

double camx=0.0, camy=0.0, camz=1.0, eyex=0.0, eyey=0.0, eyez=-1.0, upx=0.0, upy=1.0, upz=0.0;
float fov=37.5,fps,sec,star_mag=3.0, speed=0.0,accel=0.0,oldaccel,d,zoom=10.0,aratio;
int frames=0,paused=0,cmode=1,currsource=0,currtarget=3;
int demomode=0,bench=0,help=0,plabels=0,slabels=0,init=0;
int fullscreen=0,info=1,realtime=0,jpeg_quality=90,atmospheres=1;
char sbuf[100],game_string[20]="640x480:16";
extern int joyavailable,joyactive;
static void Idle(void),Reshape(int,int),Camera(void),
	OnScreenInfo(void),Display(void);


static void Reshape( int x, int y )
{
   width=x; height=y;
   if (x>y) aratio=sqrt(x*x+y*y)/y;
   else aratio=sqrt(x*x+y*y)/x;
   glViewport( 0, 0, width, height );
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(fov,width/(double)height,RADIUSSCALE(0.1),DISTCORRECTION(1000.0));
   glMatrixMode( GL_MODELVIEW );
}


static void OnScreenInfo(void)
{
   time_t t;
   struct tm *tm;
   int i,j;

   glDisable(GL_TEXTURE_2D);
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);
   if (slabels) { /* Star labels on ? */
   	glColor3f(0.3,1.0,0.3);
   	i=0;
   	while (stars[i].magnitude<star_mag) {
   		if (i>NUMSTARS) break;
   		glRasterPos3f(stars[i].posx,stars[i].posy,stars[i].posz);
   		printstring(GLUT_BITMAP_HELVETICA_10,stars[i].Name);
   		i++;
   	}
   }
   glColor3f(1.0,1.0,1.0);
   if (plabels) /* Planets labels on ? */
   	for (i=0;i<NUMBODIES;i++) {
   		if (planets[i].Type>=RING) continue;
   		if (planets[i].Type==SATELLITE) {
			/* Distance from planet to camera */
   			d=DISTANCE(planets[i].posx-camx,planets[i].posy-camy,
   			  	planets[i].posz-camz);

			/* If it's too far don't show satellite labels */
  			if (d>DISTCORRECTION(0.1)) continue;
   		}
   		glRasterPos3f(planets[i].posx,
   			planets[i].posy-RADIUSSCALE(planets[i].Radius),
   			planets[i].posz);
   		printstring(GLUT_BITMAP_HELVETICA_10,planets[i].Name);
   	}

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0,(float) width,(float) height,0.0,0.0,1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   if (help) {
   	i=width/2-120;
   	j=height/2-180;
	glRasterPos2i(i,j);
        printstring(GLUT_BITMAP_HELVETICA_10,"Home/End: Select previous/next body");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"SHIFT + Home/End: Selects previous/next source body in B2B camera mode");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"Insert: Current target trail on/off (planets only)");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"a : Atmosphere on/off");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"t : Texture on/off");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"l : Lighting on/off");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"f : Flat/Smooth shading model");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"s : Stars on/off");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"S : Screenshot");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"m/M : Increase/Decrease stars brightness");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"d : Demo mode on/off");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"n : Place camera near current target planet");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"c : Camera modes");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"o/O : Wider/Narrower FOV");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"p : Pause");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"r : Real Time on/off");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"b : Distant bodies labels on/off");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"B : Stars labels on/off");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"i : Info (HUD) on/off");
        if (joyavailable) {
        	glRasterPos2i(i,j+=15);
                printstring(GLUT_BITMAP_HELVETICA_10,"j : Joystick control on/off");
        }
        glRasterPos2i(i,j+=25);
        printstring(GLUT_BITMAP_HELVETICA_10,"+/- : Increase/Decrease timefactor *");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"Arrow keys : Camera rotation *");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"Page Up/Down : Increase/decrease speed");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"z/Z : Increase/decrease zoom factor");
	glRasterPos2i(i,j+=15);
        printstring(GLUT_BITMAP_HELVETICA_10,"* hold SHIFT for faster operation");
   }
   if (!info) {
   	Reshape(width,height);
   	if (lighting) glEnable(GL_LIGHTING);
   	if (texture) glEnable(GL_TEXTURE_2D);
   	return;
   }
   glRasterPos2i(0,20);
   t=(int) ((10092.0+days)*24.0*3600.0);
   tm=localtime(&t);
   strftime(sbuf,80,"%m / %d / %Y  %X (Local Time)",tm);
   printstring(GLUT_BITMAP_HELVETICA_10,sbuf);
   if (cmode) {
           glRasterPos2i(0,30);
           sprintf(sbuf,"Distance from Sun (million Km): %.2f",DISTANCE(planets[currtarget].posx,
                           planets[currtarget].posy,planets[currtarget].posz)*DFACTOR);
           printstring(GLUT_BITMAP_HELVETICA_10,sbuf);
   }
   glRasterPos2i(0,height-5);
   sprintf(sbuf,"Camera distance from Sun (million Km): %.2f",DISTANCE(camx,camy,camz)*DFACTOR);
   printstring(GLUT_BITMAP_HELVETICA_10,sbuf);
   glRasterPos2i(0,height-15);
   sprintf(sbuf,"Time factor: %.2f minutes / iteration",timefactor*24.0*60.0);
   printstring(GLUT_BITMAP_HELVETICA_10,sbuf);
   glRasterPos2i(0,height-25);
   sprintf(sbuf,"Camera speed (Km / iteration): %.2f",speed*10E6*DFACTOR);
   printstring(GLUT_BITMAP_HELVETICA_10,sbuf);
   if (cmode>1) {
        glRasterPos2i(0,height-35);
        sprintf(sbuf,"Zoom factor: %.2f",10.0/zoom);
        printstring(GLUT_BITMAP_HELVETICA_10,sbuf);
   }
   glRasterPos2i(width-120,height-5);
   switch (cmode) {
   	case 0: printstring(GLUT_BITMAP_HELVETICA_10,"Free Camera");
   		glRasterPos2i(0,10);
   		sprintf(sbuf," ");
   		break;
   	case 1: printstring(GLUT_BITMAP_HELVETICA_10,"Linked Camera");
   		glRasterPos2i(0,10);
                sprintf(sbuf,"Tracking %s",planets[currtarget].Name);
   		break;
   	case 2:	printstring(GLUT_BITMAP_HELVETICA_10,"Body to Body Camera");
   		glRasterPos2i(0,10);
   		sprintf(sbuf,"%s as seen from %s",planets[currtarget].Name,planets[currsource].Name);
		break;	
   	case 3: printstring(GLUT_BITMAP_HELVETICA_10,"Orbiter Camera");
   		glRasterPos2i(0,10);
   		sprintf(sbuf,"Orbiting around %s",planets[currtarget].Name);
   		break;
   	default: break;
   }
   printstring(GLUT_BITMAP_HELVETICA_10,sbuf);
   sprintf(sbuf,"FOV: %.1f  FPS: %.1f",fov,fps);
   glRasterPos2i(width-120,height-15);
   printstring(GLUT_BITMAP_HELVETICA_10,sbuf);
   if (demomode) {
   	glRasterPos2i(width-120,height-25);
   	printstring(GLUT_BITMAP_HELVETICA_10,"Demo Mode");
   }
   if (paused) {
   	glRasterPos2i(width-30,10);
   	printstring(GLUT_BITMAP_HELVETICA_10,"Pause");
   }
   if (joyactive && joyavailable) {
   	glRasterPos2i(width-70,25);
   	printstring(GLUT_BITMAP_HELVETICA_10,"Joystick Active");
   }
   Reshape(width,height);
   if (lighting) glEnable(GL_LIGHTING);
   if (texture) glEnable(GL_TEXTURE_2D);
}


static void Idle( void )
{
   int sat;
   double rad,x,y,z,o,p;

   if (!init) {
	init=1;
	width=glutGet(GLUT_WINDOW_WIDTH);
	height=glutGet(GLUT_WINDOW_HEIGHT);
	Reshape(width,height); /* Make sure Reshape is called */      
	Init();
   	if (bench) days=1000.0;
   	UpdatePositions(days,0);
   	camx=planets[currtarget].posx/1.005; 
   	camy=planets[currtarget].posy; 
   	camz=planets[currtarget].posz/1.005;
   	while (planets[currtarget].Type>=RING)
   	 	currtarget=(currtarget+1)%NUMBODIES;
   	while (planets[currsource].Type>=RING)
   	 	currsource=(currsource+1)%NUMBODIES;
   	sec=glutGet(GLUT_ELAPSED_TIME);
   	glutDisplayFunc( Display );
   	glutKeyboardFunc( Key );
   	glutSpecialFunc( Special );
   	glutMouseFunc( Mouse );
   	glutMotionFunc( Motion );
	glutTimerFunc(10000,TimerDemo,0);
   }

   
   if (bench & (frames==1000)) {
   	 sec=(glutGet(GLUT_ELAPSED_TIME)-sec)/1000.0;
#ifdef WIN32
         sprintf(sbuf,"\n%.2f frames/sec\n\n",(float)frames/sec);
         MessageBox(NULL,sbuf,"Overall Performance", MB_OK);
	 if (fullscreen) glutLeaveGameMode();
         ShowCursor(TRUE);
#else
         printf("\n%.2f frames/sec\n\n",(float)frames/sec);
         glutSetCursor(GLUT_CURSOR_INHERIT);
#endif

         exit(0);
   }
   
   if (!paused) {
   	/* Update positions to Julian day "days" */
   	days+=timefactor;
   	UpdatePositions(days,0); /* 0 tells UpdatePositions to process all bodies */
   }

   if (!(frames%50))
   	fps=50.0/gettime();
   if (realtime)
	days=time(NULL)/3600.0/24.0-10092.0; /* days = NOW */

   switch (cmode) {
	case 2: /* Body to Body Camera, simply places the camera near the current
		   target with current source line view */
   		if (planets[currtarget].Type==SATELLITE) {
   			   /* If current target is a satellite take also into
   			   account host planet data */
			   sat=currtarget;
			   rad=RADIUSSCALE(planets[sat].Radius*zoom);
			   d=DISTANCE(planets[sat].posx-planets[currsource].posx,
			   	planets[sat].posy-planets[currsource].posy,
			   	planets[sat].posz-planets[currsource].posz);
			   if (rad>d) rad=d;
			   camx=(planets[sat].posx-planets[currsource].posx)/d*
			   	(d-rad)+planets[currsource].posx;
			   camy=(planets[sat].posy-planets[currsource].posy)/d*
			   	(d-rad)+planets[currsource].posy;
			   camz=(planets[sat].posz-planets[currsource].posz)/d*
			   	(d-rad)+planets[currsource].posz;
		} else {
		   	   rad=RADIUSSCALE(planets[currtarget].Radius*zoom);
			   d=DISTANCE(planets[currtarget].posx-planets[currsource].posx,
			   	planets[currtarget].posy-planets[currsource].posy,
			   	planets[currtarget].posz-planets[currsource].posz);
			   if (rad>d) rad=d;
			   camx=(planets[currtarget].posx-planets[currsource].posx)/d*
			   	(d-rad)+planets[currsource].posx;
			   camy=(planets[currtarget].posy-planets[currsource].posy)/d*
	   			(d-rad)+planets[currsource].posy;
			   camz=(planets[currtarget].posz-planets[currsource].posz)/d*
			   	(d-rad)+planets[currsource].posz;
		}

		upx=0.0;
		upy=cos(DEG2RAD(planets[currsource].Degrees));
		upz=-sin(DEG2RAD(planets[currsource].Degrees));
		break;

	case 3: /* Orbiter Camera */

		if (currtarget==0) break;
	
		/* Set Camera in equatorial orbit */
		x=RADIUSSCALE(planets[currtarget].Radius*zoom);
		y=0.0;
		z=0.0;
	
		/* Rotate camera to simulate orbit */
		Rotation(-days*10.0,0.0,1.0,0.0,&x,&y,&z);
	
		/* Rotate orbit plane to match planet tilt */
		o=cos(DEG2RAD(planets[currtarget].Degrees+planets[planets[currtarget].Sat].Degrees)
   			+planets[planets[currtarget].Sat].Inclination);
		p=sin(DEG2RAD(planets[currtarget].Degrees+planets[planets[currtarget].Sat].Degrees)
			+planets[planets[currtarget].Sat].Inclination);

		camx = planets[currtarget].posx+x;
		camy = planets[currtarget].posy+y*o+z*p;
		camz = planets[currtarget].posz+y*p+z*o;

		/* Do the same with up vector */
		x=0.0;
		y=1.0;
		z=0.0;

		upx = x;
		upy = y*o+z*p;
		upz = -y*p+z*o;
		break;

	default: break;
   }
   if (cmode) {
        /* Points camera to current active target */
	eyex=planets[currtarget].posx-camx;
 	eyey=planets[currtarget].posy-camy;
	eyez=planets[currtarget].posz-camz;
	d=DISTANCE(eyex,eyey,eyez);
	eyex/=d; eyey/=d; eyez/=d;
	if (d<RADIUSSCALE(planets[currtarget].Radius*1.5)) speed*=(double)demomode;
   } else if (joyactive && joyavailable) joystick();
   speed+=accel;
   accel/=1.5;
   if ((accel==0.0) && (fabs(speed)<0.00001)) speed=0.0;
   camx+=eyex*speed; camy+=eyey*speed; camz+=eyez*speed;
   if (init)
   	glutPostRedisplay();
}


/* Stand alone function for future enhancements */
static void Camera(void)
{
  gluLookAt(camx,camy,camz,
  	    camx+eyex,camy+eyey,camz+eyez, 
  	    upx, upy, upz);
}


static void Display( void )
{
   double a,c,dist;
   int i,j;

   glLoadIdentity();
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   Camera();
   if (drawstars) {
   	glDisable(GL_LIGHTING);
   	glPushMatrix();
   	glCallList(Stars);
   	for (i=1;i<=NUMBODIES;i++)
   		if ((planets[i].Type<=ASTEROID) && (planets[i].TrailEnabled))
			glCallList(planets[i].Trail);
   	glPopMatrix();
   	if (lighting) glEnable(GL_LIGHTING);
   	if (texture) glEnable(GL_TEXTURE_2D);
   }

   glEnable(GL_DEPTH_TEST);
   glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
   glMaterialfv(GL_FRONT, GL_AMBIENT, White);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, White);
   glMaterialfv(GL_FRONT, GL_EMISSION, White);
   glBindTexture(GL_TEXTURE_2D, planets[0].textures[0]);
   glPushMatrix();
   if (texture) SunBillBoard(); else glCallList(planets[0].Sphere);
   glPopMatrix();
   glMaterialfv(GL_FRONT, GL_EMISSION, Black);   
   for (i=1;i<NUMBODIES;i++) {

	if (!atmospheres)
		if (planets[i].Type==ATMOSPHERE) continue;

	if ((planets[i].Type==RING) || (planets[i].Type==ATMOSPHERE))
		j=planets[i].Sat;
	else
		j=i;

	/* Find angle between camera and body */
	a=(planets[j].posx-camx)*eyex+(planets[j].posy-camy)*eyey+(planets[j].posz-camz)*eyez;
	dist=DISTANCE(planets[j].posx-camx,planets[j].posy-camy,planets[j].posz-camz);
	c=a/(DISTANCE(eyex,eyey,eyez)*dist);
	/* An extra check for sick Microsoft compilers */
	c=acos(RANGE(c,-1.0,1.0))*180/PI;

	/* Is the angle big enough so that body is not visible? */
	if (c>(fov/2.0*aratio+180.0/PI*atan(RADIUSSCALE(planets[j].Radius)/dist))) {
		d=0.0; /* Yes, skip this body */
		glPopMatrix();
		continue;
	} else
		d=planets[j].Radius/fov/dist; /* No, but is it far? */

	glPushMatrix();

	/* Move body to its position in the orbit */
        glTranslatef(planets[j].posx,planets[j].posy,planets[j].posz);

	/* Adjust body tilt */
   	glRotatef(-planets[j].Degrees-planets[planets[j].Sat].Degrees-planets[planets[j].Sat].Inclination-90.0,1.0,0.0,0.0);

	/* Rotate body around its axis */
   	glRotatef(planets[j].DeltaRotation,0.0,0.0,1.0);

	if (planets[i].Type==ATMOSPHERE)
		glDisable(GL_DEPTH_TEST);

   	if (d>DISTCORRECTION(5.0)) { /* Body is visible and near the camera, use full detail */
   		if (!texture) {
   			glMaterialfv(GL_FRONT, GL_AMBIENT, planets[j].Color);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, planets[j].Color);
		} else {
   			glMaterialfv(GL_FRONT, GL_AMBIENT, White);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, White);
		}
   		glCallList(planets[i].Sphere);
   	} else if (d>DISTCORRECTION(0.1)) /* Body is visible but too far, use low detail instead */
   		if (planets[i].Type<=ASTEROID) { /* Skip rings and atmosphere */
   			if (texture) glDisable(GL_TEXTURE_2D);
   			glMaterialfv(GL_FRONT, GL_AMBIENT, planets[j].Color);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, planets[j].Color);
   			glCallList(planets[i].Sphere+1);
   			if (texture) glEnable(GL_TEXTURE_2D);
   		}
   	if (planets[i].Type==RING)
   		if (lighting) glEnable(GL_LIGHTING);
   	if (planets[i].Type==ATMOSPHERE)
   		glEnable(GL_DEPTH_TEST);

   	glPopMatrix();
   }
   OnScreenInfo(); /* Draw info stuff */
   glutSwapBuffers();
   frames++;
}

                                                    
#ifdef WIN32
int PASCAL
 WinMain(HANDLE hInst, HANDLE hPrevInst, LPSTR lpszCmndLine,int cmdShow )
#else
 int main(int argc, char *argv[])
#endif
{
#ifdef WIN32
   int argc=1;
   char *argv[1];
   char *s="ssystem.exe";

   argv[0]=s;
#endif   
   glutInitWindowSize(width,height);
   glutInit( &argc, argv);
#ifdef WIN32
   ParseCmdLineWIN32(lpszCmndLine);
#else
   ParseCmdLine(argc,argv);
#endif
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
   if (fullscreen) {
#ifdef WIN32
   	glutGameModeString(game_string);
        glutEnterGameMode();
        ShowCursor(FALSE);
#else
	glutCreateWindow( "ssystem 1.6" );
	glutFullScreen();
        glutSetCursor(GLUT_CURSOR_NONE);
#endif
   } else glutCreateWindow( "ssystem 1.6" );

   joydetect();
   demomode=!bench;
   speed=-(10.0/10E6/DFACTOR);

   glutIdleFunc( Idle );
   glutReshapeFunc( Reshape );
   glutMainLoop();
   return 0;
}
