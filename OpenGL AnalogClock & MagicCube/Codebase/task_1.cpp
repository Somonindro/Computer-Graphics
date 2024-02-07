#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <iostream>

float clockRadius = 0.5f;
float handSize = 0.16f;
float pendulumLength = 0.5f;
float hourAngle = 0.0f;
float minuteAngle = 0.0f;
float secondAngle = 0.0f;
float pendulumAngle = 0.0f;

void drawMarks(int wid,int div,int lim,float range){
    glLineWidth(wid);
    glBegin(GL_LINES);
    for (int i = 0; i < lim; ++i) {
        float angle = i * div * 3.14159 / 180;
        float x1 = (clockRadius - .1) * cos(angle);
        float y1 = (clockRadius - .1) * sin(angle);
        float x2 = (clockRadius - range) * cos(angle);
        float y2 = (clockRadius - range) * sin(angle);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    }
    glEnd();
}

void drawHand(int angle,float add)
{
    glPushMatrix();
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, handSize+add);
    glEnd();
    glPopMatrix();
}

void drawBelowPart()
{
    glBegin(GL_LINE_STRIP);
        glVertex2d(-.23,-pendulumLength+0.05);
        glVertex2d(-0.23,-2*pendulumLength);
        glVertex2d(0,-2*pendulumLength-0.1);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex2d(.23,-pendulumLength+0.05);
        glVertex2d(0.23,-2*pendulumLength);
        glVertex2d(0,-2*pendulumLength-0.1);
    glEnd();
}

void drawCircle(float radius,float delta)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(360);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(x, y+delta);
    }
    glEnd();
}

void drawPendulum()
{
    glPushMatrix();
    
    glTranslatef(0.0f, -pendulumLength, 0.0f);
    glRotatef(pendulumAngle, 0.0f, 0.0f, 1.0f);

    drawCircle(0.03,-pendulumLength);
    
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, -pendulumLength);
    glEnd();

    glPopMatrix();
}


void drawClock() {

    drawCircle(clockRadius,0);// Draw the clock face
    drawBelowPart();

    drawMarks(2.5,30,12,0.2);// Draw the hour marks
    drawMarks(2,6,60,0.13);// Draw the minute marks

    glLineWidth(5);
    drawHand(hourAngle,0);// Draw the hour hand
    drawHand(minuteAngle,0.1f);// Draw the minute hand
    drawHand(secondAngle,0.2f);// Draw the second hand

    drawPendulum();
}


void updateClock(int value) {

    time_t currentTime;
    time(&currentTime);
    struct tm *localTime = localtime(&currentTime);
    //std::cout<<localTime->tm_hour<<":"<<localTime->tm_min<<":"<<localTime->tm_sec<<std::endl;

    secondAngle = -(6.0f * localTime->tm_sec);
    minuteAngle = -(6.0f * localTime->tm_min);
    hourAngle = -(30.0f * (localTime->tm_hour % 12) + localTime->tm_min / 2.0f);

    // Update pendulum angle using physics (simple harmonic motion)
    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    pendulumAngle = 20.0f * sinf(2.0f * 3.1416f * time / 2.0f);

    glutPostRedisplay();
    glutTimerFunc(10, updateClock, 0);
}


void reshape(int width, int height) {

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the coordinate system based on the aspect ratio
    if (width > height) {
        float aspectRatio = (float)width / (float)height;
        glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);
    } else {
        float aspectRatio = (float)height / (float)width;
        glOrtho(-1.0, 1.0, -aspectRatio, aspectRatio, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
}


// Function to display the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    drawClock();
    glFlush();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 800);
    glutCreateWindow("Analog Clock with Pendulum");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(0, updateClock, 0);
    glutMainLoop();
    return 0;
}
