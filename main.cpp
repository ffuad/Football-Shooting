#include<GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<math.h>
#include<bits/stdc++.h>

#include<iostream>
#include<Windows.h>
#include<MMSystem.h>

using namespace std;

float currentX = 5, currentY = -5, currentZ = 58.5;
GLfloat xTarget = 16, yTarget = -5, zTarget = 33;
GLfloat fcurrentX = 5, fcurrentY = -5, fcurrentZ = 58.5;
bool restart = false;
bool restart1 = false;

double Txval=0,Tyval=0,Tzval=0;
double windowHeight=1800, windowWidth=1000;
GLfloat alpha = 0.0, theta = 0.0, gama = 0.0,zeta=0.0, axis_x=0.0, axis_y=0.0, axis_z = 0.0;
GLboolean bRotate = false, uRotate = false, oRotate= false;
bool chk = true;
GLUquadricObj *quadratic,*quadratic1;
GLboolean bR = false,light_0_on = false, light_1_on = true, light_2_on = false;
GLboolean amb_light = true, dif_light = true, spec_light = true;

GLdouble ttx,tty,ttz=0;
GLfloat theta_pakha = 0;


GLint skybox[6],grass;
const int SKY_FRONT=0,SKY_RIGHT=1,SKY_LEFT=2,SKY_BACK=3,SKY_UP=4,SKY_DOWN=5;
GLdouble movcord[3]={-150,-10,200};

GLfloat xaxis = 0,yaxis = 1,zaxis = 0;

GLfloat eyeX = 10,eyeY = 0,eyeZ = 100;

GLfloat lookX = 0,lookY = 0,lookZ = 0;

GLfloat lpan = 0,tpan = 0, bpan = 0, rpan = 0;

GLfloat roll = 0,dx = 0,dy = 0,dz = 0,yaw = 0,pitch = 0;


GLfloat a_x=1, a_y=1, a_z=1;
float p1i = 0, p1j = 0, p1k = 0, p1x,p1y,p1z;
float p2i = 0, p2j = 0, p2k = 0, p2x,p2y,p2z;

bool mov_P1=0, mov_P2=0;

GLuint ID_[100];
int i=1;
float getPt( float n1 , float n2 , float perc ) {
    float diff = n2 - n1;
    return n1 + ( diff * perc );
}

struct Point{
    float x,y,z;
};

vector<Point>ballPathPoint;
int pos;
bool KickBall;

float BallMoving = 1;
GLuint LoadBMP(const char *fileName)
{
	FILE *file;
	unsigned char header[54],*data;
	unsigned int dataPos,size,width, height;
	file = fopen(fileName, "rb");
	fread(header, 1, 54, file);				//Windows BMP begin with 54 byte header
	dataPos		= *(int*)&(header[0x0A]);	//dec10, Actual BMP data
	size		= *(int*)&(header[0x22]);	//dec34, BMP Size
	width		= *(int*)&(header[0x12]);	//dec18, Image Width
	height		= *(int*)&(header[0x16]);	//dec22, Image Height
	if (size == NULL)
		size = width * height * 3;
	if (dataPos == NULL)
		dataPos = 54;
	data = new unsigned char[size];
	fread(data, 1, size, file);
	fclose(file);
	GLuint texture;
	glGenTextures(1, &texture);				//Generate (allocate) 1 texture name
	glBindTexture(GL_TEXTURE_2D, texture);	//Bind the 2D texture



	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//MAG filter
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	//MIN filter

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data); //target, level, internalFormat, width, height,border, format, type, data
	return texture;
}




void light(GLfloat x, GLfloat y, GLfloat z, GLenum L, bool light_on, bool spot)
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {.5, .5, .5, 1.0};
    GLfloat light_diffuse[]  = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat light_position[] = { x, y, z, 1.0 };

    glEnable(L);

    if(light_on){

        if(amb_light) glLightfv( L, GL_AMBIENT, light_ambient);
        else glLightfv( L, GL_AMBIENT, no_light);

        if(dif_light) glLightfv( L, GL_DIFFUSE, light_diffuse);
        else glLightfv( L, GL_DIFFUSE, no_light);

        if(spec_light) glLightfv( L, GL_SPECULAR, light_specular);
        else glLightfv( L, GL_SPECULAR, no_light);

        glLightfv( L, GL_POSITION, light_position);
    }

    else{
        glLightfv( L, GL_AMBIENT, no_light);
        glLightfv( L, GL_DIFFUSE, no_light);
        glLightfv( L, GL_SPECULAR, no_light);
        glLightfv( L, GL_POSITION, no_light);
    }

    if(spot){
        GLfloat spot_direction[] = { 1.0, 1.0, 1 };
        glLightfv(L, GL_SPOT_DIRECTION, spot_direction);
        glLightf( L, GL_SPOT_CUTOFF, 55);
    }

}











