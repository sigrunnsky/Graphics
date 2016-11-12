/* Sigrunn Sky HW5
 * Used my HW 5 as a starting point
 * referenced Exammple 15 from class on how to apply textures. 
 * http://www.swiftless.com/opengltuts.html for help with fps
 * Lighting
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
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  f          First Person Toggle
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
unsigned int texture[10]; //textures
int    box=1;    //  Draw sky
int    sky[2];   //  Sky textures
 float tpi = 2 * 3.141592;

//int fps= 0;         //First person toggle

// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  60;  // Ambient intensity (%)
int diffuse   = 90;  // Diffuse intensity (%)
int specular  =  10;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   10;  // Elevation of light

//fps
// int turn = 45.0;  // turning degrees.. 300 fo you can see the trees.

// double xPoint = 0;  //Eyeballs pointing in x & z position 
// double zPoint = 0;

// double xEye = -5; //Eyeball location
// double yEye = 1;
// double zEye = 5;
//angle of rotation
float xpos = 5, ypos = 1, zpos = 5, xrot = 0, yrot = 270, angle=0.0;


/* 
 *  Draw sky box
 */
static void Sky(double D)
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,sky[0]);
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
   glBindTexture(GL_TEXTURE_2D,sky[1]);
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
   float white[] = {1,0,0,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,1,0,0); //funky
   glScaled(dx,dy,dz);
   //  Offset, scale and rotate
     
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
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

   glBindTexture(GL_TEXTURE_2D,texture[tex2]);

   glNormal3d(0,1,0);
   glBegin(GL_TRIANGLE_FAN);

   
     glTexCoord2f(0.5,0.5); 
      glVertex3d(0.0, halfheight, 0.0);

      for(double j = 0.0; j < 360; j+=.125) {
        glTexCoord2f(-0.5*Cos(j)+0.5, 0.5*Sin(j)+0.5);
        glVertex3d(radius * cos(j), halfheight, radius * sin(j));
      }
   glEnd();


   glBindTexture(GL_TEXTURE_2D,texture[tex2]);
   //Bottom

   glNormal3d(0,-1,0);
   glBegin(GL_TRIANGLE_FAN);
    
    glTexCoord2f(0.5,0.5);
      glVertex3d(0.0, -halfheight, 0.0);

      for(double j = 0.0; j < 360; j+=.125) {
         glTexCoord2f(-0.5*Cos(j)+0.5, 0.5*Sin(j)+0.5);
         glVertex3d(radius * cos(j), -halfheight, radius * sin(j));
      }
   glEnd();

   

   glPopMatrix(); 
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

    glNormal3f(0,1,0);   
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
    glTexCoord2f(0,0); glVertex3f(x-3,y+4,z+11);
    glTexCoord2f(10,0); glVertex3f(x-3,y+5,z+11);
    glTexCoord2f(10,1); glVertex3f(x-3,y+16,z);
    glTexCoord2f(0,1); glVertex3f(x-3,y+15,z);

    glTexCoord2f(0,0); glVertex3f(x+19,y+4,z+11);
    glTexCoord2f(10,0); glVertex3f(x+19,y+5,z+11);
    glTexCoord2f(10,1); glVertex3f(x+19,y+16,z);
    glTexCoord2f(0,1); glVertex3f(x+19,y+15,z);

    glTexCoord2f(0,0); glVertex3f(x-3,y+4,z-11);
    glTexCoord2f(10,0); glVertex3f(x-3,y+5,z-11);
    glTexCoord2f(10,1); glVertex3f(x-3,y+16,z);
    glTexCoord2f(0,1); glVertex3f(x-3,y+15,z);

    glTexCoord2f(0,0); glVertex3f(x+19,y+4,z-11);
    glTexCoord2f(10,0); glVertex3f(x+19,y+5,z-11);
    glTexCoord2f(10,1); glVertex3f(x+19,y+16,z);
    glTexCoord2f(0,1); glVertex3f(x+19,y+15,z);

    //
    glTexCoord2f(0,0); glVertex3f(x-3,y+4,z+11);
    glTexCoord2f(10,0); glVertex3f(x-3,y+5,z+11);
    glTexCoord2f(10,1); glVertex3f(x+19,y+5,z+11);
    glTexCoord2f(0,1); glVertex3f(x+19,y+4,z+11);

    glTexCoord2f(0,0); glVertex3f(x-3,y+4,z-11);
    glTexCoord2f(10,0); glVertex3f(x-3,y+5,z-11);
    glTexCoord2f(10,1); glVertex3f(x+19,y+5,z-11);
    glTexCoord2f(0,1); glVertex3f(x+19,y+4,z-11);
    glEnd();


    // glPopMatrix();
    
    

    glPopMatrix();

   
}
static void cabin2 ( double x, double y, double z, double th, double dx, double dy, double dz)
{
	glPushMatrix();
      // Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz); 

	glColor3f(1,1,1);
    
    //Side
    cyl(x-8, y+0.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+1.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+2.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+3.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+4.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+5.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+6.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x-8, y+7.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    // cyl(x-8, y+8.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    // cyl(x-8, y+9.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    // cyl(x-8, y+10.5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);

    //Opposite Side
    cyl(x+8, y+0, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+1, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+2, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+3, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+4, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+5, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+6, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    cyl(x+8, y+7, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    // cyl(x+8, y+8, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    // cyl(x+8, y+9, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
    // cyl(x+8, y+10, z+8, 0.05, 5, 0.05 , 90, 10, 2,7,8);
 	
    
    
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
static void ground(double x, double y, double z, double th)
 {

  glPushMatrix();

  glTranslated(x,y,z);
  glRotated(th,1,0,0);
  glScaled(50,50,50); 

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[0]);


  
   glColor3f(1,1,1);
  
     glBegin(GL_QUADS);
    glNormal3f(0,1,0);
   // glTexCoord2f(25,25);
  
  
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   glPopMatrix();
// 	 //double radius= 10;
//    //int tNum = 100;
  

//     glPushMatrix();

//     glTranslated(x,y,z);
//     glRotated(th,0,1,0);
//     glScaled(radius,radius,radius);

//     glEnable(GL_TEXTURE_2D);
//     glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
//     glColor3f(1, 1, 1); //dim grey slate
//     glBindTexture(GL_TEXTURE_2D, texture[0]);
   
//     double i; 

//     glNormal3d(0,1,0);
//     glBegin(GL_TRIANGLE_FAN);

   
//      glTexCoord2f(0.5,0.5); 
//       glVertex3d(0.0, halfheight, 0.0);

//       for(double j = 0.0; j < 360; j+=.125) {
//         glTexCoord2f(-0.5*Cos(j)+0.5, 0.5*Sin(j)+0.5);
//         glVertex3d(radius * cos(j), halfheight, radius * sin(j));
//       }
//    glEnd();

//    glBindTexture(GL_TEXTURE_2D, texture[5]);
//    //Bottom
//    glBegin(GL_TRIANGLE_FAN);
//    glNormal3d(0,-1,0);
//       glVertex3d(0.0, -halfheight, 0.0);

//       for(double j = 0.0; j < 360; j+=.125) {
//          glVertex3d(radius * cos(j), -halfheight, radius * sin(j));
//       }
//    glEnd();

//    glBegin(GL_TRIANGLE_STRIP);
//    for (int i = 0; i <= 360; i++) {
//       const float texx = (i / (float) 360);
//       double x = radius * Cos(i);
//       double y = halfheight;
//       double z = radius * Sin(i);

//       glNormal3d(x, y, z);

//       glTexCoord2f(texx,20); glVertex3d(x, y, z);
//       glTexCoord2f(texx,0); glVertex3d(x, -y, z);
//    }
//    glEnd();

//    glPopMatrix(); 

    //  /* Top of Cylinder */
    // glBegin(GL_TRIANGLE_FAN);
    // glTexCoord2f(0.5,0.5); 
    // glVertex3d(0.0, 0, 0.0);

    //   for(i = 0.0; i <= 360; i+=10) {
    //     glTexCoord2f(-0.5*Cos(i)+0.5, 0.5*Sin(i)+0.5);
    //     glVertex3d(radius * Cos(i), 0, radius * Sin(i));
    //   }
    // glEnd();
    // //cyl(double x,double y,double z, double dx,double dy,double dz,
    //        //double th, float radius, float halfheight, int tex)
    
    //  /* Top of Cylinder */
    // glBegin(GL_TRIANGLE_FAN);
    //   glTexCoord2f(0.5,0.5); 
    //   glVertex3d(0.0, -5, 0.0);

    //   for(i = 0.0; i <= 360; i+=10) {
    //     glTexCoord2f(-0.5*Cos(i)+0.5, 0.5*Sin(i)+0.5);
    //     glVertex3d(radius * Cos(i), -5, radius * Sin(i));
    //   }
    // glEnd();
    

    //glPopMatrix();

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

  
//experimenting with making all colors into parameters
static void SuperGift(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, int wrap, int wraplid)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,0,0);
   glScaled(dx,dy,dz);
   //  Cube for the gift itself
   
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, texture[wrap]);

   
   glBegin(GL_QUADS);
   //  Front
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   //  Back
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   //  Right
   glNormal3f(1,0,0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   //  Left
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   //  Top
   glNormal3f(0,1,0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   //  Bottom
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   //  End
   glEnd();


   glBindTexture(GL_TEXTURE_2D,texture[wraplid]);
   glBegin(GL_QUADS);
   
   //Now create the Lid of the Gift
   //  Front
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(-1.1,.5, 1.1);
   glTexCoord2f(4,0); glVertex3f(+1.1,.5, 1.1);
   glTexCoord2f(4,1); glVertex3f(+1.1,+1.1, 1.1);
   glTexCoord2f(0,1); glVertex3f(-1.1,+1.1, 1.1);
   //  Back
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0); glVertex3f(+1.1,.5,-1.1);
   glTexCoord2f(4,0); glVertex3f(-1.1,.5,-1.1);
   glTexCoord2f(4,1); glVertex3f(-1.1,+1.1,-1.1);
   glTexCoord2f(0,1); glVertex3f(+1.1,+1.1,-1.1);
   //  Right
   glNormal3f(1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1.1,.5,+1.1);
   glTexCoord2f(4,0); glVertex3f(+1.1,.5,-1.1);
   glTexCoord2f(4,1); glVertex3f(+1.1,+1.1,-1.1);
   glTexCoord2f(0,1); glVertex3f(+1.1,+1.1,+1.1);
   //  Left
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0); glVertex3f(-1.1,.5,-1.1);
   glTexCoord2f(4,0); glVertex3f(-1.1,.5,+1.1);
   glTexCoord2f(4,1); glVertex3f(-1.1,+1.1,+1.1);
   glTexCoord2f(0,1); glVertex3f(-1.1,+1.1,-1.1);
   //  Top
   glNormal3f(0,1,0);
   glTexCoord2f(0,0); glVertex3f(-1.1,+1.1,+1.1);
   glTexCoord2f(4,0); glVertex3f(+1.1,+1.1,+1.1);
   glTexCoord2f(4,4); glVertex3f(+1.1,+1.1,-1.1);
   glTexCoord2f(0,4); glVertex3f(-1.1,+1.1,-1.1);
   //  Bottom
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0); glVertex3f(-1.1,.5,-1.1);
   glTexCoord2f(1,0); glVertex3f(+1.1,.5,-1.1);
   glTexCoord2f(1,1); glVertex3f(+1.1,.5,+1.1);
   glTexCoord2f(0,1); glVertex3f(-1.1,.5,+1.1);
   //  End
   glEnd();

   
   glPopMatrix();
}


