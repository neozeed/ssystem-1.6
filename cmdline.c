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

#ifdef WIN32

/* Why the hell doesn't the evil empire use the argc,argv combo? */
void ParseCmdLineWIN32(char *s)
{
 int err=0;
 char *tmp;
 
 tmp=strtok(s," ");
 while ((tmp!=NULL) && (!err)) {
 	err=0;
        if (tmp[0]=='-') {
                if (!strcmp(tmp,"-bench")) {
                        bench=1;
                        tmp=strtok(NULL," ");
                        continue;
                }
                if (!strcmp(tmp,"-fullscreen")) {
                        fullscreen=1;
                        tmp=strtok(NULL," ");
                        if (tmp==NULL) continue;
			if (tmp[0]!='-') {
				strcpy(game_string,tmp);
                                tmp=strtok(NULL," ");
                        };
			continue;
                }
                err=1;
        }
 };
 if (err) {
        error("ssystem [-bench] [-fullscreen [mode_string]]");
	exit(-1);
 }

}

#else

void ParseCmdLine(int n, char **s)
{
 int i=1,err=0;
 
 while ((i<n) && (!err)) {
 	err=0;
	if (!strcmp(s[i],"-bench")) {
		bench=1;
		i++;
		continue;
	}
	if (!strcmp(s[i],"-fullscreen")) {
		fullscreen=1;
		i++;
		continue;
	}
	if (!strcmp(s[i],"-h"))
		err=1;
	if (!strcmp(s[i],"-?"))
		err=1;
	if (!strcmp(s[i],"--help"))
		err=1;
 };
 if (err) {
	error("Valid command line options:\n");
        error("\n ssystem [-bench] [-fullscreen] \n\n");
	exit(-1);
 }
}

#endif
