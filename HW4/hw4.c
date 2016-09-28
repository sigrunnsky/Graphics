/*  HW4: Sigrunn Sky
 //Used Ex8.c from class as my starting point
 //Used https://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html To help me make the triangles for the christmas tree
 //Used http://prideout.net/archive/colors.php to help me pick colors
 *
 *  Key bindings:
 *  a          Toggle axes
 *  arrows     Change view angle
 *  0          Reset view angle
 *  ESC        Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world

//  Cosine and Sine in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)

   {
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(.5,.3,0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(.5,.2,0);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(.5,.2,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(.50,.2,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(.5,.2,0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(.5,.2,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
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
      
      //  Triangles
      glBegin(GL_TRIANGLES);           // 1st pyramid
      // Front
      glColor3f(0.196, 0.804, 0.196);     
      glVertex3f( 0, .8, 0);
      glColor3f(0.000, 0.392, 0.000);     
      glVertex3f(-1, -.8, 1);
      glColor3f(0.000, 0.392, 0.000);    
      glVertex3f(1.0, -.8, 1.0);
 
      // Right
      glColor3f(0.196, 0.804, 0.196);    
      glVertex3f(0.0, .8, 0.0);
      glColor3f(0.000, 0.392, 0.000);    
      glVertex3f(1.0, -.8, 1.0);
      glColor3f(0.000, 0.392, 0.000);    
      glVertex3f(1.0, -.8, -1.0);
 
      // Back
      glColor3f(0.196, 0.804, 0.196);   
      glVertex3f(0.0, .8, 0.0);
      glColor3f(0.000, 0.392, 0.000);   
      glVertex3f(1.0, -.8, -1.0);
      glColor3f(0.000, 0.392, 0.000);    
      glVertex3f(-1.0, -.8, -1.0);
 
      // Left
      glColor3f(0.196, 0.804, 0.196);      
      glVertex3f( 0.0, .8, 0.0);
      glColor3f(0.000, 0.392, 0.000);   
      glVertex3f(-1.0,-.8,-1.0);
      glColor3f(0.000, 0.392, 0.000);       
      glVertex3f(-1.0,-.8, 1.0);
      glEnd();   // Done drawing the pyramid
      
      glRotated(45,0,1,0);
      glBegin(GL_TRIANGLES);  //2nd pyramid
      // Front
      glColor3f(0.196, 0.804, 0.196);     
      glVertex3f( 0, 0, 0);
      glColor3f(0.000, 0.392, 0.000);
      glVertex3f(-1.4, -2, 1.4);
      glColor3f(0.000, 0.392, 0.000);
      glVertex3f(1.4, -2.0, 1.4);
 
      // Right
      glColor3f(0.196, 0.804, 0.196);     
      glVertex3f(0, 0, 0);
      glColor3f(0.000, 0.392, 0.000);   
      glVertex3f(1.4, -2.0, 1.4);
      glColor3f(0.000, 0.392, 0.000);
      glVertex3f(1.4, -2.0, -1.4);
 
      // Back
      glColor3f(0.196, 0.804, 0.196);   
      glVertex3f(0.0, 0.0, 0.0);
      glColor3f(0.000, 0.392, 0.000);   
      glVertex3f(1.4, -2.0, -1.4);
      glColor3f(0.000, 0.392, 0.000);
      glVertex3f(-1.4, -2.0, -1.4);
 
      // Left
      glColor3f(0.196, 0.804, 0.196);       
      glVertex3f( 0.0, 0.0, 0.0);
      glColor3f(0.000, 0.392, 0.000); 
      glVertex3f(-1.4,-2.0,-1.4);
      glColor3f(0.000, 0.392, 0.000);       
      glVertex3f(-1.4,-2.0, 1.4);
      glEnd();


      glRotated(25,0,1,0);
      glBegin(GL_TRIANGLES);  //3rd pyramid
      // Front
      
      glColor3f(0.196, 0.804, 0.196); // dark green
      glVertex3f( 0, -.8, 0);
      glColor3f(0.000, 0.392, 0.000);
      glVertex3f(-1.5, -3.3, 1.5);
      glColor3f(0.000, 0.392, 0.000);
      glVertex3f(1.5, -3.3, 1.5);
 
      // Right
      glColor3f(0.196, 0.804, 0.196);
      glVertex3f(0, -.8, 0);
      glColor3f(0.000, 0.392, 0.000);
      glVertex3f(1.5, -3.3, 1.5);
      glColor3f(0.000, 0.392, 0.000);
      glVertex3f(1.5, -3.3, -1.5);
 
      // Back
      glColor3f(0.196, 0.804, 0.196);     
      glVertex3f(0.0, -.8, 0.0);
      glColor3f(0.000, 0.392, 0.000);
      glVertex3f(1.5, -3.3, -1.5);
      glColor3f(0.000, 0.392, 0.000);
      glVertex3f(-1.5, -3.3, -1.5);
 
      // Left
      glColor3f(0.196, 0.804, 0.196);   
      glVertex3f( 0.0, -.8, 0.0);
      glColor3f(0.000, 0.392, 0.000);
      glVertex3f(-1.5,-3.3,-1.5);
      glColor3f(0.000, 0.392, 0.000);
      glVertex3f(-1.5,-3.3, 1.5);
      
      glEnd();
      glPopMatrix();
   }

   static void Star(double x, double y, double z, double dx,double dy,double dz,
                 double th, double r, double g, double b)
   {
      //  Save transformation
      glPushMatrix();
      //  Offset
      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      glScaled(dx,dy,dz);

      //loop lines

      glColor3f(r, g ,b);
      glBegin(GL_LINE_LOOP);
     

      glVertex3f(0.0,2.0,0.0);
      glVertex3f(0.7,0.7,0.0);
      glVertex3f(1.7,0.7,0.0);
      glVertex3f(1,0.0,0.0);
      glVertex3f(1.5,-1,0.0);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(-1.5,-1,0.0);
      glVertex3f(-1,0.0,0.0);
      glVertex3f(-1.7,.7,0.0);
      glVertex3f(-.7,.7,0.0);


      glEnd();
      glPopMatrix(); //( Jesus When I uncomment this my presents disapear...)
   }

  static void Gift(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
   {
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube for the gift itself
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(1,0,0);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(1,0,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(1,0,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(1,0,0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(1,0,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   glBegin(GL_QUADS);

   //Now create the Lid of the Gift
   //  Front
   glColor3f(1,.1,.6);
   glVertex3f(-1.1,.5, 1.1);
   glVertex3f(+1.1,.5, 1.1);
   glVertex3f(+1.1,+1.1, 1.1);
   glVertex3f(-1.1,+1.1, 1.1);
   //  Back
   glColor3f(1.0,.1,.6);
   glVertex3f(+1.1,.5,-1.1);
   glVertex3f(-1.1,.5,-1.1);
   glVertex3f(-1.1,+1.1,-1.1);
   glVertex3f(+1.1,+1.1,-1.1);
   //  Right
   glColor3f(1,.10,.6);
   glVertex3f(+1.1,.5,+1.1);
   glVertex3f(+1.1,.5,-1.1);
   glVertex3f(+1.1,+1.1,-1.1);
   glVertex3f(+1.1,+1.1,+1.1);
   //  Left
   glColor3f(1,.10,.6);
   glVertex3f(-1.1,.5,-1.1);
   glVertex3f(-1.1,.5,+1.1);
   glVertex3f(-1.1,+1.1,+1.1);
   glVertex3f(-1.1,+1.1,-1.1);
   //  Top
   glColor3f(1,.10,.6);
   glVertex3f(-1.1,+1.1,+1.1);
   glVertex3f(+1.1,+1.1,+1.1);
   glVertex3f(+1.1,+1.1,-1.1);
   glVertex3f(-1.1,+1.1,-1.1);
   //  Bottom
   glColor3f(1,.10,.6);
   glVertex3f(-1.1,.5,-1.1);
   glVertex3f(+1.1,.5,-1.1);
   glVertex3f(+1.1,.5,+1.1);
   glVertex3f(-1.1,.5,+1.1);
   //  End
   glEnd();


   glColor3f(1,2,0);
   glBegin(GL_LINE_LOOP);
   glVertex3f(-.1 , +2,-.1); //create loopies up top
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.2 , +2,-.2);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.3 , +2,-.3);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.4 , +2,-.4);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.5 , +2,-.5);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.1 , +2,+.1);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.2 , +2,-.2);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.3 , +2,-.3);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.4 , +2,-.4);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.5 , +2,-.5);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.1 , +2,+.1);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.2 , +2,+.2);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.3 , +2,+.3);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.4 , +2,+.4);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.5 , +2,+.5);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.1 , +2,+.1);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.2 , +2,+.2);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.3 , +2,+.3);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.4 , +2,+.4);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.5 , +2,+.5);
   
   glEnd();
   
   //  Undo transformations
   glPopMatrix();
}

//experimenting with making all colors into parameters
static void SuperGift(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double mr, double mg, double mb, double tr, double tg, double tb, double lr, double lg, double lb )
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube for the gift itself
   glBegin(GL_QUADS);
   //  Front
   glColor3f(mr,mg,mb);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(mr,mg,mb);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(mr,mg,mb);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(mr,mg,mb);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(mr,mg,mb);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(mr,mg,mb);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();

   glBegin(GL_QUADS);
   //Now create the Lid of the Gift
   //  Front
   glColor3f(tr,tg,tb);
   glVertex3f(-1.1,.5, 1.1);
   glVertex3f(+1.1,.5, 1.1);
   glVertex3f(+1.1,+1.1, 1.1);
   glVertex3f(-1.1,+1.1, 1.1);
   //  Back
   glColor3f(tr,tg,tb);
   glVertex3f(+1.1,.5,-1.1);
   glVertex3f(-1.1,.5,-1.1);
   glVertex3f(-1.1,+1.1,-1.1);
   glVertex3f(+1.1,+1.1,-1.1);
   //  Right
   glColor3f(tr,tg,tb);
   glVertex3f(+1.1,.5,+1.1);
   glVertex3f(+1.1,.5,-1.1);
   glVertex3f(+1.1,+1.1,-1.1);
   glVertex3f(+1.1,+1.1,+1.1);
   //  Left
   glColor3f(tr,tg,tb);
   glVertex3f(-1.1,.5,-1.1);
   glVertex3f(-1.1,.5,+1.1);
   glVertex3f(-1.1,+1.1,+1.1);
   glVertex3f(-1.1,+1.1,-1.1);
   //  Top
   glColor3f(tr,tg,tb);
   glVertex3f(-1.1,+1.1,+1.1);
   glVertex3f(+1.1,+1.1,+1.1);
   glVertex3f(+1.1,+1.1,-1.1);
   glVertex3f(-1.1,+1.1,-1.1);
   //  Bottom
   glColor3f(tr,tg,tb);
   glVertex3f(-1.1,.5,-1.1);
   glVertex3f(+1.1,.5,-1.1);
   glVertex3f(+1.1,.5,+1.1);
   glVertex3f(-1.1,.5,+1.1);
   //  End
   glEnd();

   glBegin(GL_LINE_LOOP);
   
   glColor3f(lr,lg,lb);
   glVertex3f(-.1 , +2,-.1); //create Decorations up top of gift
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.2 , +2,-.2);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.3 , +2,-.3);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.4 , +2,-.4);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.5 , +2,-.5);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.1 , +2,+.1);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.2 , +2,-.2);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.3 , +2,-.3);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.4 , +2,-.4);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.5 , +2,-.5);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.1 , +2,+.1);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.2 , +2,+.2);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.3 , +2,+.3);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.4 , +2,+.4);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(-.5 , +2,+.5);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.1 , +2,+.1);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.2 , +2,+.2);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.3 , +2,+.3);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.4 , +2,+.4);
   glVertex3f(+.1 , +1,-.1);
   glVertex3f(+.5 , +2,+.5);
   
   glEnd();
   //  Undo transformations
   glPopMatrix();
}

void display()
{
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Decide what to draw
   cube(0, 0, 0, 0.05, 0.7 ,0.05 , 0);
   Tree(0, 2, 0, 0.3 ,0.5 ,0.3 , 0);
   Star(0, 2.4 , 0, .15, .15, .15 , 0, 1, 1, 0);
   Gift(0, -19, 3, 3, 1.5, 2 , 0);
   cube(0, 0, 2, 0.08, 0.7, 0.08 , 120);
   
   Tree(0,2.3,2, 0.4,0.6,0.4 , 120);
   Star(0,2.8,2, .10,.10,.10 , 0, 1, .5, 0);
   
   SuperGift(5, -32, -3, 3, 3, 4, 45, 0.545, 0.0, 0.3 ,0.294, 0.0, 0.510, 0.941, 1.0, 1.0);
         
   cube(2,0,2 , 0.05,0.7,0.05 , 0);
   Tree(2,2,2, 0.3,0.5,0.3 , 0);
   Star(2,2.4,2, .15,.15,.15 , 0, 1, 1, 0);        
     
   
   //  White
   glColor3f(1,1,1);
   //  Draw axes
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
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,5);
   //  Print the text string
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   //  Render the scene
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;

   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
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
   Project();
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
  //double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  // zh = fmod(90*t,360);
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("HW4: Sigrunn Sky");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