void material_color(float am_r, float am_g, float am_b, float df_r,float df_g,float df_b)
{
    GLfloat mat_ambient[] = { am_r, am_g, am_b, 1.0 };
    GLfloat mat_diffuse[] = { df_r, df_g, df_b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

}
static GLfloat v_box[8][3] =
{
    {-1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {-1.0, 0.0, 1.0},
    {1.0, 0.0, 1.0},

    {-1.0, 1.0, 0.0},
    {1.0, 1.0, 0.0},
    {-1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0}
};

static GLubyte quadIndices[6][4] =
{
    {0,2,3,1},
    {0,2,6,4},
    {2,3,7,6},
    {1,3,7,5},
    {1,5,4,0},
    {6,7,5,4}
};

static void getNormal3p
(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}


void drawBox(float am_r, float am_g, float am_b, float df_r,float df_g,float df_b,float sp_r=1.0,float sp_g=1.0,float sp_b=1.0,float shininess = 50)
{
    material_color(am_r,am_g,am_r, df_r,df_g,df_b);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_box[quadIndices[i][0]][0], v_box[quadIndices[i][0]][1], v_box[quadIndices[i][0]][2],
                    v_box[quadIndices[i][1]][0], v_box[quadIndices[i][1]][1], v_box[quadIndices[i][1]][2],
                    v_box[quadIndices[i][2]][0], v_box[quadIndices[i][2]][1], v_box[quadIndices[i][2]][2]);

        glVertex3fv(&v_box[quadIndices[i][0]][0]);glTexCoord2f(1,1);
        glVertex3fv(&v_box[quadIndices[i][1]][0]);glTexCoord2f(1,0);
        glVertex3fv(&v_box[quadIndices[i][2]][0]);glTexCoord2f(0,0);
        glVertex3fv(&v_box[quadIndices[i][3]][0]);glTexCoord2f(0,1);
    }
    glEnd();
}

void draw_Sphere()
{
    glPushMatrix();
    glScalef(.2,.2,.2);
    material_color(1,1,1,1,1,1);

    glutSolidSphere(2,100,100);
    glPopMatrix();


}

void draw_sphere(int scalex, int scaley, GLfloat r)
{


    int i, j;
    GLfloat v[scalex*scaley][3];

    for (i=0; i<scalex; ++i)
    {
        for (j=0; j<scaley; ++j)
        {
            v[i*scaley+j][0]=r*cos(j*1.5*M_PI/scaley)*cos(i*M_PI/(1.5*scalex));
            v[i*scaley+j][1]=r*sin(i*M_PI/(1.5*scalex));
            v[i*scaley+j][2]=r*sin(j*1.5*M_PI/scaley)*cos(i*M_PI/(1.5*scalex));
        }
    }

    glBegin(GL_QUADS);
    for (i=0; i<scalex-1; ++i)
    {
        for (j=0; j<scaley; ++j)
        {
            glTexCoord2f(0,0);
            glVertex3fv(v[i*scaley+j]);
            glTexCoord2f(1,0);
            glVertex3fv(v[i*scaley+(j+1)%scaley]);
            glTexCoord2f(1,1);
            glVertex3fv(v[(i+1)*scaley+(j+1)%scaley]);
            glTexCoord2f(0,1);
            glVertex3fv(v[(i+1)*scaley+j]);
        }
    }
    glEnd();


}

void draw_tree()
{
    glTranslatef(0,6,0);
    glScalef(2,2,2);
    quadratic = gluNewQuadric();
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glBindTexture(GL_TEXTURE_2D, ID_[9]);
        //glTranslatef(0,4,0);
        glRotatef(90,1,0,0);
        material_color(1,1,1,1,1,1);
        gluCylinder(quadratic,0.4f,0.4f,6.0f,32,32);
        //draw_cylinder(.5,4,100);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,-2,0);
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glBindTexture(GL_TEXTURE_2D, ID_[10]);
            glTranslatef(0,4,0);
            glScalef(.2,.3,.3);
            draw_sphere(15,15,6.5);
            glDisable(GL_TEXTURE_GEN_T);
            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glBindTexture(GL_TEXTURE_2D, ID_[10]);
            glTranslatef(0,2,0);
            glScalef(.3,.3,.3);
            //glRotatef(90,0,1,0);
            //glRotatef(180,1,0,0);
            draw_sphere(15,15,6.5);
            glDisable(GL_TEXTURE_GEN_T);
            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    glPopMatrix();


}


