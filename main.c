/*
Copyright (C) 2003 - Derek John Evans 

This file is part of Yeti3D Portable Engine

Yeti3D is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Original Source: 2003 - Derek J. Evans <derek@theteahouse.com.au>
Prepared for public release: 10/24/2003 - Derek J. Evans <derek@theteahouse.com.au>
*/

/*
**  This demo using SDL was made by OneThirty8, but is basically a
**  modification of the DirectDraw demo by Derek J. Evans.  The idea here
**  is to do basically the same thing, but also to be portable to
**  Mac OSX and Linux.  Surprisingly, this runs fairly well on the
**  Dreamcast without using the 3D hardware.
*/
//#include <pspkernel.h>
//#include <pspdebug.h>

#include <pspmoduleinfo.h>
#include <pspctrl.h>

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "game.h"

/* Define the module info section */
PSP_MODULE_INFO("yeti3d_psp", 1, 1, 1);

/* Define the main thread's attribute value (optional) */
//PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);


framebuffer_t framebuffer; //gives us somewhere to draw our stuff.
yeti_t yeti;
int oldTick = 0;		// Used to help limit the framerate.
int done = 0;			// Keeps track of whether or not we're still playing.
SDL_Surface *screen;	// our video surface
SDL_Event event;

void sdl3d_flip() // checks whether we're ready to draw a frame, and if so it draws it.
{
	int currentTick;
	int waitTicks;
    
	currentTick=SDL_GetTicks();

	oldTick += YETI_VIEWPORT_INTERVAL;
	waitTicks = (oldTick + (YETI_VIEWPORT_INTERVAL)) - currentTick;
	oldTick = currentTick;

	/* Only loop through the game loop and draw if enough
	   time has passed since the last frame.
	   The viewport_to_video function takes the frame we've drawm to
	   our 15-bpp 'framebuffer' in memory, converts it to 16-bit,
	   and throws it onto our actual video surface.  Look at yeti.c
	   to see what is going on under the hood here. */
	//if(waitTicks>0)
	//{
	//	SDL_Delay(waitTicks);
	//}

	game_loop(&yeti);
    
	viewport_to_video( (rgb555_t*)screen->pixels, screen->pitch, &yeti.viewport, 0xf800, 0x07e0, 0x001f);
	
	SDL_Flip(screen);
}



int main(int argc, char *argv[])
{
	//Uint8* keys;  // to keep track of keypresses later on.

	SceCtrlData pad;

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	//Initialize SDL video.
	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0 )
	{
		printf("Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);
	SDL_ShowCursor(0);
	
	// set up our video surface to have the same width and height as our viewport
	screen = SDL_SetVideoMode(YETI_VIEWPORT_WIDTH, YETI_VIEWPORT_HEIGHT, 16, SDL_SWSURFACE);
	if ( screen == NULL )
	{
		printf("Unable to set %d, %d 16-bit video: %s\n",YETI_VIEWPORT_WIDTH,
               YETI_VIEWPORT_HEIGHT, SDL_GetError());
		exit(1);
	}
  
	//Initiallize Yeti3D
	yeti_init(&yeti, &framebuffer, &framebuffer, textures, palette, lua);
	game_init(&yeti);

	while(done==0) // While we're not done playing, keep looping.
	{

		sceCtrlReadBufferPositive(&pad, 1);

		sdl3d_flip();
		/*while ( SDL_PollEvent(&event) ) // Here, if the user presses ESCAPE, we quit.
		{
			if ( event.type == SDL_QUIT )  {  done = 1;  }
			if ( event.type == SDL_KEYDOWN )
			{
				if ( event.key.keysym.sym == SDLK_ESCAPE ) { done = 1; }
			}
		} */

		if (pad.Buttons & PSP_CTRL_SELECT){
			sceKernelExitGame();
		}
 
		//keys = SDL_GetKeyState(NULL);
		yeti.keyboard.up     = pad.Buttons & PSP_CTRL_UP;
		yeti.keyboard.down   = pad.Buttons & PSP_CTRL_DOWN;
		yeti.keyboard.left   = pad.Buttons & PSP_CTRL_LEFT;
		yeti.keyboard.right  = pad.Buttons & PSP_CTRL_RIGHT;
		yeti.keyboard.a      = pad.Buttons & PSP_CTRL_SELECT;
		yeti.keyboard.b      = pad.Buttons & PSP_CTRL_START;
		yeti.keyboard.l      = pad.Buttons & PSP_CTRL_TRIANGLE;
		yeti.keyboard.r      = pad.Buttons & PSP_CTRL_CIRCLE;
		yeti.keyboard.select = pad.Buttons & PSP_CTRL_LTRIGGER;
	}

	return 0;
}









