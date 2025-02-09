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

/*
  Initialization routines:
  
  	- Read texture images
  	- Initialize planets' data
  	- Create display lists
  	- Other initialization routines
*/
#ifdef __WIN32__
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef __FreeBSD__
#include <malloc.h>
#endif
#include <time.h>
#include "jpeglib.h"
#include <setjmp.h>
#include "ssystem.h"

//void glBindTexture (GLenum target, GLuint texture){}

GLfloat LightPos[4] = {0.0, 0.0, 0.0, 1.0};
GLfloat ambient[4] = {0.2, 0.2, 0.2, 0.0};
GLfloat White[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat Black[4] = {0.0, 0.0, 0.0, 1.0};
int texture=1,smodel=GL_SMOOTH,lighting=1,drawstars=1;
int Stars,red,polaris;
int ImgWidth, ImgHeight, components;
int width=640, height=480; /* default WIDTH and HEIGHT */
planetdata planets[MAXBODIES];
stardata stars[NUMSTARS];
double days,timefactor=1/(24.0*60.0*2.0); /* iterarion = 30 seconds */
int slices=16,stacks=16,NUMBODIES,border=0,filter=GL_LINEAR;
char texturepath[100]=SDATADIR;
unsigned char tmpimg[258*258*4],*open_image;
int openimgwidth=0,openimgheight=0,openbitmap=1;
extern float star_mag;

static void SetUpTrails(void);
static void Spheroid(double, float, float, float, float, int, int, planetdata *, int);
static void Print(char *);
GLubyte *read_JPEG_file (char *, int *, int *, int *);

void Init( void )
{
   time_t t;
   

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   t=time(NULL);
   srand(t);
   days=t/3600.0/24.0-10092.0;
   ReadConfigFile();
   SetUpTrails();
   Print("Setting up Stars");
   InitStars(star_mag,1);
   glShadeModel(smodel);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
   glEnable(GL_TEXTURE_2D);
   glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   if (openbitmap)
        free(open_image);
}

/* A quick and dirty Print funtion for the opening screen
 *
 * I know it's not the best way to do this but it will only have impact in the
 * loading time ;-)
 */
static void Print(char *s)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0,(float) width,(float) height,0.0,0.0,1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   if (openbitmap) {
   	   if (!openimgwidth)
           	open_image=read_JPEG_file("splash.jpg",&openimgwidth,&openimgheight, &components);
           glPixelZoom((float)width/(float)openimgwidth,(float)height/(float)openimgheight);
           glRasterPos2i(0,height-1);
           glDrawPixels(openimgwidth,openimgheight,GL_RGB,GL_UNSIGNED_BYTE,open_image);
   } else
        glClear( GL_COLOR_BUFFER_BIT);
   glColor3f(1.0,1.0,1.0);
   glRasterPos2i(width/2-strlen(s)*4,height/2);
   printstring(GLUT_BITMAP_8_BY_13,s);
   glutSwapBuffers();
}

/*  Spheroid drawing routine based on gluSphere Mesa function (src-glu/quadric.c)
 *  patched to allow multiple texture objects and flattening.
 */
