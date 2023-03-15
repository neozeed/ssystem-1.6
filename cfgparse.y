%{
#include <math.h>  /* For math functions, cos(), sin(), etc. */
#include <time.h>
#include "ssystem.h"


void yyerror(char *s);
void SetUpRings(int, char *, float, float), SetUpAtmosphere(int, char *, float, int),SetUpBody(int, char *, float);
void InitSun(char *, double, int);
int yylex(void);
int yylineno=0;
int body_index=1,last_planet=0;
planetdata planets[MAXBODIES];
extern FILE *cfgin;
extern int border,filter,smodel,paused,demomode,plabels,slabels,info,
		realtime,cmode,currtarget,currsource,jpeg_quality;
extern float speed,star_mag;
struct tm date;
%}
     
%union {
     int	ival;
     double     dval;
     char  	name[100];
}
     
%token <ival>  INTEGER
%token <dval>  DOUBLE
%token <name>  NAME
%token <name>  SUN PLANET ASTEROID SATELLITE RING ATMOSPHERE
%token <name>  DATA_DIR OPENBITMAP BORDER FILTER SHADING DATE PAUSE REALTIME
%token <name>  PLANETS_LABELS STARS_LABELS INFO STAR_BRIGHT STAR_SIZE CAM_SPEED DEMO
%token <name>  ATMOSPHERES CAM_MODE TARGET SOURCE SLICES STACKS JPEG_QUALITY EOL

%expect 3
     
%%

input:	
	| input line
     ;
     
line:	DATA_DIR '=' NAME  { strcpy(texturepath,$3); }
        | OPENBITMAP '=' INTEGER { openbitmap=$3; }
	| ATMOSPHERES '=' INTEGER { atmospheres=$3; }
	| BORDER '=' INTEGER   { border=$3;}
	| FILTER '=' NAME   {
				    if (!strcmp($3,"GL_NEAREST")) filter=GL_NEAREST;
				    else filter=GL_LINEAR;
			       }
	| SHADING '=' NAME {
				if (strcmp($3,"GL_FLAT")) smodel=GL_SMOOTH;
				else smodel=GL_FLAT;
			   }	
	| DATE '=' NAME NAME   { 
			 sscanf($3,"%d-%d-%d",&date.tm_mon,&date.tm_mday,&date.tm_year);
			 sscanf($4,"%d:%d:%d",&date.tm_hour,&date.tm_min,&date.tm_sec);
                	 date.tm_mon--;
                	 days=mktime(&date)/3600.0/24.0-10092.0;
			}
	| PAUSE '=' INTEGER   { paused=$3; }
	| REALTIME '=' INTEGER   { realtime=$3; }
	| DEMO '=' INTEGER   { demomode=$3; }
	| PLANETS_LABELS '=' INTEGER   { plabels=$3; }
	| STARS_LABELS '=' INTEGER   { slabels=$3; }
	| INFO '=' INTEGER   { info=$3; }
	| STAR_BRIGHT '=' DOUBLE   { star_mag=$3; }
	| STAR_SIZE '=' DOUBLE	{ star_size=$3; }
	| CAM_SPEED '=' DOUBLE   { speed=$3; }
	| CAM_MODE '=' INTEGER   { cmode=$3; }
	| TARGET '=' INTEGER   { currtarget=$3; }
	| SOURCE '=' INTEGER   { currsource=$3; }
	| SLICES '=' INTEGER   { slices=$3; }
	| STACKS '=' INTEGER   { stacks=$3; }
	| JPEG_QUALITY '=' INTEGER   { jpeg_quality=$3;
                		     if ((jpeg_quality<0) || (jpeg_quality>100)) {
                        		error("Invalid jpeg_quality value\n");
                        		exit(-1);
                		     }
				    }
	| sun
	| body
	;

sun:	SUN '{' NAME DOUBLE INTEGER '}'
	{
	  sprintf(planets[0].Name,"The Sun");
	  InitSun($3,$4,$5);
	}

body:	planet
	| asteroid
	;

planet:		PLANET NAME '{' NAME INTEGER INTEGER INTEGER DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE INTEGER
		{
		 strcpy(planets[body_index].Name,$2);
		 planets[body_index].Color[0]=$5/255.0;
		 planets[body_index].Color[1]=$6/255.0;
		 planets[body_index].Color[2]=$7/255.0;
		 planets[body_index].Radius=$8;
		 planets[body_index].xflat=$9;
		 planets[body_index].yflat=$10;
		 planets[body_index].zflat=$11;
		 planets[body_index].Degrees=$13;
		 planets[body_index].Rotation=$14;
		 planets[body_index].Inclination=$15*PI/180.0;
		 planets[body_index].AscendingNode=$16*PI/180.0;
		 planets[body_index].Perihelion=$17*PI/180.0;
		 planets[body_index].MeanDistance=DISTCORRECTION($18);
		 planets[body_index].DailyMotion=$19*PI/180.0;
		 planets[body_index].OrbitalPeriod=$20;
		 planets[body_index].Eccentricity=$21;
		 planets[body_index].MeanLongitude=$22*PI/180.0;
		 planets[body_index].CustomFunction=$23;
		 planets[body_index].Sat=0;
		 planets[body_index].TrailEnabled=0;
		 planets[body_index].Type=PLANET;
		 SetUpBody(body_index,$4,$12);
		 last_planet=body_index;
		 body_index++;
		}
	    atmosphere rings satellites '}'
	;