void drawfield()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 1);
    glPushMatrix();
    glScalef(10,.01,8);
    glRotatef(90,0,1,0);
    drawBox(1,1,1,0,0,0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void draw_room()
{
    //floor
    //glEnable(GL_TEXTURE_2D);
    glScalef(-.2,-.2,-.2);
    glPushMatrix();
    glTranslatef(0,0,-500000);
    //glScalef(.01,40,40);
    glScalef(1000000,.01,1000000);
    //drawBox(.5,1,.5,.5,0,0);
    glPopMatrix();
    //floor

    //wall1
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ID_[12]);
    glPushMatrix();
    glTranslatef(0,0,30);
    glScalef(2500,2500,.01);
    glRotatef(180,0,0,1);
    material_color(1,1,1,1,1,1);
    drawBox(1,0,0,.5,0,0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //wall1

    glPopMatrix();
}

void drawboundary()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ID_[8]);
    glPushMatrix();
        glTranslatef(22,-7.01,3);
        glScalef(47,0.01,81);
        drawBox(1,1,1,0,0,0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    ///vertical lines
    glPushMatrix();
        glTranslatef(-0.3,-7,7);
        glScalef(0.3,0.01,65);
        drawBox(1,1,1,0,0,0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(40.4,-7,7);
        glScalef(0.3,0.01,65);
        drawBox(1,1,1,0,0,0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-23,-7,3.6);
        glScalef(0.3,0.01,80.6);
        drawBox(1,1,1,0,0,0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(67,-7,3.6);
        glScalef(0.3,0.01,80.6);
        drawBox(1,1,1,0,0,0);
    glPopMatrix();
    ///horizontal lines

    glPushMatrix();
        glTranslatef(40.7,-7,7);
        glRotatef(-90,0,1,0);
        glScalef(0.3,0.01,41.2);
        drawBox(1,1,1,0,0,0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(40.7,-7,72);
        glRotatef(-90,0,1,0);
        glScalef(0.3,0.01,41.2);
        drawBox(1,1,1,0,0,0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-25,-7,5);
        glRotatef(90,0,1,0);
        glScalef(0.3,0.01,94);
        drawBox(1,1,1,0,0,0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-26,-7,82);
        glRotatef(90,0,1,0);
        glScalef(0.3,0.01,94);
        drawBox(1,1,1,0,0,0);
    glPopMatrix();
}

void floodlight(GLfloat x,GLfloat y,GLfloat z)
{
    ///base
    glPushMatrix();
        glTranslatef(-22.5,-7.01,-2);
        //glRotatef(90,0,1,0);
        glScalef(2,2,2.5);
        drawBox(.5,.5,0,.5,.5,0);
    glPopMatrix();
    ///cylinder stand
    glPushMatrix();
        GLUquadricObj *quadric;
        quadric = gluNewQuadric();
        glTranslatef(-22.5,30,-2);
        glRotatef(90,1,0,0);
        //glScalef(2,2,2.5);
        material_color(0,0,0,1,1,1);
        gluCylinder(quadric,0.5,0.5,35,50,50);
    glPopMatrix();
    ///lights
    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(30,1,0,0);
    glPushMatrix();
        quadric = gluNewQuadric();
        glTranslatef(-22.5,35,0);
        glRotatef(90,1,0,0);
        //glScalef(2,2,2.5);
        material_color(0,0,0,1,1,1);
        gluCylinder(quadric,0.5,0.5,12,50,50);
    glPopMatrix();
    for(int i=0;i<16;i+=4){
    glPushMatrix();
        glTranslatef(-22,35-i,-1.5);
        glScalef(4,0.4,0.1);
        drawBox(1,1,0,1,1,0);
    glPopMatrix();
    }
    for(int i=0;i<16;i+=4){
    glPushMatrix();
        glTranslatef(-22,35-i,0.5);
        glRotatef(180,0,1,0);
        //glScalef(0.8,1,1);
        material_color(0.8,0.8,0.8,0.8,0.8,0.8);
        glutSolidCone(1,2,50,50);
    glPopMatrix();
    }
        for(int i=0;i<16;i+=4){
    glPushMatrix();
        glTranslatef(-19,35-i,0.5);
        glRotatef(180,0,1,0);
        material_color(0.8,0.8,0.8,0.8,0.8,0.8);
        glutSolidCone(1,2,50,50);
    glPopMatrix();
    }
        for(int i=0;i<16;i+=4){
    glPushMatrix();
        glTranslatef(-25,35-i,0.5);
        glRotatef(180,0,1,0);
        material_color(0.8,0.8,0.8,0.8,0.8,0.8);
        glutSolidCone(1,2,50,50);
    glPopMatrix();
    }
    glPopMatrix();
}

void drawchair()
{
    ///LEGS
    glPushMatrix();
    glTranslatef(-3.8,0,3.2);
    glPushMatrix();
    glScaled(0.2,1.5,0.2);
    glTranslatef(-6,0,-6);
    drawBox(.5,.5,.5,.5,0,0);
    glPopMatrix();

    glPushMatrix();
    glScaled(0.2,1.5,0.2);
    glTranslatef(6,0,-6);
    drawBox(.5,.5,.5,.5,0,0);
    glPopMatrix();

    glPushMatrix();
    glScaled(0.2,1.5,0.2);
    glTranslatef(-6,0,6);
    drawBox(.5,.5,.5,.5,0,0);
    glPopMatrix();

    glPushMatrix();
    glScaled(0.2,1.5,0.2);
    glTranslatef(6,0,6);
    drawBox(.5,.5,.5,.5,0,0);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-.2);
    glPushMatrix();
    glScaled(2,0.3,1.8);
    glTranslatef(-1,-0.2,1.1);
    drawBox(.5,.5,.5,.5,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5,-4,2);
    glScaled(0.3,2.1,1.8);
    drawBox(.5,.5,.5,.5,0,0);
    glPopMatrix();
    glPopMatrix();
}





void goalpost()
{
//    GLUquadricObj *quadratic,*quadratic1;
    quadratic = gluNewQuadric();
    quadratic1 = gluNewQuadric();

//

    glPushMatrix();
    glTranslatef(-2,2.6,-42);
    glScalef(3,3,3);
    glRotatef(90,1,0,0);

    glPushMatrix();
    glTranslatef(-0.06,0,0);
    material_color(1,1,1, 1,1,1);
    gluCylinder(quadratic,0.08f,0.08f,3.0f,32,32);
    glPopMatrix();

    glPushMatrix();
    //glTranslatef(3,-1.5,0);
    glRotatef(90,1,0,0);
    material_color(1,1,1, 1,1,1);
    gluCylinder(quadratic,0.08f,0.08f,1.5f,32,32);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3,0,0);
    glRotatef(90,1,0,0);
    material_color(1,1,1, 1,1,1);
    gluCylinder(quadratic,0.08f,0.08f,1.5f,32,32);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3,0,0);
    material_color(1,1,1, 1,1,1);
    gluCylinder(quadratic,0.08f,0.08f,3.0f,32,32);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.04,0,0);
    glRotatef(270,0,1,0);
    material_color(1,1,1, 1,1,1);
    gluCylinder(quadratic1,0.08f,0.08f,3.14f,32,32);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.04,-1.5,0);
    glRotatef(270,0,1,0);
    material_color(1,1,1,1,1,1);
    gluCylinder(quadratic1,0.08f,0.08f,3.0f,32,32);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-1.5,0);
    for(int i=0;i<15;i++){
        glPushMatrix();
        glTranslatef(3-i*.2,0,0);
        material_color(1,1,1, 1,1,1);
        gluCylinder(quadratic,0.02f,0.02f,3.0f,32,32);
        glPopMatrix();
    }
    for(int i=0;i<7;i++){
        glPushMatrix();
        glTranslatef(3,i*.2,0);
        material_color(1,1,1, 1,1,1);
        gluCylinder(quadratic,0.02f,0.02f,3.0f,32,32);
        glPopMatrix();
    }
    for(int i=0;i<7;i++){
        glPushMatrix();
        glTranslatef(0,i*.2,0);
        material_color(1,1,1, 1,1,1);
        gluCylinder(quadratic,0.02f,0.02f,3.0f,32,32);
        glPopMatrix();
    }

    for(int i=0;i<7;i++)
    {
        glPushMatrix();
        glTranslatef(0,i*.1,0);
        glRotatef(90,0,1,0);
        gluCylinder(quadratic,0.01f,0.01f,3.0f,32,32);
        glPopMatrix();
    }
    for(int i=0;i<15;i++)
    {
        glPushMatrix();
        glTranslatef(3-i*.2,1.5,0);
        glRotatef(90,1,0,0);
        //glRotatef(90,0,1,0);
        gluCylinder(quadratic,0.01f,0.01f,1.5f,32,32);
        glPopMatrix();
    }
    for(int i=0;i<15;i++)
    {
        glPushMatrix();
        glTranslatef(0,0,3-i*.2);
        glRotatef(270,1,0,0);
        //glRotatef(90,0,1,0);
        gluCylinder(quadratic,0.02f,0.02f,1.5f,32,32);
        glPopMatrix();
    }
    for(int i=0;i<15;i++)
    {
        glPushMatrix();
        glTranslatef(3,0,3-i*.2);
        glRotatef(270,1,0,0);
        //glRotatef(90,0,1,0);
        gluCylinder(quadratic,0.02f,0.02f,1.5f,32,32);
        glPopMatrix();
    }
    for(int i=0;i<15;i++)
    {
        glPushMatrix();
        glTranslatef(0,0,3-i*.2);
        glRotatef(270,1,0,0);
        glRotatef(90,0,1,0);
        gluCylinder(quadratic,0.02f,0.02f,3.0f,32,32);
        glPopMatrix();
    }
    glPopMatrix();

    glPopMatrix();
}

