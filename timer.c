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


void TimerDemo(int i)
{

 if (demomode && cmode) {
 	cmode=(rand()%3)+1;
 	do {
 		currtarget=rand()%NUMBODIES;
 		currsource=rand()%NUMBODIES;
 	} while ((currsource==currtarget) || (planets[currsource].Type>=RING)
 		|| (planets[currtarget].Type>=RING));
        zoom=rand()%50+5.0;
        if (cmode==1) { /* Linked camera */
        	Key('n',0,0);
        }
 }
 glutTimerFunc(10000,TimerDemo,0);
}
