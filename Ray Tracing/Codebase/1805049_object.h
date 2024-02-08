#include "1805049_ray.h"

class Object;
extern vector <Light*> lights;
extern vector <SpotLight*> spotlights;
extern vector <Object*> objects;
extern int recursionLevel;
extern point pos; //camera position

class Object {
public:
		point reference_point;
		double length;
        int shine;
		Color color;
        double ambientCoefficient,diffuseCoefficient,specularCoefficient,reflectionCoefficient;
		
		Object(){

		}

		void setColor(Color color){
            this->color = color;
        }
        

        virtual Color getColorAt(point point){
            return Color(this->color.red, this->color.green, this->color.blue);
        }
    	
		void setShine(int shine){
            this->shine = shine;
        }

        void setLength(double length){
            this->length = length;
        }

		void setCoefficients(double ambientCoefficient,double diffuseCoefficient,double specularCoefficient,double reflectionCoefficient){
            this->ambientCoefficient = ambientCoefficient;
            this->diffuseCoefficient = diffuseCoefficient;
            this->specularCoefficient = specularCoefficient;
            this->reflectionCoefficient = reflectionCoefficient;
        }   

        virtual void draw() = 0;

		virtual double intersect_handler(Ray ray, Color &color, int level) = 0;

        virtual Ray getNormal(point point, Ray incidentRay) = 0;
        
		virtual double intersect(Ray ray, Color &color, int level)
        {
            double t = intersect_handler(ray, color, level);

            if(t < 0) return -1;
            if(level == 0) return t;

            point intersectionPoint = add(ray.origin , mul(ray.direction , t));
            Color colorAtIntersection = getColorAt(intersectionPoint);

            color.red = colorAtIntersection.red * ambientCoefficient;
            color.green = colorAtIntersection.green * ambientCoefficient;
            color.blue = colorAtIntersection.blue * ambientCoefficient;

            Ray normal = getNormal(intersectionPoint,ray);

            for(int i = 0; i < lights.size(); i++)
            {
                point lightPosition = lights[i]->position;
                point lightDirection = sub(intersectionPoint , lightPosition);
                lightDirection = normalize(lightDirection);

                // double fallOffParameter = 0.000002;
                
                Ray lightRay = Ray(lightPosition, lightDirection);// light ray from intersection point to light source
                Ray normal = getNormal(intersectionPoint,lightRay); // find normal at intersection point

                lightRay.origin = add(lightRay.origin , mul(lightRay.direction , 1e-9));
                
                double distance = len(sub(intersectionPoint , lightPosition));
                double scaling_factor = exp(-lights[i]->fallOffParameter * distance * distance);
                if(distance < 1e-5) continue;

                bool obscured = false;

                for(Object *obj : objects){
                    double t3 = obj->intersect_handler(lightRay, color, 0);
                    if(t3 > 0 && t3 + 1e-5 < distance){
                        obscured = true;
                        break;
                    }
                }

                if(!obscured){
                    double val = max(0.0, -dotProduct(lightRay.direction , normal.direction))*scaling_factor; // lambert value (cosine of angle between normal and light ray)
                    
                    point temp = sub(lightRay.direction , mul(normal.direction,2*dotProduct(lightRay.direction , normal.direction)));
                    Ray reflection = Ray(intersectionPoint, temp); //Reflected ray from intersection point
                    double phong = max(0.0,-dotProduct(ray.direction , reflection.direction));

                    color.red += diffuseCoefficient * val * colorAtIntersection.red;
                    color.green += diffuseCoefficient * val * colorAtIntersection.green;
                    color.blue += diffuseCoefficient * val * colorAtIntersection.blue;
                    
                    color.red += specularCoefficient * pow(phong,shine) *scaling_factor;
                    color.green += specularCoefficient * pow(phong,shine) *scaling_factor;
                    color.blue += specularCoefficient * pow(phong,shine) * scaling_factor;

                }
            }

            for(int i = 0; i < spotlights.size(); i++)
            {
                point lightPosition = spotlights[i]->pointLight.position;
                point lightDirection = sub(intersectionPoint , lightPosition);
                lightDirection = normalize(lightDirection);

                // double fallOffParameter = 0.0000002;
                double dot = dotProduct(lightDirection , spotlights[i]->direction);
                double angle = acos(fabs(dot/(len(lightDirection)*len(spotlights[i]->direction)))) * (180.0/PI);

                if(angle<spotlights[i]->cutoffAngle){
                    bool obscured = false;
                    Ray lightRay = Ray(spotlights[i]->pointLight.position, normalize((intersectionPoint , spotlights[i]->pointLight.position)));

                    double distance = len(sub(intersectionPoint , lightRay.origin));
                    double scaling_factor = exp(-spotlights[i]->fallOffParameter * distance * distance);

                    for(Object *obj : objects){
                        double t3 = obj->intersect_handler(lightRay, color, 0);
                        if(t3 > 0 && t3 < distance){
                            // cout<<"check"<<endl;
                            obscured = true;
                            break;
                        }
                    }

                    if(!obscured){
                        double val = max(0.0, -dotProduct(lightRay.direction , normal.direction))*scaling_factor; // lambert value (cosine of angle between normal and light ray)                       
                        point temp = sub(lightRay.direction , mul(normal.direction,2*dotProduct(lightRay.direction , normal.direction)));

                        Ray reflection = Ray(intersectionPoint, normalize(temp)); //Reflected ray from intersection point
                        double phong = max(0.0,-dotProduct(ray.direction , reflection.direction));

                        color.red += diffuseCoefficient * val * colorAtIntersection.red;
                        color.green += diffuseCoefficient * val * colorAtIntersection.green;
                        color.blue += diffuseCoefficient * val * colorAtIntersection.blue;

                        color.red += specularCoefficient * pow(phong,shine)*scaling_factor;
                        color.green += specularCoefficient * pow(phong,shine)*scaling_factor;                  
                        color.blue += specularCoefficient * pow(phong,shine)*scaling_factor;
                    }
                }
            }

            if(level < recursionLevel){
                Ray normal = getNormal(intersectionPoint,ray); // find normal at intersection point

                // find reflected ray
                point temp2 = sub(ray.direction , mul(normal.direction , 2*dotProduct(ray.direction , normal.direction)));
                Ray reflectionRay = Ray(intersectionPoint, temp2);

                reflectionRay.origin = add(reflectionRay.origin , mul(reflectionRay.direction , 1e-9));

                int nearestObjectIndex = -1;
                double t = -1;
                double tMin = 1e9;

                for(int k=0;k<(int)objects.size();k++)
                {
                    t = objects[k]->intersect(reflectionRay,color, 0);
                    if(t> 0 && t<tMin)
                    {
                        tMin = t ;
                        nearestObjectIndex = k;
                    }
                }

                if(nearestObjectIndex != -1)
                {
                    Color colorTemp(0,0,0); // refelction color
                    double t = objects[nearestObjectIndex]->intersect(reflectionRay,colorTemp, level+1);
                    
                    color.red += colorTemp.red * reflectionCoefficient;
                    color.green += colorTemp.green * reflectionCoefficient;
                    color.blue += colorTemp.blue * reflectionCoefficient;

                }
                              
            }
            return t;
        }
};

