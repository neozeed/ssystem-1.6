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

void Key( unsigned char key, int x, int y )
{
   double xx,yy,zz;
   
   switch (key) {
      case 27: /* ESC ... bye bye */
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
         break;
      case 'c': /* Toggles camera mode */
      		cmode=(cmode+1)%4;
      		switch (cmode) {
      			case 0:
      				upx=upz=0.0;
      				upy=1.0;
      				eyey=0.0;
      				break;
      			case 2: if (currtarget==currsource) {
      					currsource=(currsource+1)%NUMBODIES;
      					while (planets[currsource].Type==RING)
      						currsource=(currsource+1)%NUMBODIES;
      				}
      				break;
      			default: break;
      		}
      		break;
      case 'p': paused=!paused;
      		break;
      case 'f': smodel=(smodel==GL_FLAT) ? GL_SMOOTH : GL_FLAT;
      		glShadeModel(smodel);
      		break;
      case 'o': if (fov<179.5) fov+=0.5;
      		break;
      case 'O': if (fov>0.5) fov-=0.5;
      		break;
      case 't': texture=!texture;
      		break;
      case 'r': realtime=!realtime;
      		break;
      case 's': drawstars=!drawstars;
      		break;
      case 'S': sprintf(sbuf,"sshot%d.jpg",frames);
      		ScreenShot(sbuf);
      		break;
      case 'm': glDeleteLists(Stars,1);
      		star_mag+=0.1;
      		InitStars(star_mag,0);
      		break;
      case 'M': glDeleteLists(Stars,1);
      		star_mag-=0.1;
      		InitStars(star_mag,0);
      		break;
      case 'n': /* Set camera near the current target */
      		if (currtarget==0) break;
      		if (planets[currtarget].Type==SATELLITE) {
      			xx=planets[planets[currtarget].Sat].posx-planets[currtarget].posx;
      			yy=planets[planets[currtarget].Sat].posy-planets[currtarget].posy;
      			zz=planets[planets[currtarget].Sat].posz-planets[currtarget].posz;
			d=DISTANCE(xx,yy,zz);
      			camx=planets[planets[currtarget].Sat].posx-xx/d*(d+RADIUSSCALE(planets[currtarget].Radius*8.0));
                	camy=planets[planets[currtarget].Sat].posy-yy/d*(d+RADIUSSCALE(planets[currtarget].Radius*8.0));
      			camz=planets[planets[currtarget].Sat].posz-zz/d*(d+RADIUSSCALE(planets[currtarget].Radius*8.0));
                	eyex=planets[planets[currtarget].Sat].posx-camx;
                	eyey=planets[planets[currtarget].Sat].posy-camy;
                	eyez=planets[planets[currtarget].Sat].posz-camz;
                	d=DISTANCE(eyex,eyey,eyez);
                	eyex/=d; eyey/=d; eyez/=d;
      			xx=planets[currtarget].posx-planets[planets[currtarget].Sat].posx;
      			yy=planets[currtarget].posy-planets[planets[currtarget].Sat].posy;
      			zz=planets[currtarget].posz-planets[planets[currtarget].Sat].posz;
                	upx=0.0; 
                	upy=1.0; 
                	upz=-yy/zz;
      		}
      		else {
			d=DISTANCE(planets[currtarget].posx,
   				planets[currtarget].posy,planets[currtarget].posz);
      			camx=planets[currtarget].posx/d*
      				(d-RADIUSSCALE(planets[currtarget].Radius*8.0));
                	camy=planets[currtarget].posy;
                	camz=planets[currtarget].posz/d*
                		(d-RADIUSSCALE(planets[currtarget].Radius*8.0));
                	eyex=planets[currtarget].posx-camx;
                	eyey=planets[currtarget].posy-camy;
                	eyez=planets[currtarget].posz-camz;
                	upx=0.0; upy=1.0; upz=0.0;
                	d=DISTANCE(eyex,eyey,eyez);
                	eyex/=d; eyey/=d; eyez/=d;
                }
                break;
      case 'h': help=!help;
      		break;
      case 'd': demomode=!demomode;
      		break;
      case 'l': lighting=!lighting;
      		break;
      case 'b': plabels=!plabels;
      		break;
      case 'B': slabels=!slabels;
      		break;
      case 'i': info=!info;
                break;
      case 'j': joyactive=!joyactive;
                break;
      case '+': if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) timefactor*=1.1;
      		else timefactor+=1/(3600.0*24.0); /* one second/iteration */
		break;
      case '-': if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) timefactor/=1.1;
      		else timefactor-=1/(3600.0*24.0); /* one second/iteration */
		break;
      case 'z': if (cmode>1) if (zoom<100.0) zoom+=1.0;
      		break;
      case 'Z': if (cmode>1) if (zoom>5.0) zoom-=1.0;
      		break;
      case 'a': atmospheres=!atmospheres;
      		break;
    }
}

                                                    
void Special(int k, int x, int y)
{
  double rot,xx,yy,zz;
  int m;
  
  m=glutGetModifiers(); 
  if (m & GLUT_ACTIVE_SHIFT) rot=0.05;
  else rot=0.003;
  switch (k) {
      case 's': if (m & GLUT_ACTIVE_CTRL){
      			sprintf(sbuf,"frame%d.ppm",frames);
      			ScreenShot(sbuf);
      		}
      		break;
      case GLUT_KEY_INSERT: if (planets[currtarget].Type<=ASTEROID)
      				planets[currtarget].TrailEnabled=!planets[currtarget].TrailEnabled;
                break;
      case GLUT_KEY_LEFT:Rotation(-rot,upx,upy,upz,&eyex,&eyey,&eyez);
      		break;
      case GLUT_KEY_RIGHT: Rotation(rot,upx,upy,upz,&eyex,&eyey,&eyez);
      		break;
      case GLUT_KEY_DOWN: xx=upy*eyez-upz*eyey;
      			yy=-upx*eyez+upz*eyex;
      			zz=upx*eyey-upy*eyex;
      			Rotation(rot,xx,yy,zz,&upx,&upy,&upz);
      			Rotation(rot,xx,yy,zz,&eyex,&eyey,&eyez);
      		break;
      case GLUT_KEY_UP:   xx=upy*eyez-upz*eyey;
      			  yy=-upx*eyez+upz*eyex;
      			  zz=upx*eyey-upy*eyex;
      			  Rotation(-rot,xx,yy,zz,&upx,&upy,&upz);
      			  Rotation(-rot,xx,yy,zz,&eyex,&eyey,&eyez);
      		break;
      case GLUT_KEY_PAGE_UP:	accel+=10E-7/DFACTOR;
      		break;
      case GLUT_KEY_PAGE_DOWN:	accel-=10E-7/DFACTOR;
                break;
      case GLUT_KEY_HOME: if (m & GLUT_ACTIVE_SHIFT) {
      				currsource--;
      				while (planets[currsource].Type>=RING) currsource--;
      				if (currsource<0) {
      					currsource=NUMBODIES-1;
      					while (planets[currsource].Type>=RING) currsource--;
      				}
      				if (cmode==2)
      					if (currsource==currtarget) {
      						currsource--;
      						while (planets[currsource].Type>=RING)
      							currsource--;
      						if (currsource<0) {
      							currsource=NUMBODIES-1;
      							while (planets[currsource].Type>=RING) currsource--;
      						}
      					}
      			  } else {
      			  	currtarget--;
      				while (planets[currtarget].Type>=RING)
      					currtarget--;
      				if (currtarget<0) {
      					currtarget=NUMBODIES-1;
      					while (planets[currtarget].Type>=RING) currtarget--;
      				}
      				if (cmode==2)
      					if (currsource==currtarget) {
      						currtarget--;
      						while (planets[currtarget].Type>=RING)
      							currtarget--;
      						if (currtarget<0) {
      							currtarget=NUMBODIES-1;
      							while (planets[currtarget].Type>=RING) currtarget--;
      						}
      					}
      			  }
      			  break;
      case GLUT_KEY_END: if (m & GLUT_ACTIVE_SHIFT) {
      				currsource=(currsource+1)%NUMBODIES;
      				while (planets[currsource].Type>=RING)
      					currsource=(currsource+1)%NUMBODIES;
      				if (cmode==2)
      					if (currsource==currtarget) {
      						currsource=(currsource+1)%NUMBODIES;
      						while (planets[currsource].Type>=RING)
      							currsource=(currsource+1)%NUMBODIES;
      					}
      			  } else {
      			  	currtarget=(currtarget+1)%NUMBODIES;
      				while (planets[currtarget].Type>=RING)
      					currtarget=(currtarget+1)%NUMBODIES;
      				if (cmode==2)
      					if (currtarget==currsource) {
      						currtarget=(currtarget+1)%NUMBODIES;
      						while (planets[currtarget].Type>=RING)
      				      			currtarget=(currtarget+1)%NUMBODIES;
      				      	}
      			  }
      			  break;
  }
}
