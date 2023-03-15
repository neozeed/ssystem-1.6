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

static int oldmousex,oldmousey;

void Mouse(int button, int state, int x, int y)
{
 if ((button==GLUT_LEFT_BUTTON) && (state==GLUT_DOWN)) {
	oldmousex=x;
	oldmousey=y;
 }
}

/* Moves camera with the mouse */
void Motion(int x, int y)
{
 static double xx,yy,zz;
      
 if (!cmode) {
        Rotation((x-oldmousex)/100.0,upx,upy,upz,&eyex,&eyey,&eyez);
	xx=upy*eyez-upz*eyey;
      	yy=-upx*eyez+upz*eyex;
      	zz=upx*eyey-upy*eyex;
      	Rotation((y-oldmousey)/100.0,xx,yy,zz,&eyex,&eyey,&eyez);
 }
 oldmousex=x;
 oldmousey=y;
}
