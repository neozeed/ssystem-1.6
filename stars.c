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
#ifdef __WIN32__
#include <windows.h>
#endif
#include "ssystem.h"

float star_size=0.3;

void InitStars(float magcorr, int read_file)
{ 
   static char line[100],*tmp;
   FILE *f=NULL;
   static float ascention,declination,old_mag,magnitude;
   static double a,b,x[4],y[4],z[4];
   int i,j;

   if (read_file) {   
	   sprintf(line,"%s/stars.dat",texturepath);
	   f=fopen(line,"rt");
	   if (f==NULL) {
	   	error("Couldn't find star catalog file");
	   	exit(-1);
	   }
   }
   for (i=0;i<NUMSTARS;i++) {
   	if (read_file) {
		fgets(line,100,f);
		if (feof(f)) break;
		while (line[0]=='#') fgets(line,100,f);
		j=0;
		while (line[j]!=',') j++;
		line[j]='\0';
		tmp=line+j+1;
		strcpy(stars[i].Name,line);
		sscanf(tmp,"%f,%f,%f,%c\n",&ascention,&declination,
			&stars[i].magnitude,&stars[i].type);
			
		/* Translate declination/ascention coord into x,y,z */
		stars[i].posx=DISTCORRECTION(500.0)*cos(-ascention)*cos(declination);
		stars[i].posy=DISTCORRECTION(500.0)*sin(declination);
		stars[i].posz=DISTCORRECTION(500.0)*sin(-ascention)*cos(declination);
	}
	red=0;
	/* Print some stars in red color for accuracy testing */
	if (!strcmp(stars[i].Name,"Polaris")) { red=1; polaris=i; }
	if (!strcmp(stars[i].Name,"Dubhe")) red=1;
	if (!strcmp(stars[i].Name,"Merak")) red=1;
	if (!strcmp(stars[i].Name,"Phecda")) red=1;
	if (!strcmp(stars[i].Name,"Megrez")) red=1;
	if (!strcmp(stars[i].Name,"Alioth")) red=1;
	if (!strcmp(stars[i].Name,"Mizar")) red=1;


	/* Translate magnitude scale into 0-1 color scale.  This is a bit 
	  tricky and didn't find the right translation yet (it's very
	  monitor gamma dependant) */
/*
 *	stars[i].color=1.0/(1.46+stars[i].magnitude)+0.2; 
 *	stars[i].color=pow(1.0/(stars[i].magnitude+2.46),0.5);
 *	stars[i].color=0.8/log(1.46+stars[i].magnitude);
 */
	magnitude=magcorr*2.42/((2.46+stars[i].magnitude)*2.42);
	if (magnitude>1.0) magnitude=1.0;
	
	/* Only 20 magnitudes levels to speed up rendering process */
	magnitude=(int)ceil(magnitude*20.0)/20.0;
	switch (stars[i].type) {
		case 'O':stars[i].color[0]=0.8;
			 stars[i].color[1]=0.8;
			 stars[i].color[2]=1.0;
			 break;
		case 'B':stars[i].color[0]=0.9;
			 stars[i].color[1]=0.9;
			 stars[i].color[2]=1.0;
			 break;
		case 'A':stars[i].color[0]=1.0;
			 stars[i].color[1]=1.0;
			 stars[i].color[2]=1.0;
			 break;
		case 'F':stars[i].color[0]=1.0;
			 stars[i].color[1]=1.0;
			 stars[i].color[2]=0.8;
			 break;
		case 'G':stars[i].color[0]=1.0;
			 stars[i].color[1]=1.0;
			 stars[i].color[2]=0.7;
			 break;
		case 'K':stars[i].color[0]=1.0;
			 stars[i].color[1]=0.9;
			 stars[i].color[2]=0.8;
			 break;
		case 'M':
		case 'C':
		case 'S':stars[i].color[0]=1.0;
			 stars[i].color[1]=0.6;
			 stars[i].color[2]=0.6;
			 break;
	}
	stars[i].color[0]*=magnitude;
	stars[i].color[1]*=magnitude;
	stars[i].color[2]*=magnitude;
   }
   if (read_file) fclose(f);


   old_mag=100.0;
   i=0;
   Stars = glGenLists(1);
   glNewList( Stars, GL_COMPILE );
   glEnable(GL_BLEND);
   while (stars[i].magnitude<magcorr) {
   	glPushMatrix();
   	glTranslatef(stars[i].posx,stars[i].posy,stars[i].posz);
	a=atan2(stars[i].posz,stars[i].posx)-PI/2.0;
 	b=PI+atan2(stars[i].posy,sqrt(stars[i].posx*stars[i].posx+stars[i].posz*stars[i].posz));
 	x[0]=x[3]=y[0]=y[1]=-(star_size+0.1/(stars[i].magnitude+2.4));
  	x[1]=x[2]=y[2]=y[3]=star_size+0.1/(stars[i].magnitude+2.4);
   	z[0]=z[1]=z[2]=z[3]=0.0;
	for (j=0;j<4;j++) {
        	Rotation(b,1.0,0.0,0.0,&x[j],&y[j],&z[j]);
                Rotation(a,0.0,1.0,0.0,&x[j],&y[j],&z[j]);
        }
        glColor3f(stars[i].color[0],stars[i].color[1],stars[i].color[2]);
        glBindTexture(GL_TEXTURE_2D, planets[0].textures[0]);
   	glBegin(GL_QUADS);
 	glTexCoord2f(0.0,0.0);  glVertex3f(x[0],y[0],z[0]);
  	glTexCoord2f(0.0,1.0);  glVertex3f(x[1],y[1],z[1]);
   	glTexCoord2f(1.0,1.0);  glVertex3f(x[2],y[2],z[2]);
    	glTexCoord2f(1.0,0.0);  glVertex3f(x[3],y[3],z[3]);
        glEnd();
   	glPopMatrix();
   	i++;
   }
   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);
   glBegin(GL_POINTS);
   while (i<NUMSTARS) {
	glColor3f(stars[i].color[0],stars[i].color[1],stars[i].color[2]);
	glVertex3f(stars[i].posx,stars[i].posy,stars[i].posz);
	i++;
   }
   glEnd();
   glEndList();
}

