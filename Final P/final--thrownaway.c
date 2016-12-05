/* Sigrunn Sky Final Project
 * Used my HW 5 as a starting point
 * referenced Exammple 15 from class on how to apply textures. 
 * http://www.swiftless.com/opengltuts.html for help with fps and fog 
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (1/5)
 *  F8         Change ball increment
 *  F9         Invert bottom normal
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  a/w/s/d    move about the scene
 *  arrows     Change view angle
 *  PgDn/PgUp  Rotate veiw
 *  ESC        Exit
 */
#include "CSCIx229.h"
//All of these are taken from EX13.
int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=2;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
unsigned int texture[15]; //textures
int    skycube[2];   //  Sky textures

// Light values
int smooth    =   0;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model

int ambient   =  40;  // Ambient intensity (%)
int diffuse   = 90;  // Diffuse intensity (%)
int specular  =  10;  // Specular intensity (%)
int shin =   20;  // Shininess (power of two)
//float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   10;  // Elevation of light

float xpos = 10, ypos = 1, zpos = 6, xrot = 0, yrot = 270, angle=0.0;

float lastx, lasty;

GLfloat density = 0.1; //set the density to 0.1 

GLfloat fogColor[4] = {.85, .85, .85, 5.0}; //set the for color to light grey/ white

/* 
 *  Skycube
 * borrowed from EX25
 */
static void Sky(double D)
{
   
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,skycube[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   glBindTexture(GL_TEXTURE_2D,skycube[1]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}
/*
 *  Draw vertex in polar coordinates with normal
 */


static void cyl(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, float radius, float halfheight, int tex, int tex2)
{
     //  Set specular color to red
   // float white[] = {1,0,0,1};
   // float black[] = {0,0,0,1};
   // glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   // glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   // glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation

   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,1,0,0); //funky
   glScaled(dx,dy,dz);
   //  Offset, scale and rotate
     
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


   glColor3f(1.0, 1.0, 1.0);

   glBindTexture(GL_TEXTURE_2D,texture[tex]);
   
   
  glBegin(GL_TRIANGLE_STRIP);
   for (int i = 0; i <= 360; i++) {
      const float texx = (i / (float) 360);
      double x = radius * Cos(i);
      double y = halfheight;
      double z = radius * Sin(i);

      glNormal3d(x, y, z);

      glTexCoord2f(texx,20); glVertex3d(x, y, z);
      glTexCoord2f(texx,0); glVertex3d(x, -y, z);
   }
   glEnd();

   
   double j;
   glBindTexture(GL_TEXTURE_2D,texture[tex2]);

   glNormal3d(0,1,0);
   glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5); 
      glVertex3d(0.0, halfheight, 0.0);

      for(j = 0.0; j <= 360; j+=1) {
        glTexCoord2f(0.5*cos(j)+0.5, 0.5*sin(j)+0.5);
        glVertex3d(radius * cos(j), halfheight, radius * sin(j));
      }
   glEnd();


   glBindTexture(GL_TEXTURE_2D,texture[tex2]);
   //Bottom

   glNormal3d(0,-1,0);
   glBegin(GL_TRIANGLE_FAN);
    
    glTexCoord2f(0.5,0.5);
      glVertex3d(0.0, -halfheight, 0.0);

      for(j = 0.0; j <= 360; j+=1) {
         glTexCoord2f(0.5*cos(j)+0.5, 0.5*sin(j)+0.5);
         glVertex3d(radius * cos(j), -halfheight, radius * sin(j));
      }
   glEnd();

   

   glPopMatrix(); 
   glDisable(GL_TEXTURE_2D); 
}
// static void Vertex(double th,double ph)
// {
//    double x = Sin(th)*Cos(ph);
//    double y = Cos(th)*Cos(ph);
//    double z =         Sin(ph);
//    //  For a sphere at the origin, the position
//    //  and normal vectors are the same
//    glNormal3d(x,y,z);
//    glVertex3d(x,y,z);
// }
// static void lights(double x,double y,double z)
// {
//    int th,ph;
//    float yellow[] = {1.0,1.0,0.0,1.0};
//    float Emission[]  = {0.9,0.9,0,1.0};
//    //  Save transformation
//    glPushMatrix();
//    //  Offset, scale and rotate
//    glTranslated(x,y,z);
//    glScaled(1,1,1);
//    //  White ball
//    glColor3f(1,1,1);
//    glMaterialfv(GL_FRONT,GL_SHININESS,shin);
//    glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
//    glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
//    //  Bands of latitude
//    for (ph=-90;ph<90;ph+=inc)
//    {
//       glBegin(GL_QUAD_STRIP);
//       for (th=0;th<=360;th+=2*inc)
//       {
//          Vertex(th,ph);
//          Vertex(th,ph+inc);
//       }
//       glEnd();
//    }
//    //  Undo transofrmations
//    glPopMatrix();
// }

static void lights(double x,double y,double z)
{ 
  
  float yellow[]  = {1.000, 1, 0.980, 0.5};//yellow
  //GLfloat amb[] = {.87, 1.0 , 0, 1.0};//yellow
  float emi[]  = {1,1,1,1.0};
  //GLfloat diff[] = {.87 ,1 ,0 , 1}; //Black Color
  GLfloat spec[] = {.87 ,1 ,0 , 1}; //Green Color
  glPushMatrix();
  //glEnable(GL_LIGHTING);
  //   glEnable (GL_LIGHT1);
  //glEnable(GL_COLOR_MATERIAL);
  //  Save transformation

 

 // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
 // glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, yellow);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shin);

  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,emi);

   //  Offset, scale and rotate
  glTranslated(x,y,z);
  double r = 1;
  glScaled(r,r,r);
   //  White ball/

  glColor3f(.87,1,0);
  glutSolidSphere(r,16,16);

  //  Undo transofrmations
  glPopMatrix();
//  glFlush();
}

