#include <windows.h>

//#include "LimitSingleInstance.H"

#include "../main.h"




HGLRC openglrenderingcontext = 0;//opengl rendering context


HDC hDC = 0;//Private GDI Device Context
HWND hWnd = 0;//Holds Our Window Handle
HINSTANCE hInstance;//Holds The Instance Of The Application

bool keys[256];//Array Used For The Keyboard Routine
bool active = TRUE;//Window Active Flag Set To TRUE By Default
bool fullscreen = TRUE;//Fullscreen Flag Set To Fullscreen Mode By Default




droorinthing thedraw;




void ReSizeGLScene(GLsizei width, GLsizei height)
{
    
}





LRESULT CALLBACK WndProc(   HWND    hWnd,    // Handle For This Window
                            UINT    uMsg,    // Message For This Window
                            WPARAM  wParam,  // Additional Message Information
                            LPARAM  lParam)  // Additional Message Information
{
switch (uMsg)                               // Check For Windows Messages
    {
        case WM_ACTIVATE:                       // Watch For Window Activate Message
        {
            if (!HIWORD(wParam))                    // Check Minimization State
            {
                active = TRUE;                    // Program Is Active
            }
            else
            {
                active = FALSE;                   // Program Is No Longer Active
            }

            return 0;                       // Return To The Message Loop
        }

        case WM_SYSCOMMAND:                     // Intercept System Commands
        {
            switch (wParam)                     // Check System Calls
            {
                case SC_SCREENSAVE:             // Screensaver Trying To Start?
                case SC_MONITORPOWER:               // Monitor Trying To Enter Powersave?
                return 0;                   // Prevent From Happening
            }
            break;                          // Exit
        }

        case WM_CLOSE:                          // Did We Receive A Close Message?
        {
            PostQuitMessage(0);                 // Send A Quit Message
            return 0;                       // Jump Back
        }

        case WM_KEYDOWN:                        // Is A Key Being Held Down?
        {
            keys[wParam] = TRUE;                    // If So, Mark It As TRUE
            return 0;                       // Jump Back
        }

        case WM_KEYUP:                          // Has A Key Been Released?
        {
            keys[wParam] = FALSE;                   // If So, Mark It As FALSE
            return 0;                       // Jump Back
        }

        case WM_SIZE:                           // Resize The OpenGL Window
        {
            ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));       // LoWord=Width, HiWord=Height
            return 0;                       // Jump Back
        }
    }
// Pass All Unhandled Messages To DefWindowProc
return DefWindowProc(hWnd,uMsg,wParam,lParam);
}





















int DrawGLScene()
{
    POINT mousepos;
    if (GetCursorPos(&mousepos))
    {
        ScreenToClient(hWnd, &mousepos);
        //mousepos now says where in the window it is!
    }

    

    thedraw.draw();

    return true;
}








void KillGLWindow()
{
    thedraw.end();

    if (fullscreen)                             // Are We In Fullscreen Mode?
    {
        ChangeDisplaySettings(NULL,0);                  // If So Switch Back To The Desktop
        ShowCursor(TRUE);                       // Show Mouse Pointer
    }

    if (openglrenderingcontext)                                // Do We Have A Rendering Context?
    {
        if (!wglMakeCurrent(NULL,NULL))                 // Are We Able To Release The DC And RC Contexts?
        {
                MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        }

        if (!wglDeleteContext(openglrenderingcontext))                 // Are We Able To Delete The RC?
        {
                    MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        }
        openglrenderingcontext=NULL;                           // Set RC To NULL
    }

    if (hDC && !ReleaseDC(hWnd,hDC))                    // Are We Able To Release The DC
    {
        MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hDC=NULL;                           // Set DC To NULL
    }

    if (hWnd && !DestroyWindow(hWnd))                   // Are We Able To Destroy The Window?
    {
        MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hWnd=NULL;                          // Set hWnd To NULL
    }

    if (!UnregisterClass("OpenGL",hInstance))               // Are We Able To Unregister Class
    {
        MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hInstance=NULL;                         // Set hInstance To NULL
    }
}






