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
 *   Astronomical library
 *   Written February 1999 by Larry Manley
 *   
 *   Used in ssystem with permission, thanks Larry :-)
 *
 *   Based on information in "Astronomical Algorithms", by Jean Meeus, 1991
 *
 *
 *
 *   Here you'll find the custom functions for the nine planets and the Moon.
 *   They return longitude, latitude and distance from the Sun in AU (the
 *   MoonPos and every custom satellite funcition must return the distance
 *   from its host planet in planet radii)
 */

#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846
#define DEG2RAD(x) ((x)*PI/180.0)
#define RAD2DEG(x) ((x)*180.0/PI)

#include "vsop87.dat" /* Hardcoded VSOP87 data */

/* Not used elsewhere yet ... but it will be */
double JulianDay(int Y, int M, int D)
{
	double JD;
	int A, B;

	if (M < 3)
	{
		Y -= 1;
		M += 12;
	}
	A = (int)floor((double)Y / 100.0);
	B = 2 - A + (int)floor((double)A / 4.0);
	JD = floor(365.25*((double)Y+4716.0)) + floor(30.6001*((double)M+1)) + D + B - 1524.5;

	return JD;
}

/* Planetary positioning algorithms
 * Abridged VSOP87 theory
 * Longitude and latitude calculated in radians
 * Radius calculated in AU
 */

double ReadTerms(double T, double *data)
{
    double sum=0.0;
    int i, j;

    i=(int)data[0];
    for (j=0; j<i; j++)
        sum += (data[j*3+1] * cos(data[j*3+2] + (data[j*3+3] * T)));
    return (sum/1.0E8);
}


void MercuryPos(double JD, double *Long, double *Lat, double *Rad)
{
    double t, L0, L1, L2, L3, L4, L5;
    double B0, B1, B2, B3, B4;
    double R0, R1, R2, R3;

    t = (JD - 2451545.0) / 365250.0;    /* Calculate tau */
    
    L0 = ReadTerms(t, mercuryL0);
    L1 = ReadTerms(t, mercuryL1);
    L2 = ReadTerms(t, mercuryL2);
    L3 = ReadTerms(t, mercuryL3);
    L4 = ReadTerms(t, mercuryL4);
    L5 = ReadTerms(t, mercuryL5);
    *Long = L0+t*(L1+t*(L2+t*(L3+t*(L4+t*L5))));

    B0 = ReadTerms(t, mercuryB0);
    B1 = ReadTerms(t, mercuryB1);
    B2 = ReadTerms(t, mercuryB2);
    B3 = ReadTerms(t, mercuryB3);
    B4 = ReadTerms(t, mercuryB4);
    *Lat = B0+t*(B1+t*(B2+t*(B3+t*B4)));

    R0 = ReadTerms(t, mercuryR0);
    R1 = ReadTerms(t, mercuryR1);
    R2 = ReadTerms(t, mercuryR2);
    R3 = ReadTerms(t, mercuryR3);
    *Rad = R0+t*(R1+t*(R2+t*R3));
}

void VenusPos(double JD, double *Long, double *Lat, double *Rad)
{
    double t, L0, L1, L2, L3, L4, L5;
    double B0, B1, B2, B3, B4;
    double R0, R1, R2, R3, R4;

    t = (JD - 2451545.0) / 365250.0;    /* Calculate tau */

    L0 = ReadTerms(t, venusL0);
    L1 = ReadTerms(t, venusL1);
    L2 = ReadTerms(t, venusL2);
    L3 = ReadTerms(t, venusL3);
    L4 = ReadTerms(t, venusL4);
    L5 = ReadTerms(t, venusL5);
    *Long = L0+t*(L1+t*(L2+t*(L3+t*(L4+t*L5))));

    B0 = ReadTerms(t, venusB0);
    B1 = ReadTerms(t, venusB1);
    B2 = ReadTerms(t, venusB2);
    B3 = ReadTerms(t, venusB3);
    B4 = ReadTerms(t, venusB4);
    *Lat = B0+t*(B1+t*(B2+t*(B3+t*B4)));
    
	R0 = ReadTerms(t, venusR0);
    R1 = ReadTerms(t, venusR1);
    R2 = ReadTerms(t, venusR2);
    R3 = ReadTerms(t, venusR3);
    R4 = ReadTerms(t, venusR4);
    *Rad = R0+t*(R1+t*(R2+t*(R3+t*R4)));
}

void EarthPos(double JD, double *Long, double *Lat, double *Rad)
{
    double t, L0, L1, L2, L3, L4, L5;
    double B0, B1;
    double R0, R1, R2, R3, R4;

    t = (JD - 2451545.0) / 365250.0;    /* Calculate tau */

    L0 = ReadTerms(t, earthL0);
    L1 = ReadTerms(t, earthL1);
    L2 = ReadTerms(t, earthL2);
    L3 = ReadTerms(t, earthL3);
    L4 = ReadTerms(t, earthL4);
    L5 = ReadTerms(t, earthL5);
    *Long = L0+t*(L1+t*(L2+t*(L3+t*(L4+t*L5))));

    B0 = ReadTerms(t, earthB0);
    B1 = ReadTerms(t, earthB1);
    *Lat = B0+t*B1;

    R0 = ReadTerms(t, earthR0);
    R1 = ReadTerms(t, earthR1);
    R2 = ReadTerms(t, earthR2);
    R3 = ReadTerms(t, earthR3);
    R4 = ReadTerms(t, earthR4);
    *Rad = R0+t*(R1+t*(R2+t*(R3+t*R4)));
}