void drawgallery()
{
//gallery1
    glPushMatrix();

    glPushMatrix();
    glTranslatef(0,0,4);
    glScalef(1,1,2);
    for(int i=1;i<=10;i++)
    {
        glPushMatrix();
        glTranslatef(-30-i,0,0);
        glScalef(5-i*.5,2*i,40);
        drawBox(1.3,1.3-i*.1,.5,.5,0,0);
        glPopMatrix();
    }
    glPopMatrix();
// gallery 2
    glPushMatrix();
    glTranslatef(-10,0,4);
    glScalef(1,1,2);
    for(int i=1;i<=10;i++)
    {
        glPushMatrix();
        glTranslatef(80+i,0,0);
        glScalef(5-i*.5,2*i,40);
        drawBox(1.3,1.3-i*.1,.5,.5,0,0);
        glPopMatrix();
    }
    glPopMatrix();

//gallery3
    glPushMatrix();
        glTranslatef(60,0,30);
        glRotatef(270,0,1,0);
        glScalef(1,1,2);
        for(int i=1;i<=10;i++)
        {
            glPushMatrix();
            glTranslatef(-30-i,0,0);
            glScalef(5-i*.5,2*i,40);
            drawBox(1.3,1.3-i*.1,.5,.5,0,0);
            glPopMatrix();
        }
    glPopMatrix();
    glPopMatrix();

}