static void Vertex(double th, double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}
/* comes from EX13

 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */


// static void Vertex1(double th,double ph)
// {
//    glColor3f(1 , 1 , 1);
//    glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
// }

 //   static void snowglobe(double r)
 //   {
   	
 // 	float Emission[]  = {0.0,0.0,0.01*emission,1.0};
 //   	glTranslated(0,5,0);
 //    glRotated(0,0,0,0);

   
 //    glColor4f(1,1,1,0.23);
 //    glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
	//  glEnable(GL_BLEND); 
 //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 //   // glColor4ub(1,1,1,.33);

 //    glutSolidSphere(r, 100, 100);

 //   	glDisable(GL_BLEND);
      
 // }

static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball

   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   glColor3f(1,1,1);
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

void camera (void) {
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on teh x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
}

void display()
{
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

 //  Sky(10*dim);

   //  Undo previous transformations
   glLoadIdentity();
   camera();
   //  Perspective - set eye position
   // xPoint = +2*dim*Sin(turn); //Set the camera vector based on the turn (degrees)
   // zPoint = -2*dim*Cos(turn);
   // gluLookAt(xEye,yEye,zEye, xPoint+xEye,yEye,zPoint+zEye, 0,1,0); 

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);


   //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      //float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};

      float position[]  = {5,0,0,1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(position[0],position[1], position[2] , 0.1);
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
      glLightfv(GL_LIGHT0,GL_POSITION,position);

   //Objects in scene
  
   //sides(0, 0, 0, 10, 10,10,0);

   
 

   cyl(0, .3, 0 ,  0.1, 0.5, 0.1 , 0, 1, 2,2,2);
   Tree(0, 2, 0, 0.35 ,0.5 ,0.35 , 0);
   Tree(0,1.5, 0, .5, .7, .5, 45);
   Tree(0,1, 0, .7, .9, .7, 30);
   SuperGift(0, .27, 1, .5, .4, .5, 0, 3, 4);
   SuperGift(-1, .17, 0, .3, .3, .3, 30, 6, 3);
   cyl(1, .3, 1 ,  0.1, 0.5, 0.1 , 0, 1, 2,2,2);
   Tree(1, 2, 1, 0.4 ,0.6 ,0.4 , 45);
   Tree(1,1.5, 1, .6, .8, .6, 0);
   Tree(1,1, 1, .8, 1, .8, 60);
   
   cyl(-1.5, .3, 2 ,  0.1, 0.5, 0.1 , 0, 1, 2,2,2);
   Tree(-1.5, 2, 2, 0.2 ,0.5 ,0.2 , 60);
   Tree(-1.5,1.5, 2, .4, .6, .4, 80);
   Tree(-1.5,1, 2, .5, .7, .5, 10);
   
   cyl(-3, .3, -1 ,  0.1, 0.5, 0.1 , 0, 1, 2,2,2);
   Tree(-3, 2, -1, 0.2 ,0.5 ,0.2 ,30);
   Tree(-3,1.5, -1, .4, .6, .4, 40);
   Tree(-3,1, -1, .5, .7, .5, 50);

   cyl(2, .3, 3 ,  0.1, 0.5, 0.1 , 0, 1, 2,2,2);
   Tree(2, 2, 3, 0.2 ,0.5 ,0.2 , 0);
   Tree(2,1.5, 3, .4, .6, .4, 65);
   Tree(2,1, 3, .5, .7, .5, 40);

   cabinC(0, -2.5, 10 , 0, 1, 1, 1);
    
    
   
  //  gluQuadricTexture(quadric, true);
// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
// glEnable(GL_TEXTURE_GEN_S);
// glEnable(GL_TEXTURE_GEN_T);
// glBindTexture(GL_TEXTURE_2D, texture[9]);
// glutSolidTorus(1, 2, 10, 10);
//   snowglobe(25);
   
  // ground(0,-20,0,0);

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
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */

// void special(int key,int x,int y)
// {
  
//       double move = 0.05; 
//       //  Right arrow key - increase angle by 1 degrees
//       if (key == GLUT_KEY_RIGHT){
//          turn += 1;
//       }
//       //  Left arrow key - decrease angle by 1 degrees
//       else if (key == GLUT_KEY_LEFT){
//          turn -= 1;
//       }
//       //   Move in the correct vector direction
//       else if (key == GLUT_KEY_UP){
//          xEye += xPoint*move;
//          zEye += zPoint*move;
//       }
//       //  
//       else if (key == GLUT_KEY_DOWN) {
//          xEye -= xPoint*move;
//          zEye -= zPoint*move;
//       }
      
      
//       turn %=360;
//    //}

//    //  Update projection
//    Project(mode?fov:0,asp,dim);
//    //  Tell GLUT it is necessary to redisplay the scene
//    glutPostRedisplay();
// }

/*
 *  GLUT calls this routine when a key is pressed
 */
void special (int key, int x, int y) {


    if (key==GLUT_KEY_PAGE_UP)
    {
    xrot -= 5;
    if (xrot >360) xrot -= 360;
    }

    if (key==GLUT_KEY_PAGE_DOWN)
    {
    xrot += 5;
    if (xrot < -360) xrot += 360;
    }

    if (key==GLUT_KEY_UP)
    {
    // float xrotrad;
    // float yrotrad;
    float yrotrad = (yrot / 180 * 3.141592654f);
   // xrotrad = (xrot / 180 * 3.141592654f); 
    xpos += (sin(yrotrad));
    zpos -= (cos(yrotrad));
    ypos = 1; //(sin(xrotrad));
    }

    if (key==GLUT_KEY_DOWN)
    {
    // float xrotrad, yrotrad;
    float yrotrad = (yrot / 180 * 3.141592654f);
   // xrotrad = (xrot / 180 * 3.141592654f); 
    xpos -= (sin(yrotrad));
    zpos += (cos(yrotrad)) ;
    ypos = 1; // (sin(xrotrad));
    }

    if (key == GLUT_KEY_RIGHT)
    {
    yrot += 5;
    if (yrot >360) yrot -= 360;
    }

    if (key==GLUT_KEY_LEFT)
    {
    yrot -= 5;
    if (yrot < -360)yrot += 360;
    }
    if (key==27)
    {
    exit(0);
    } 
    Project(mode?fov:0,asp,dim);
//    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
    }



/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(mode?fov:0,asp,dim);
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
   glutSpecialFunc(special);
   //glutKeyboardFunc(key);
   glutIdleFunc(display);

   texture[0] = LoadTexBMP("tex/snow.bmp");
   texture[1] = LoadTexBMP("tex/pine2.bmp");
   texture[2] = LoadTexBMP("tex/bark.bmp");
   texture[3] = LoadTexBMP("tex/wrap1.bmp");
   texture[4] = LoadTexBMP("tex/wrap2.bmp");
   texture[5] = LoadTexBMP("tex/sky.bmp");
   texture[6] = LoadTexBMP("tex/wrap3.bmp");
   texture[7] = LoadTexBMP("tex/cabinwood.bmp");
   texture[8] = LoadTexBMP("tex/cabintrunk.bmp");
//   texture[9] = LoadTexBMP("tex/reef.bmp");
 
   sky[0] = LoadTexBMP("tex/sky0.bmp");
   sky[1] = LoadTexBMP("tex/sky1.bmp");

   

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
