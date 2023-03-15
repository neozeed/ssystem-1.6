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
#ifdef LINUXJOY
#include <linux/joystick.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#endif
#ifdef WIN32
#include <mmsystem.h>
#endif

/*
  And another function based on David Bucciarelli's demos.
*/

int joyavailable=0,joyactive=0,joy_fd;
char axes=2;
char buttons=2;
int version=0x000800;
int *axis;
int *button;

void joystick(void)
{
#ifdef WIN32
  MMRESULT res;
  JOYINFO joy;
  static double xx,yy,zz;
  static int x,y;

  res=joyGetPos(JOYSTICKID1,&joy);

  if(res==JOYERR_NOERROR) {
    joyavailable=1;
    x=(joy.wXpos>>12)-8;
    y=(joy.wYpos>>12)-8;
    if ((x<2) && (x>-2)) x=0;
    if ((y<2) && (y>-2)) y=0;
    if(joyactive) {
      if (x)
      	Rotation(x*0.001,upx,upy,upz,&eyex,&eyey,&eyez);
      if (y) {
        xx=upy*eyez-upz*eyey;
        yy=-upx*eyez+upz*eyex;
        zz=upx*eyey-upy*eyex;
        Rotation(y*0.001,xx,yy,zz,&eyex,&eyey,&eyez);
      }
      if(joy.wButtons & JOY_BUTTON1)
        accel+=1.0E-8;
      if(joy.wButtons & JOY_BUTTON2)
        accel-=1.0E-8;
      if((joy.wButtons & JOY_BUTTON1) & (joy.wButtons & JOY_BUTTON2) ||
      	(joy.wButtons & JOY_BUTTON3) || (joy.wButtons & JOY_BUTTON4))
      	accel=speed=0.0;
    }
  } else
    joyavailable=0;
#endif

#ifdef LINUXJOY
 static struct js_event js;
 static int status,x,y;
 static double xx,yy,zz; 
 

 status=read(joy_fd, &js, sizeof(struct js_event));

 if (status>0) {
    switch (js.type & ~JS_EVENT_INIT) {
    	case JS_EVENT_BUTTON:
    			button[js.number]=js.value;
    			break;
    	case JS_EVENT_AXIS:
    			axis[js.number]=js.value;
    			break;
    }
    x=axis[0]/4096; /* 16 levels */
    y=axis[1]/4096;
 }
 
 if(joyactive) {
      if (x) Rotation(x*0.001,upx,upy,upz,&eyex,&eyey,&eyez);
      if (y) {
        xx=upy*eyez-upz*eyey;
        yy=-upx*eyez+upz*eyex;
        zz=upx*eyey-upy*eyex;
        Rotation(y*0.001,xx,yy,zz,&eyex,&eyey,&eyez);
      }
      if(button[0])
        accel+=1.0E-8;
      if(button[1])
        accel-=1.0E-8;
      if((button[0] && button[1]) || button[2] || button[3])
        accel=speed=0.0;
 }
#endif
}


void joydetect(void)
{
#ifdef WIN32

  MMRESULT res;
  JOYINFO joy;

  res=joyGetPos(JOYSTICKID1,&joy);
  if(res==JOYERR_NOERROR)
  	joyavailable=1;
  else
  	joyavailable=0;

#endif

#ifdef LINUXJOY

 if ((joy_fd=open("/dev/js0", O_RDONLY)) < 0)
 	joyavailable=0;
 else {
 	joyavailable=1;
        ioctl(joy_fd, JSIOCGVERSION, &version);
        ioctl(joy_fd, JSIOCGAXES, &axes);
        ioctl(joy_fd, JSIOCGBUTTONS, &buttons);
                                 	
 	printf("%d axes %d buttons joystick detected (%d.%d.%d driver version)\n",
 		axes,buttons,version >> 16, (version >> 8) & 0xff, version & 0xff);
 	
 	fcntl(joy_fd, F_SETFL, O_NONBLOCK);
 	axis = calloc(axes, sizeof(int));
 	button = calloc(buttons, sizeof(int));
 }
#endif

}