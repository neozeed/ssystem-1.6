%{
#include <math.h>
#include "ssystem.h"

int reserverd_word(char *s);
extern int yylineno;
#define YY_NO_UNPUT
%}

%option noyywrap

DIGIT    [0-9]
TEXT     [a-z_\-:/0-9.]*

%%



data_directory		return DATA_DIR;
border			return BORDER;
atmospheres		return ATMOSPHERES;
openbitmap              return OPENBITMAP;
filter			return FILTER;
shading			return SHADING;
date			return DATE;
pause			return PAUSE;
realtime		return REALTIME;
demo			return DEMO;
planets_labels		return PLANETS_LABELS;
stars_labels		return STARS_LABELS;
info			return INFO;
star_brightness_factor	return STAR_BRIGHT;
star_size		return STAR_SIZE;
camera_speed		return CAM_SPEED;
camera_mode		return CAM_MODE;
target			return TARGET;
source			return SOURCE;
slices			return SLICES;
stacks			return STACKS;
jpeg_quality		return JPEG_QUALITY;
sun			return SUN;
planet			return PLANET;
satellite		return SATELLITE;
asteroid		return ASTEROID;
atmosphere		return ATMOSPHERE;
ring			return RING;


{DIGIT}+    {	cfglval.ival=atoi(yytext); return INTEGER; }

{DIGIT}+"."{DIGIT}* { cfglval.dval=atof(yytext); return DOUBLE; }

{TEXT}		    { strcpy(cfglval.name,yytext); return NAME; }


"\#"(.)*"\n"	yylineno++;
"/*"		{
                              register int c;

                              for ( ; ; )
                                  {
                                  while ( (c = input()) != '*' &&
                                          c != EOF )
                                      ;    /* eat up text of comment */

                                  if ( c == '*' )
                                      {
                                      while ( (c = input()) == '*' )
                                          ;
                                      if ( c == '/' )
                                          break;    /* found the end */
                                      }

                                  if ( c == EOF )
                                      {
                                      error( "EOF in comment" );
                                      break;
                                      }
                                  }
		}
[ \t]+
\n		{ yylineno++;}
"{"		return('{');
"}"		return('}');
"="		return('=');
";"		return(';');
.

%%