static void cabin ( double x, double y, double z, double th, double dx, double dy, double dz)
{
     

    glPushMatrix();
      // Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz); 

    glColor3f(1,1,1);

        //Front
    cyl(x, y+0, z, 0.05, 5, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+1, z, 0.05, 5, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+2, z, 0.05, 5, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+3, z, 0.05, 5, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+4, z, 0.05, 5, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+5, z, 0.05, 5, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+6, z, 0.05, 5, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+7, z, 0.05, 4.5, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+8, z, 0.05, 4, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+9, z, 0.05, 3.5, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+10, z, 0.05, 3, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+11, z, 0.05, 2.5, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+12, z, 0.05, 2, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+13, z, 0.05, 1.5, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+14, z, 0.05, 1, 0.05 , 90, 10, 2, 7,8);
    cyl(x, y+15, z, 0.05, .5, 0.05 , 90, 10, 2, 7,8);
    

    //Back
    cyl(x+16, y+0, z, 0.05, 5, 0.05 , 90, 10, 2, 7,8);
    cyl(x+16, y+1, z, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+16, y+2, z, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+16, y+3, z, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+16, y+4, z, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+16, y+5, z, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+16, y+6, z, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+16, y+7, z, 0.05, 4.5, 0.05 , 90, 10, 2, 7,8);
    cyl(x+16, y+8, z, 0.05, 4, 0.05 , 90, 10, 2, 7,8);
    cyl(x+16, y+9, z, 0.05, 3.5, 0.05 , 90, 10, 2, 7,8);
    cyl(x+16, y+10, z, 0.05, 3, 0.05 , 90, 10, 2, 7,8);
    cyl(x+16, y+11, z, 0.05, 2.5, 0.05 , 90, 10, 2, 7,8);
    cyl(x+16, y+12, z, 0.05, 2, 0.05 , 90, 10, 2, 7,8);
    cyl(x+16, y+13, z, 0.05, 1.5, 0.05 , 90, 10, 2, 7,8);
    cyl(x+16, y+14, z, 0.05, 1, 0.05 , 90, 10, 2, 7,8);
    cyl(x+16, y+15, z, 0.05, .5, 0.05 , 90, 10, 2, 7,8);
    
    //glColor3f(1,1,1);
  
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[7]);
    glBegin(GL_QUADS);

    //bottom left wood pannel  
    glTexCoord2f(0,0); glVertex3f(x+19,y+4,z+11);
    glTexCoord2f(10,0); glVertex3f(x+19,y+15,z);
    glTexCoord2f(10,10); glVertex3f(x-3,y+15,z);
    glTexCoord2f(0,10); glVertex3f(x-3,y+4,z+11);
    //bottom right wood panel
    glTexCoord2f(0,0); glVertex3f(x+19,y+4,z-11);
    glTexCoord2f(10,0); glVertex3f(x+19,y+15,z);
    glTexCoord2f(10,10); glVertex3f(x-3,y+15,z);
    glTexCoord2f(0,10); glVertex3f(x-3,y+4,z-11);
    //top left wood panel
    glTexCoord2f(0,0); glVertex3f(x+19,y+5,z+11);
    glTexCoord2f(10,0); glVertex3f(x+19,y+16,z);
    glTexCoord2f(10,10); glVertex3f(x-3,y+16,z);
    glTexCoord2f(0,10); glVertex3f(x-3,y+5,z+11);
    //top right wood panel
    glTexCoord2f(0,0); glVertex3f(x+19,y+5,z-11);
    glTexCoord2f(10,0); glVertex3f(x+19,y+16,z);
    glTexCoord2f(10,10); glVertex3f(x-3,y+16,z);
    glTexCoord2f(0,10); glVertex3f(x-3,y+5,z-11);
    //imbetween - right 

    //Imbetween areas on the triangle top
    glNormal3f(-1,0,0);  
    glTexCoord2f(0,0); glVertex3f(x-3,y+4,z+11);
    glTexCoord2f(1,0); glVertex3f(x-3,y+5,z+11);
    glTexCoord2f(1,5); glVertex3f(x-3,y+16,z);
    glTexCoord2f(0,5); glVertex3f(x-3,y+15,z);
    
    glNormal3f(1,0,0);  
    glTexCoord2f(0,0); glVertex3f(x+19,y+4,z+11);
    glTexCoord2f(1,0); glVertex3f(x+19,y+5,z+11);
    glTexCoord2f(1,5); glVertex3f(x+19,y+16,z);
    glTexCoord2f(0,5); glVertex3f(x+19,y+15,z);

    glNormal3f(-1,0,0);  
    glTexCoord2f(0,0); glVertex3f(x-3,y+4,z-11);
    glTexCoord2f(1,0); glVertex3f(x-3,y+5,z-11);
    glTexCoord2f(1,5); glVertex3f(x-3,y+16,z);
    glTexCoord2f(0,5); glVertex3f(x-3,y+15,z);

    glNormal3f(1,0,0);  
    glTexCoord2f(0,0); glVertex3f(x+19,y+4,z-11);
    glTexCoord2f(1,0); glVertex3f(x+19,y+5,z-11);
    glTexCoord2f(1,5); glVertex3f(x+19,y+16,z);
    glTexCoord2f(0,5); glVertex3f(x+19,y+15,z);

    glNormal3f(0,1,0);  
    glTexCoord2f(0,0); glVertex3f(x-3,y+4,z+11);
    glTexCoord2f(1,0); glVertex3f(x-3,y+5,z+11);
    glTexCoord2f(1,5); glVertex3f(x+19,y+5,z+11);
    glTexCoord2f(0,5); glVertex3f(x+19,y+4,z+11);

    glNormal3f(0,-1,0);  
    glTexCoord2f(0,0); glVertex3f(x-3,y+4,z-11);
    glTexCoord2f(1,0); glVertex3f(x-3,y+5,z-11);
    glTexCoord2f(1,5); glVertex3f(x+19,y+5,z-11);
    glTexCoord2f(0,5); glVertex3f(x+19,y+4,z-11);


    glEnd();
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_QUADS);

    //This is the snoww
    
    //top right snowy roof panel
    glNormal3f(1,1,0); 
    glTexCoord2f(0,0); glVertex3f(x+19,y+6,z+11);
    glTexCoord2f(10,0); glVertex3f(x+19,y+17,z);
    glTexCoord2f(10,10); glVertex3f(x-3,y+17,z);
    glTexCoord2f(0,10); glVertex3f(x-3,y+6,z+11);
    //top right wood panel
    glNormal3f(-1,1,0); 
    glTexCoord2f(0,0); glVertex3f(x+19,y+6,z-11);
    glTexCoord2f(10,0); glVertex3f(x+19,y+17,z);
    glTexCoord2f(10,10); glVertex3f(x-3,y+17,z);
    glTexCoord2f(0,10); glVertex3f(x-3,y+6,z-11);
    //imbetween - right 

    //Imbetween areas on the triangle top
    glNormal3f(-1,0,0); 
    glTexCoord2f(0,0); glVertex3f(x-3,y+5,z+11);
    glTexCoord2f(1,0); glVertex3f(x-3,y+6,z+11);
    glTexCoord2f(1,5); glVertex3f(x-3,y+17,z);
    glTexCoord2f(0,5); glVertex3f(x-3,y+16,z);

    glNormal3f(1,0,0); 
    glTexCoord2f(0,0); glVertex3f(x+19,y+5,z+11);
    glTexCoord2f(1,0); glVertex3f(x+19,y+6,z+11);
    glTexCoord2f(1,5); glVertex3f(x+19,y+17,z);
    glTexCoord2f(0,5); glVertex3f(x+19,y+16,z);

    glNormal3f(-1,0,0); 
    glTexCoord2f(0,0); glVertex3f(x-3,y+5,z-11);
    glTexCoord2f(1,0); glVertex3f(x-3,y+6,z-11);
    glTexCoord2f(1,5); glVertex3f(x-3,y+17,z);
    glTexCoord2f(0,5); glVertex3f(x-3,y+16,z);

    glNormal3f(1,0,0); 
    glTexCoord2f(0,0); glVertex3f(x+19,y+5,z-11);
    glTexCoord2f(1,0); glVertex3f(x+19,y+6,z-11);
    glTexCoord2f(1,5); glVertex3f(x+19,y+17,z);
    glTexCoord2f(0,5); glVertex3f(x+19,y+16,z);

    glNormal3f(0,1,0); 
    glTexCoord2f(0,0); glVertex3f(x-3,y+5,z+11);
    glTexCoord2f(1,0); glVertex3f(x-3,y+6,z+11);
    glTexCoord2f(1,5); glVertex3f(x+19,y+6,z+11);
    glTexCoord2f(0,5); glVertex3f(x+19,y+5,z+11);

    glNormal3f(0,-1,0); 
    glTexCoord2f(0,0); glVertex3f(x-3,y+5,z-11);
    glTexCoord2f(1,0); glVertex3f(x-3,y+6,z-11);
    glTexCoord2f(1,5); glVertex3f(x+19,y+6,z-11);
    glTexCoord2f(0,5); glVertex3f(x+19,y+5,z-11);
    glEnd();




    glBindTexture(GL_TEXTURE_2D,texture[9]);
    glBegin(GL_QUADS);

    //Window
    glNormal3f(0,0,-1); 
    glTexCoord2f(0,0); glVertex3f(x+9,y+2,z-8.6);
    glTexCoord2f(1,0); glVertex3f(x+6,y+2,z-8.6);
    glTexCoord2f(1,1); glVertex3f(x+6,y+5,z-8.6);
    glTexCoord2f(0,1); glVertex3f(x+9,y+5,z-8.6);
    
    //Window Other side
    glNormal3f(0,0,1); 
    glTexCoord2f(0,0); glVertex3f(x+9,y+2,z+8.6);
    glTexCoord2f(1,0); glVertex3f(x+6,y+2,z+8.6);
    glTexCoord2f(1,1); glVertex3f(x+6,y+5,z+8.6);
    glTexCoord2f(0,1); glVertex3f(x+9,y+5,z+8.6);
    // glPopMatrix();
    
    glEnd();

    glBindTexture(GL_TEXTURE_2D,texture[11]);
    glBegin(GL_QUADS);

    //Window sides
    //Bottom
    glNormal3f(0,-1,0);  
    glTexCoord2f(0,0); glVertex3f(x+9,y+2,z+8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+2,z+8.9);
    glTexCoord2f(1,1); glVertex3f(x+6,y+2,z+8);
    glTexCoord2f(0,1); glVertex3f(x+9,y+2,z+8);
    //top
    glNormal3f(0,1,0);  
    glTexCoord2f(0,0); glVertex3f(x+9,y+5,z+8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+5,z+8.9);
    glTexCoord2f(1,1); glVertex3f(x+6,y+5,z+8);
    glTexCoord2f(0,1); glVertex3f(x+9,y+5,z+8);
    //left
    glNormal3f(0,0,1);  
    glTexCoord2f(0,0); glVertex3f(x+9,y+2,z+8.9);
    glTexCoord2f(1,0); glVertex3f(x+9,y+5,z+8.9);
    glTexCoord2f(1,1); glVertex3f(x+9,y+5,z+8);
    glTexCoord2f(0,1); glVertex3f(x+9,y+2,z+8);
    //right
    glNormal3f(0,0,-1);  
    glTexCoord2f(0,0); glVertex3f(x+6,y+2,z+8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+5,z+8.9);
    glTexCoord2f(1,1); glVertex3f(x+6,y+5,z+8);
    glTexCoord2f(0,1); glVertex3f(x+6,y+2,z+8);

    //Other side Window sides
    //Bottom
    glNormal3f(0,-1,0);  
    glTexCoord2f(0,0); glVertex3f(x+9,y+2,z-8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+2,z-8.9);
    glTexCoord2f(1,1); glVertex3f(x+6,y+2,z-8);
    glTexCoord2f(0,1); glVertex3f(x+9,y+2,z-8);
    //top
    glNormal3f(0,1,0);  
    glTexCoord2f(0,0); glVertex3f(x+9,y+5,z-8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+5,z-8.9);
    glTexCoord2f(1,1); glVertex3f(x+6,y+5,z-8);
    glTexCoord2f(0,1); glVertex3f(x+9,y+5,z-8);
    //left
    glNormal3f(0,0,1);  
    glTexCoord2f(0,0); glVertex3f(x+9,y+2,z-8.9);
    glTexCoord2f(1,0); glVertex3f(x+9,y+5,z-8.9);
    glTexCoord2f(1,1); glVertex3f(x+9,y+5,z-8);
    glTexCoord2f(0,1); glVertex3f(x+9,y+2,z-8);
    //right
    glNormal3f(0,0,-1);  
    glTexCoord2f(0,0); glVertex3f(x+6,y+2,z-8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+5,z-8.9);
    glTexCoord2f(1,1); glVertex3f(x+6,y+5,z-8);
    glTexCoord2f(0,1); glVertex3f(x+6,y+2,z-8);

    //criss cross on window
    glNormal3f(0,0,1);  
    glTexCoord2f(0,0); glVertex3f(x+7.4,y+2,z+8.9);
    glTexCoord2f(1,0); glVertex3f(x+7.4,y+5,z+8.9);
    glTexCoord2f(1,1); glVertex3f(x+7.6,y+5,z+8.9);
    glTexCoord2f(0,1); glVertex3f(x+7.6,y+2,z+8.9);

    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(x+9,y+3.6,z+8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+3.6,z+8.9);
    glTexCoord2f(1,1); glVertex3f(x+6,y+3.4,z+8.9);
    glTexCoord2f(0,1); glVertex3f(x+9,y+3.4,z+8.9);

    glNormal3f(0,0,-1);  
    glTexCoord2f(0,0); glVertex3f(x+7.4,y+2,z-8.9);
    glTexCoord2f(1,0); glVertex3f(x+7.4,y+5,z-8.9);
    glTexCoord2f(1,1); glVertex3f(x+7.6,y+5,z-8.9);
    glTexCoord2f(0,1); glVertex3f(x+7.6,y+2,z-8.9);

    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(x+9,y+3.6,z-8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+3.6,z-8.9);
    glTexCoord2f(1,1); glVertex3f(x+6,y+3.4,z-8.9);
    glTexCoord2f(0,1); glVertex3f(x+9,y+3.4,z-8.9);


    //sides thicker
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(x+6,y+2,z+8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+5,z+8.9);
    glTexCoord2f(1,1); glVertex3f(x+6.2,y+5,z+8.9);
    glTexCoord2f(0,1); glVertex3f(x+6.2,y+2,z+8.9);

    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(x+9,y+2.4,z+8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+2.4,z+8.9);
    glTexCoord2f(1,1); glVertex3f(x+6,y+2,z+8.9);
    glTexCoord2f(0,1); glVertex3f(x+9,y+2,z+8.9);

    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(x+8.8,y+2,z+8.9);
    glTexCoord2f(1,0); glVertex3f(x+8.8,y+5,z+8.9);
    glTexCoord2f(1,1); glVertex3f(x+9,y+5,z+8.9);
    glTexCoord2f(0,1); glVertex3f(x+9,y+2,z+8.9);

    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(x+9,y+5,z+8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+5,z+8.9);
    glTexCoord2f(1,1); glVertex3f(x+6,y+4.8,z+8.9);
    glTexCoord2f(0,1); glVertex3f(x+9,y+4.8,z+8.9);

    //other side sides thickwr

    //bottom
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(x+9,y+2.4,z-8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+2.4,z-8.9);
    glTexCoord2f(1,1); glVertex3f(x+6,y+2,z-8.9);
    glTexCoord2f(0,1); glVertex3f(x+9,y+2,z-8.9);
    //left
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(x+6,y+2,z-8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+5,z-8.9);
    glTexCoord2f(1,1); glVertex3f(x+6.2,y+5,z-8.9);
    glTexCoord2f(0,1); glVertex3f(x+6.2,y+2,z-8.9);
    
    //right
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(x+8.8,y+2,z-8.9);
    glTexCoord2f(1,0); glVertex3f(x+8.8,y+5,z-8.9);
    glTexCoord2f(1,1); glVertex3f(x+9,y+5,z-8.9);
    glTexCoord2f(0,1); glVertex3f(x+9,y+2,z-8.9);
    //top
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(x+9,y+5,z-8.9);
    glTexCoord2f(1,0); glVertex3f(x+6,y+5,z-8.9);
    glTexCoord2f(1,1); glVertex3f(x+6,y+4.8,z-8.9);
    glTexCoord2f(0,1); glVertex3f(x+9,y+4.8,z-8.9);

    glEnd();



    glBindTexture(GL_TEXTURE_2D,texture[10]);
    glBegin(GL_QUADS);
    //door
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(x+16.6,y,z+6);
    glTexCoord2f(1,0); glVertex3f(x+16.6,y,z+3);
    glTexCoord2f(1,1); glVertex3f(x+16.6,y+6,z+3);
    glTexCoord2f(0,1); glVertex3f(x+16.6,y+6,z+6);
    

    glEnd();

    glBindTexture(GL_TEXTURE_2D,texture[11]);
    glBegin(GL_QUADS);

    //door sides
    //Bottom
    glNormal3f(0,-1,0); 
    glTexCoord2f(0,0); glVertex3f(x+16.6,y,z+6);
    glTexCoord2f(1,0); glVertex3f(x+16.6,y,z+3);
    glTexCoord2f(1,1); glVertex3f(x+16,y,z+3);
    glTexCoord2f(0,1); glVertex3f(x+16,y,z+6);
    //top

    glNormal3f(0,1,0); 
    glTexCoord2f(0,0); glVertex3f(x+16.6,y+6,z+6);
    glTexCoord2f(1,0); glVertex3f(x+16.6,y+6,z+3);
    glTexCoord2f(1,1); glVertex3f(x+16,y+6,z+3);
    glTexCoord2f(0,1); glVertex3f(x+16,y+6,z+6);

    //left
    glNormal3f(0,0,1); 
    glTexCoord2f(0,0); glVertex3f(x+16.6,y,z+6);
    glTexCoord2f(1,0); glVertex3f(x+16.6,y+6,z+6);
    glTexCoord2f(1,1); glVertex3f(x+16,y+6,z+6);
    glTexCoord2f(0,1); glVertex3f(x+16,y,z+6);

    //right
    glNormal3f(0,0,-1); 
    glTexCoord2f(0,0); glVertex3f(x+16.6,y,z+3);
    glTexCoord2f(1,0); glVertex3f(x+16.6,y+6,z+3);
    glTexCoord2f(1,1); glVertex3f(x+16,y+6,z+3);
    glTexCoord2f(0,1); glVertex3f(x+16,y,z+3);

    glEnd();
     

     //float Emission[]  = {0.0,1.0,0.0, .5};
   // windowl = 

     //  glEnable(GL_LIGHTING);
      //    glEnable (GL_LIGHT1);
    //       glEnable(GL_NORMALIZE);
 
   // glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    glBindTexture(GL_TEXTURE_2D,texture[12]);
    glBegin(GL_QUADS);

   // glEnable(GL_COLOR_MATERIAL);  
   // glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   // glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission);
    
    
    //front window of house
    glNormal3f(1,0,0); 
    glTexCoord2f(0,0); glVertex3f(x+16.5,y+2,z);
    glTexCoord2f(1,0); glVertex3f(x+16.5,y+2,z-6);
    glTexCoord2f(1,1); glVertex3f(x+16.5,y+7,z-6);
    glTexCoord2f(0,1); glVertex3f(x+16.5,y+7,z);
    
    // glDisable(GL_LIGHTING);
    //s glDisable(GL_LIGHT1);
    
   // glDisable(glColorMaterial);
    glEnd();


    glBindTexture(GL_TEXTURE_2D,texture[11]);
    glBegin(GL_QUADS);

    //front Window sides
    //Bottom
    glNormal3f(0,-1,0); 
    glTexCoord2f(0,0); glVertex3f(x+16,y+2,z-6);
    glTexCoord2f(1,0); glVertex3f(x+16.9,y+2,z-6);
    glTexCoord2f(1,1); glVertex3f(x+16.9,y+2,z);
    glTexCoord2f(0,1); glVertex3f(x+16,y+2,z);
    //top
    glNormal3f(0,1,0);  
    glTexCoord2f(0,0); glVertex3f(x+16,y+7,z-6);
    glTexCoord2f(1,0); glVertex3f(x+16.9,y+7,z-6);
    glTexCoord2f(1,1); glVertex3f(x+16.9,y+7,z);
    glTexCoord2f(0,1); glVertex3f(x+16,y+7,z);
    //left
    glNormal3f(0,0,-1);  
    glTexCoord2f(0,0); glVertex3f(x+16.9,y+2,z-6);
    glTexCoord2f(1,0); glVertex3f(x+16.9,y+7,z-6);
    glTexCoord2f(1,1); glVertex3f(x+16,y+7,z-6);
    glTexCoord2f(0,1); glVertex3f(x+16,y+2,z-6);
    //right
    glNormal3f(0,0,1);  
    glTexCoord2f(0,0); glVertex3f(x+16.9,y+2,z);
    glTexCoord2f(1,0); glVertex3f(x+16.9,y+7,z);
    glTexCoord2f(1,1); glVertex3f(x+6,y+7,z);
    glTexCoord2f(0,1); glVertex3f(x+6,y+2,z);

    //criss cross on window
    glNormal3f(1,0,0);  
    glTexCoord2f(0,0); glVertex3f(x+16.9,y+2,z-3.2);
    glTexCoord2f(1,0); glVertex3f(x+16.9,y+7,z-3.2);
    glTexCoord2f(1,1); glVertex3f(x+16.9,y+7,z-2.8);
    glTexCoord2f(0,1); glVertex3f(x+16.9,y+2,z-2.8);

    glNormal3f(1,0,0);  
    glTexCoord2f(0,0); glVertex3f(x+16.9,y+4.7,z);
    glTexCoord2f(1,0); glVertex3f(x+16.9,y+4.7,z-6);
    glTexCoord2f(1,1); glVertex3f(x+16.9,y+4.3,z-6);
    glTexCoord2f(0,1); glVertex3f(x+16.9,y+4.3,z);

    //thicker sides
    //bottom
    glNormal3f(1,0,0); 
    glTexCoord2f(0,0); glVertex3f(x+16.9,y+2,z);
    glTexCoord2f(1,0); glVertex3f(x+16.9,y+2,z-6);
    glTexCoord2f(1,1); glVertex3f(x+16.9,y+2.3,z-6);
    glTexCoord2f(0,1); glVertex3f(x+16.9,y+2.3,z);

    //top
    glNormal3f(1,0,0); 
    glTexCoord2f(0,0); glVertex3f(x+16.9,y+7,z);
    glTexCoord2f(1,0); glVertex3f(x+16.9,y+7,z-6);
    glTexCoord2f(1,1); glVertex3f(x+16.9,y+6.7,z-6);
    glTexCoord2f(0,1); glVertex3f(x+16.9,y+6.7,z);

    //right
    glNormal3f(1,0,0); 
    glTexCoord2f(0,0); glVertex3f(x+16.9,y+2,z);
    glTexCoord2f(1,0); glVertex3f(x+16.9,y+2,z-.3);
    glTexCoord2f(1,1); glVertex3f(x+16.9,y+7,z-.3);
    glTexCoord2f(0,1); glVertex3f(x+16.9,y+7,z);

    //left
    glNormal3f(1,0,0); 
    glTexCoord2f(0,0); glVertex3f(x+16.9,y+2,z-6);
    glTexCoord2f(1,0); glVertex3f(x+16.9,y+2,z-5.7);
    glTexCoord2f(1,1); glVertex3f(x+16.9,y+7,z-5.7);
    glTexCoord2f(0,1); glVertex3f(x+16.9,y+7,z-6);

    glEnd();

   //DoorMat
  glBindTexture(GL_TEXTURE_2D,texture[14]);
  glBegin(GL_QUADS);

  glNormal3f(0,1,0); 
  glTexCoord2f(0,0); glVertex3f(x+19,y-.25,z+6.5);
  glTexCoord2f(1,0); glVertex3f(x+19,y-.25,z+2.5);
  glTexCoord2f(1,1); glVertex3f(x+16.5,y-.25,z+2.5);
  glTexCoord2f(0,1); glVertex3f(x+16.5,y-.25,z+6.5);
  
  
  glEnd();  
    //FOOTPATHS
  glBindTexture(GL_TEXTURE_2D,texture[13]);
  glBegin(GL_QUADS);

  glNormal3f(0,1,0); 
  glTexCoord2f(0,0); glVertex3f(x+16,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+19,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+19,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+16,y-.3,z+6);
  
  glNormal3f(0,1,0); 
  glTexCoord2f(0,0); glVertex3f(x+19,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+22,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+22,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+19,y-.3,z+6);
  
  glNormal3f(0,1,0); 
  glTexCoord2f(0,0); glVertex3f(x+22,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+25,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+25,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+22,y-.3,z+6);
  
  glNormal3f(0,1,0); 
  glTexCoord2f(0,0); glVertex3f(x+25,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+28,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+28,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+25,y-.3,z+6);

  glTexCoord2f(0,0); glVertex3f(x+28,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+31,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+31,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+28,y-.3,z+6);

  glTexCoord2f(0,0); glVertex3f(x+31,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+34,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+34,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+31,y-.3,z+6);
  
  glTexCoord2f(0,0); glVertex3f(x+34,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+37,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+37,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+34,y-.3,z+6);

  glTexCoord2f(0,0); glVertex3f(x+37,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+40,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+40,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+37,y-.3,z+6);
  
  glTexCoord2f(0,0); glVertex3f(x+40,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+43,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+43,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+40,y-.3,z+6);

  glTexCoord2f(0,0); glVertex3f(x+43,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+46,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+46,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+43,y-.3,z+6);

  glTexCoord2f(0,0); glVertex3f(x+46,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+49,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+49,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+46,y-.3,z+6);

  glTexCoord2f(0,0); glVertex3f(x+49,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+52,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+52,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+49,y-.3,z+6);
  
  glTexCoord2f(0,0); glVertex3f(x+52,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+55,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+55,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+52,y-.3,z+6);

  glTexCoord2f(0,0); glVertex3f(x+55,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+58,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+58,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+55,y-.3,z+6);
  
  glTexCoord2f(0,0); glVertex3f(x+58,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+61,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+61,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+58,y-.3,z+6);

  glTexCoord2f(0,0); glVertex3f(x+61,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+64,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+64,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+61,y-.3,z+6);
  
  glTexCoord2f(0,0); glVertex3f(x+64,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+67,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+67,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+64,y-.3,z+6);

  glTexCoord2f(0,0); glVertex3f(x+67,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+70,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+70,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+67,y-.3,z+6);
  
  glTexCoord2f(0,0); glVertex3f(x+70,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+73,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+73,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+70,y-.3,z+6);

  glTexCoord2f(0,0); glVertex3f(x+73,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+76,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+76,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+73,y-.3,z+6);

  glTexCoord2f(0,0); glVertex3f(x+76,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+79,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+79,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+76,y-.3,z+6);
  
  glTexCoord2f(0,0); glVertex3f(x+79,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+82,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+82,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+79,y-.3,z+6);

  glTexCoord2f(0,0); glVertex3f(x+82,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+85,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+85,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+82,y-.3,z+6);

  glTexCoord2f(0,0); glVertex3f(x+85,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+88,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+88,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+85,y-.3,z+6);
  
  glTexCoord2f(0,0); glVertex3f(x+88,y-.3,z+3);
  glTexCoord2f(1,0); glVertex3f(x+91,y-.3,z+3);
  glTexCoord2f(1,1); glVertex3f(x+91,y-.3,z+6);
  glTexCoord2f(0,1); glVertex3f(x+88,y-.3,z+6);
  glEnd();


  

  glPopMatrix();
   
}
static void cabin2 ( double x, double y, double z, double th, double dx, double dy, double dz)
{
    glPushMatrix();
    // Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz); 

    //glColor3f(1,1,1);
    
    //Side
    cyl(x-8, y+0.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+1.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+2.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+3.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+4.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+5.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+6.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+7.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
   
    //Opposite Side
    cyl(x+8, y+0, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+1, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+2, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+3, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+4, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+6, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+7, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);

    
    
    glPopMatrix();
}
static void cabinC(double x, double y, double z, double th, double dx, double dy, double dz)
{
  glPushMatrix();
      // Offset
  glTranslated(x,y,z);
  glRotated(th,1,0,0);
  glScaled(dx,dy,dz); 

  glColor3f(1,1,1);
  
  cabin(0, 2, 0 , 0, 0.3, 0.3, 0.3);
  cabin2(0, 2, 0 , 90, 0.3, 0.3, 0.3);

  

  glPopMatrix();

}
static void ground(double x, double y, double z, double th, double r)
 {

  glPushMatrix();

  glTranslated(x,y,z);
  glRotated(th,1,0,0);
  glScaled(r,0,r); 

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texture[0]);

 glColor3f(1,1,1);

  glBegin(GL_QUADS);
  glNormal3f(0,1,0);

   glTexCoord2f(0,0); glVertex3f(-1,1,1);
   glTexCoord2f(50,0); glVertex3f(1,1,1);
   glTexCoord2f(50,50); glVertex3f(1,1,-1);
   glTexCoord2f(0,50); glVertex3f(-1,1,-1);
   glEnd();
   glPopMatrix();

}

