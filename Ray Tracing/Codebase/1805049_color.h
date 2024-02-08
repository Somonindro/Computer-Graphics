#include<bits/stdc++.h>
#include "bitmap_image.hpp"
#include <GL/glut.h>
using namespace std;

extern bitmap_image image;

#define PI 3.14159265358979323846

class Color{
    public:
        double red;
        double green;
        double blue;

    Color()
    {
        red = 0.0;
        green = 0.0;
        blue = 0.0;
    }

    Color(double red, double green, double blue)
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    void setColor(double red, double green, double blue)
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    void update()//restrict color value between 0.0 to 1.0
    {
        if (red > 1.0) red = 1.0;
        if (red < 0.0) red = 0.0;
        if (green > 1.0) green = 1.0;
        if (green < 1.0) green = 0.0;
        if (blue > 1.0) blue = 1.0;
        if (blue < 0.0) blue = 0.0;
    }

    void printColor()
    {
        cout<<red<<" "<<green<<" "<<blue<<endl;
    }
};