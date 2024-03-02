![ssystem-saturn](https://github.com/neozeed/ssystem-1.6/assets/9031439/86957ed4-bb98-4d02-b93e-a89f08c2ec4e)


OpenGL ssystem 1.6 Feb-26-1999
------------------------------


What is ssystem:
----------------

	ssystem is an OpenGL Solar System simulator. Visit the Sun, the
  nine planets and a few major satellites (catalog based background stars are
  also supported) with four camera modes.


Accuracy notes:
---------------

	Although ssystem is not 100% accurate I've tried to keep it as close
  to reality as possible. However, ssystem's goal has always been visual
  appearence rather than accuracy, so you may find many bugs in positioning
  algorithms.

	Only planets and Moon positions are within a range of real positions, 
  all other satellites (even though their orbits are, more or less, accurate)
  are NOT in their real positions (yet).


System requeriments:
--------------------

	-Linux/UNIX or Windows 95/NT OS
	-OpenGL library (Mesa 3.0 recommended)
	-GLUT library (v3.7 or higher required)
	-JPEG library (the IJG one)
	-3D accelerator (NOT REQUIRED but HIGHLY RECOMENDED) tested on:
		Voodoo Graphics (linux and win95/98)
		Voodoo2		(linux and win95/98)
		Voodoo Rush	(linux and win95/98)
		Riva 128	(win95/98)
		Riva TNT	(win95/98)
		Permedia	(win95/98)

	If your 3d chipset is not listed above and ssystem runs hardware
  accelerated let me know.

	See Makefile for compile time options.



Command line options:
---------------------

	-bench	

		Aborts program execution after 1000 frames, displaying
		framerate

	-fullscreen [mode_string]

		Runs program in full screen mode. Under Linux/UNIX machines
		you won't be able to switch video modes so you don't have
		to supply a mode_string (in fact, ssystem will complain if
		you do so).

		Windows 95/98 version lets you choose the videomode. Just 
		add a mode string after the fullscreen keyword. Mode string
		sintax is:

			WIDTHxHEIGHT:COLOR_DEPTH@REFRESH_RATE

		Refresh rate is optional. So if you want to run ssystem at
		640x480 32 bits colordepth your command line is something like:

			ssystem -fullscreen 640x480:32

		If you want a 100 Hz refresh rate then type:

			sssytem -fullscreen 640x480:32@100


		NOTE: NOT ALL VIDEOMODES ARE SUPPORTED. ONLY A FEW ONES
		WORK ON MY COMPUTER. IF THE VIDEO MODE YOU'VE SELECTED IS 
		NOT SUPPORTED SSYSTEM USES CURRENT VIDEO MODE.


Performance:
------------


	Hardware acceleration is not required, but highly recommended for
  smooth framerates. I get 83fps (running on Linux) with the "-bench" command
  line option on my Celery 450 and 56fps at 1024x768:32 (running on win98
  with a Riva TNT card). Ssystem rocks in this video mode.

	The Planets' Sphere detail (SLICES and STACKS) is the key factor in 
  performance, people with slow CPUs should use lower values (see ssystem.conf
  file).


Keyboard bindings:
------------------

	Home/End: Selects previous/next target body
	SHIFT + Home/End: Selects previous/next source body in B2B camera mode
	Insert: Current planet trail on/off
	h : Online help
	a : Atmosphere on/off
	t : Texture on/off
	l : Lighting on/off
	f : Flat/Smooth shading model
	r : Real time on/off
	s : Stars on/off
	S : Screenshot (in JPEG format)
	m/M : Increase/Decrease star brightness
	d : Demo mode on/off
	n : Moves camera near current planet
	c : Camera modes
	o/O : Wider/Narrower FOV
	p : Pause
	b : Distant bodies labels on/off
	B : Stars labels on/off
	i : Info (HUD) on/off
	j : Joystick control on/off
	Page Up/Down: Increase/decrease speed
	z/Z: Increase/decrease zoom factor
		(only available in B2B and orbiter camera modes)

  Hold SHIFT for faster operation in the following key bindings:

	+/-: Increase/Decrease time factor
	Arrow keys: Rotate camera


Camera modes:
-------------

	There are 4 camera modes:

	-Linked: This is the default mode. Camera follows the selected body.
	 'n' is very useful in this mode.

	-Body to Body: This mode lets you view body X as seen from body Y.

	-Orbiter: Camera moves around the select body in a circular orbit (
	 completes an orbit in 0.5 local days). Try the zoom key 'z' in this
	 mode.

	-Free Camera: You control the camera with arrow keys/josytick/mouse.


Camera movement:
----------------

 -Keyboard

	When in free camera mode arrow keys rotates camera, Page Up/Down 
	increase/decrease camera speed.

	Linked mode is a bit different, here arrow keys are useless. You can
  	move the camera towards the planet by increasing speed. Once you're
	near the planet program sets camera speed to zero.


 -Mouse

	In free camera mode you can control the camera with the mouse by left
	clicking over the window (keep it pressed)  and moving the mouse.

 -Joystick

	As with the mouse you have to select free camera mode, then activate
	joystick (j). X and Y axis control camera, buttons A and B control
	acceleration. Press C or D button buttons to set camera speed to 0
	(if your joystick has only two buttons you can achieve this by
	pressing A and B simultaneously)

  

Known bugs:
-----------

	Outer bodies movement is a bit jerky. Although ssystem uses double
  precision floating point arithmetic, Mesa uses simple precision arithmetic
  internally. Tuning scales i've been able to reduce this annoying effect
  (only Charon is highly affected). The good news: I know how this effect
  can be solved; the bad news: this will requiere a total rewrite of large
  parts of the code so i guess it should wait for the next release :-(

	If you're seeing black lines across the planets this is not a bug.
  Ssystem default to "unbordered" textures (which work fine on most 3D
  hardware) but software-only OpenGL drivers and maybe some hardware
  accelerated drivers requiere bordered textures. Simply edit ssystem.conf
  and set border=1 (remove the # char in line 18). Some 3D hardware won't
  work if border is set to 1 (3Dfx).

	Fullscreen modes will only work properly under drivers supporting
  the current video mode (*nix) or the video mode you have supplied in the
  command line (win32). If you force fullscreen mode and your hardware does
  not support the video mode ssystem won't work (properly).
	



ACKNOWLEDGEMENTS:
-----------------


-Brian Paul (Mesa)
	http://www.ssec.wisc.edu/~brianp/MesQ.html 

-Daryll Strauss (Linux Glide)


-David Bucciarelli (Mesa 3dfx driver)
	http://www-hmw.caribel.pisa.it/fxmesa/index.shtml

-Curtis L. Olson (star catalog stuff)


-This software is based in part on the work of The Independent JPEG Group


-Galilean and Saturn satellites computed using code extracted with
 permission from XEphem, (c) 1997 Elwood Charles Downey
	http://iraf.noao.edu/~ecdowney/xephem.html

-Keith Burnett's planet positioning algorithm used with persmission.
	http://www.xylem.demon.co.uk/kepler/

-Nacho (beta testing)

-Kevin Kamel (US mirror)

-Rafael Barbera (v1.4 performance improvement trick)

-Aaron Worley and his orrery simulation (configuration file and many ideas)
	http://hirame.hiram.edu/~worleyam/hitch/index.html

-Elias Martenson (SUN Makefile)

-Andrey Zakhvatov (FreeBSD stuff)

-Mike Oliphant and Rob Clark (screenshot function)   
        http://www.ling.ed.ac.uk/~oliphant/pyro

-John Kilburg and Bill Spitzak (SGI Makefiles)

-Bjorn Jonsson (Io, Europa and Saturn textures)

-Larry Manley (MoonPos, PlutoPos function and VSOP87 stuff, see astrolib.c)

-Tigran A. Aivazian (bug reports and suggestions)

-Axel Groll (extensive beta testing)

-And of course, thank you :-)


NOTE:
-----

	This is my first OpenGL program and i guess it will be always in
  development, so I'd really appreciate your feedback. 

	Constructive criticism is always welcome. :-)

	Check ssytem homepage for updates and news:

			http://www1.las.es/~amil/ssystem

Raul Alonso
<amil@las.es>