static void Tree(double x, double y, double z, double dx,double dy,double dz,
                 double th)
   {  // Save transformation
      glPushMatrix();
      // Offset
      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      glScaled(dx,dy,dz);
      
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D, texture[1]);
      
      //  Triangles
      glBegin(GL_TRIANGLES);           // 1st pyramid
      // Front
      glNormal3f(cos(45),1,sin(45));
      glTexCoord2f(0,0);  glVertex3f(0, 1, 0); //top point
      glTexCoord2f(1,0);  glVertex3f(1, 0, 0); 
      glTexCoord2f(.5,1); glVertex3f(0, 0, 1);
 
      // Right
      glNormal3f(cos(45),1,-sin(45));
      glTexCoord2f(0,0);  glVertex3f(0.0, 1, 0.0);
      glTexCoord2f(1,0);  glVertex3f(1, 0, 0);
      glTexCoord2f(.5,1); glVertex3f(0, 0, -1);
 
      // Back
      glNormal3f(-cos(45),1,-sin(45));
      glTexCoord2f(0,0);  glVertex3f(0.0, 1, 0.0);
      glTexCoord2f(1,0);  glVertex3f(-1, 0, 0);
      glTexCoord2f(.5,1); glVertex3f(0, 0, -1.0);
       
      // Left
      glNormal3f(-cos(45),1,sin(45));
      glTexCoord2f(0,0);  glVertex3f(0.0, 1, 0.0);
      glTexCoord2f(1,0);  glVertex3f(-1, 0, 0);
      glTexCoord2f(.5,1); glVertex3f(0, 0, 1.0);

      glEnd();   // Done drawing the pyramid
      
      glBegin(GL_QUADS);
      glNormal3f(0, -1, 0);

      glTexCoord2f(0,0); glVertex3f(-1, 0, 0);
      glTexCoord2f(1,0); glVertex3f(0, 0, 1);
      glTexCoord2f(1,1); glVertex3f(1, 0, 0);
      glTexCoord2f(0,1); glVertex3f(0, 0, -1);

      glEnd();

      glPopMatrix();
   }

