#include "1805049_spotlight.h"

point mul(point a, double b)
{
    return point(a.x*b,a.y*b,a.z*b);
}

double dotProduct(point a, point b)
{
    return a.x*b.x+a.y*b.y+a.z*b.z;
}

point add(point a, point b)
{
    return point(a.x+b.x,a.y+b.y,a.z+b.z);
}

point sub(point a, point b)
{
    return point(a.x-b.x,a.y-b.y,a.z-b.z);
}

double len(point p) 
{
    return sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
}

point crossProduct(point a,point b)
{
    return point(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);
}

point rodriguesFormula(point x, point a, double theta) 
{   

    double rad = theta*PI/180;

    point p1 = mul(x,cos(rad));
    point p2 = mul(a,((1-cos(rad))*dotProduct(a,x)));
    point p3 = mul(crossProduct(a,x),sin(rad));
    
    return add(add(p1,p2),p3);

}

point normalize(point p)
{
    double len = sqrt(p.x*p.x+p.y*p.y+p.z*p.z);

    p.x /= len;
    p.y /= len;
    p.z /= len;
    
    return p;
}

double calculate_ab(vector<point> points , int i , double ys , char c)
{

    double temp = (points[i].y - ys)/(points[i].y - points[(i+1)%3].y);

    if(c=='x') return points[i].x - (points[i].x - points[(i+1)%3].x)*temp;

    else return points[i].z - (points[i].z - points[(i+1)%3].z)*temp;

}

double determinant(double ara[3][3]){
	double v1 = ara[0][0] * (ara[1][1] * ara[2][2] - ara[1][2] * ara[2][1]);
	double v2 = ara[0][1] * (ara[1][0] * ara[2][2] - ara[1][2] * ara[2][0]);
	double v3 = ara[0][2] * (ara[1][0] * ara[2][1] - ara[1][1] * ara[2][0]);
	return v1 - v2 + v3;
}

vector<double> reAdjust(vector<double>x_ab,vector<double>z_ab,double Min_X,double Max_X)
{
    // reAdjusting x coordinates

    x_ab[0] = min(max(x_ab[0],Min_X),Max_X);
    x_ab[1] = min(max(x_ab[1],Min_X),Max_X);

    vector<double>vec(4);

    vec[0] = x_ab[0]; //xa
    vec[1] = x_ab[1]; //xb
    vec[2] = z_ab[0]; //za
    vec[3] = z_ab[1]; //zb

    if(x_ab[0] >= x_ab[1])
    {
        swap(vec[0],vec[1]);
        swap(vec[2],vec[3]);
    }

    return vec;
}

vector<double> findLeftRightIntersectingCol(vector<point> points , double ys , double Min_X , double Max_X)
{
    vector<double>x_ab(2);
    vector<double>z_ab(2);
    int index = 0;
    
    for(int i=0;i<3;i++)
    {
        if(points[i].y != points[(i+1)%3].y && ys >= min(points[i].y,points[(i+1)%3].y) && ys <= max(points[i].y,points[(i+1)%3].y))
        {
            x_ab[index] = calculate_ab(points,i,ys,'x');
            z_ab[index] = calculate_ab(points,i,ys,'z');
            index++;
        }
    }

    vector<double>vec = reAdjust(x_ab,z_ab,Min_X,Max_X);

    return vec;

}