void MarsPos(double JD, double *Long, double *Lat, double *Rad)
{
    double t, L0, L1, L2, L3, L4, L5;
    double B0, B1, B2, B3, B4;
    double R0, R1, R2, R3, R4;

    t = (JD - 2451545.0) / 365250.0;    /* Calculate tau */

    L0 = ReadTerms(t, marsL0);
    L1 = ReadTerms(t, marsL1);
    L2 = ReadTerms(t, marsL2);
    L3 = ReadTerms(t, marsL3);
    L4 = ReadTerms(t, marsL4);
    L5 = ReadTerms(t, marsL5);
    *Long = L0+t*(L1+t*(L2+t*(L3+t*(L4+t*L5))));

    B0 = ReadTerms(t, marsB0);
    B1 = ReadTerms(t, marsB1);
    B2 = ReadTerms(t, marsB2);
    B3 = ReadTerms(t, marsB3);
    B4 = ReadTerms(t, marsB4);
    *Lat = B0+t*(B1+t*(B2+t*(B3+t*B4)));

    R0 = ReadTerms(t, marsR0);
    R1 = ReadTerms(t, marsR1);
    R2 = ReadTerms(t, marsR2);
    R3 = ReadTerms(t, marsR3);
    R4 = ReadTerms(t, marsR4);
    *Rad = R0+t*(R1+t*(R2+t*(R3+t*R4)));
}

void JupiterPos(double JD, double *Long, double *Lat, double *Rad)
{
    double t, L0, L1, L2, L3, L4, L5;
    double B0, B1, B2, B3, B4, B5;
    double R0, R1, R2, R3, R4, R5;

    t = (JD - 2451545.0) / 365250.0;    /* Calculate tau */

    L0 = ReadTerms(t, jupiterL0);
    L1 = ReadTerms(t, jupiterL1);
    L2 = ReadTerms(t, jupiterL2);
    L3 = ReadTerms(t, jupiterL3);
    L4 = ReadTerms(t, jupiterL4);
    L5 = ReadTerms(t, jupiterL5);
    *Long = L0+t*(L1+t*(L2+t*(L3+t*(L4+t*L5))));

    B0 = ReadTerms(t, jupiterB0);
    B1 = ReadTerms(t, jupiterB1);
    B2 = ReadTerms(t, jupiterB2);
    B3 = ReadTerms(t, jupiterB3);
    B4 = ReadTerms(t, jupiterB4);
    B5 = ReadTerms(t, jupiterB5);
    *Lat = B0+t*(B1+t*(B2+t*(B3+t*(B4+t*B5))));

    R0 = ReadTerms(t, jupiterR0);
    R1 = ReadTerms(t, jupiterR1);
    R2 = ReadTerms(t, jupiterR2);
    R3 = ReadTerms(t, jupiterR3);
    R4 = ReadTerms(t, jupiterR4);
    R5 = ReadTerms(t, jupiterR5);
    *Rad = R0+t*(R1+t*(R2+t*(R3+t*(R4+t*R5))));
}

void SaturnPos(double JD, double *Long, double *Lat, double *Rad)
{
    double t, L0, L1, L2, L3, L4, L5;
    double B0, B1, B2, B3, B4, B5;
    double R0, R1, R2, R3, R4, R5;

    t = (JD - 2451545.0) / 365250.0;    /* Calculate tau */

    L0 = ReadTerms(t, saturnL0);
    L1 = ReadTerms(t, saturnL1);
    L2 = ReadTerms(t, saturnL2);
    L3 = ReadTerms(t, saturnL3);
    L4 = ReadTerms(t, saturnL4);
    L5 = ReadTerms(t, saturnL5);
    *Long = L0+t*(L1+t*(L2+t*(L3+t*(L4+t*L5))));

    B0 = ReadTerms(t, saturnB0);
    B1 = ReadTerms(t, saturnB1);
    B2 = ReadTerms(t, saturnB2);
    B3 = ReadTerms(t, saturnB3);
    B4 = ReadTerms(t, saturnB4);
    B5 = ReadTerms(t, saturnB5);
    *Lat = B0+t*(B1+t*(B2+t*(B3+t*(B4+t*B5))));

    R0 = ReadTerms(t, saturnR0);
    R1 = ReadTerms(t, saturnR1);
    R2 = ReadTerms(t, saturnR2);
    R3 = ReadTerms(t, saturnR3);
    R4 = ReadTerms(t, saturnR4);
    R5 = ReadTerms(t, saturnR5);
    *Rad = R0+t*(R1+t*(R2+t*(R3+t*(R4+t*R5))));
}

