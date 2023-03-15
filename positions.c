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

	Calculate planets positions in a given date.
	
	The main algorithm uses elements dated Julian 2450680.5 (8/20/1997) so 
	it's quite accurate for dates near this.
	
	To be improved in the future ...
	
	Keith Burnett's planet positioning algorithm used with persmission.
	
*/

#include "ssystem.h"

static double SolveKepler(double, double);

static double SolveKepler(double M, double ec)
{
 static double E,d,delta;
 
 E=M;
 d=E-ec*sin(E)-M;
 while (fabs(d)>1.0E-8) {
	delta=d/(1.0L-ec*cos(E));
	E=E-delta;
	d=E-ec*sin(E)-M;
 }
 return E;
}


/* Based on Keith Burnett's QBASIC code found here:
	http://www.xylem.demon.co.uk/kepler/
*/
void UpdatePositions(double days, int num)
{
 static int j,k;
 static double e,M,E,r,v,o,p,i,x,y,z,l,b;

 if (!num) {
 	j=1;
 	k=NUMBODIES;
 } else {
 	j=num;
 	k=j+1;
 }
 for (j=1;j<k;j++) {
 	if (planets[j].Type>=RING) continue;
 	e=planets[j].Eccentricity;
 	M=planets[j].DailyMotion*days+planets[j].MeanLongitude-planets[j].Perihelion;
 	E=SolveKepler(M,e);
 	r=planets[j].MeanDistance*(1.0-e*cos(E));
 	v=2.0L*atan(sqrt((1.0L+e)/(1.0L-e))*tan(E/2.0L));
 	o=planets[j].AscendingNode;
 	p=planets[j].Perihelion;
 	i=planets[j].Inclination;
 	planets[j].posx=r*(cos(o)*cos(v+p-o)-sin(o)*sin(v+p-o)*cos(i));
 	planets[j].posz=-r*(sin(o)*cos(v+p-o)+cos(o)*sin(v+p-o)*cos(i));
 	planets[j].posy=r*(sin(v+p-o)*sin(i));
 	if (planets[j].CustomFunction) {
		switch(planets[j].CustomFunction) {
			case 1:MercuryPos(days+2450680.5, &l, &b, &r); break;
			case 2:VenusPos(days+2450680.5, &l, &b, &r); break;
			case 3:EarthPos(days+2450680.5, &l, &b, &r); break;
			case 4:MarsPos(days+2450680.5, &l, &b, &r); break;
			case 5:JupiterPos(days+2450680.5, &l, &b, &r); break;
			case 6:SaturnPos(days+2450680.5, &l, &b, &r); break;
			case 7:UranusPos(days+2450680.5, &l, &b, &r); break;
			case 8:NeptunePos(days+2450680.5, &l, &b, &r); break;
			case 9:PlutoPos(days+2450680.5, &l, &b, &r); break;
			case 10:MoonPos(days+2450680.5, &l, &b, &r); break;
		}
		r=DISTCORRECTION(r);
	        planets[j].posz=r*cos(b)*cos(l);
                planets[j].posx=r*cos(b)*sin(l);
                planets[j].posy=r*cos(b)*sin(b);
 	}
 	if (planets[j].Sat) {
 		/* satellites coords are expressed in host planet radii, convert 
 		 * to global coord ... similar to RADIUSSCALE macro */
 		planets[j].posx=planets[j].posx*planets[planets[j].Sat].Radius*0.0005;
 		planets[j].posy=planets[j].posy*planets[planets[j].Sat].Radius*0.0005;
 		planets[j].posz=planets[j].posz*planets[planets[j].Sat].Radius*0.0005;

		if (!planets[j].CustomFunction) {
			/* Rotate satellite to match host planet tilt + satellite orbit
			 * tilt. We could do this with glRotate when drawing objects
			 * but then we don't know real satellite coords so it's
			 * impossible to point the camera accurately */
			i=-atan2(planets[planets[j].Sat].posx,planets[planets[j].Sat].posz);
		 	o=cos(i);
		 	p=sin(i);
		 	x = planets[j].posx*o+planets[j].posz*p;
		     	y = planets[j].posy;
	     		z = -planets[j].posx*p+planets[j].posz*o;
		 	o=cos(DEG2RAD(planets[planets[j].Sat].Degrees)+planets[j].Inclination);
		 	p=sin(DEG2RAD(planets[planets[j].Sat].Degrees)+planets[j].Inclination);
	 		planets[j].posx = planets[planets[j].Sat].posx+x;
	     		planets[j].posy = planets[planets[j].Sat].posy+y*o+z*p;
	     		planets[j].posz = planets[planets[j].Sat].posz-y*p+z*o;
	 	} else { 
	 		/* Customs functions return always corrected values */
	 		planets[j].posx += planets[planets[j].Sat].posx;
	     		planets[j].posy += planets[planets[j].Sat].posy;
	     		planets[j].posz += planets[planets[j].Sat].posz;
	     	}
 	}
 	
 	/* Rotate body. Due to the rendering process (where every body is moved
 	 * from origin to its x,y,z position) it's neccesary for us to correct
 	 * rotation angle so we undo the implicit rotation in that translation */
	i=atan2(planets[j].posz,planets[j].posx)*180.0L/PIl;
	e=planets[j].Rotation*(days/360.252502L); /* convert earth days to local */
 	planets[j].DeltaRotation=(e-floor(e))*360.0L-i+85.0L;

 }
}

