#include "1805049_point.h"

class Light{
    public:
        double fallOffParameter;
        point position;

    Light(){

    }

    Light(point position, double fallOffParameter){
        this->position = position;
        this->fallOffParameter = fallOffParameter;
    }

    void setPos(point position)
    {
        this->position = position;
    }

    void setFallOffParameter(double fallOffParameter)
    {
        this->fallOffParameter = fallOffParameter;
    }

    void draw()
    {
        glPointSize(10);
        glBegin(GL_POINTS);
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(position.x, position.y, position.z);
        glEnd();
    }

    void printLight()
    {
        cout<<position.x<<" "<<position.y<<" "<<position.z<<endl;
        cout<<fallOffParameter<<endl;
    }

};