static void Spheroid(double radius, float xflat, float yflat, float zflat, float randomness, int slices, int stacks, planetdata *pdata, int textured)
{
   static GLfloat rho, drho, theta, dtheta;
   GLfloat x, y, z;
   GLfloat s, t, ds, dt;
   GLint i, j, k, imin, imax,tex;
   double *rarray1,*rarray2,*tmp;

 
   drho = PI / (GLfloat) stacks;
   dtheta = 2.0 * PI / (GLfloat) slices;

/* Allocate memory for surface randomness arrays */
   rarray1=malloc((slices+1)*3*sizeof(double));
   rarray2=malloc((slices+1)*3*sizeof(double));

   if (textured) {

      if (stacks%pdata->texheight) {
      	error("Incorrect STACKS value, edit configuration file\n");
      	exit(-1);
      }

      if (slices%pdata->texwidth) {
      	error("Incorrect SLICES value, edit configuration file\n");
      	exit(-1);
      }

      ds = 1.0 / slices * pdata->texwidth;
      dt = 1.0 / stacks * pdata->texheight;
      t = 1.0;
        imin = 0;
        imax = stacks;
    

      /* Initialize surface randomness arrays */
      for (i=0;i<(slices+1)*3;i++) {
      	rarray1[i]=(1.0-randomness)+(randomness*2.0*rand())/RAND_MAX;
      	rarray2[i]=(1.0-randomness)+(randomness*2.0*rand())/RAND_MAX;
      }
      
      for (i=imin;i<imax;i++) {
	 rho = i * drho;
	 if (!(i%(imax/pdata->texheight))) t=1.0;
	 tmp=rarray1;
	 rarray1=rarray2;
	 rarray2=tmp;
	 for (k=0;k<(slices+1)*3;k++)
      	 	rarray2[k]=(1.0-randomness)+(randomness*2.0*rand())/RAND_MAX;

	 for (k=0;k<pdata->texwidth;k++) {
	     /* Find the texture map we should apply in this iteration */
	     tex=imax/pdata->texheight;
	     tex=(pdata->texwidth*(pdata->texheight-1)-i/tex*pdata->texwidth)+k;
	     glBindTexture(GL_TEXTURE_2D, pdata->textures[tex]);
	     glBegin( GL_QUAD_STRIP );
             s = 0.0;
             /* overwrite last element with first element data */
             rarray1[(k+1)*slices/pdata->texwidth*3]=rarray1[k*slices/pdata->texwidth*3];
             rarray1[(k+1)*slices/pdata->texwidth*3+1]=rarray1[k*slices/pdata->texwidth*3+1];
             rarray1[(k+1)*slices/pdata->texwidth*3+2]=rarray1[k*slices/pdata->texwidth*3+2];
             rarray2[(k+1)*slices/pdata->texwidth*3]=rarray2[k*slices/pdata->texwidth*3];
             rarray2[(k+1)*slices/pdata->texwidth*3+1]=rarray2[k*slices/pdata->texwidth*3+1];
             rarray2[(k+1)*slices/pdata->texwidth*3+2]=rarray2[k*slices/pdata->texwidth*3+2];
	     for (j=(k*slices/pdata->texwidth);j<=((k+1)*slices/pdata->texwidth);j++) {
		theta = (j==slices) ? 0.0 : j * dtheta;
		x = -sin(theta) * sin(rho) * rarray1[j*3];
		y = cos(theta) * sin(rho) * rarray1[j*3+1];
		z = cos(rho) * rarray1[j*3+2];
		glNormal3f( x, y, z );
		glTexCoord2f(s,t);
		glVertex3f( x*radius*(1.0-xflat), y*radius*(1.0-yflat), z*radius*(1.0-zflat));
		x = -sin(theta) * sin(rho+drho) * rarray2[j*3];
		y = cos(theta) * sin(rho+drho) * rarray2[j*3+1];
		z = cos(rho+drho) * rarray2[j*3+2];
		glNormal3f( x, y, z );
		glTexCoord2f(s,t-dt);
		s += ds;
		glVertex3f( x*radius*(1.0-xflat), y*radius*(1.0-yflat), z*radius*(1.0-zflat));
	     }
	     glEnd();
	 }
	 t -= dt;
      }
   } else {
      ds=1.0/slices;
      dt=1.0/stacks;
      imin=0;
      imax=stacks;
      for (i=imin;i<imax;i++) {
         rho = i * drho;
         glBegin( GL_QUAD_STRIP );
         for (j=0;j<=slices;j++) {
            theta = (j==slices) ? 0.0 : j * dtheta;
            x = -sin(theta) * sin(rho);
            y = cos(theta) * sin(rho);
            z = cos(rho);
            glNormal3f( x, y, z );
            glVertex3f( x*radius, y*radius, z*radius );
            x = -sin(theta) * sin(rho+drho);
            y = cos(theta) * sin(rho+drho);
            z = cos(rho+drho);
            glNormal3f( x, y, z );
            glVertex3f( x*radius, y*radius, z*radius );
         }
         glEnd();
      }
   }
   free(rarray1);
   free(rarray2);
}





/* Read texture, split large textures into 256x256 pieces and set up
   display lists */