void UranusPos(double JD, double *Long, double *Lat, double *Rad)
{
    double t, L0, L1, L2, L3, L4;
    double B0, B1, B2, B3, B4;
    double R0, R1, R2, R3, R4;

    t = (JD - 2451545.0) / 365250.0;    /* Calculate tau */

    L0 = ReadTerms(t, uranusL0);
    L1 = ReadTerms(t, uranusL1);
    L2 = ReadTerms(t, uranusL2);
    L3 = ReadTerms(t, uranusL3);
    L4 = ReadTerms(t, uranusL4);
    *Long = L0+t*(L1+t*(L2+t*(L3+t*L4)));

    B0 = ReadTerms(t, uranusB0);
    B1 = ReadTerms(t, uranusB1);
    B2 = ReadTerms(t, uranusB2);
    B3 = ReadTerms(t, uranusB3);
    B4 = ReadTerms(t, uranusB4);
    *Lat = B0+t*(B1+t*(B2+t*(B3+t*B4)));

    R0 = ReadTerms(t, uranusR0);
    R1 = ReadTerms(t, uranusR1);
    R2 = ReadTerms(t, uranusR2);
    R3 = ReadTerms(t, uranusR3);
    R4 = ReadTerms(t, uranusR4);
    *Rad = R0+t*(R1+t*(R2+t*(R3+t*R4)));
}

void NeptunePos(double JD, double *Long, double *Lat, double *Rad)
{
    double t, L0, L1, L2, L3, L4;
    double B0, B1, B2, B3, B4;
    double R0, R1, R2, R3;

    t = (JD - 2451545.0) / 365250.0;    /* Calculate tau */

    L0 = ReadTerms(t, neptuneL0);
    L1 = ReadTerms(t, neptuneL1);
    L2 = ReadTerms(t, neptuneL2);
    L3 = ReadTerms(t, neptuneL3);
    L4 = ReadTerms(t, neptuneL4);
    *Long = L0+t*(L1+t*(L2+t*(L3+t*L4)));

    B0 = ReadTerms(t, neptuneB0);
    B1 = ReadTerms(t, neptuneB1);
    B2 = ReadTerms(t, neptuneB2);
    B3 = ReadTerms(t, neptuneB3);
    B4 = ReadTerms(t, neptuneB4);
    *Lat = B0+t*(B1+t*(B2+t*(B3+t*B4)));

    R0 = ReadTerms(t, neptuneR0);
    R1 = ReadTerms(t, neptuneR1);
    R2 = ReadTerms(t, neptuneR2);
    R3 = ReadTerms(t, neptuneR3);
    *Rad = R0+t*(R1+t*(R2+t*R3));
}

