
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include<iostream>
#include<vector>
using namespace std;

float radius = 0;

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

// Global variables
GLfloat eyex = 4, eyey = 4, eyez = 4;
GLfloat centerx = 0, centery = 0, centerz = 0;//look vector = center - eye
GLfloat upx = 0, upy = 1, upz = 0;//up . look != 0 and up ar look er direction same na

/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0,0,0);
        glVertex3f(1,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,0,0);
        glVertex3f(0,1,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,0);
        glVertex3f(0,0,1);
    glEnd();
}

//structure of a point
struct point
{
    float x,y,z;
};


// generate vertices for +X face only by intersecting 2 circular planes
// (longitudinal and latitudinal) at the given longitude/latitude angles
std::vector<std::vector<point>> buildUnitPositiveX(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;
    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;
    std::vector<std::vector<point>> vertices(pointsPerRow);
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis

    
    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            vertices[i].push_back({v[0],v[1],v[2]});
        }
    }

    return vertices;
}

//draw sphere using the vertices generated by buildUnitPositiveX
void drawSphereOneFace(int subdivision)
{
    std::vector<std::vector<point>> vertices = buildUnitPositiveX(subdivision);
    int pointsPerRow = (int)pow(2, subdivision) + 1;
    // draw sphere
    glBegin(GL_QUADS);
    for(unsigned int i = 0; i < pointsPerRow - 1; ++i)
    {
        for(unsigned int j = 0; j < pointsPerRow - 1; ++j)
        {
            // 4 vertices of a quad
            glVertex3f(vertices[i][j].x, vertices[i][j].y, vertices[i][j].z);// 1st vertex
            glVertex3f(vertices[i][j+1].x, vertices[i][j+1].y, vertices[i][j+1].z);// 2nd vertex
            glVertex3f(vertices[i+1][j+1].x, vertices[i+1][j+1].y, vertices[i+1][j+1].z);// 3rd vertex
            glVertex3f(vertices[i+1][j].x, vertices[i+1][j].y, vertices[i+1][j].z);// 4th vertex
        }
    }
    glEnd();
}

void drawSphereFace(float x,float y,float z,float r,float g,float b,float yAngle,float zAngle)
{
    glPushMatrix();
    glColor3f(r,g,b);   
    glTranslatef(x,y,z);
    glRotated(yAngle,0,1,0);
    glRotated(zAngle,0,0,1);
    glScalef(radius,radius,radius);
    drawSphereOneFace(4);
    glPopMatrix();
}

void drawSphere()
{
    //side faces
    drawSphereFace(1-(radius*sqrt(2)),0,0,  0,0,1,  0,0);
    drawSphereFace(0,0,-1+(radius*sqrt(2)),  0,1,0,  90,0);
    drawSphereFace(-1+(radius*sqrt(2)),0,0,  0,0,1,  180,0);
    drawSphereFace(0,0,1-(radius*sqrt(2)),  0,1,0,  270,0);

    //top and bottom faces
    drawSphereFace(0,-1+(radius*sqrt(2)),0,  1,0,0,  0,-90);
    drawSphereFace(0,1-(radius*sqrt(2)),0,  1,0,0,  0,90);

}