static void treeC(double x, double y, double z, double th, double dx, double dy, double dz)
{
    glPushMatrix();
      // Offset
  glTranslated(x,y,z);
  glRotated(th,1,0,0);
  glScaled(dx,dy,dz); 

  cyl(x, .3, z,  0.08, 0.5, 0.08 , 0, 1, 2,2,2);
   Tree(x, 2.2, z, 0.3, .4, .3, 30);
   Tree(x, 1.8, z, 0.4 ,0.6 ,0.4 , 0);
   Tree(x,1.5, z, .5, .7, .5, 45);
   Tree(x,1, z, .7, .9, .7, 30);
   
   cyl(x+1, .3, z+1 ,  0.1, 0.5, 0.1 , 0, 1, 2,2,2);
   Tree(x+1, 2, z+1, 0.4 ,0.6 ,0.4 , 45);
   Tree(x+1,1.5, z+1, .6, .8, .6, 0);
   Tree(x+1,1, z+1, .8, 1, .8, 60);
   
   //short tree
   cyl(x-1.5, .3, z+2 ,  0.05, 0.4, 0.05 , 0, 1, 2,2,2);
   Tree(x-1.5, 1.5, z+2, 0.2 ,0.5 ,0.2 , 40);
   Tree(x-1.5,1, z+2, .35, .6, .35, 80);
   Tree(x-1.5,.5, z+2, .45, .7, .45, 10);
   
   cyl(x-3, .3, z-1 ,  0.1, 0.5, 0.1 , 0, 1, 2,2,2);
   Tree(x-3, 2, z-1, 0.15 ,0.5 ,0.15 ,30);
   Tree(x-3,1.5, z-1, .45, .6, .45, 40);
   Tree(x-3,1, z-1, .55, .7, .55, 50);

    //big tree 
   cyl(x+2, .3, z+3 ,  0.1, 0.5, 0.1 , 0, 1, 2,2,2);
   Tree(x+2, 2.7, z+3, 0.4 ,0.6 ,0.4 , 45);
   Tree(x+2, 2.3, z+3, 0.5 ,0.7 ,0.5 , 0);
   Tree(x+2,1.8, z+3, .6, .8, .6, 65);
   Tree(x+2,1.3, z+3, .7, .9, .7, 40);

   cyl(x-1.5, .3, z-2 ,  0.1, 0.5, 0.1 , 0, 1, 2,2,2);
   Tree(x-1.5, 2, z-2, 0.2 ,0.5 ,0.2 , 60);
   Tree(x-1.5,1.5, z-2, .4, .6, .4, 80);
   Tree(x-1.5, 1, z-2, .5, .7, .5, 10);
   
   cyl(x+3, .3, z-1 ,  0.1, 0.5, 0.1 , 0, 1, 2,2,2);
   Tree(x+3, 2, z-1, 0.2 ,0.5 ,0.2 ,30);
   Tree(x+3,1.5, z-1, .4, .6, .4, 40);
   Tree(x+3,1, z-1, .5, .7, .5, 50);

   cyl(x-2, .3, z-3 ,  0.05, 0.5, 0.05 , 0, 1, 2,2,2);
   Tree(x-2, 2.2, z-3, 0.2 ,0.5 ,0.2 , 0);
   Tree(x-2,1.7, z-3, .3, .6, .3, 65);
   Tree(x-2,1.2, z-3, .4, .7, .4, 40);

   cyl(x-5, .3, z+1 ,  0.1, 0.55, 0.1 , 0, 1, 2,2,2);
   Tree(x-5,1.5, z+1, 0.4 ,0.6 ,0.4 , 45);
   Tree(x-5,1, z+1, .6, .8, .6, 0);
   Tree(x-5,.5, z+1, .8, 1, .8, 60);
   
   cyl(x+5.5, .3, z+2 ,  0.13, 0.5, 0.13 , 0, 1, 2,2,2);
   Tree(x+5.5, 1.7, z+2, 0.3 ,0.4 ,0.3 , 60);
   Tree(x+5.5,1.2, z+2, .4, .6, .4, 80);
   Tree(x+5.5,.7, z+2, .6, .7, .6, 10);
   
   cyl(x-8, .3, z-1 ,  0.1, 1, 0.1 , 0, 1, 2,2,2);
   Tree(x-8, 3, z-1, 0.2 ,0.5 ,0.2 ,30);
   Tree(x-8,2.5, z-1, .4, .6, .4, 40);
   Tree(x-8,2, z-1, .5, .7, .5, 50);

   cyl(x-2, .3, z+3 ,  0.1, 0.5, 0.1 , 0, 1, 2,2,2);
   Tree(x-2, 2, z+3, 0.2 ,0.5 ,0.2 , 0);
   Tree(x-2,1.5, z+3, .4, .6, .4, 65);
   Tree(x-2,1, z+3, .5, .7, .5, 40);

   cyl(x-1, .3, z-2 ,  0.1, 1, 0.1 , 0, 1, 2,2,2);
   Tree(x-1, 2.5, z-2, 0.2 ,0.5 ,0.2 , 60);
   Tree(x-1,2, z-2, .4, .6, .4, 80);
   Tree(x-1,1.5, z-2, .5, .7, .5, 10);
  
  glPopMatrix();
}