void PlutoPos(double JD, double *Long, double *Lat, double *Rad)
{
	double j, s, p, a, T, sumL, sumB, sumR, sa, ca;
	double j2, j3, s2, p2, p3, p4, p5, p6;

	sumL = sumB = sumR = 0.0;

	T = (JD - 2451545.0) / 36525.0;

	j = 34.35 + (3034.9057 * T);
	j = DEG2RAD(j);
	j2 = 2.0 * j;
	j3 = 3.0 * j;

	s = 50.08 + (1222.1138 * T);
	s = DEG2RAD(s);
	s2 = 2.0 * s;

	p = 238.96 + (144.9600 * T);
	p = DEG2RAD(p);
	p2 = 2.0 * p;
	p3 = 3.0 * p;
	p4 = 4.0 * p;
	p5 = 5.0 * p;
	p6 = 6.0 * p;

	a = p; sa = sin(a); ca = cos(a);
	sumL += (-19798886 * sa) + (19848454 * ca);
	sumB += (-5453098 * sa) + (-14974876 * ca);
	sumR += (66867334 * sa) + (68955876 * ca);

	a = p2; sa = sin(a); ca = cos(a);
	sumL += (897499 * sa) + (-4955707 * ca);
	sumB += (3527363 * sa) + (1672673 * ca);
	sumR += (-11826086 * sa) + (-333765 * ca);

	a = p3; sa = sin(a); ca = cos(a);
	sumL += (610820 * sa) + (1210521 * ca);
	sumB += (-1050939 * sa) + (327763 * ca);
	sumR += (1593657 * sa) + (-1439953 * ca);

	a = p4; sa = sin(a); ca = cos(a);
	sumL += (-341639 * sa) + (-189719 * ca);
	sumB += (178691 * sa) + (-291925 * ca);
	sumR += (-18948 * sa) + (482443 * ca);

	a = p5; sa = sin(a); ca = cos(a);
	sumL += (129027 * sa) + (-34863 * ca);
	sumB += (18763 * sa) + (100448 * ca);
	sumR += (-66634 * sa) + (-85576 * ca);

	a = p6; sa = sin(a); ca = cos(a);
	sumL += (-38215 * sa) + (31061 * ca);
	sumB += (-30594 * sa) + (-25838 * ca);
	sumR += (30841 * sa) + (-5765 * ca);

	a = s - p; sa = sin(a); ca = cos(a);
	sumL += (20349 * sa) + (-9886 * ca);
	sumB += (4965 * sa) + (11263 * ca);
	sumR += (-6140 * sa) + (22254 * ca);

	a = s; sa = sin(a); ca = cos(a);
	sumL += (-4045 * sa) + (-4904 * ca);
	sumB += (310 * sa) + (-132 * ca);
	sumR += (4434 * sa) + (4443 * ca);

	a = s + p; sa = sin(a); ca = cos(a);
	sumL += (-5885 * sa) + (-3238 * ca);
	sumB += (2036 * sa) + (-947 * ca);
	sumR += (-1518 * sa) + (641 * ca);

	a = s + p2; sa = sin(a); ca = cos(a);
	sumL += (-3812 * sa) + (3011 * ca);
	sumB += (-2 * sa) + (-674 * ca);
	sumR += (-5 * sa) + (792 * ca);

	a = s + p3; sa = sin(a); ca = cos(a);
	sumL += (-601 * sa) + (3468 * ca);
	sumB += (-329 * sa) + (-563 * ca);
	sumR += (518 * sa) + (518 * ca);

	a = s2 - p2; sa = sin(a); ca = cos(a);
	sumL += (1237 * sa) + (463 * ca);
	sumB += (-64 * sa) + (39 * ca);
	sumR += (-13 * sa) + (-221 * ca);

	a = s2 - p; sa = sin(a); ca = cos(a);
	sumL += (1086 * sa) + (-911 * ca);
	sumB += (-94 * sa) + (210 * ca);
	sumR += (837 * sa) + (-494 * ca);

	a = s2; sa = sin(a); ca = cos(a);
	sumL += (595 * sa) + (-1229 * ca);
	sumB += (-8 * sa) + (-160 * ca);
	sumR += (-281 * sa) + (616 * ca);

	a = j - s; sa = sin(a); ca = cos(a);
	sumL += (2484 * sa) + (-485 * ca);
	sumB += (-177 * sa) + (259 * ca);
	sumR += (260 * sa) + (-395 * ca);

	a = j - s + p; sa = sin(a); ca = cos(a);
	sumL += (839 * sa) + (-1414 * ca);
	sumB += (17 * sa) + (234 * ca);
	sumR += (-191 * sa) + (-396 * ca);

	a = j - p3; sa = sin(a); ca = cos(a);
	sumL += (-964 * sa) + (1059 * ca);
	sumB += (582 * sa) + (-285 * ca);
	sumR += (-3218 * sa) + (370 * ca);

	a = j - p2; sa = sin(a); ca = cos(a);
	sumL += (-2303 * sa) + (-1038 * ca);
	sumB += (-298 * sa) + (692 * ca);
	sumR += (8019 * sa) + (-7869 * ca);

	a = j - p; sa = sin(a); ca = cos(a);
	sumL += (7049 * sa) + (747 * ca);
	sumB += (157 * sa) + (201 * ca);
	sumR += (105 * sa) + (45637 * ca);

	a = j; sa = sin(a); ca = cos(a);
	sumL += (1179 * sa) + (-358 * ca);
	sumB += (304 * sa) + (825 * ca);
	sumR += (8623 * sa) + (8444 * ca);

	a = j + p; sa = sin(a); ca = cos(a);
	sumL += (393 * sa) + (-63 * ca);
	sumB += (-124 * sa) + (-29 * ca);
	sumR += (-896 * sa) + (-801 * ca);

	a = j + p2; sa = sin(a); ca = cos(a);
	sumL += (111 * sa) + (-268 * ca);
	sumB += (15 * sa) + (8 * ca);
	sumR += (208 * sa) + (-122 * ca);

	a = j + p3; sa = sin(a); ca = cos(a);
	sumL += (-52 * sa) + (-154 * ca);
	sumB += (7 * sa) + (15 * ca);
	sumR += (-133 * sa) + (65 * ca);

	a = j + p4; sa = sin(a); ca = cos(a);
	sumL += (-78 * sa) + (-30 * ca);
	sumB += (2 * sa) + (2 * ca);
	sumR += (-16 * sa) + ca;

	a = j + s - p3; sa = sin(a); ca = cos(a);
	sumL += (-34 * sa) + (-26 * ca);
	sumB += (4 * sa) + (2 * ca);
	sumR += (-22 * sa) + (7 * ca);

	a = j + s - p2; sa = sin(a); ca = cos(a);
	sumL += (-43 * sa) + ca;
	sumB += 3 * sa;
	sumR += (-8 * sa) + (16 * ca);

	a = j + s - p; sa = sin(a); ca = cos(a);
	sumL += (-15 * sa) + (21 * ca);
	sumB += sa - ca;
	sumR += (2 * sa) + (9 * ca);

	a = j + s; sa = sin(a); ca = cos(a);
	sumL += (-1 * sa) + (15 * ca);
	sumB += -2 * ca;
	sumR += (12 * sa) + (5 * ca);

	a = j + s + p; sa = sin(a); ca = cos(a);
	sumL += (4 * sa) + (7 * ca);
	sumB += sa;
	sumR += sa - 3.0 * ca;

	a = j + s + p3; sa = sin(a); ca = cos(a);
	sumL += sa + (5.0 * ca);
	sumB += sa - ca;
	sumR += sa;

	a = j2 - p6; sa = sin(a); ca = cos(a);
	sumL += (8 * sa) + (3 * ca);
	sumB += (-2 * sa) + (-3 * ca);
	sumR += (9 * sa) + (5 * ca);

	a = j2 - p5; sa = sin(a); ca = cos(a);
	sumL += (-3.0 * sa) + (6 * ca);
	sumB += sa + (2 * ca);
	sumR += (2 * sa) - ca;

	a = j2 - p4; sa = sin(a); ca = cos(a);
	sumL += (6 * sa) + (-13 * ca);
	sumB += (-8 * sa) + (2 * ca);
	sumR += (14 * sa) + (10 * ca);

	a = j2 - p3; sa = sin(a); ca = cos(a);
	sumL += (10 * sa) + (22 * ca);
	sumB += (10 * sa) + (-7 * ca);
	sumR += (-65 * sa) + (12 * ca);

	a = j2 - p2; sa = sin(a); ca = cos(a);
	sumL += (-57 * sa) + (-32 * ca);
	sumB += 21 * ca;
	sumR += (126 * sa) + (-233 * ca);

	a = j2 - p; sa = sin(a); ca = cos(a);
	sumL += (157 * sa) + (-46 * ca);
	sumB += (8 * sa) + (5 * ca);
	sumR += (270 * sa) + (1068 * ca);

	a = j2; sa = sin(a); ca = cos(a);
	sumL += (12 * sa) + (-18 * ca);
	sumB += (13 * sa) + (16 * ca);
	sumR += (254 * sa) + (155 * ca);

	a = j2 + p; sa = sin(a); ca = cos(a);
	sumL += (-4 * sa) + (8 * ca);
	sumB += (-2 * sa) + (-3 * ca);
	sumR += (-26 * sa) + (-2 * ca);

	a = j2 + p2; sa = sin(a); ca = cos(a);
	sumL += -5 * sa;
	sumR += 7 * sa;

	a = j2 + p3; sa = sin(a); ca = cos(a);
	sumL += (3 * sa) + (4 * ca);
	sumB += ca;
	sumR += (-11 * sa) + (4 * ca);

	a = j3 - p2; sa = sin(a); ca = cos(a);
	sumL += (-1 * sa) + (-1 * ca);
	sumB += ca;
	sumR += (4 * sa) + (-14 * ca);

	a = j3 - p; sa = sin(a); ca = cos(a);
	sumL += (6 * sa) + (-3 * ca);
	sumR += (18 * sa) + (35 * ca);

	a = j3; sa = sin(a); ca = cos(a);
	sumL += (-1 * sa) + (-2 * ca);
	sumB += ca;
	sumR += (13 * sa) + (3 * ca);

	*Long = 238.956785 + (144.96 * T) + (sumL / 1E6);
	while (*Long > 360.0) *Long -= 360.0;
	while (*Long < 0.0) *Long += 360.0;

	*Lat = -3.98202 + (sumB / 1E6);
	while (*Lat > 360.0) *Lat -= 360.0;
	while (*Lat < 0.0) *Lat += 360.0;

	*Rad = 40.7247248 + (sumR / 1E7);
	*Long = DEG2RAD(*Long);
	*Lat = DEG2RAD(*Lat);
}


