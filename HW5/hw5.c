/* Sigrunn Sky HW5
 * Used EX13.c as a template/starting point
 * https://www.opengl.org/discussion_boards/showthread.php/167115-Creating-cylinder for help on math for cyl
 * 
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
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world

int fps= 0;         //First person toggle

// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  33;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   2;  // Elevation of light

//fps
int turn =300.0;  // turning degrees.. 300 fo you can see the trees.

double xPoint = 0;  //Eyeballs pointing in x & z position 
double zPoint = 0;

double xEye = 10; //Eyeball location
double yEye = 1;
double zEye = 10;


static void cyl(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, float radius, float halfheight)
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
     
   glColor3f(.5,.3,.1);//Brown
  
   //Top
   glBegin(GL_TRIANGLE_FAN);

   glNormal3d(0,1,0);
      glVertex3d(0.0, halfheight, 0.0);

      for(double j = 0.0; j < 360; j+=.125) {
         glVertex3d(radius * cos(j), halfheight, radius * sin(j));
      }
   glEnd();

   //Bottom
   glBegin(GL_TRIANGLE_FAN);
   glNormal3d(0,-1,0);
      glVertex3d(0.0, -halfheight, 0.0);

      for(double j = 0.0; j < 360; j+=.125) {
         glVertex3d(radius * cos(j), -halfheight, radius * sin(j));
      }
   glEnd();

   glBegin(GL_TRIANGLE_STRIP);
   for (int i = 0; i <= 360; i++) {
      double x = radius * Cos(i);
      double y = halfheight;
      double z = radius * Sin(i);

      glNormal3d(x, y, z);

      glVertex3d(x, y, z);
      glVertex3d(x, -y, z);
   }
   glEnd();

   glPopMatrix(); 
}
static void flooring(double x, double y, double z, double dx,double dy,double dz,
                 double th )
{
	glPushMatrix();

	glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);

	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
   glColor3f(0.412, 0.412, 0.412); //dim grey slate
	
	glVertex3f(-1, 0, -1);
	glVertex3f(-1, 0, 1 );
	glVertex3f( 1, 0, 1);
	glVertex3f( 1, 0, -1);
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
      
      //  Triangles
      glBegin(GL_TRIANGLES);           // 1st pyramid
      // Front
      glNormal3f(cos(45),1,sin(45));
      glColor3f(0.196, 0.804, 0.196);     
      glVertex3f( 0, 1, 0); //top point
      glColor3f(0.000, 0.392, 0.000);     
      glVertex3f(1, 0, 0); 
      glColor3f(0.000, 0.392, 0.000);    
      glVertex3f(0, 0, 1);
 
      // Right
      glNormal3f(cos(45),1,-sin(45));
      glColor3f(0.196, 0.804, 0.196);    
      glVertex3f(0.0, 1, 0.0);
      glColor3f(0.000, 0.392, 0.000);    
      glVertex3f(1, 0, 0);
      glColor3f(0.000, 0.392, 0.000);    
      glVertex3f(0, 0, -1);
 
      // Back
      glNormal3f(-cos(45),1,-sin(45));
      glColor3f(0.196, 0.804, 0.196);   
      glVertex3f(0.0, 1, 0.0);
      glColor3f(0.000, 0.392, 0.000);   
      glVertex3f(-1, 0, 0);
      glColor3f(0.000, 0.392, 0.000);    
      glVertex3f(0, 0, -1.0);
 
      // Left
      glNormal3f(-cos(45),1,sin(45));
      glColor3f(0.196, 0.804, 0.196);      
      glVertex3f( 0.0, 1, 0.0);
      glColor3f(0.000, 0.392, 0.000);   
      glVertex3f(-1,0,0);
      glColor3f(0.000, 0.392, 0.000);       
      glVertex3f(0,0, 1.0);
      glEnd();   // Done drawing the pyramid
      
      glBegin(GL_QUADS);
      glColor3f(0.000, 0.392, 0.000); 
      glNormal3f(0, -1, 0);
      glVertex3f(-1, 0, 0);
      glVertex3f(0, 0, 1);
      glVertex3f(1, 0, 0);
      glVertex3f(0, 0, -1);
      glEnd();
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
   glRotated(th,0,0,0);
   glScaled(dx,dy,dz);
   //  Cube for the gift itself
   glBegin(GL_QUADS);
   //  Front
   glColor3f(mr,mg,mb);
   glNormal3f(0,0,1);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(mr,mg,mb);
   glNormal3f(0,0,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(mr,mg,mb);
   glNormal3f(1,0,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(mr,mg,mb);
   glNormal3f(-1,0,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(mr,mg,mb);
   glNormal3f(0,1,0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(mr,mg,mb);
   glNormal3f(0,-1,0);
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
   glNormal3f(0,0,1);
   glVertex3f(-1.1,.5, 1.1);
   glVertex3f(+1.1,.5, 1.1);
   glVertex3f(+1.1,+1.1, 1.1);
   glVertex3f(-1.1,+1.1, 1.1);
   //  Back
   glColor3f(tr,tg,tb);
   glNormal3f(0,0,-1);
   glVertex3f(+1.1,.5,-1.1);
   glVertex3f(-1.1,.5,-1.1);
   glVertex3f(-1.1,+1.1,-1.1);
   glVertex3f(+1.1,+1.1,-1.1);
   //  Right
   glColor3f(tr,tg,tb);
   glNormal3f(1, 0, 0);
   glVertex3f(+1.1,.5,+1.1);
   glVertex3f(+1.1,.5,-1.1);
   glVertex3f(+1.1,+1.1,-1.1);
   glVertex3f(+1.1,+1.1,+1.1);
   //  Left
   glColor3f(tr,tg,tb);
   glNormal3f(-1,0,0);
   glVertex3f(-1.1,.5,-1.1);
   glVertex3f(-1.1,.5,+1.1);
   glVertex3f(-1.1,+1.1,+1.1);
   glVertex3f(-1.1,+1.1,-1.1);
   //  Top
   glColor3f(tr,tg,tb);
   glNormal3f(0,1,0);
   glVertex3f(-1.1,+1.1,+1.1);
   glVertex3f(+1.1,+1.1,+1.1);
   glVertex3f(+1.1,+1.1,-1.1);
   glVertex3f(-1.1,+1.1,-1.1);
   //  Bottom
   glColor3f(tr,tg,tb);
   glNormal3f(0,-1,0);
   glVertex3f(-1.1,.5,-1.1);
   glVertex3f(+1.1,.5,-1.1);
   glVertex3f(+1.1,.5,+1.1);
   glVertex3f(-1.1,.5,+1.1);
   //  End
   glEnd();

   glBegin(GL_LINE_LOOP);
   glNormal3f(0, 1, 0);
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
   glVertex3f(+.1, +1, -.1);
   glEnd();
   //  Undo transformations
   glPopMatrix();
}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
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
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
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

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (!fps)
   {
      if (mode) //Perspective.
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
   }
   else 
   {
      xPoint = +2*dim*Sin(turn); //Set the camera vector based on the turn (degrees)
      zPoint = -2*dim*Cos(turn);
      gluLookAt(xEye,yEye,zEye, xPoint+xEye,yEye,zPoint+zEye, 0,1,0); 
   }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight ,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
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
   }
   else
     glDisable(GL_LIGHTING);

   cyl(0, .3, 0 ,  0.1, 0.5, 0.1 , 0, 1, 2);
   flooring(0, -.01, 0, 10 ,0 ,10 , 0);
   flooring(0, -.03, 0, 10, 0, 10, 0);
   Tree(0, 2, 0, 0.35 ,0.5 ,0.35 , 0);
   Tree(0,1.5, 0, .5, .7, .5, 45);
   Tree(0,1, 0, .7, .9, .7, 30);
   SuperGift(0, .27, 1, .5, .4, .7, 0, 0.545, 0.0, 0.3 ,0.294, 0.0, 0.510, 0.941, 1.0, 1.0);
   SuperGift(1, .17, 0, .4, .3, .3, 30, 0.7, 0.5, 0.0 ,1, 0.0, 0.310, 1, .9, .9);
   cyl(1, .3, 1 ,  0.1, 0.5, 0.1 , 0, 1, 2);
   Tree(1, 2, 1, 0.4 ,0.6 ,0.4 , 45);
   Tree(1,1.5, 1, .6, .8, .6, 0);
   Tree(1,1, 1, .8, 1, .8, 60);
   
   cyl(0, .3, 2 ,  0.1, 0.5, 0.1 , 0, 1, 2);
   Tree(0, 2, 2, 0.2 ,0.5 ,0.2 , 35);
   Tree(0,1.5, 2, .4, .6, .4, 45);
   Tree(0,1, 2, .5, .7, .5, 60);
               
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
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
   if(fps)
   {
      Print("FPS: On View Angle=%d",turn);
   }

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
void special(int key,int x,int y)
{
   if (!fps)
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
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;
   //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;
   //  Flip sign
   else if (key == GLUT_KEY_F9)
      one = -one;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   }
   else
   {
      double move = 0.05; 
      //  Right arrow key - increase angle by 1 degrees
      if (key == GLUT_KEY_RIGHT){
         turn += 1;
      }
      //  Left arrow key - decrease angle by 1 degrees
      else if (key == GLUT_KEY_LEFT){
         turn -= 1;
      }
      //   Move in the correct vector direction
      else if (key == GLUT_KEY_UP){
         xEye += xPoint*move;
         zEye += zPoint*move;
      }
      //  
      else if (key == GLUT_KEY_DOWN) {
         xEye -= xPoint*move;
         zEye -= zPoint*move;
      }
      turn %=360;
   }

   //  Update projection
   Project(mode?fov:0,asp,dim);
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
   else if (ch == 'f' || ch == 'F')
      fps = 1-fps;
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
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
   glutInitWindowSize(400,400);
   glutCreateWindow("HW5: Sigrunn Sky");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