void camera (void) {
    glRotatef(xrot,1.0,0.0,0.0);  //rotate x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0);  //rotate y-axis (up and down)
    glTranslated(-xpos,-ypos,-zpos); //translate position of our camera
}

void display()
{
  const double len=2.0;  //  Length of axes
  //  Erase the window and the depth buffer
   glClearColor (1,1,1,1.0);


 // float Specular[]  = {0.1,0.1,0.1,1.0};

  //  Light direction
  //float Position[]  = {20,20,0,1};
  
 //  Erase the window and the depth buffer
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  //  Enable Z-buffering in OpenGL
  glEnable(GL_DEPTH_TEST);



  //  Undo previous transformations
glLoadIdentity();

//skycube  
Sky(25);
  
camera();

//with help from swiftless tutorials
//Fog
glEnable (GL_FOG); //enable
glFogi (GL_FOG_MODE, GL_EXP2); //set the fog mode to GL_EXP2 
glFogfv (GL_FOG_COLOR, fogColor); //set the fog color 
glFogf (GL_FOG_DENSITY, density); //set the density
glHint (GL_FOG_HINT, GL_NICEST); // set the fog type to GL_Nicest

glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

    float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
  float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
  float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
  //  Light direction
  float Position[]  = {20,20,-5,1};
  //  Draw light position as ball (still no lighting here)
  glColor3f(1,1,1);
 // ball(Position[0],Position[1],Position[2] , 0.1);
  //  OpenGL should normalize normal vectors
  glEnable(GL_NORMALIZE);
  //  Enable lighting
  glEnable(GL_LIGHTING);
  //  glColor sets ambient and diffuse color materials
  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  //  Enable light 0
  glEnable(GL_LIGHT0);
  //  Set ambient, diffuse, specular components and position of light 0
  glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
  glLightfv(GL_LIGHT0,GL_POSITION,Position);

  lights(5.5,7,5);
  lights(5.5,6,6);
  lights(5.5,5,7);
  lights(5.5,4,8);
  ground(0,0,0,0,30);
   //Objects in scene
  treeC(5, 0, 5, 0, 1, 1, 1);
  treeC(0, 0, 5, 0, 1, 1, 1);
  treeC(5, 0, 1, 0, 1, 1, 1);
  treeC(0, 0, 0, 0, 1, 1, 1);
  treeC(-10, 0, 0, 0, 1, 1, 1);
  treeC(-10, 0, 5, 0, 1, 1, 1);
  

  cabinC(-10, -2.5, 5 , 0, 1, 1, 1);
  
  glDisable(GL_LIGHTING);

  glColor3f(1,1,1);
  if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   //zh = fmod(90*t,360.0);

   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
//with help from swiftless tutorials
void reshape (int width, int height) {
    glViewport (0, 0, (GLsizei)width, (GLsizei)height); //set the viewport to the current window specifications
    glMatrixMode (GL_PROJECTION); //set the matrix to projection
    //glLoadIdentity ();
    gluPerspective (60, (GLfloat)width / (GLfloat)height, 1.0, 1000.0); //set the perspective (angle of sight, width, height, ,depth)
    glMatrixMode (GL_MODELVIEW); //set the matrix back to model
  }
//with help from swiftless tutorials
void keyboard (unsigned char key, int x, int y) {
    if (key=='q')
    {
    xrot += 1;
    if (xrot >360) xrot -= 360;
    }

    if (key=='z')
    {
    xrot -= 1;
    if (xrot < -360) xrot += 360;
    }

    if (key=='w')
    {
    float yrotrad = (yrot / 180 * 3.141592654f);
    xpos += (sin(yrotrad)) ;
    zpos -= (cos(yrotrad)) ;
    ypos=1; // -= (sin(xrotrad)) ;
    }

    if (key=='s')
    {
    float yrotrad = (yrot / 180 * 3.141592654f);
    xpos -= (sin(yrotrad));
    zpos += (cos(yrotrad)) ;
    ypos =1; 
    }

    if (key=='d')
    {
    float yrotrad = (yrot / 180 * 3.141592654f);
    xpos += (cos(yrotrad)) * 0.2;
    zpos += (sin(yrotrad)) * 0.2;
    }

    if (key=='a')
    {
    //float yrotrad;
    float yrotrad = (yrot / 180 * 3.141592654f);
    xpos -= (cos(yrotrad)) * 0.2;
    zpos -= (sin(yrotrad)) * 0.2;
    }

    if (key==27)
    {
    exit(0);
    }
}
/*
 *  GLUT calls this routine when a key is pressed
 */
 //with help from swiftless tutorials
// void special (int key, int x, int y) {


//     if (key==GLUT_KEY_PAGE_UP)
//     {
//     xrot -= 3;
//     if (xrot >360) xrot -= 360;
//     }

//     if (key==GLUT_KEY_PAGE_DOWN)
//     {
//     xrot += 3;
//     if (xrot < -360) xrot += 360;
//     }

//     if (key==GLUT_KEY_UP)
//     {
//     float yrotrad = (yrot / 180 * 3.141592654f);
//     xpos += (sin(yrotrad));
//     zpos -= (cos(yrotrad));
//     ypos = 1; //(sin(xrotrad));
//     }

//     if (key==GLUT_KEY_DOWN)
//     {
//     float yrotrad = (yrot / 180 * 3.141592654f);
//     xpos -= (sin(yrotrad));
//     zpos += (cos(yrotrad)) ;
//     ypos = 1; // (sin(xrotrad));
//     }

//     if (key == GLUT_KEY_RIGHT)
//     {
//     yrot += 2;
//     if (yrot >360) yrot -= 360;
//     }

//     if (key==GLUT_KEY_LEFT)
//     {
//     yrot -= 2;
//     if (yrot < -360)yrot += 360;
//     }
//     if (key==27)
//     {
//     exit(0);
//     } 
//     Project(mode?fov:0,asp,dim);
//     //  Tell GLUT it is necessary to redisplay the scene
//     glutPostRedisplay();
// }
//with help from swiftless tutorials
void mouseMovement(int x, int y) {
    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
    yrot += (float) diffx;    //set the xrot to yrot with the addition of the difference in the x position
}
/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,400);
   glutCreateWindow("Final Project: Sigrunn Sky");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   //glutSpecialFunc(special);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(display);

   glutPassiveMotionFunc(mouseMovement); //check for mouse movement
  //  argv[1] = "obj/pine.obj";

 //    if (argc>=3) scale = strtod(argv[2],NULL);
 //   if (scale<=0) scale = 1;
 //   //  Set color
 //   if (argc>=6) RGBA[0] = strtod(argv[3],NULL);
 //   if (argc>=6) RGBA[1] = strtod(argv[4],NULL);
 //   if (argc>=6) RGBA[2] = strtod(argv[5],NULL);
 //   //  Load object
 // //  obj = LoadOBJ(argv[1]);

   //declare
   texture[0] = LoadTexBMP("tex/snow.bmp");
   texture[1] = LoadTexBMP("tex/pine2.bmp");
   texture[2] = LoadTexBMP("tex/bark.bmp");
   //texture[3] = LoadTexBMP("tex/wrap1.bmp");
   //texture[4] = LoadTexBMP("tex/wrap2.bmp");
   //texture[5] = LoadTexBMP("tex/sky.bmp");
   //texture[6] = LoadTexBMP("tex/wrap3.bmp");
   texture[7] = LoadTexBMP("tex/cabinwood.bmp");
   texture[8] = LoadTexBMP("tex/cabintrunk.bmp");
   texture[9] = LoadTexBMP("tex/window.bmp");
   texture[10] = LoadTexBMP("tex/door.bmp");
   texture[11] = LoadTexBMP("tex/windowside.bmp");
   texture[12] = LoadTexBMP("tex/window2.bmp");
   texture[13] = LoadTexBMP("tex/rockpath.bmp");
   texture[14] = LoadTexBMP("tex/merry.bmp");
 
   skycube[0] = LoadTexBMP("tex/sky0.bmp");
   skycube[1] = LoadTexBMP("tex/sky1.bmp");

   

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
