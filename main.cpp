


    
    #include "main.h"



    

    void droorinthing::texturator(GLuint* tex, unsigned char r, unsigned char g, unsigned char b)
    {
        if(!(*tex))
        {
            glGenTextures(1, tex) ;

            glBindTexture(GL_TEXTURE_2D, *tex);

            unsigned char text[4*4*4];//4*w*h
            for(unsigned int x = 0;x<4;x++)
            {
                for(unsigned int y = 0;y<4;y++)
                {
                    unsigned char* pix = &(text[((y*4)+x)*4]);
                    pix[0] = x*r;
                    pix[1] = y*g;
                    pix[2] = x*b;
                    pix[3] = 255;
                }
            }

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, text);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else
        {
            if(fix)
            {
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            glBindTexture(GL_TEXTURE_2D, *tex);
        }
    }





    void droorinthing::draw()
    {



        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        GLfloat mat1[] = {1.0, 0, 0, 0,    0, 1.0, 0, 0,     0, 0, 1.0, 0,     0, 0, 0, 1};
        glLoadMatrixf(mat1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        GLfloat verts[] = {0.0f,0.0f,0.0f,  1.0f,0.0f,0.0f, 1.0f,1.0f,0.0f, 0.0f,1.0f,0.0f};
        GLfloat uvs[] = {0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f,1.0f};

        glVertexPointer(3, GL_FLOAT, 0, verts);
        glTexCoordPointer(2, GL_FLOAT, 0, uvs);



        texturator(&tex1,0,60,60);


        GLuint elem[] = {0,1,2, 2,3,0};
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, elem);



        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        GLfloat mat2[] = {1, 0, 0, 0,    -0, 1, 0, 0,     0, 0, 1, 0,    -1, 0, 0, 1};
        glLoadMatrixf(mat2);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();



        texturator(&tex2,60,10,0);



        glTexCoordPointer(2, GL_FLOAT, 0, uvs);
        glVertexPointer(3, GL_FLOAT, 0, verts);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


    }

    droorinthing::droorinthing()
    {
        fix = false;
        wire = false;

        tex1 = 0;
        tex2 = 0;
    }

    
    droorinthing::~droorinthing()
    {
        
    }


    void droorinthing::start()
    {
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    void droorinthing::end()
    {
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        glDeleteTextures(1, &tex1);
        glDeleteTextures(1, &tex2);
    }

    void droorinthing::wirey()
    {
        glPolygonMode(GL_FRONT, GL_FILL);
    }

    void droorinthing::line()
    {
        glPolygonMode(GL_FRONT, GL_LINE);
    }

    void droorinthing::flipit()
    {
        if(wire)
        {
            wirey();
        }
        else
        {
            line();
        }
        wire = !wire;
    }

    void droorinthing::flipfixit()
    {
        fix = !fix;
    }
