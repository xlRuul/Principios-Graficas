#include <math.h>

#include <GL/glut.h>


#define GL_PI 3.1415f

GLfloat xRot = 0.45f;
GLfloat yRot = 0.35f;
GLboolean bCull = glIsEnabled(GL_CULL_FACE);
GLboolean bDepth = glIsEnabled(GL_DEPTH_TEST);
GLboolean bOutline = (GLboolean)true;

GLenum shademode = GL_FLAT;

GLfloat ex = 0.0f;
GLfloat ey = 0.0f;
GLfloat ez = -120.0f;
GLfloat delta = 0.01f;
GLfloat deltaR = 0.01f;

///////////////////////////////////////////////////////////


void SetupRC()
{
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set drawing color to green
    glColor3f(0.0f, 1.0f, 0.0f);

    // Set color shading model to flat
    glShadeModel(shademode);

    // Clockwise-wound polygons are front facing; this is reversed
    // because we are using triangle fans
    glFrontFace(GL_CW);
}

// Called to draw scene

void RenderScene(void)
{
    // Reset coordinate system

    glLoadIdentity();
    gluLookAt(ex, ey, ez, ex, ey, 0.0, 0.0, 1.0, 0.0);
    GLfloat x, y, angle;         // Storage for coordinates and angles
    int iPivot = 1;            // Used to flag alternating colors

    // Clear the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Turn culling on if flag is set
    if (bCull)   glEnable(GL_CULL_FACE);
    else        glDisable(GL_CULL_FACE);

    // Enable depth testing if flag is set
    if (bDepth)  glEnable(GL_DEPTH_TEST);
    else        glDisable(GL_DEPTH_TEST);

    // Draw the back side as a wireframe only, if flag is set
    if (bOutline)glPolygonMode(GL_BACK, GL_LINE);
    else        glPolygonMode(GL_BACK, GL_FILL);

    // Save matrix state and do the rotation
    glPushMatrix();
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    // Begin a triangle fan
    glBegin(GL_TRIANGLE_FAN);

    // Pinnacle of cone is shared vertex for fan, moved up z-axis
    // to produce a cone instead of a circle
    glVertex3f(0.0f, 0.0f, 75.0f);

    // Loop around in a circle and specify even points along the circle
    // as the vertices of the triangle fan

    for (angle = 0.0f; angle < (2.0f * GL_PI); angle += (GL_PI / 8.0f))

    {
        // Calculate x and y position of the next vertex
        x = 50.0f * sin(angle);
        y = 50.0f * cos(angle);

        // Alternate color between red and green

        if ((iPivot % 2) == 0)    glColor3f(0.0f, 1.0f, 0.0f);
        else			        glColor3f(1.0f, 0.0f, 0.0f);

        // Increment pivot to change color next time
        iPivot++;

        // Specify the next vertex for the triangle fan
        glVertex2f(x, y);
    }

    // Done drawing fan for cone
    glEnd();

    // Begin a new triangle fan to cover the bottom
    glBegin(GL_TRIANGLE_FAN);

    // Center of fan is at the origin
    glVertex2f(0.0f, 0.0f);

    for (angle = 0.0f; angle < (2.0f * GL_PI); angle += (GL_PI / 8.0f))
    {
        // Calculate x and y position of the next vertex
        x = 50.0f * sin(angle);
        y = 50.0f * cos(angle);

        // Alternate color between red and green
        if ((iPivot % 2) == 0)
            glColor3f(0.0f, 1.0f, 0.0f);
        else
            glColor3f(1.0f, 0.0f, 0.0f);

        // Increment pivot to change color next time
        iPivot++;

        // Specify the next vertex for the triangle fan
        glVertex2f(x, y);
    }

    // Done drawing the fan that covers the bottom
    glEnd();

    // Restore transformations
    glPopMatrix();

    // Flush drawing commands
    glFlush();//glutSwapBuffers();
}
// Called by GLUT library when the window has changed size

void ChangeSize(GLsizei w, GLsizei h)
{    // Set Viewport to window dimensions
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho (-100.0, 100.0, -100, 100, -270.0, 270.0);
    //glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    gluPerspective(60.0, 1.0, 1.5, 500.0);
    glMatrixMode(GL_MODELVIEW);
}



void SpecialKeys(int key, int x, int y)
{
    GLfloat dx, dz;


    if (key == GLUT_KEY_UP)
    {//increase distance from camera to origin
        ex *= (1.0f + deltaR);  ey *= (1.0f + deltaR);	ez *= (1.0f + deltaR);
    }

    if (key == GLUT_KEY_DOWN)
    {//reduce distance from camera to origin (close up)
        ex *= (1.0f - deltaR); ey *= (1.0f - deltaR);	ez *= (1.0f - deltaR);
    }

    if (key == GLUT_KEY_LEFT)
        //Rotate camera around origin in Oxz plane
    {
        dx = -ez;
        dz = ex;
        GLfloat s = sqrtf(ex * ex + ey * ey + ez * ez);
        ex += delta * dx;
        ez += delta * dz;
        GLfloat s1 = sqrtf(ex * ex + ey * ey + ez * ez) / s;
        ex /= s1;
        ey /= s1;
        ey /= s1;
    }

    if (key == GLUT_KEY_RIGHT)
        //Rotate camera around origin in Oxz plane
    {
        dx = -ez;
        dz = ex;
        GLfloat s = sqrtf(ex * ex + ey * ey + ez * ez);
        ex -= delta * dx;
        ez -= delta * dz;
        GLfloat s1 = sqrtf(ex * ex + ey * ey + ez * ez) / s;
        ex /= s1;
        ey /= s1;
        ey /= s1;
    }


    if (key == GLUT_KEY_F1) bCull = !bCull;
    if (key == GLUT_KEY_F2) bDepth = !bDepth;
    if (key == GLUT_KEY_F3) bOutline = !bOutline;
    if (key == GLUT_KEY_F4) {
        if (shademode == GL_FLAT) { shademode = GL_SMOOTH; }
        else { if (shademode == GL_SMOOTH) { shademode = GL_FLAT; } };
        glShadeModel(shademode);
    }

    // Refresh the Window
    glutPostRedisplay();
}


///////////////////////////////////////////////////////////

void girocamara(unsigned char tecla, int x, int y) {
    const float cameraStep = 2.0f;
    const float rotationStep =2.0f;

    switch (tecla) {
    case 'w':
    case 'W':
        ey += cameraStep;
        break;
    case 's':
    case 'S':
        ey -= cameraStep;
        break;
    case 'a':
    case 'A':
        ex += cameraStep;
        break;
    case 'd':
    case 'D':
        ex -= cameraStep;
        break;
    case 'z':
    case 'Z':
        xRot += rotationStep;
        break;
    case 'x':
    case 'X':
        xRot -= rotationStep;
        break;
    default:
        return; // Si no es una tecla válida, salir de la función sin hacer nada.
    }

    glutPostRedisplay(); // Actualizar la escena con los nuevos valores de las variables.
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE/*GLUT_DOUBLE*/ | GLUT_RGB);
    glutInitWindowSize(200, 200);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(girocamara);
    SetupRC();
    glutMainLoop();
    return 0;
}