void SetUpBody(int body, char *texfile, float randomness)
{
   int width=256,height=256,x1,x2,x3,y,i,j,k;

   sprintf(sbuf,"Setting up body: %s",planets[body].Name);
   Print(sbuf);
   
   planets[body].Image = read_JPEG_file(texfile,&ImgWidth,&ImgHeight, &components);
   if (!planets[body].Image) {
      sprintf((char *)tmpimg,"Couldn't read image %s/%s\n",texturepath,texfile);
      error((char *)tmpimg);
      exit(-1);
   }

   if (ImgWidth<width) { planets[body].texwidth=1; width=ImgWidth; }
   else planets[body].texwidth=ImgWidth/width;
   if (ImgHeight<height) { planets[body].texheight=1; height=ImgHeight; }
   else planets[body].texheight=ImgHeight/height;   
   planets[body].texnum=planets[body].texwidth*planets[body].texheight;
   planets[body].textures=malloc(planets[body].texnum*sizeof(GLuint));
   if (planets[body].textures==NULL) {
   	error("Coundn't allocate memory");
   	exit(-1);
   }
   glGenTextures(planets[body].texnum,planets[body].textures);

   for (j=0;j<planets[body].texheight;j++)
       for (i=0;i<planets[body].texwidth;i++) {
           glBindTexture(GL_TEXTURE_2D,planets[body].textures[planets[body].texwidth*j+i]);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	   if (border) {
		/* Copy pixel data to texture buffer and set up borders */
		x1=i*width;
		x2=(i*width+ImgWidth-1)%ImgWidth;
		x3=((i+1)*width)%ImgWidth;
	   	for (k=0;k<(height+2);k++) {
	   	       y=(j*height+k+ImgHeight-1)%ImgHeight;
		       memcpy(tmpimg+k*(width+2)*components+components,planets[body].Image+y*ImgWidth*components+x1*components,width*components);
		       memcpy(tmpimg+k*(width+2)*components,planets[body].Image+y*ImgWidth*components+x2*components,components);
		       memcpy(tmpimg+k*(width+2)*components+(width+1)*components,planets[body].Image+y*ImgWidth*components+x3*components,components);
		}
	        glTexImage2D( GL_TEXTURE_2D,
	                 0,
	                 components,
	                 width+2,height+2,
	                 1,
	                 components==3?GL_RGB:GL_LUMINANCE, GL_UNSIGNED_BYTE,
	                 tmpimg);
           } else {
		for (k=0;k<height;k++)
		       memcpy(tmpimg+k*width*components,planets[body].Image+ImgWidth*width*components*j+width*components*i+ImgWidth*components*k,width*components);
	        glTexImage2D( GL_TEXTURE_2D,
	                 0,
	                 components,
	                 width,height,
	                 0,
	                 components==3?GL_RGB:GL_LUMINANCE, GL_UNSIGNED_BYTE,
	                 tmpimg);
	   }
   }
   free(planets[body].Image);
   planets[body].Sphere= glGenLists(2);
   glNewList( planets[body].Sphere, GL_COMPILE );
   Spheroid(RADIUSSCALE(planets[body].Radius), planets[body].xflat, planets[body].yflat, planets[body].zflat, randomness, slices, stacks, &planets[body], 1);
   glEndList();
   glNewList( planets[body].Sphere+1, GL_COMPILE );
   Spheroid(RADIUSSCALE(planets[body].Radius), planets[body].xflat, planets[body].yflat, planets[body].zflat, randomness, 4, 4, &planets[body], 0);
   glEndList();
}