void MoonPos(double JD, double *Long, double *Lat, double *Rad)
{
	double Lp, Lpr, D, Dr, M, Mr, Mp, Mpr, F, Fr, A1, A2, A3, E;
	double T, T2, T3, T4, sumL, sumB, sumR;

	sumL = sumB = sumR = 0.0;
	T = (JD - 2451545.0) / 36525.0;
	T2 = T*T;
	T3 = T*T2;
	T4 = T3 *T;

	/* Compute moon's mean longitude <Lp>, in degrees */
	Lp = 218.3164591 + (481267.88134236 * T) - (0.0013268 * T2) +
		 (T3 / 538841.0) - (T4 / 65194000);
	while (Lp > 360.0) Lp -= 360.0;
	while (Lp < 0.0) Lp += 360.0;
	Lpr = DEG2RAD(Lp);     /* Create radians version for trig functions below */

	/* Mean elongation of the moon <D>, in degrees */
	D = 297.8502042 + (445267.1115168 * T) - (0.0016300 * T2) +
		(T3 / 545868.0) - (T4 / 113065000.0);
	while (D > 360.0) D -= 360.0;
	while (D < 0.0) D += 360.0;
	Dr = DEG2RAD(D);

	/* Sun's mean anomaly <M>, in degrees */
	M = 357.5291092 + (35999.0502909 * T) - (0.0001536 * T2) +
		(T3 / 24490000.0);
	while (M > 360.0) M -= 360.0;
	while (M < 0.0) M += 360.0;
	Mr = DEG2RAD(M);

	/* Moon's mean anomaly <Mp>, in degrees */
	Mp = 134.9634114 + (477198.8676313 * T) + (0.0089970 * T2) +
		 (T3 / 69699.0) - (T4 / 14712000.0);
	while (Mp > 360.0) Mp -= 360.0;
	while (Mp < 0.0) Mp += 360.0;
	Mpr = DEG2RAD(Mp);

	/* Moon's argument of latitude <F> */
	F = 93.2720993 + (483202.0175273 * T) - (0.0034029 * T2) -
		(T3 / 3526000.0) + (T4 / 863310000.0);
	while (F > 360.0) F -= 360.0;
	while (F < 0.0) F += 360.0;
	Fr = DEG2RAD(F);

	A1 = 119.75 + (131.849 * T);
	while (A1 > 360.0) A1 -= 360.0;
	while (A1 < 0.0) A1 += 360.0;
	
	A2 = 53.09 + (479264.290 * T);
	while (A2 > 360.0) A2 -= 360.0;
	while (A2 < 0.0) A2 += 360.0;
	
	A3 = 313.45 + (481266.484 * T);
	while (A3 > 360.0) A3 -= 360.0;
	while (A3 < 0.0) A3 += 360.0;

	E = 1.0 - (0.002516 * T) - (0.0000074 * T2);

	A1 = DEG2RAD(A1);
	A2 = DEG2RAD(A2);
	A3 = DEG2RAD(A3);

	sumL += 6288774 * sin(Mpr);
	sumL += 1274027 * sin(2.0*Dr - Mpr);
	sumL += 658314 * sin(2.0*Dr);
	sumL += 213618 * sin(2.0*Mpr);
	sumL += (-185116 * sin(Mr)) * E;
	sumL += -114332 * sin(2.0 * Fr);
	sumL += 58793 * sin(2.0*Dr - 2.0*Mpr);
	sumL += (57066 * sin(2.0*Dr - Mr - Mpr)) * E;
	sumL += 53322 * sin(2.0*Dr + Mpr);
	sumL += (45758 * sin(2.0*Dr - Mr)) * E;
	sumL += (-40923 * sin(Mr - Mpr)) * E;
	sumL += -34720 * sin(Dr);
	sumL += (-30383 * sin(Mr + Mpr)) * E;
	sumL += 15327 * sin(2.0*Dr - 2.0*Fr);
	sumL += -12528 * sin(Mpr + 2.0*Fr);
	sumL += 10980 * sin(Mpr - 2.0*Fr);
	sumL += 10675 * sin(4.0*Dr - Mpr);
	sumL += 10034 * sin(3.0*Mpr);
	sumL += 8548 * sin(4.0*Dr - 2.0*Mpr);
	sumL += (-7888 * sin(2.0*Dr + Mr - Mpr)) * E;
	sumL += (-6766 * sin(2.0*Dr + Mr)) * E;
	sumL += -5163 * sin(Dr - Mpr);
	sumL += (4987 * sin(Dr + Mr)) * E;
	sumL += (-4036 * sin(2.0*Dr - Mr + Mpr)) * E;
	sumL += 3994 * sin(2.0*Dr + 2.0*Mpr);
	sumL += 3861 * sin(4.0*Dr);
	sumL += 3665 * sin(2.0*Dr - 3.0*Mpr);
	sumL += (-2689 * sin(Mr - 2.0*Mpr)) * E;
	sumL += -2602 * sin(2.0*Dr - Mpr + 2.0*Fr);
	sumL += (2390 * sin(2.0*Dr - Mr - 2.0*Mpr)) * E;
	sumL += -2348 * sin(Dr + Mpr);
	sumL += (2236 * sin(2.0*Dr - 2.0*Mr)) * E*E;
	sumL += (-2120 * sin(Mr + 2.0*Mpr)) * E;
	sumL += (-2069 * sin(2.0*Mr)) * E*E;
	sumL += (2048 * sin(2.0*Dr - 2.0*Mr - Mpr)) * E*E;
	sumL += -1773 * sin(2.0*Dr + Mpr - 2.0*Fr);
	sumL += -1595 * sin(2.0*Dr + 2.0*Fr);
	sumL += (1215 * sin(4.0*Dr - Mr - Mpr)) * E;
	sumL += -1110 * sin(2.0*Mpr + 2.0*Fr);
	sumL += -892 * sin(3.0*Dr - Mpr);
	sumL += (-810 * sin(2.0*Dr + Mr + Mpr)) * E;
	sumL += (759 * sin(4.0*Dr - Mr - 2.0*Mpr)) * E;
	sumL += (-713 * sin(2.0*Mr - Mpr)) * E*E;
	sumL += (-700 * sin(2.0*Dr + 2.0*Mr - Mpr)) * E*E;
	sumL += (691 * sin(2.0*Dr + Mr - 2.0*Mpr)) * E;
	sumL += (596 * sin(2.0*Dr - Mr - 2.0*F)) * E;
	sumL += 549 * sin(4.0*Dr + Mpr);
	sumL += 537 * sin(4.0*Mpr);
	sumL += (520 * sin(4.0*Dr - Mr)) * E;
	sumL += -487 * sin(Dr - 2.0*Mpr);
	sumL += (-399 * sin(2.0*Dr + Mr - 2.0*Fr)) * E;
	sumL += -381 * sin(2.0*Mpr - 2.0*Fr);
	sumL += (351 * sin(Dr + Mr + Mpr)) * E;
	sumL += -340 * sin(3.0*Dr - 2.0*Mpr);
	sumL += 330 * sin(4.0*Dr - 3.0*Mpr);
	sumL += (327 * sin(2.0*Dr - Mr + 2.0*Mpr)) * E;
	sumL += (-323 * sin(2.0*Mr + Mpr)) * E*E;
	sumL += (299 * sin(Dr + Mr - Mpr)) * E;
	sumL += 294 * sin(2.0*Dr + 3.0*Mpr);
	sumL += 3958 * sin(A1);
	sumL += 1962 * sin(Lpr - Fr);
	sumL += 318 * sin(A2);
	*Long = Lp + (sumL / 1E6);
	while (*Long > 360.0) *Long -= 360.0;
	while (*Long < 0.0) *Long += 360.0;

	sumB += 5128122 * sin(Fr);
	sumB += 280602 * sin(Mpr + Fr);
	sumB += 277693 * sin(Mpr - Fr);
	sumB += 173237 * sin(2.0*Dr - Fr);
	sumB += 55413 * sin(2.0*Dr - Mpr + Fr);
	sumB += 46271 * sin(2.0*Dr - Mpr - Fr);
	sumB += 32573 * sin(2.0*Dr + Fr);
	sumB += 17198 * sin(2.0*Mpr + Fr);
	sumB += 9266 * sin(2.0*Dr + Mpr - Fr);
	sumB += 8822 * sin(2.0*Mpr - Fr);
	sumB += (8216 * sin(2.0*Dr - Mr - Fr)) * E;
	sumB += 4324 * sin(2.0*Dr - 2.0*Mpr - Fr);
	sumB += 4200 * sin(2.0*Dr + Mpr + Fr);
	sumB += (-3359 * sin(2.0*Dr + Mr - Fr)) * E;
	sumB += (2463 * sin(2.0*Dr - Mr - Mpr + Fr)) * E;
	sumB += (2211 * sin(2.0*Dr - Mr + Fr)) *E;
	sumB += (2065 * sin(2.0*Dr - Mr - Mpr - Fr)) * E;
	sumB += (-1870 * sin(Mr - Mpr - Fr)) * E;
	sumB += 1828 * sin(4.0*Dr - Mpr - Fr);
	sumB += (-1794 * sin(Mr + Fr)) * E;
	sumB += -1749 * sin(3.0*Fr);
	sumB += (-1565 * sin(Mr - Mpr - Fr)) * E;
	sumB += -1491 * sin(Dr - Fr);
	sumB += (-1475 * sin(Mr + Mpr + Fr)) * E;
	sumB += (-1410 * sin(Mr + Mpr - Fr)) * E;
	sumB += (-1344 * sin(Mr - Fr)) * E;
	sumB += -1335 * sin(Dr - Fr);
	sumB += 1107 * sin(3.0*Mpr + Fr);
	sumB += 1021 * sin(4.0*Dr - Fr);
	sumB += 833 * sin(4.0*Dr - Mpr + Fr);
	sumB += 777 * sin(Mpr - 3.0*Fr);
	sumB += 671 * sin(4.0*Dr - 2.0*Mpr + Fr);
	sumB += 607 * sin(2.0*Dr - 3.0*Fr);
	sumB += 596 * sin(2.0*Dr + 2.0*Mpr - Fr);
	sumB += (491 * sin(2.0*Dr - Mr + Mpr - Fr)) * E;
	sumB += -451 * sin(2.0*Dr - 2.0*Mpr + Fr);
	sumB += 439 * sin(3.0*Mpr - Fr);
	sumB += 422 * sin(2.0*Dr + 2.0*Mpr + Fr);
	sumB += 421 * sin(2.0*Dr - 3.0*Mpr - Fr);
	sumB += (-366 * sin(2.0*Dr + Mr - Mpr + Fr)) * E;
	sumB += (-351 * sin(2.0*Dr + Mr + Fr)) * E;
	sumB += 331 * sin(4.0*Dr + Fr);
	sumB += (315 * sin(2.0*Dr - Mr + Mpr + Fr)) * E;
	sumB += (302 * sin(2.0*Dr - 2.0*Mr - Fr)) * E*E;
	sumB += -283 * sin(Mpr + 3.0*Fr);
	sumB += (-229 * sin(2.0*Dr + Mr + Mpr - Fr)) * E;
	sumB += (223 * sin(Dr + Mr - Fr)) * E;
	sumB += (223 * sin(Dr + Mr + Fr)) * E;
	sumB += (-220 * sin(Mr - 2.0*Mpr - Fr)) * E;
	sumB += (-220 * sin(2.0*Dr + Mr - Mpr - Fr)) * E;
	sumB += -185 * sin(Dr + Mpr + Fr);
	sumB += (181 * sin(2.0*Dr - Mr - 2.0*Mpr - Fr)) * E;
	sumB += (-177 * sin(Mr + 2.0*Mpr + Fr)) * E;
	sumB += 176 * sin(4.0*Dr - 2.0*Mpr - Fr);
	sumB += (166 * sin(4.0*Dr - Mr - Mpr - Fr)) * E;
	sumB += -164 * sin(Dr + Mpr - Fr);
	sumB += 132 * sin(4.0*Dr + Mpr - Fr);
	sumB += -119 * sin(Dr - Mpr - Fr);
	sumB += (115 * sin(4.0*Dr - Mr - Fr)) * E;
	sumB += (107 * sin(2.0*Dr - 2.0*Mr + Fr)) * E;
	sumB += (-2235 * sin(Lpr));
	sumB += 382 * sin(A3);
	sumB += 175 * sin(A1 - Fr);
	sumB += 175 * sin(A1 + Fr);
	sumB += 127 * sin(Lpr - Mpr);
	sumB += -115 * sin(Lpr + Mpr);
	*Lat = sumB / 1E6;
	while (*Lat > 360.0) *Lat -= 360.0;
	while (*Lat < -360.0) *Lat += 360.0;

	sumR += -20905355 * cos(Mpr);
	sumR += -3699111 * cos(2.0*Dr - Mpr);
	sumR += -2955968 * cos(2.0*Dr);
	sumR += -569925 * cos(2.0*Mpr);
	sumR += (48888 * cos(Mr)) * E;
	sumR += -3149 * cos(2.0 * Fr);
	sumR += 246158 * cos(2.0*Dr - 2.0*Mpr);
	sumR += (-152138 * cos(2.0*Dr - Mr - Mpr)) * E;
	sumR += -170733 * cos(2.0*Dr + Mpr);
	sumR += (-204586 * cos(2.0*Dr - Mr)) * E;
	sumR += (-129620 * cos(Mr - Mpr)) * E;
	sumR += 108743 * cos(Dr);
	sumR += (104755 * cos(Mr + Mpr)) * E;
	sumR += 10321 * cos(2.0*Dr - 2.0*Fr);
	sumR += 79661 * cos(Mpr - 2.0*Fr);
	sumR += -34782 * cos(4.0*Dr - Mpr);
	sumR += -23210 * cos(3.0*Mpr);
	sumR += -21636 * cos(4.0*Dr - 2.0*Mpr);
	sumR += (24208 * cos(2.0*Dr + Mr - Mpr)) * E;
	sumR += (30824 * cos(2.0*Dr + Mr)) * E;
	sumR += -8379 * cos(Dr - Mpr);
	sumR += (-16675 * cos(Dr + Mr)) * E;
	sumR += (-12831 * cos(2.0*Dr - Mr + Mpr)) * E;
	sumR += -10445 * cos(2.0*Dr + 2.0*Mpr);
	sumR += -11650 * cos(4.0*Dr);
	sumR += 14403 * cos(2.0*Dr - 3.0*Mpr);
	sumR += (-7003 * cos(Mr - 2.0*Mpr)) * E;
	sumR += (10056 * cos(2.0*Dr - Mr - 2.0*Mpr)) * E;
	sumR += 6322 * cos(Dr + Mpr);
	sumR += (-9884 * cos(2.0*Dr - 2.0*Mr)) * E*E;
	sumR += (5751 * cos(Mr + 2.0*Mpr)) * E;
	sumR += (-4950 * cos(2.0*Dr - 2.0*Mr - Mpr)) * E*E;
	sumR += 4130 * cos(2.0*Dr + Mpr - 2.0*Fr);
	sumR += (-3958 * cos(4.0*Dr - Mr - Mpr)) * E;
	sumR += 3258 * cos(3.0*Dr - Mpr);
	sumR += (2616 * cos(2.0*Dr + Mr + Mpr)) * E;
	sumR += (-1897 * cos(4.0*Dr - Mr - 2.0*Mpr)) * E;
	sumR += (-2117 * cos(2.0*Mr - Mpr)) * E*E;
	sumR += (2354 * cos(2.0*Dr + 2.0*Mr - Mpr)) * E*E;
	sumR += -1423 * cos(4.0*Dr + Mpr);
	sumR += -1117 * cos(4.0*Mpr);
	sumR += (-1571 * cos(4.0*Dr - Mr)) * E;
	sumR += -1739 * cos(Dr - 2.0*Mpr);
	sumR += -4421 * cos(2.0*Mpr - 2.0*Fr);
	sumR += (1165 * cos(2.0*Mr + Mpr)) * E*E;
	sumR += 8752 * cos(2.0*Dr - Mpr - 2.0*Fr);
	*Rad = 385000.56 + (sumR / 1000.0);    /* Center of Earth to center of Moon in kilometers */
	*Rad /= 6378.0; 	/* Return distance in Earth radii */
        *Long = DEG2RAD(*Long);
        *Lat = DEG2RAD(*Lat);
}