void player()
{
    quadratic = gluNewQuadric();

    glPushMatrix();
        //glTranslatef(10,4,-10);
        glScalef(5,5,5);
        //material_color(0,0,0,0,0,1);
        glPushMatrix();
        glTranslatef(0,3.8,0);
        glScalef(.5,.5,.5);
        draw_Sphere();
        glPopMatrix();
    ///neck
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glBindTexture(GL_TEXTURE_2D, ID_[7]);
        material_color(1,1,1, 1,1,1);
        glTranslatef(0,3.9,0);
        glRotatef(90,1,0,0);
        gluCylinder(quadratic,0.05f,0.05f,.8f,32,32);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    ///body
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, ID_[6]);
        glTranslated(0,2,-.1);
        glScalef(.2,1.5,.25);
        drawBox(1,1,1,1,1,1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, ID_[7]);
    ///legs
        glPushMatrix();
        glTranslatef(-.11,.6,-.1);
        glScalef(.05,1.5,.1);
        drawBox(1,1,1,1,1,1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(.13,.6,-.1);
        glScalef(.05,1.5,.1);
        drawBox(1,1,1,1,1,1);
        glPopMatrix();
    ///hands
        glPushMatrix();
        glTranslatef(.55,2.5,0);
        glRotatef(180,1,0,0);
        glRotatef(160,0,0,1);
        glRotatef(90,0,1,0);
        glScalef(.05,1,.05);
        drawBox(1,1,1,1,1,1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-.15,3.4,0);
        glRotatef(160,0,0,1);
        glRotatef(90,0,1,0);
        glScalef(.05,1,.05);
        drawBox(1,1,1,1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}


GLfloat xx=0;

int chhk=0;
void bird_swap()
{
    if(chhk){
        int tmp = eyeY;
        eyeY = eyeZ;
        eyeZ = tmp;

        tmp = yaxis;
        yaxis = zaxis;
        zaxis = tmp;
    }
    else {
        int tmp = eyeY;
        eyeY = eyeZ;
        eyeZ = tmp;

        tmp = yaxis;
        yaxis = zaxis;
        zaxis = tmp;
    }
}

void draw_audi()
{
    glPushMatrix();
    glScalef(.2,.2,.1);
    glTranslatef(0,-2.5,0);
    drawBox(1,1,1,0,0,1);
    glPopMatrix();

    glPushMatrix();
    glScalef(.35,.5,.1);
    glTranslatef(-.45,-3,0);
    drawBox(1,1,0,0,0,1);
    glPopMatrix();

    glPushMatrix();
    glScalef(.2,.1,.25);
    glTranslatef(0,-15,0);
    drawBox(1,0,0,0,0,1);
    glPopMatrix();

    glPushMatrix();
    glScalef(.2,.25,.1);
    glTranslatef(0,-8,-3);
    drawBox(1,0,0,0,0,1);
    glPopMatrix();
}

void draw_people()
{
    glPushMatrix();
    //glTranslatef(ttx,20,ttz);
    glRotatef(180,0,1,0);
    glTranslatef(-1,0,29);
    glScalef(2,2,2);
    draw_audi();
    glPopMatrix();
}

//void collid()
//{
//    if()
//}

class Sound {
    public:
        void ballKick()
        {
            PlaySound(NULL, 0, 0);
            PlaySound(TEXT("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\ball.wav"), NULL, SND_ASYNC | SND_FILENAME);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            PlaySound(NULL, 0, 0);
        }
        void ballReceived()
        {
            PlaySound(NULL, 0, 0);
            PlaySound(TEXT("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\ball.wav"), NULL, SND_ASYNC | SND_FILENAME);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            PlaySound(NULL, 0, 0);
        }
};
Sound sound;



void target()
{
    glPushMatrix();
        glTranslatef(xTarget, yTarget, zTarget);
        //glTranslatef(ttx,0,ttz);
        glScalef(.2,.2,.2);
        //material_color(1,0,0,1,0,0);
        draw_Sphere();
    glPopMatrix();
}



void ball()
{
    glPushMatrix();
        ballPathPoint.clear();
        if(restart1) currentX = fcurrentX, currentY = fcurrentY, currentZ = fcurrentZ;

        float x1 = currentX, y1 = currentY, z1 = currentZ, x3 = xTarget, y3 = yTarget, z3 = zTarget;
        float x2=(x1+x3)/2 , y2=max(y1,y3) + 12, z2=(z1+z3)/2;
        for( float i = 0 ; i < 1 ; i += 0.01 ) {
            float xa = getPt(x1, x2, i);
            float ya = getPt(y1, y2, i);
            float za = getPt(z1, z2, i);
            float xb = getPt(x2, x3, i);
            float yb = getPt(y2, y3, i);
            float zb = getPt(z2, z3 ,i);

            float x = getPt(xa, xb, i);
            float y = getPt(ya, yb, i);
            float z = getPt(za, zb, i);

            Point point3d;
            point3d.x = x; point3d.y = y; point3d.z = z;
            ballPathPoint.push_back(point3d);


            //cout<<point3d.y<<" " <<point3d.z <<endl;
        }

        if(currentX != xTarget or currentY != yTarget or currentZ != zTarget){
            glPushMatrix();
                glBegin(GL_POINTS);
                glTranslatef(currentX, currentY, currentZ);
                for(auto it:ballPathPoint){
                    glColor3f(1, 1, 1);
                    glVertex3f(it.x,it.y,it.z);
                }
                glEnd();
            glPopMatrix();
        }
//        glPushMatrix();
//            glBegin(GL_POINTS);
//            glTranslatef(currentX, currentY, currentZ);
//            for(auto it:ballPathPoint){
//                glColor3f(1, 1, 1);
//                glVertex3f(it.x,it.y,it.z);
//            }
//            glEnd();
//        glPopMatrix();
        if(KickBall){
            if(pos>=(int)ballPathPoint.size()) pos = ballPathPoint.size() - 1;

            glPushMatrix();
                glTranslatef(ballPathPoint[pos].x, ballPathPoint[pos].y, ballPathPoint[pos].z);
                glEnable(GL_TEXTURE_2D);
                glEnable(GL_TEXTURE_GEN_S);
                glEnable(GL_TEXTURE_GEN_T);
                glBindTexture(GL_TEXTURE_2D, ID_[4]);
                    draw_Sphere();
                //glDeleteTextures(1,&ID_[4]);
                glDisable(GL_TEXTURE_GEN_T);
                glDisable(GL_TEXTURE_GEN_S);
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();

            // we have arrived at our desired location
            // need to move ball's starting position
            if(pos == ballPathPoint.size()-1){
                currentX = ballPathPoint[pos].x; currentY = ballPathPoint[pos].y; currentZ = ballPathPoint[pos].z;
                xTarget = currentX; yTarget = currentY; zTarget = currentZ;
                pos = 0;
                KickBall = false;
                sound.ballReceived();
                return;
            }
            pos++;
            std::this_thread::sleep_for(std::chrono::milliseconds(9));
//            pos += BallMoving;
        }

    glPopMatrix();
}


int playerPerp = 0;
float ii=.1;
void all()
{


    glPushMatrix();


    glPushMatrix();
        glTranslatef(0,0,20);
///audience
        for(int i=0;i<50;i++){
            glPushMatrix();
                glTranslatef(i+1,0,18);
                glScalef(.2,.2,.2);
                player();
            glPopMatrix();
        }

        for(int i=0;i<50;i++){
            glPushMatrix();
                glTranslatef(i+1,3,16);
                glScalef(.2,.2,.2);
                player();
            glPopMatrix();
        }
        for(int i=0;i<50;i++){
            glPushMatrix();
                glTranslatef(i+1,6,13);
                glScalef(.2,.2,.2);
                player();
            glPopMatrix();
        }

        for(int i=0;i<50;i++){
            glPushMatrix();
                glTranslatef(-42+7,6,40+i);
                glRotatef(90,0,1,0);
                glScalef(.2,.2,.2);
                player();
            glPopMatrix();
        }

        for(int i=0;i<50;i++){
            glPushMatrix();
                glTranslatef(80,8,40+i);
                glRotatef(90,0,1,0);
                glScalef(.2,.2,.2);
                player();
            glPopMatrix();
        }

        glPushMatrix();
            glTranslatef(2,-6,20);
            drawgallery();
        glPopMatrix();
///tree
        for(int i=0;i<15;i++){
            glPushMatrix();
                glTranslatef(-40+i*10,0,135);
                draw_tree();
            glPopMatrix();

        }
        for(int i=0;i<12;i++){
            glPushMatrix();
                glTranslatef(-50,0,135-i*10);
                draw_tree();
            glPopMatrix();

        }
        for(int i=0;i<12;i++){
            glPushMatrix();
                glTranslatef(100,0,135-i*10);
                draw_tree();
            glPopMatrix();

        }


        glPushMatrix();
            glTranslatef(0,0,20);
            glPushMatrix();
                glTranslatef(0,-6.4,40);
                glScalef(4,1,4);
                drawfield();
            glPopMatrix();
                glPushMatrix();
                drawboundary();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(-8,5,-20);
                glRotatef(45,0,1,0);
                glScalef(1,2,1);
                floodlight(-1,10,-12);
            glPopMatrix();

            glPushMatrix();
                glTranslatef(85,5,12);
                glRotatef(-45,0,1,0);
                glScalef(1,2,1);
                floodlight(0,10,-12);
            glPopMatrix();

            glPushMatrix();
                glTranslatef(18,0,50);
                goalpost();
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();
            ///player1
            glPushMatrix();
                p2x = 35, p2y = -7.5, p2z = 60;
                glTranslatef(p2x+p2i,p2y+p2j,p2z+p2k);
                glScalef(.4,.4,.4);
                player();
            glPopMatrix();


            ///player2
            glPushMatrix();
                p1x = 5, p1y = -7.5, p1z = 60;
                glTranslatef(p1x+p1i,p1y+p1j,p1z+p1k);
                glScalef(.4,.4,.4);
                player();
            glPopMatrix();


            glPushMatrix();
                glTranslatef(currentX, currentY, currentZ);
                glEnable(GL_TEXTURE_2D);
                glEnable(GL_TEXTURE_GEN_S);
                glEnable(GL_TEXTURE_GEN_T);
                glBindTexture(GL_TEXTURE_2D, ID_[4]);
                //glTranslatef(0,ttz,0);
                draw_Sphere();
                //glDeleteTextures(1,&ID_[3]);
                glDisable(GL_TEXTURE_GEN_T);
                glDisable(GL_TEXTURE_GEN_S);
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();
            glPushMatrix();
            target();
            ball();
            glPopMatrix();

            glPushMatrix();

                glTranslatef(20+ii,-7.5,30);
                glScalef(.5,.5,.5);
                player();

            glPopMatrix();
        glPopMatrix();

    glPopMatrix();
    glPopMatrix();



    glutPostRedisplay();
}

void draw_ground()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,ID_[11]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5000,-10,5000);
	glTexCoord2f(800.0f, 0.0f); glVertex3f(5000,-10,-5000);
	glTexCoord2f(800.0f, 800.0f); glVertex3f(-5000,-10,-5000);
	glTexCoord2f(0.0f, 800.0f); glVertex3f(-5000,-10,5000);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLineWidth(5.0);

	//all();
}