void SetUpRings(int body, char *texfile,float inner_radius, float outter_radius)
{
   unsigned char *localimage;
   int i,j;

   sprintf(sbuf,"Setting up rings: %s",planets[planets[body].Sat].Name);
   Print(sbuf);

   
   planets[body].Image = read_JPEG_file(texfile,&ImgWidth,&ImgHeight,&components);
   if (!planets[body].Image) {
      sprintf((char *)tmpimg,"Couldn't read image %s/%s\n",texturepath,texfile);
      error((char *)tmpimg);
      exit(-1);
   }
   
   localimage=malloc(256*256*4);
   if (localimage==NULL) {
   	error("Coundn't allocate memory");
   	exit(-1);
   }
   
   for (i=0;i<(256*256);i++) {
	localimage[i*4]=planets[body].Image[i*3];
	localimage[i*4+1]=planets[body].Image[i*3+1];
	localimage[i*4+2]=planets[body].Image[i*3+2];
	j=(planets[body].Image[i*3]+planets[body].Image[i*3+1]+planets[body].Image[i*3+2])/3;
	localimage[i*4+3]=j;
   }
   planets[body].texnum=1;
   planets[body].textures=malloc(planets[body].texnum*sizeof(GLuint));
   if (planets[body].textures==NULL) {
   	error("Coundn't allocate memory");
   	exit(-1);
   }
   glGenTextures(planets[body].texnum,planets[body].textures);
   glBindTexture(GL_TEXTURE_2D, planets[body].textures[0]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glTexImage2D( GL_TEXTURE_2D,
                 0,
                 4,
                 ImgWidth,ImgHeight,
                 0,
                 GL_RGBA, GL_UNSIGNED_BYTE,
                 localimage );
   free(planets[body].Image);
   free(localimage);



   planets[body].Object = gluNewQuadric();
   gluQuadricTexture( planets[body].Object, GL_TRUE );   
   planets[body].Sphere= glGenLists(1);
   glNewList( planets[body].Sphere, GL_COMPILE );
   glBindTexture(GL_TEXTURE_2D, planets[body].textures[0]);
   glDisable(GL_CULL_FACE);
   glEnable(GL_BLEND);
   gluDisk(planets[body].Object,
           RADIUSSCALE(inner_radius),
           RADIUSSCALE(outter_radius),
           slices*2,1);
   glDisable(GL_BLEND);        
   glEnable(GL_CULL_FACE);
   glEndList();
}


void SetUpAtmosphere(int body, char *texfile, float level, int cut)
{
   int width=256,height=256,x1,x2,x3,y,i,j,k;
   unsigned char localimage[258*258*4];

   sprintf(sbuf,"Setting up atmosphere: %s",planets[planets[body].Sat].Name);
   Print(sbuf);

   planets[body].Image = read_JPEG_file(texfile,&ImgWidth,&ImgHeight, &components);
   if (!planets[body].Image) {
      sprintf((char *)tmpimg,"Couldn't read image %s/%s\n",texturepath,texfile);
      error((char *)tmpimg);
      exit(-1);
   }

   if (ImgWidth<width) { planets[body].texwidth=1; width=ImgWidth; }
   else planets[body].texwidth=ImgWidth/width;
   if (ImgHeight<height) { planets[body].texheight=1; height=ImgHeight; }
   else planets[body].texheight=ImgHeight/height;   
   planets[body].texnum=planets[body].texwidth*planets[body].texheight;
   planets[body].textures=malloc(planets[body].texnum*sizeof(GLuint));
   if (planets[body].textures==NULL) {
   	error("Coundn't allocate memory");
   	exit(-1);
   }
   glGenTextures(planets[body].texnum,planets[body].textures);

   for (j=0;j<planets[body].texheight;j++)
       for (i=0;i<planets[body].texwidth;i++) {
           glBindTexture(GL_TEXTURE_2D,planets[body].textures[planets[body].texwidth*j+i]);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	   if (border) {
		/* Copy pixel data to texture buffer and set up borders */
		x1=i*width;
		x2=(i*width+ImgWidth-1)%ImgWidth;
		x3=((i+1)*width)%ImgWidth;
	   	for (k=0;k<(height+2);k++) {
	   	       y=(j*height+k+ImgHeight-1)%ImgHeight;
		       memcpy(tmpimg+k*(width+2)*components+components,planets[body].Image+y*ImgWidth*components+x1*components,width*components);
		       memcpy(tmpimg+k*(width+2)*components,planets[body].Image+y*ImgWidth*components+x2*components,components);
		       memcpy(tmpimg+k*(width+2)*components+(width+1)*components,planets[body].Image+y*ImgWidth*components+x3*components,components);
		}

		for (k=0;k<(258*258);k++) {
			localimage[k*(components+1)]=tmpimg[k*components];
			localimage[k*(components+1)+1]=tmpimg[k*components+1];
			localimage[k*(components+1)+2]=tmpimg[k*components+2];
			if (components==3)
				y=(tmpimg[k*3]+tmpimg[k*3+1]+tmpimg[k*3+2])/3;
			else
				y=tmpimg[k];
			if (y<cut) y=0;
			localimage[k*(components+1)+components]=(unsigned char) ((float)y/level);
		}

	        glTexImage2D( GL_TEXTURE_2D,
	                 0,
	                 components+1,
	                 width+2,height+2,
	                 1,
	                 components==3?GL_RGBA:GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,
	                 localimage);

           } else {
		for (k=0;k<height;k++)
		       memcpy(tmpimg+k*width*components,planets[body].Image+ImgWidth*width*components*j+width*components*i+ImgWidth*components*k,width*components);
   		
		for (k=0;k<(256*256);k++) {
			localimage[k*(components+1)]=tmpimg[k*components];
			localimage[k*(components+1)+1]=tmpimg[k*components+1];
			localimage[k*(components+1)+2]=tmpimg[k*components+2];
			if (components==3)
				y=(tmpimg[k*3]+tmpimg[k*3+1]+tmpimg[k*3+2])/3;
			else
				y=tmpimg[k];
			if (y<cut) y=0;
			localimage[k*(components+1)+components]=(unsigned char) ((float)y/level);
		}

	        glTexImage2D( GL_TEXTURE_2D,
	                 0,
	                 components+1,
	                 width,height,
	                 0,
	                 components==3?GL_RGBA:GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,
	                 localimage);
	   }
   }
   free(planets[body].Image);
   planets[body].Sphere= glGenLists(2);
   glNewList( planets[body].Sphere, GL_COMPILE );
   glEnable(GL_BLEND);
   Spheroid(RADIUSSCALE(planets[body].Radius), planets[planets[body].Sat].xflat, planets[planets[body].Sat].yflat, planets[planets[body].Sat].zflat, 0.0, slices, stacks, &planets[body], 1);
   glDisable(GL_BLEND);
   glEndList();
}



static void SetUpTrails(void)
{
 int i,j;
 double d;

 for (i=1;i<NUMBODIES;i++) {	
 	 if (planets[i].Type>ASTEROID) continue;
	 sprintf(sbuf,"Setting up trails: %s",planets[i].Name);
   	 Print(sbuf);
	 planets[i].Trail=glGenLists(1);
	 glNewList(planets[i].Trail, GL_COMPILE );
	 glColor3f(planets[i].Color[0],planets[i].Color[1],planets[i].Color[2]);
	 glBegin(GL_LINE_LOOP);
	 d=0.0;
	 for (j=0;j<1000;j++) {
	 	UpdatePositions(d,i);
	 	glVertex3f(planets[i].posx,planets[i].posy,planets[i].posz);
	 	d+=planets[i].OrbitalPeriod/1000.0;
	 }
	 glEnd();
	 glEndList();
 }
}



void InitSun(char *filename, double blend, int cutlevel)
{ 
   static int i;
   static double j;
   static char *localimage;

   Print("Setting up The Sun");

   
   planets[0].Radius=2.0; /*Real Radius is 109.6*/
   planets[0].Degrees=0.0;
   planets[0].DeltaRotation=0.0;
   planets[0].Rotation=360.0/28.0;
   planets[0].Inclination=0.0;
   planets[0].Sat=0;
   planets[0].posx=planets[0].posy=planets[0].posz=0.0;
   sprintf(planets[0].Name,"Sun");

   planets[0].Image = read_JPEG_file(filename,&ImgWidth,&ImgHeight,&components);
   if (!planets[0].Image) {
      sprintf((char *)tmpimg,"Couldn't read image %s/sun.jpg\n",texturepath);
      error((char *)tmpimg);
      exit(-1);
   }

   localimage=malloc(128*128*4);
   if (localimage==NULL) {
   	error("Coundn't allocate memory");
   	exit(-1);
   }

   for (i=0;i<(128*128);i++) {
	localimage[i*4]=planets[0].Image[i*3];
	localimage[i*4+1]=planets[0].Image[i*3+1];
	localimage[i*4+2]=planets[0].Image[i*3+2];
	j=(planets[0].Image[i*3]+planets[0].Image[i*3+1]+planets[0].Image[i*3+2])*blend/3.0;
	if (j<(double)cutlevel) j=0.0;
	localimage[i*4+3]=j>255.0?255:(int)j;
   }

   planets[0].textures=malloc(sizeof(GLuint));
   if (planets[0].textures==NULL) {
   	error("Coundn't allocate memory");
   	exit(-1);
   }

   glGenTextures(1,planets[0].textures);
   glBindTexture(GL_TEXTURE_2D, planets[0].textures[0]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);                 
   glTexImage2D( GL_TEXTURE_2D,
                 0,
                 4,
                 ImgWidth,ImgHeight,
                 0,
                 GL_RGBA, GL_UNSIGNED_BYTE,
                 localimage );
   free(planets[0].Image);
   free(localimage);
} 



