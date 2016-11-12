#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>          // for sqrt function

enum Axis {
    X, Y, Z             // only for convenience
};
class Camera {
public:
    GLfloat pos[3];     // xyz position where camera is located
    GLfloat dir[3];     // xyz position where camera is looked at
    GLfloat gazeV[3];   // view vector
    GLfloat rightV[3];  // right vector
    GLfloat upV[3];     // up vector
    Camera(){
        pos[X] = 0;
        pos[Y] = 75;
        pos[Z] = 0;
        dir[X] = 0;
        dir[Y] = 75;
        dir[Z] = -100;
        upV[X] = 0;
        upV[Y] = 1;
        upV[Z] = 0;
        calculateVectors();
    }
    void calculateVectors() {
     gazeV[X] = dir[X] - pos[X];
     gazeV[Z] = dir[Z] - pos[Z];
        float mag = sqrt(gazeV[X]*gazeV[X]+gazeV[Z]*gazeV[Z]);
        gazeV[X] /= mag;        // normalize gaze vector
        gazeV[Z] /= mag;
        rightV[X] = gazeV[Z];   // rightV = gazeV x upV
        rightV[Z] = -gazeV[X];
    }
};

int window_width = 800;
int window_height = 800;
GLfloat window_centerx = window_width / 2;
GLfloat window_centery = window_height / 2;
 
GLfloat light_position[] = { 200, 400, 200, 1.0 }; // Light position coordinates
GLfloat blackColor[] = { 0.0, 0.0, 0.0, 1.0 };     // Black color RGB values
GLfloat whiteColor[] = { 1.0, 1.0, 1.0, 1.0 };     // White color RGB values
 
Camera cam;
bool keys[256];                                    // Array Used For The Keyboard Routine
 
void myInit(void) {
    // Any initialization before the main loop of GLUT goes here
 
    glEnable(GL_DEPTH_TEST);              // Enable depth Buffering
    glEnable(GL_COLOR_MATERIAL);          // Enable color tracking
    glEnable(GL_NORMALIZE);               // Enables vector normalization (optional)
    glClearColor(0.4f, 0.4f, 0.8f, 1.0f); // Set initial value of color buffer (Set background color also)
 
    // Lighting initialization
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    GLfloat ambientColor[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // Ambient light RGBs (You can modify the arguments if needed.)
 
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);   //Set position.
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);      //Set ambient light.
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteColor);        //Set diffuse component.
    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteColor);       //Set specular component.
 
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); // Set light position
 
    // Initialize camera
    glMatrixMode(GL_PROJECTION);       // Switch  to projection matrix
    glLoadIdentity();                  // Clear current matrix to identity matrix
    gluPerspective(80, 1, 1, 1000000); // Set projection of camera
 
    glMatrixMode(GL_MODELVIEW);        // Switch  to modelview matrix
    glLoadIdentity();                  // Clear current matrix to identity matrix
 
    gluLookAt(cam.pos[X], cam.pos[Y], cam.pos[Z], cam.dir[X], cam.dir[Y],
            cam.dir[Z], 0, 1, 0);     // Set camera position and direction
}
 
void myDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // Clear color buffer with glClearColor() RGB values
    glLoadIdentity();
 
    gluLookAt(cam.pos[X], cam.pos[Y], cam.pos[Z], cam.dir[X], cam.dir[Y],
            cam.dir[Z], cam.upV[X], cam.upV[Y], cam.upV[Z]); // Set camera position and direction
 
    // draw a surface
    glColor3d(0.4f, 0.4f, 0.4f);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(200.0f, 10.0f, 300.0f);    // Top Left
    glVertex3f(-200.0f, 10.0f, 300.0f);   // Top Right
    glVertex3f(-200.0f, 10.0f, -300.0f);  // Bottom Right
    glVertex3f(200.0f, 10.0f, -300.0f);   // Bottom Left
    glEnd();
    glPopMatrix();
    glutSwapBuffers();                    // Swap buffers in double buffered scene
 
}
int main(int argc, char* argv[]) {
//   Initialize GLUT and create window
 
    glutInit(&argc, argv);                                 // Glut initialization function
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  // Set display mode
    glutInitWindowSize(window_width, window_height);           // Set window size
    glutInitWindowPosition(100, 100);                          // Set window position
    glutCreateWindow("OpenGL FPS Camera");                     // Create new window
    glutDisplayFunc(myDisplay);                                // Set display function
    myInit();                                                  // Call defined init function
    glutMainLoop();                                            // Start main loop after initialization
}