void drawTriangle(){
    glTranslatef(radius/sqrt(3), radius/sqrt(3), radius/sqrt(3));
    glScalef(1-(radius*sqrt(2)), 1-(radius*sqrt(2)), 1-(radius*sqrt(2)));
    glBegin(GL_TRIANGLES);  
        glVertex3f( 1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();
}

void drawPyramidFace(float xAngle,float yAngle,float a, float b)
{
    glPushMatrix();
    glRotated(xAngle,1,0,0);
    glRotated(yAngle,0,1,0);
    glColor3f(a, b, 1.0f);     
    drawTriangle();
    glPopMatrix();
}

void drawUpperPyramid()
{
    //upper pyramid
    drawPyramidFace(0,0,  1.0f, 0.0f);
    drawPyramidFace(0,90,  0.0f, 1.0f);
    drawPyramidFace(0,180,  1.0f, 0.0f);
    drawPyramidFace(0,270,  0.0f, 1.0f);
}

void drawLowerPyramid()
{
    //lower pyramid
    drawPyramidFace(180,0,  1.0f, 0.0f);
    drawPyramidFace(180,90,  0.0f, 1.0f);
    drawPyramidFace(180,180,  1.0f, 0.0f);
    drawPyramidFace(180,270,  0.0f, 1.0f);
}

void drawOctahedron()
{
    drawUpperPyramid();
    drawLowerPyramid();
}

void drawSegmentedCylinder(float angle, float height) {

    int numSegments = 100;  // Number of segments for the circular base
    float segmentAngle = angle*(3.1416/180);  // Angle between two segments

    glPushMatrix();
    glTranslatef(0,-(sqrt(2)-2*radius)/2,0);
    glRotatef(70.53/2,0,1,0); 

    //the sides of the cylinder
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= numSegments; ++i) {
        float theta = i*segmentAngle/numSegments;

        float xCoordinate = radius * cos(theta);//the coordinates of the points on the circle
        float yCoordinate = radius * sin(theta);

        glVertex3f(xCoordinate, 0.0f, yCoordinate);  // Lower vertex for the side
        glVertex3f(xCoordinate, height, yCoordinate);  // Upper vertex for the side
    }
    glEnd();
    glPopMatrix();

}

void drawCylinderFace(float xAngle,float yAngle)
{
    glPushMatrix();
    glColor3f(1,1,0);
    glRotatef(yAngle,0,1,0);
    glRotatef(xAngle,1,0,0);
    glTranslatef((1-sqrt(2)*radius)/2,(1-sqrt(2)*radius)/2,0);
    glRotatef(45,0,0,1);
    drawSegmentedCylinder(70.53f, sqrt(2)-2*radius);
    glPopMatrix();
}

void drawUpperCylinders()
{
    //the upper faces
    drawCylinderFace(0,0);
    drawCylinderFace(0,90);
    drawCylinderFace(0,180);
    drawCylinderFace(0,270);
}

void drawLowerCylinders()
{
    //the lower faces
    drawCylinderFace(180,0);
    drawCylinderFace(180,90);
    drawCylinderFace(180,180);
    drawCylinderFace(180,270);
}

void drawSideCylinders()
{
    //the side faces
    drawCylinderFace(90,0);
    drawCylinderFace(90,90);
    drawCylinderFace(90,180);
    drawCylinderFace(90,270);
}

void drawCylinder()
{
    drawUpperCylinders();
    drawLowerCylinders();
    drawSideCylinders();

}


/*  Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */
void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    // control viewing (or camera)
    gluLookAt(eyex,eyey,eyez,//camera position
              centerx,centery,centerz,//camera kon point a takaye ache
              upx,upy,upz);

    //drawAxes();
    drawOctahedron();
    drawCylinder();
    drawSphere();

    glutSwapBuffers();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    double v = 0.25;
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;
    switch (key) {

    // Control what is shown
    case 'd':
        eyex += v * (upy*lz);
        eyez += v * (-lx*upy);
        s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case 'a':
        eyex += v * (-upy*lz);
        eyez += v * (lx*upy);
        s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case 'w':
        eyey += v;
        break;
    case 's':
        eyey -= v;
        break;

    case ',':
        radius += (1/(16*sqrt(2)));
        if(radius > (1/sqrt(2)))
            radius = (1/sqrt(2));
        //cout << radius << endl;
        break;

    case '.':
        radius -= (1/(16*sqrt(2)));
        if(radius < 0)
            radius = 0;
        //cout << radius << endl;
        break;

    // Control exit
    case 27:    // ESC key
        exit(0);    // Exit window
        break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x,int y) {
    double v = 0.25;
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;
    switch (key) {
    case GLUT_KEY_LEFT:
        eyex += v * (upy*lz);
        eyez += v * (-lx*upy);
        s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case GLUT_KEY_RIGHT:
        eyex += v * (-upy*lz);
        eyez += v * (lx*upy);
        s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case GLUT_KEY_UP:
        eyey += v;
        break;
    case GLUT_KEY_DOWN:
        eyey -= v;
        break;
    
    
    
    default:
        return;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(640, 640);               // Set the window's initial width & height
    glutInitWindowPosition(50, 50);             // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing");      // Create a window with the given title
    glutDisplayFunc(display);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);         // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener);        // Register callback handler for special-key event
    initGL();                                   // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop
    return 0;
}