satellites:
	  | satellite
	  | satellites satellite

satellite:	SATELLITE NAME '{' NAME INTEGER INTEGER INTEGER DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE INTEGER
		{
		 strcpy(planets[body_index].Name,$2);
		 planets[body_index].Color[0]=$5/255.0;
		 planets[body_index].Color[1]=$6/255.0;
		 planets[body_index].Color[2]=$7/255.0;
		 planets[body_index].Radius=$8;
		 planets[body_index].xflat=$9;
		 planets[body_index].yflat=$10;
		 planets[body_index].zflat=$11;
		 planets[body_index].Degrees=$13;
		 planets[body_index].Rotation=$14;
		 planets[body_index].Inclination=$15*PI/180.0;
		 planets[body_index].AscendingNode=$16*PI/180.0;
		 planets[body_index].Perihelion=$17*PI/180.0;
		 planets[body_index].MeanDistance=DISTCORRECTION($18);
		 planets[body_index].DailyMotion=$19*PI/180.0;
		 planets[body_index].OrbitalPeriod=$20;
		 planets[body_index].Eccentricity=$21;
		 planets[body_index].MeanLongitude=$22*PI/180.0;
		 planets[body_index].Type=SATELLITE;
		 planets[body_index].Sat=last_planet;
		 planets[body_index].CustomFunction=$23;
		 SetUpBody(body_index,$4,$12);
		 body_index++;
		}
		atmosphere '}'
	;

asteroid:	ASTEROID NAME '{' NAME INTEGER INTEGER INTEGER DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE INTEGER
		{
		 strcpy(planets[body_index].Name,$2);
		 planets[body_index].Color[0]=$5/255.0;
		 planets[body_index].Color[1]=$6/255.0;
		 planets[body_index].Color[2]=$7/255.0;
		 planets[body_index].Radius=$8;
		 planets[body_index].xflat=$9;
		 planets[body_index].yflat=$10;
		 planets[body_index].zflat=$11;
		 planets[body_index].Degrees=$13;
		 planets[body_index].Rotation=$14;
		 planets[body_index].Inclination=$15*PI/180.0;
		 planets[body_index].AscendingNode=$16*PI/180.0;
		 planets[body_index].Perihelion=$17*PI/180.0;
		 planets[body_index].MeanDistance=DISTCORRECTION($18);
		 planets[body_index].DailyMotion=$19*PI/180.0;
		 planets[body_index].OrbitalPeriod=$20;
		 planets[body_index].Eccentricity=$21;
		 planets[body_index].MeanLongitude=$22*PI/180.0;
		 planets[body_index].Sat=0;
		 planets[body_index].CustomFunction=$23;
		 planets[body_index].Type=ASTEROID;
		 SetUpBody(body_index,$4,$12);
		 last_planet=body_index;
		 body_index++;
		}
		satellites '}'
	;

atmosphere:
	 |  ATMOSPHERE '{' NAME INTEGER INTEGER INTEGER DOUBLE DOUBLE INTEGER '}'
		{
		 planets[body_index].Color[0]=$4/255.0;
		 planets[body_index].Color[1]=$5/255.0;
		 planets[body_index].Color[2]=$6/255.0;
		 planets[body_index].Radius=$7;
		 planets[body_index].Sat=last_planet;
		 planets[body_index].Type=ATMOSPHERE;
		 SetUpAtmosphere(body_index,$3,$8,$9);
		 body_index++;
		}
	;

rings:	
	| ring
	| rings ring
	;

ring:		RING '{' NAME INTEGER INTEGER INTEGER DOUBLE DOUBLE '}'
		{
		 planets[body_index].Color[0]=$4/255.0;
		 planets[body_index].Color[1]=$5/255.0;
		 planets[body_index].Color[2]=$6/255.0;
		 planets[body_index].Sat=last_planet;
		 planets[body_index].Type=RING;
		 SetUpRings(body_index,$3,$7,$8);
		 body_index++;
		}
	;


%%

void yyerror(char *s)
{
 fprintf(stderr,"%s in line %d of configuration file\n",s,yylineno);
 exit(-1);
}

/* Read configuration file */
void ReadConfigFile(void)
{ 
   char s[200];

   sprintf(s,"%s/ssystem.conf",texturepath);
   cfgin=fopen(s,"rt");
   if (cfgin==NULL) {
	sprintf(s,"ssystem.conf");
   	cfgin=fopen(s,"rt");
   	if (cfgin==NULL) {
   		error("Couldn't find configuration file");
   		exit(-1);
   	}
   }
   yyparse();
   fclose(cfgin);
   NUMBODIES=body_index;
}
