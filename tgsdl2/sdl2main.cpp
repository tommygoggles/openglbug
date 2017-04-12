    //linker options
    //-lmingw32 -lSDL2main -lSDL2

    //include SDL2 dirs-
    //compiler-
    //SDL2-2.0.4\i686-w64-mingw32\include\SDL2
    //linker-
    //SDL2-2.0.4\i686-w64-mingw32\lib


    #include <SDL.h>
    #include "../main.h"



    droorinthing thedraw;


    int main(int argc, char *argv[])
    {
        SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);

        SDL_Window* mywindow = SDL_CreateWindow("press q or w to toggle fixes...",100,100,640,480,SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);

        SDL_GL_CreateContext(mywindow);

        thedraw.start();

        bool loop = true;
        
        while (loop)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    loop = false;

                if (event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        loop = false;
                        break;
                    case SDLK_q:
                        thedraw.flipfixit();
                    break;

                    case SDLK_w:
                        thedraw.flipit();
                    break;




                    default:
                        break;
                    }
                }
            }
            thedraw.draw();
            SDL_GL_SwapWindow(mywindow);
        }
        thedraw.end();


        return 0;
    }

