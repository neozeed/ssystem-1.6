#
#	ssystem 1.6 Makefile for pgcc/egcs compiler (+HPUX Extensions)
#
#	Remove -DLINUXJOY if your linux kernel doesn't support joysticks or
#	you're running other OS
#
#	Even if you're running Linux, make sure your joystick driver is v1.x
#	I've tested it with the v1.2.13 included in the 2.2.1 kernel.

CC= cc

# Your Mesa base directory
MESADIR=/opt/Mesa

PREFIX=/opt/ssystem
BINDIR=${PREFIX}/bin
LIBDIR=${PREFIX}/lib

# Where textures and star catalog files will be located, you may override this
# setting at run time
SDATADIR=$(LIBDIR)

#CFLAGS = -Wall -O6 -I$(MESADIR)/include -fomit-frame-pointer -funroll-loops \
#	-fexpensive-optimizations -march=pentium -malign-loops=2 \
#	-malign-jumps=2 -malign-functions=2 -DLINUXJOY -DSDATADIR=\"${SDATADIR}\"

CFLAGS = -Ae -I$(MESADIR)/include -I/opt/jpeg-6/include -DSDATADIR=\"${SDATADIR}\"

LDFLAGS= -L/usr/X11R6/lib -L$(MESADIR)/lib -L/opt/jpeg-6/lib -ljpeg -lglut -lMesaGLU -lMesaGL \
	  -lXext -L/usr/contrib/X11R6/lib -lXmu -lXi -lX11 -lm

OBJ = cfgparse.tab.o lex.cfg.o ssystem.o init.o positions.o joystick.o \
      cmdline.o keyboard.o mouse.o scrnsht.o sun.o timer.o util.o astrolib.o \
      jpeg.o stars.o HPalloca.o

LIBS = 

all: ssystem

clean:
	rm -f *.o *~ ssystem .depend cfgparse.tab.* cfgparse.output lex.cfg.c

ssystem: $(OBJ)
	$(CC) -o ssystem $(OBJ) $(LDFLAGS) $(LIBS) 

lex.cfg.c: cfglex.l cfgparse.tab.c
	flex -i -Pcfg cfglex.l

cfgparse.tab.c: cfgparse.y
	bison -d -v -p cfg cfgparse.y

isdir:
	test -d $(BINDIR) || mkdirhier $(BINDIR)
	test -d $(LIBDIR) || mkdirhier $(LIBDIR)

	
install: ssystem isdir
	cp -f -p  *.jpg $(SDATADIR)
	install -c -m 0644 stars.dat $(SDATADIR)
	install -c -m 0644 ssystem.conf $(SDATADIR)
	install -c -m 0755 ssystem $(BINDIR)
	
.c.o:
	$(CC) -c $(CFLAGS) $< 

dep:
	gcc -MM -I$(MESADIR)/include *.c > .depend

# DO NOT DELETE
astrolib.o: astrolib.c vsop87.dat
cfgparse.tab.o: cfgparse.tab.c ssystem.h
cmdline.o: cmdline.c ssystem.h cfgparse.tab.h
init.o: init.c ssystem.h cfgparse.tab.h
joystick.o: joystick.c ssystem.h cfgparse.tab.h
jpeg.o: jpeg.c ssystem.h cfgparse.tab.h
keyboard.o: keyboard.c ssystem.h cfgparse.tab.h
lex.cfg.o: lex.cfg.c ssystem.h cfgparse.tab.h
mouse.o: mouse.c ssystem.h cfgparse.tab.h
positions.o: positions.c ssystem.h cfgparse.tab.h
scrnsht.o: scrnsht.c ssystem.h cfgparse.tab.h
ssystem.o: ssystem.c ssystem.h cfgparse.tab.h
stars.o: stars.c ssystem.h cfgparse.tab.h
sun.o: sun.c ssystem.h cfgparse.tab.h
timer.o: timer.c ssystem.h cfgparse.tab.h
util.o: util.c ssystem.h cfgparse.tab.h
HPalloca.o:HPalloca.c
