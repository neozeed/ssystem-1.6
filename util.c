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

/* A couple of functions borrowed from David Bucciarelli's demos */
void printstring(void *font, char *string)
{
  int i=0;

  while (string[i])
    glutBitmapCharacter(font,string[i++]);
}

float gettime(void)
{
  static float told=0.0f;
  float tnew,ris;
    
  tnew=glutGet(GLUT_ELAPSED_TIME);
      
  ris=tnew-told;
        
  told=tnew;
          
  return ris/1000.0;
}

/* Rotates (rx,ry,rz) point about the (ax,ay,az) axis 'angle' radians,
  borrowed from Mesa */
void Rotation( double angle, double ax, double ay, double az,
                         double *rx, double *ry, double *rz)
{
   double mag, s, c;
   double x,y,z,xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

   s = sin(angle);
   c = cos(angle);

   mag = DISTANCE(ax,ay,az);

   if (mag == 0.0) return;

   x = ax/mag;
   y = ay/mag;
   z = az/mag;

   xx = x * x;
   yy = y * y;
   zz = z * z;
   xy = x * y;
   yz = y * z;
   zx = z * x;
   xs = x * s;
   ys = y * s;
   zs = z * s;
   one_c = 1.0 - c;

   x = *rx*((one_c * xx) + c);
   y = *rx*((one_c * xy) - zs);
   z = *rx*((one_c * zx) + ys);

   x += *ry*((one_c * xy) + zs);
   y += *ry*((one_c * yy) + c);
   z += *ry*((one_c * yz) - xs);

   x += *rz*((one_c * zx) - ys);
   y += *rz*((one_c * yz) + xs);
   z += *rz*((one_c * zz) + c);
   
   *rx=x; *ry=y; *rz=z;
}
