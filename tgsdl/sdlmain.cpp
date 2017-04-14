
#include <SDL/SDL.h>



#include "../main.h"





droorinthing thedraw;



void clear_events()
{
	SDL_Event event;
	SDL_Delay(100);
	while(SDL_PollEvent(&event))
		SDL_Delay(10);
}

int main(int argc, char ** argv)
{
    int width = 640;
    int height = 480;

	bool done = false;




	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK);
	SDL_JoystickEventState(SDL_ENABLE);
	SDL_JoystickOpen(0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 1);
	SDL_SetVideoMode(width,height,16,SDL_DOUBLEBUF | SDL_SWSURFACE | SDL_OPENGL);


	clear_events();

	SDL_Event event;


	Uint32 start_ticks = 0, end_ticks = 0;
    Uint8* keystate = NULL;
    int numkeys = 0;


	thedraw.start();


	while(!done)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
				//case SDL_KEYDOWN:
				//case SDL_JOYBUTTONDOWN:
				done = true;
				break;
			}
		}


        keystate = SDL_GetKeyState(&numkeys);
        if ( keystate[SDLK_ESCAPE] )
        {
            done = true;
        }
        





        thedraw.draw();


		glFlush();//what does it do? is it needed?

		SDL_GL_SwapBuffers();

	}
	thedraw.end();
	SDL_Quit();
	return 0;
}