void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum(-5+lpan,5+rpan, -5+bpan, 5+tpan, 5+xx, 150);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    if(playerPerp) {
        gluLookAt(eyeX,eyeY,eyeZ, lookX+dx,lookY+dy,lookZ+dz, xaxis,yaxis,zaxis);
    }

    else{
        gluLookAt(eyeX,eyeY,eyeZ, lookX+dx,lookY+dy,lookZ+dz, xaxis,yaxis,zaxis);
    }
    glViewport(-0,0,windowHeight+100,windowWidth+100);


    glRotatef( theta, axis_x, axis_y, axis_z );

    glRotatef(2,0,0,1);
    glScalef(a_x, a_y, a_z);
    glPushMatrix();
        glTranslatef(ttx,0,ttz);
        all();
    glPopMatrix();
    glPushMatrix();
        draw_ground();
    glPopMatrix();

//    glPushMatrix();
//    glTranslatef(ttx,0,ttz);
//    drawBox(1,1,0,1,1,1);
//    glPopMatrix();




    light(-22,-10,-20, GL_LIGHT0, light_0_on, false);
    light(45,50,100.0, GL_LIGHT1, light_1_on, false);
    light(4,-2,0, GL_LIGHT2, light_2_on, true);


    glFlush();
    glutSwapBuffers();
}

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {

        // to move the target
        case 'i': // up
            zTarget--;
            break;
        case 'k': // down
            zTarget++;
            break;
        case 'j': // left
            xTarget--;
            break;
        case 'l': // right
            xTarget++;
            break;
        case 'I': // up
            yTarget++;
            break;
        case 'K': // down
            yTarget--;
            break;
        case 'o':
            KickBall = true;
            sound.ballKick();
            break;
        case 'm':
            restart = true;
            break;
    ///goalkeeper move
        case 'M':
            restart = false;
            break;
    ///reposition ball at the start
        case 'h':
            restart1 = true;
            break;
    ///player movement
        case 'v':
            mov_P1 = 1;
            break;
        case 'V':
            mov_P1 = 0;
            break;
        case 'b':
            mov_P2 = 1;
            break;
        case 'B':
            mov_P2 = 0;
            break;


    ///perspective change
        case 'p':
            //eyeX = p1x;
            //eyeY = p1y;
            eyeZ = p1z;
            playerPerp = true;
            break;
        case 'P':
            eyeX = 20;
            eyeY = 10;
            eyeZ = 120;
            playerPerp = false;
            break;

    ///bird view
    case 'c':
        chhk = 1;
        bird_swap();
        break;
    case 'C':
        chhk = 0;
        bird_swap();
        break;
    ///panning
    case '[':
        lpan-=1;
        rpan-=1;
        glutPostRedisplay();
        break;

    case ']':
        lpan+=1;
        rpan +=1;
        glutPostRedisplay();
        break;

    case ';':
        tpan-=1;
        bpan-=1;
        glutPostRedisplay();
        break;

    case ':':
        tpan+=1;
        bpan +=1;
        glutPostRedisplay();
        break;
    //object rotation in respect of x axis
    case 'R':
    case 'r':
        bRotate = !bRotate;
        axis_x=1.0;
        axis_y=0.0;
        glutPostRedisplay();
        break;

    //object rotation in respect of z axis
    case 'W':
    case 'w':
        bRotate = !bRotate;
        axis_z=1.0;
        glutPostRedisplay();
        break;

    //object rotation in respect of y axis
    case 'e':
    case 'E':
        bRotate = !bRotate;
        axis_y=1.0;
        glutPostRedisplay();
        break;

//    ///fan
//    case 'v':
//        oRotate = !oRotate;
//        glutPostRedisplay();
//        break;

//    case 'b':
//        chk = false;
//        glutPostRedisplay();
//        break;
//    case 'B':
//        chk = true;
//        glutPostRedisplay();
//        break;

    ///zoom

    case 'f':
        xx+=.2;
        glutPostRedisplay();
        break;
    case 'F':
        xx-=.2;
        glutPostRedisplay();
        break;

    case '=': //zoom in
        eyeZ++;
        lookZ++;
        glutPostRedisplay();
        break;
    case '-': //zoom out
        eyeZ--;
        lookZ--;
        glutPostRedisplay();
        break;
    case '.': //zoom up
        eyeY++;
        lookY++;
        glutPostRedisplay();
        break;
    case '/': //zoom down
        eyeY--;
        lookY--;
        glutPostRedisplay();
        break;

    case 'X':
        eyeX++;
        lookX++;
        glutPostRedisplay();
        break;
    case 'x':
        eyeX--;
        lookX--;
        glutPostRedisplay();
        break;

    ///yaw
    case 'y':

        yaw+=1;

        if( yaw==90 || yaw ==0)
        {
            yaw=0;
            dx=0;
            break;
        }
        dx =  abs(eyeZ-lookZ)*tan(yaw*3.1415/180);

        lookY=lookY;
        break;

    case 'Y':

        yaw-=1;

        if(yaw==-90 || yaw==0)
        {
            yaw = 0;
            dx=0;
            break;
        }

        dx = abs(eyeZ-lookZ)*tan(yaw*3.1415/180);

        lookY=lookY;

        break;

    ///pitch
    case '2':

        pitch+=1;

        if( pitch==90 || pitch ==0)
        {
            pitch=0;
            dy=0;
            break;
        }
        dy =  abs(eyeZ-lookZ)*tan(pitch*3.1415/180);

        lookX=lookX;
        break;
    case '1':
        pitch-=1;
        if(pitch==-90 || pitch==0)
        {
            pitch = 0;
            dy=0;
            break;
        }
        dy = abs(eyeZ-lookZ)*tan(pitch*3.1415/180);

        lookX=lookX;
        break;


    ///    roll
    case '4':
        roll+=0.5;
        xaxis = cos(roll);
        yaxis= sin(roll);

        break;
    case '3':
        roll-=0.5;

        xaxis = cos(roll);
        yaxis= sin(roll);
        break;

    ///zooming using scaling
    case 'a':
        a_x+=.05;
        a_y+=.05;
        a_z+=.05;
        break;

    case 'A':
        a_x-=.05;
        a_y-=.05;
        a_z-=.05;
        break;

    ///lights
    //light 1 toggle
    case '0':
        light_0_on = !light_0_on;
        break;
    //light 2 toggle
    case '9':
        light_1_on = !light_1_on;
        break;
    //light 3 toggle(spot Light)
    case '8':
        light_2_on = !light_2_on;
        break;
    //ambient light toggle
    case '7':
        amb_light = !amb_light;
        break;
    //diffusion light toggle
    case '6':
        dif_light = !dif_light;
        break;
    //specular light toggle
    case '5':
        spec_light = !spec_light;
        break;

    case 27:	// Escape key
        exit(1);

    }
}

