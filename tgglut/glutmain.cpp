#include "../main.h"



#ifdef __APPLE__
#include <GLUT/glut.h>
//-framework GLUT
#else
#include <GL/glut.h>
#endif





droorinthing thedraw;
int windowid;


void display(void)
{
    thedraw.draw();
    glutSwapBuffers();
}

void idle(void)
{

	glutPostRedisplay();
}

void visible(int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc(idle);
  else
    glutIdleFunc(0);
}



///ascii only :( x and y are mouse pos?
void readkeys (unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27: // Escape key
        
		break;

		case '2':
       thedraw.flipfixit();
    break;

    case '3':
      thedraw.flipit();
    break;



	}
}


void reshape(int width, int height)
{
    
}


void mousefunc(int button, int state, int x, int y)
{
    
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	windowid = glutCreateWindow("mer");

	glutDisplayFunc(display);
	glutVisibilityFunc(visible);
	glutKeyboardFunc(readkeys);
	glutMouseFunc(mousefunc);
	glutReshapeFunc(reshape);

	glutReshapeWindow(640,480);



	thedraw.start();

	glutMainLoop();///never returns!!!!

}