bool CreateGLContext(HDC* devicecontext, HGLRC* oglrndrcntxt, int bits)//bits???
{
    static  PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),// Size Of This Pixel Format Descriptor
        1,//Version Number
        PFD_DRAW_TO_WINDOW |//Format Must Support Window
        PFD_SUPPORT_OPENGL |//Format Must Support OpenGL
        PFD_DOUBLEBUFFER,//Must Support Double Buffering
        PFD_TYPE_RGBA,//Request An RGBA Format
        bits,///example had 32 bit Color Depth
        0, 0, 0, 0, 0, 0,// Color Bits Ignored
        0,//No Alpha Buffer
        0,//Shift Bit Ignored
        0,//No Accumulation Buffer
        0, 0, 0, 0,//Accumulation Bits Ignored
        32,//32 bit? 24 Bit Z-Buffer (Depth Buffer) - was 16
        8,// 8 bit Stencil Buffer - was 0
        0,// # of Auxiliary Buffers
        PFD_MAIN_PLANE,// Main Drawing Layer
        0,// Reserved
        0, 0, 0//Layer Masks Ignored
    };

    GLuint PixelFormat = ChoosePixelFormat(*devicecontext,&pfd);
    if (!PixelFormat)
    {
        MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }

    if(!SetPixelFormat(*devicecontext,PixelFormat,&pfd))
    {
        MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }

    HGLRC tempContext;

    if (!(tempContext = wglCreateContext(*devicecontext)))// Are We Able To Get A Rendering Context?
    {
        MessageBox(NULL,"Can't Create A temp GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }

    if(!wglMakeCurrent(*devicecontext, tempContext))// Try To Activate The Rendering Context
    {
        MessageBox(NULL,"Can't Activate The temp GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }

    #ifdef TG_useopenglextensions
    if (GLEW_OK != glewInit())
	{
		MessageBox(NULL,"GLEW not initialised.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
	}
    #endif

    /*
    if(MessageBox(NULL,(const char*)glGetString(GL_VERSION),"Your OpenGL Version:-",MB_YESNO|MB_ICONQUESTION)==IDYES)
    {
        if(MessageBox(NULL,(const char*)glGetString(GL_VENDOR),"Your OpenGL Vendor:-",MB_YESNO|MB_ICONQUESTION)==IDYES)
        {
            if(MessageBox(NULL,(const char*)glGetString(GL_RENDERER),"Your OpenGL Renderer:-",MB_YESNO|MB_ICONQUESTION)==IDYES)
            {
                if(MessageBox(NULL,(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION),"Your GLSL Version:-",MB_YESNO|MB_ICONQUESTION)==IDYES)
                {
                    if(MessageBox(NULL,"No more..","lol",MB_YESNO|MB_ICONQUESTION)==IDYES)
                    {

                    }
                }
            }
        }
    }*/


    #ifdef TG_useopenglextensions
	if(wglewIsSupported("WGL_ARB_pixel_format") == 1)
	{
	    const int attribList[] =//list of integer attributes. Every two elements in the list is an attribute/value pair. The attribute "0" represents the end of the list
        {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, 32,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,
            0//0 = End
        };

	    int pixelFormats[10];
        unsigned int numFormats;

        if (wglChoosePixelFormatARB(*devicecontext, attribList, NULL, 10, pixelFormats, &numFormats))
        {
            for (unsigned int i = 0;i<numFormats;i++)
            {
                //pixelFormats[i] is useable.. but need a new window;
            }
        }
	}



    if(wglewIsSupported("WGL_ARB_create_context") == 1)//new function to replace wglCreateContext
    {
        int attribs[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 1,
            WGL_CONTEXT_FLAGS_ARB, 0,
            0
        };

        HGLRC newone = wglCreateContextAttribsARB(*devicecontext,0, attribs);
        if (newone)
        {
            *oglrndrcntxt = newone;
            wglMakeCurrent(NULL,NULL);
            wglDeleteContext(tempContext);
            wglMakeCurrent(*devicecontext, *oglrndrcntxt);
        }
        else
        {
            *oglrndrcntxt = tempContext;
        }

    }
    else
    {	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
        *oglrndrcntxt = tempContext;
    }
    #else
    *oglrndrcntxt = tempContext;
    #endif

    if (!*oglrndrcntxt)
    {
        return false;
    }

	return true;
}








//https://www.opengl.org/wiki/Tutorial:_OpenGL_3.1_The_First_Triangle_%28C%2B%2B/Win%29
//https://www.opengl.org/wiki/Creating_an_OpenGL_Context_%28WGL%29

//Windows does not allow the user to change the pixel format of a window.
//You get to set it exactly once. Therefore, if you want to use a different pixel format
//from the one your fake context used (for sRGB or multisample framebuffers, or just different bit-depths of buffers),
//you must destroy the window entirely and recreate it after we are finished with the dummy context.

bool CreateGLWindow(const char* title, int width, int height, int bits, bool fullscreenflag)
{
    hInstance = GetModuleHandle(NULL);// Grab An Instance For Our Window

    WNDCLASS    wc;// Windows Class Structure
    wc.style        = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;// Redraw On Move, And Own DC For Window
    wc.lpfnWndProc      = (WNDPROC) WndProc;        // WndProc Handles Messages
    wc.cbClsExtra       = 0;                        // No Extra Window Data
    wc.cbWndExtra       = 0;                        // No Extra Window Data
    wc.hInstance        = hInstance;                // Set The Instance
    wc.hIcon        = LoadIcon(NULL, IDI_WINLOGO);  // Load The Default Icon
    wc.hCursor      = LoadCursor(NULL, IDC_ARROW);  // Load The Arrow Pointer
    wc.hbrBackground    = NULL;                     // No Background Required For GL
    wc.lpszMenuName     = NULL;                     // We Don't Want A Menu
    wc.lpszClassName    = "OpenGL";                 // Set The Class Name

    if (!RegisterClass(&wc))                        // Attempt To Register The Window Class
    {
        MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }

    fullscreen = fullscreenflag;// Set The Global Fullscreen Flag

    if (fullscreen)// Attempt Fullscreen Mode?
    {
        DEVMODE dmScreenSettings;                   // Device Mode
        memset(&dmScreenSettings,0,sizeof(dmScreenSettings));       // Makes Sure Memory's Cleared
        dmScreenSettings.dmSize=sizeof(dmScreenSettings);       // Size Of The Devmode Structure
        dmScreenSettings.dmPelsWidth    = width;            // Selected Screen Width
        dmScreenSettings.dmPelsHeight   = height;           // Selected Screen Height
        dmScreenSettings.dmBitsPerPel   = bits;             // Selected Bits Per Pixel
        dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
        if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
        {
            // If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
            if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","TG",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
            {
                fullscreen = false;
            }
            else
            {
                // Pop Up A Message Box Letting User Know The Program Is Closing.
                MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
                return false;
            }
        }
    }

    DWORD dwStyle;// Window Style
    DWORD dwExStyle;// Window Extended Style

    if (fullscreen)
    {
        dwExStyle = WS_EX_APPWINDOW; // Window Extended Style
        dwStyle = WS_POPUP;          // Windows Style
        ShowCursor(FALSE);           // Hide Mouse Pointer
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // Window Extended Style
        dwStyle = WS_OVERLAPPEDWINDOW;                    // Windows Style
    }

    RECT WindowRect;                 // Grabs Rectangle Upper Left / Lower Right Values
    WindowRect.left=(long)0;         // Set Left Value To 0
    WindowRect.right=(long)width;    // Set Right Value To Requested Width
    WindowRect.top=(long)0;          // Set Top Value To 0
    WindowRect.bottom=(long)height;  // Set Bottom Value To Requested Height

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // Adjust Window To True Requested Size

    if (!(hWnd=CreateWindowEx(  dwExStyle,          // Extended Style For The Window
                "OpenGL",                           // Class Name
                title,                              // Window Title
                WS_CLIPSIBLINGS |                   // Required Window Style
                WS_CLIPCHILDREN |                   // Required Window Style
                dwStyle,                            // Selected Window Style
                0, 0,                               // Window Position
                WindowRect.right-WindowRect.left,   // Calculate Adjusted Window Width
                WindowRect.bottom-WindowRect.top,   // Calculate Adjusted Window Height
                NULL,                               // No Parent Window
                NULL,                               // No Menu
                hInstance,                          // Instance
                NULL)))                             // Don't Pass Anything To WM_CREATE
        {
            KillGLWindow();
            MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
            return FALSE;
        }

    if (!(hDC=GetDC(hWnd)))// Did We Get A Device Context?
    {
        KillGLWindow();
        MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if (!(CreateGLContext(&hDC, &openglrenderingcontext, bits)))
    {
        KillGLWindow();
        return FALSE;
    }

    ShowWindow(hWnd,SW_SHOW);  // Show The Window
    SetForegroundWindow(hWnd); // Slightly Higher Priority
    SetFocus(hWnd);            // Sets Keyboard Focus To The Window

    if (! myengine->initgl() )
    {
        KillGLWindow();
        MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    ReSizeGLScene(width, height);

    return true;//ok
}





















//#include "winscreenmodes.h"

int WINAPI WinMain( HINSTANCE   hInstance,              // Instance - ALL UNUSED!!
                    HINSTANCE   hPrevInstance,          // Previous Instance - unused
                    LPSTR       lpCmdLine,              // Command Line Parameters - unused
                    int         nCmdShow)               // Window Show State - unused
{
    /*CLimitSingleInstance instancelimiter(    (char*)TEXT("Global\\{64c12072-f81d-4a2f-b856-cbfe8cbd7ba1}")      );///http://www.guidgen.com/
    if (instancelimiter.IsAnotherInstanceRunning())
    {
        MessageBox(NULL,"Already running!!","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return -1;
    }*/

    if (MessageBox(NULL,"Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
    {
        fullscreen = false;
    }

    thedraw.start();
    

    //screenmodelist listy;
    //int mode = listy.numofmodes;
    int width = 100;//listy.size[(mode-1)*2];
    int height = 100;//listy.size[((mode-1)*2)+1];



    if (!CreateGLWindow("mer",640,480,32,fullscreen))
    {
        
        return -1;//quit
    }


    MSG msg;// Windows Message Structure
    bool done = false;


    

    while(!done)
    {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                done = true;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else// If There Are No Messages
        {
            if (active)// Program Active? - otherwise, did i ought to update anyway? or send a pause game message to engine?
            {
                if (keys[VK_ESCAPE])
                {
                    done = true;
                }
                else
                {
                    //

                    DrawGLScene();
                    SwapBuffers(hDC);// Swap Buffers (Double Buffering)
                    glFinish();

                    
                }
            }
            if (keys[VK_F1])
            {
                keys[VK_F1] = false;///set false!
                KillGLWindow();
                fullscreen =! fullscreen;

                if (fullscreen)
                {
                    if (!CreateGLWindow("mer",width,height,32,fullscreen))
                    {
                        
                        return -1;
                    }
                }
                else
                {
                    if (!CreateGLWindow("mer",640,480,32,fullscreen))
                    {
                        
                        return -1;
                    }
                }
            }
        }
    }
    // Shutdown
    KillGLWindow();

    thedraw.end();

    return (msg.wParam);//which is 27??
}