void myKeyboard(unsigned char key, int x, int y) {
    keys[(int) key] = true;
}
 
void myKeyboardUp(unsigned char key, int x, int y) {
    keys[(int) key] = false;
}
 
int main(int argc, char* argv[]) {
//   Initialize GLUT and create window
 
    glutInit(&argc, argv);                                 // Glut initialization function
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  // Set display mode
    glutInitWindowSize(window_width, window_height);           // Set window size
    glutInitWindowPosition(100, 100);                          // Set window position
    glutCreateWindow("OpenGL FPS Camera");                     // Create new window
    glutDisplayFunc(myDisplay);                                // Set display function
    glutKeyboardFunc(myKeyboard);      /* newly added*/        // store pressed keys
    glutKeyboardUpFunc(myKeyboardUp);  /* newly added*/        // reset pressed key on keyup
    myInit();                                                  // Call defined init function
    glutMainLoop();                                            // Start main loop after initialization
}

void keyboardEffect() {
    if (keys[27]) {                  // exit when pressed ESC
        exit(0);
    }
    if (keys['W'] || keys['w']) {    // move forward
        cam.pos[X] += cam.gazeV[X];
        cam.pos[Z] += cam.gazeV[Z];
        cam.dir[X] += cam.gazeV[X];
        cam.dir[Z] += cam.gazeV[Z];
    }
    if (keys['S'] || keys['s']) {    // move backward
        cam.pos[X] -= cam.gazeV[X];
        cam.pos[Z] -= cam.gazeV[Z];
        cam.dir[X] -= cam.gazeV[X];
        cam.dir[Z] -= cam.gazeV[Z];
    }
    if (keys['A'] || keys['a']) {    // move left
        cam.pos[X] += cam.rightV[X];
        cam.pos[Z] += cam.rightV[Z];
        cam.dir[X] += cam.rightV[X];
        cam.dir[Z] += cam.rightV[Z];
 
    }
    if (keys['D'] || keys['d']) {    // move right
        cam.pos[X] -= cam.rightV[X];
        cam.pos[Z] -= cam.rightV[Z];
        cam.dir[X] -= cam.rightV[X];
        cam.dir[Z] -= cam.rightV[Z];
    }
}
void myDisplay(void) {
    ...
    keyboardEffect();   /* newly added */
    glLoadIdentity();
    gluLookAt(cam.pos[X], cam.pos[Y], cam.pos[Z], cam.dir[X], cam.dir[Y],
            cam.dir[Z], cam.upV[X], cam.upV[Y], cam.upV[Z]); // Set camera position and direction
 
    // draw a surface
    ...
    glutSwapBuffers();                    // Swap buffers in double buffered scene
    glutPostRedisplay(); /* newly added */
 
}
GLfloat mouse_x = window_centerx;                              // mouse coordinates
GLfloat mouse_y = window_centery;
 
void myMouse(int x, int y) {
    mouse_x = x;
    mouse_y = y;
}
 
int main(int argc, char* argv[]) {
    ...
    glutKeyboardFunc(myKeyboard);               // store pressed keys
    glutKeyboardUpFunc(myKeyboardUp);           // reset pressed key on keyup
    glutPassiveMotionFunc(myMouse);             // store mouse coordinates
    myInit();                                   // Call defined init function
    glutMainLoop();                             // Start main loop after initialization
}