//infinite checkerboard floor
struct Floor : public Object
{
    point dir;

    Floor(){

    }

    Floor(int tileWidth)
    {
        length = tileWidth;
    }

    virtual Color getColorAt(point point)
    {
        int tileX = floor(point.x/ length);
		int tileY = floor(point.y /length);

		if (((tileX + tileY) % 2) == 0)
		{
			return Color(1,1,1);
		}
		else
		{
			return Color(0,0,0);
		}
    }

    virtual Ray getNormal(point point1, Ray incidentRay)
    {
        if(incidentRay.direction.z > 0) {
            // dir.setValue(0,0,1); 
            // dir.print();

            return Ray(point1, point(0, 0, 1));
        }
        else {
            return Ray(point1, point(0, 0, -1));
        }

        // return(Ray(point1, point(0,0,1)));
    }

    virtual void draw()
    {
        // pos.print();
        // Determine the range of tiles to draw in front, back, left, and right of the camera
        int numTilesToDraw = 20;
        int startX = pos.x / length - numTilesToDraw;
        int endX = pos.x / length + numTilesToDraw;
        int startY = pos.y / length - numTilesToDraw;
        int endY = pos.y / length + numTilesToDraw;

        // Draw the tiles within the specified range
        for (int i = startX; i <= endX; i++)
        {
            for (int j = startY; j <= endY; j++)
            {
                if ((i + j) % 2 == 0)
                    glColor3f(1, 1, 1); // white
                else
                    glColor3f(0, 0, 0); // black

                glBegin(GL_QUADS);
                {
                    glVertex3f(i * length, j * length, 0);
                    glVertex3f((i + 1) * length, j * length, 0);
                    glVertex3f((i + 1) * length, (j + 1) * length, 0);
                    glVertex3f(i * length, (j + 1) * length, 0);
                }
                glEnd();
            }
        }
    }

