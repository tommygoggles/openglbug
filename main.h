
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
    #ifdef _MSC_VER && !__INTEL_COMPILER//silly visual studio
    #include <windows.h>
    #endif
#include <GL/gl.h>
    //link library
    //opengl32
#endif





class droorinthing
{
    public:
    void draw();
    void start();
    void end();
    void wirey();
    void line();
    void texturator(GLuint* tex, unsigned char r, unsigned char g, unsigned char b);
    void flipit();
    void flipfixit();
       
    bool fix;
    bool wire;

    GLuint tex1;
    GLuint tex2;
    
    droorinthing();
    ~droorinthing();
};
