#include "1805049_functionality.h"

class Ray{
    public:
        point origin;
        point direction;

    Ray(){

    }
    
    Ray(point origin, point direction){
        this->origin = origin;
        direction = normalize(direction);
        this->direction = direction;
    }

    void setRay(point origin, point direction){
        this->origin = origin;
        direction = normalize(direction);
        this->direction = direction;
    }

    void printRay()
    {
        cout<<"Origin : "<<origin.x<<" "<<origin.y<<" "<<origin.z<<endl;
        cout<<"Direction : "<<direction.x<<" "<<direction.y<<" "<<direction.z<<endl;
    }
};