void SpecialInput(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        ttz -= .1;
        break;
    case GLUT_KEY_DOWN:
//do something here
        ttz += .1;
        break;
    case GLUT_KEY_LEFT:
//do something here
        ttx -= 1;
        break;
    case GLUT_KEY_RIGHT:
//do something here
        ttx += 1;
        break;
    }
    cout<<"Starting Point "<<ttx<<","<<tty<<","<<ttz<<" \n";

    glutPostRedisplay();
}

void animate()
{
    if (bRotate == true)
    {
        theta += 0.5;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }
    glutPostRedisplay();
/////fan
//    if (oRotate == true)
//    {
//        GLfloat x = 0.5;
//        if(!chk) x -= .1;
//        else if(chk) x +=.1;
//        zeta += x;
//        if(zeta> 360.0)
//            zeta -= 360.0*floor(zeta/360.0);
//    }

    if(restart)
    {
        ii+=.05;
        if(ii>=4.5) ii=-4.5;

    }

    if(mov_P2){
        if(zTarget>p1z+p1k) p1k += .1;
        else if(zTarget<p1z+p1k) p1k -= .1;
        if(xTarget>p1x+p1i) p1i += .1;
        else if(xTarget<p1x+p1i) p1i -= .1;
    }

    if(mov_P1){
        if(zTarget>p2z+p2k) p2k += .1;
        else if(zTarget<p2z+p2k) p2k -= .1;
        if(xTarget>p2x+p2i) p2i += .1;
        else if(xTarget<p2x+p2i) p2i -= .1;
    }

    glutPostRedisplay();

}


void tex()
{
    glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


    ID_[1] = LoadBMP("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\foot1.bmp");
    ID_[2] = LoadBMP("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\goalnet.bmp");
    ID_[3] = LoadBMP("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\up.bmp");
    ID_[4] = LoadBMP("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\football.bmp");
    ID_[5] = LoadBMP("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\back.bmp");
    ID_[6] = LoadBMP("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\jersey.bmp");
    ID_[7] = LoadBMP("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\skin.bmp");
    ID_[8] = LoadBMP("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\ground.bmp");
    ID_[9] = LoadBMP("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\wood.bmp");
    ID_[10] = LoadBMP("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\tree1.bmp");
    ID_[11] = LoadBMP("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\grass.bmp");
    ID_[12] = LoadBMP("F:\\ACADEMIC\\4.2\\GRAPHICS LAB\\New folder\\lab1\\up.bmp");

}

int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(1,1);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("drawing room");
    //glutFullScreen();

    tex();

    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutIdleFunc(animate);

    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    glutSpecialFunc(SpecialInput);

    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutIdleFunc(animate);

    glutMainLoop();

    return 0;
}
