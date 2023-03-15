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

#include "ssystem.h"

void SunBillBoard( void )
{
 double x[4],y[4],z[4];
 double alfa,beta;
 int i;
 
#define SIZE RADIUSSCALE(109.0)
 
 x[0]=x[3]=y[0]=y[1]=SIZE;
 x[1]=x[2]=y[2]=y[3]=-SIZE;
 z[0]=z[1]=z[2]=z[3]=0.0;

/* BILLBOARDING effect, rotates sun tile so it shows always the same face and
   angle to camera */
 alfa=atan2(camz,camx)-PI/2.0;
 beta=atan2(camy,sqrt(camx*camx+camz*camz));
 for (i=0;i<4;i++) {
	Rotation(beta,1.0,0.0,0.0,&x[i],&y[i],&z[i]);
 	Rotation(alfa,0.0,1.0,0.0,&x[i],&y[i],&z[i]);
 } 	
 
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 glBegin(GL_QUADS);
 glTexCoord2f(0.0,0.0);  glVertex3f(x[0],y[0],z[0]);
 glTexCoord2f(0.0,1.0);  glVertex3f(x[1],y[1],z[1]); 
 glTexCoord2f(1.0,1.0);  glVertex3f(x[2],y[2],z[2]); 
 glTexCoord2f(1.0,0.0);  glVertex3f(x[3],y[3],z[3]); 
 glEnd();
 glDisable(GL_BLEND);
}
