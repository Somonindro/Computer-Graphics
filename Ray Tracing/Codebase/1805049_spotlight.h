#include "1805049_light.h"

class SpotLight{
    public:
        double fallOffParameter;
        Light pointLight;
        point direction;
        double cutoffAngle; // in degree

    SpotLight(){

    }

    SpotLight(Light pointLight, point direction,double fallOffParameter, double cutoffAngle){
        this->pointLight = pointLight;
        this->direction = direction;
        this->fallOffParameter = fallOffParameter;
        this->cutoffAngle = cutoffAngle;
    }

    void setSpotLight(Light pointLight, point direction,double fallOffParameter ,double cutoffAngle){
        this->pointLight = pointLight;
        this->direction = direction;
        this->fallOffParameter = fallOffParameter;
        this->cutoffAngle = cutoffAngle;
    }

    void setFallOffParameter(double fallOffParameter)
    {
        this->fallOffParameter = fallOffParameter;
    }

    void draw()
    {
        glPointSize(15);
        glBegin(GL_POINTS);
        glColor3f(1.0,1.0,1.0);
        glVertex3f(pointLight.position.x, pointLight.position.y, pointLight.position.z);
        glEnd();
    }

    void printSpotLight()
    {
        cout<<"SpotLight"<<endl;
        cout<<"Position : "<<pointLight.position.x<<" "<<pointLight.position.y<<" "<<pointLight.position.z<<endl;
        cout<<"Direction : "<<direction.x<<" "<<direction.y<<" "<<direction.z<<endl;
        cout<<cutoffAngle<<endl;
        cout<<fallOffParameter<<endl;
    }

};