    virtual double intersect_handler(Ray ray, Color &color, int level)
    {
        point normal = point(0, 0, 1);
        double dotP = dotProduct(normal , ray.direction);

        double t = - dotProduct(normal , ray.origin) / dotP;

        if (t > 1000) t = -1; // clip far away objects
        
        return t;
    }
};

struct Triangle: public Object
{
    point a;
    point b; 
    point c;
    point normal;

    Triangle(){

    }

    Triangle(point a, point b, point c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    virtual Ray getNormal(point point, Ray incidentRay)
    {
        normal = crossProduct(sub(c, a) , sub(b , a));
        normal = normalize(normal);
        
        if(dotProduct(incidentRay.direction , normal) > 0){//
            normal.setValue(-normal.x, -normal.y, -normal.z);
            return Ray(point, normal);
        }
        else{
            return Ray(point, normal);
        }
    }

    virtual void draw(){
        glColor3f(color.red, color.green, color.blue);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
        glEnd();
    }

    virtual double intersect_handler(Ray ray, Color &color, int level){

        double betaMatrix[3][3] = {
            {a.x - ray.origin.x, a.x - c.x, ray.direction.x},
            {a.y - ray.origin.y, a.y - c.y, ray.direction.y},
            {a.z - ray.origin.z, a.z - c.z, ray.direction.z}
        };
        double gammaMatrix[3][3] = {
            {a.x - b.x, a.x - ray.origin.x, ray.direction.x},
            {a.y - b.y, a.y - ray.origin.y, ray.direction.y},
            {a.z - b.z, a.z - ray.origin.z, ray.direction.z}
        };
        double tMatrix[3][3] = {
            {a.x - b.x, a.x - c.x, a.x - ray.origin.x},
            {a.y - b.y, a.y - c.y, a.y - ray.origin.y},
            {a.z - b.z, a.z - c.z, a.z - ray.origin.z}
        };
        double AMat[3][3] {
            {a.x - b.x, a.x - c.x, ray.direction.x},
            {a.y - b.y, a.y - c.y, ray.direction.y},
            {a.z - b.z, a.z - c.z, ray.direction.z}
        };

        double Adeterminant = determinant(AMat);
        double beta = determinant(betaMatrix) / Adeterminant;
        double gamma = determinant(gammaMatrix) / Adeterminant;
        double t = determinant(tMatrix) / Adeterminant;

        if (beta + gamma < 1 && beta > 0 && gamma > 0 && t > 0){
            return t;
        }
        else{
            return -1;
        }
    }

};

struct Sphere : public Object{

    Sphere(){
    }

    Sphere(point center, double radius){
        reference_point = center;
        length = radius;
    }

    virtual Ray getNormal(point point, Ray incidentRay){
        return Ray(point, sub(point , reference_point));
    }

    virtual void draw(){
        int stacks = 30;
        int slices = 20;

        point points[100][100];
        int i, j;
        double h, r;
        // generate points
        for (i = 0; i <= stacks; i++)
        {
            h = length * sin(((double)i / (double)stacks) * (PI / 2));
            r = length * cos(((double)i / (double)stacks) * (PI / 2));
            for (j = 0; j <= slices; j++)
            {
                points[i][j].x = r * cos(((double)j / (double)slices) * 2 * PI);
                points[i][j].y = r * sin(((double)j / (double)slices) * 2 * PI);
                points[i][j].z = h;
            }
        }
        //draw quads using generated points
        for (i = 0; i < stacks; i++)
        {
            glPushMatrix();
            glTranslatef(reference_point.x, reference_point.y, reference_point.z);
            glColor3f(color.red, color.green, color.blue);
            for (j = 0; j < slices; j++)
            {
                glBegin(GL_QUADS);
                {
                    //upper hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
                }
                glEnd();
            }
            glPopMatrix();
        }
    }

    virtual double intersect_handler(Ray ray, Color &color, int level){

        ray.origin = sub(ray.origin , reference_point); // adjust ray origin
        
        double a = 1;
        double b = 2 * dotProduct(ray.direction , ray.origin);
        double c = dotProduct(ray.origin , ray.origin) - (length*length);

        double discriminant = pow(b, 2) - 4 * a * c;
        double t = -1;
        if (discriminant < 0){
            t = -1;
        }
        else{
            
            if(fabs(a) < 1e-5)
            {
                t = -c/b;
                return t;
            }

            double t1 = (-b - sqrt(discriminant)) / (2 * a);
            double t2 = (-b + sqrt(discriminant)) / (2 * a);

            if(t2<t1) swap(t1, t2);

            if (t1 > 0){
                t = t1;
            }
            else if (t2 > 0){
                t = t2;
            }
            else{
                t = -1;
            }
        }

        return t;
    }
};