#include "1805049_color.h"

// during operation , point is converted into 4x1 matrix in matrix.h file ( transformPoint function )

class point
{
	public: 
    
        double x,y,z;

    point()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

	point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    point(const point &p){
        x = p.x;
        y = p.y;
        z = p.z;
    }

    void setValue(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void print()
    {
        cout<<x<<" "<<y<<" "<<z<<endl;
    }

};