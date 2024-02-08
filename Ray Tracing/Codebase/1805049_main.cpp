#include "1805049_object.h"

int recursionLevel;
int fovY, aspectRatio, nearDistance, farDistance;
int checkerboardCellWidth;
double ambientCheckerboard, diffuseCheckerboard, reflectionCheckerboard; // no specular component for checkerboard
int noOfObjects;
int imageHeight,imageWidth;
double cameraHeight,cameraAngle,angle;
int drawaxes;
bitmap_image image;

vector <Object*> objects;
vector <Light*> lights;
vector <SpotLight*> spotlights;

// position of camera
point pos(0,-160,60);

// up, right and look direction
point up(0,0,1);
point rightV(1,0,0);
point look(0,1,0);

double ROT_ANG = PI/180;
int numSegments;
double windowWidth = 600, windowHeight = 600;
double viewAngle = 80;

void drawAxes()
{
	glColor3f(0.0, 1.0, 0.0);//green
	glBegin(GL_LINES);
	{
		glVertex3f( 100,0,0);
		glVertex3f(-100,0,0);

		glVertex3f(0,-100,0);
		glVertex3f(0, 100,0);

		glVertex3f(0,0, 100);
		glVertex3f(0,0,-100);
	}
	glEnd();
}

void capture()
{
	for(int i=0;i<imageWidth;i++)
	{
		for(int j=0;j<imageHeight;j++)
		{
			image.set_pixel(i, j, 0, 0, 0);
		}
	}

	double planeDistance = (windowHeight / 2.0) / tan((PI * viewAngle) / (360.0));

	point temp1 = add(pos , mul(look , planeDistance));
	point temp2 = add(temp1 , mul(up , (windowHeight / 2.0)));
	point topLeft = sub(temp2 , mul(rightV , (windowWidth / 2.0)));

	double du = windowWidth / (imageWidth*1.0);
	double dv = windowHeight / (imageHeight*1.0);

	// Choose middle of the grid cell
	topLeft = sub(add(topLeft , mul(rightV , du / 2.0)) , mul(up , dv / 2.0));

	int nearestObjectIndex = -1;
	double t,tMin;

	cout << "Point buffer generation done" << endl;

	for(int i=0;i<imageWidth;i++)
	{
		for(int j=0;j<imageHeight;j++)
		{
			// calculate current pixel
			point pixel = sub(add(topLeft , (mul(mul(rightV , du) , i*1.0))) , (mul(mul(up , dv) , j*1.0)));

			// cast ray from EYE to (curPixel-eye) direction ; eye is the position of the camera
			point direction = sub(pixel,pos);
			Ray ray(pos,direction);
			Color color;

			// find nearest object
			tMin = -1;
			nearestObjectIndex = -1;
			for(int k=0;k<(int)objects.size();k++)
			{
				t = objects[k]->intersect(ray,color, 0);
				if(t>0 && (nearestObjectIndex == -1 || t<tMin) )
				{
					tMin = t ;
					nearestObjectIndex = k;
				}
			}

			if(nearestObjectIndex != -1)// if nearest object found
			{
				color = Color(0,0,0);
				double t = objects[nearestObjectIndex]->intersect(ray, color, 1);

				// color.update();

				if(color.red > 1) color.red = 1;
				if(color.red < 0) color.red = 0;
				if(color.green > 1) color.green = 1;
				if(color.green < 0) color.green = 0;
				if(color.blue > 1) color.blue = 1;
				if(color.blue < 0) color.blue = 0;
							
				image.set_pixel(i, j, 255*color.red, 255*color.green, 255*color.blue);
			}
		}
	}

	image.save_image("Out.bmp");
	cout<<"Image Saved"<<endl;		
}

void rotate3D(point &vec,point &axis,double ang)
{
	vec = add(mul(vec,cos(ang)) , mul(crossProduct(axis,vec),sin(ang)));
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '0':
			capture();
			break;
		case '1':
			// rotate LEFT 
			rotate3D(rightV,up,ROT_ANG);
			rotate3D(look,up,ROT_ANG);
			break;
		case '2':
			// rotate right
			rotate3D(rightV,up,-ROT_ANG);
			rotate3D(look,up,-ROT_ANG);
			break;
		case '3':
			// rotate UP
			rotate3D(up,rightV,ROT_ANG);
			rotate3D(look,rightV,ROT_ANG);
			break;
		case '4':
			// rotate DOWN
			rotate3D(up,rightV,-ROT_ANG);
			rotate3D(look,rightV,-ROT_ANG);
			break;
		case '5':
			// tilt CLKWISE
			rotate3D(rightV,look,ROT_ANG);
			rotate3D(up,look,ROT_ANG);
			break;
		case '6':
			// tilt COUNTER CLKWISE
			rotate3D(rightV,look,-ROT_ANG);
			rotate3D(up,look,-ROT_ANG);
			break;
		default:
			break;
	}
}

void mouseListener(int button, int state, int x, int y)
{	
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN)
			{
				// cout<<"Left Button Pressed"<<endl;
				drawaxes=1-drawaxes;
			}
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_UP:	
			pos = add(pos , mul(look , 3));
			break;
		case GLUT_KEY_DOWN:	
			pos = sub(pos , mul(look , 3));
			break;
		case GLUT_KEY_RIGHT:
			pos = add(pos , mul(rightV , 3));
			break;
		case GLUT_KEY_LEFT:
			pos = sub(pos , mul(rightV , 3));
			break;
		case GLUT_KEY_PAGE_UP:
			pos = add(pos , mul(up , 3));
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos = sub(pos , mul(up , 3));
			break;
		default:
			break;
	}
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(pos.x, pos.y, pos.z, 
			pos.x + look.x, pos.y + look.y, pos.z + look.z, 
			up.x, up.y, up.z);

	glMatrixMode(GL_MODELVIEW);
	if(drawaxes==1) drawAxes();

    for (int i=0; i<objects.size(); i++){
		Object *object = objects[i];
		object->draw();
	}

	for (int i=0; i<lights.size(); i++){
		lights[i]->draw();
	}

	for(int i=0;i<spotlights.size();i++){
		SpotLight* l = spotlights[i];
		spotlights[i]->draw();
	}

	glutSwapBuffers();
}


void animate(){
	glutPostRedisplay();
}

void takeInput()
{
	string objectType;
	ifstream in("scene.txt");
	in >> nearDistance >> farDistance >> fovY >> aspectRatio;
	in >> recursionLevel;
	in >> imageHeight; //imageHeight = no of pixels in y direction

	imageWidth = imageHeight;

	Object *floor;
	
	in >> checkerboardCellWidth;
	in >> ambientCheckerboard >> diffuseCheckerboard >> reflectionCheckerboard;

	floor = new Floor(checkerboardCellWidth);
	floor->setCoefficients(ambientCheckerboard,diffuseCheckerboard,0.0,reflectionCheckerboard);
	floor->setShine(0); //shine = 0
	// cout << floor->shine << endl;
	objects.push_back(floor);

	in >> noOfObjects;

	for(int i=0;i<noOfObjects;i++)
	{
		in >> objectType;
		Object *obj;

		if(objectType == "sphere")
		{
			obj = new Sphere();

			Color color;
			vector<double> coefficient(4);
			int shine;

			in >> obj->reference_point.x >> obj->reference_point.y >> obj->reference_point.z;
			in >> obj->length;
			in >> color.red >> color.green >> color.blue;
			for (int i = 0; i < 4; i++) {
				in >> coefficient[i];
			}
			in >> shine;

			obj->setColor(color);
			obj->setCoefficients(coefficient[0], coefficient[1], coefficient[2], coefficient[3]);
			obj->setShine(shine);

			// obj->reference_point.print();
			// cout << obj->length << endl;
			// obj->color.printColor();
			// for (int i = 0; i < 4; i++) {
			// 	cout << obj->coefficients[i] << " ";
			// }
			// cout << endl;

			objects.push_back(obj);
		}
		else if(objectType == "pyramid")
		{ 
			point lowestPoint; //lowest point co-ordinate
			double width, height;
			Color color;
			vector<double> coefficient(4);
			int shine;
			in >> lowestPoint.x >> lowestPoint.y >> lowestPoint.z;
			in >> width >> height;
			in >> color.red >> color.green >> color.blue;
			for (int i = 0; i < 4; i++) {
				in >> coefficient[i];
			}
			in >> shine;

			point p1 = point(lowestPoint.x, lowestPoint.y, lowestPoint.z);
			point p2 = point(lowestPoint.x + width, lowestPoint.y, lowestPoint.z);
			point p3 = point(lowestPoint.x + width, lowestPoint.y + width, lowestPoint.z);
			point p4 = point(lowestPoint.x, lowestPoint.y + width, lowestPoint.z);
			point p5 = point(lowestPoint.x + width / 2.0, lowestPoint.y + width / 2.0, lowestPoint.z + height);

			// 4 faces of pyramid

			obj = new Triangle(p1, p2, p5); //front face ... anti-clockwise
			obj->setColor(color);
			obj->setCoefficients(coefficient[0], coefficient[1], coefficient[2], coefficient[3]);
			obj->setShine(shine);
			objects.push_back(obj);

			obj = new Triangle(p3, p5, p4); //back face ... anti-clockwise
			obj->setColor(color);
			obj->setCoefficients(coefficient[0], coefficient[1], coefficient[2], coefficient[3]);
			obj->setShine(shine);
			objects.push_back(obj);

			obj = new Triangle(p1, p5, p4); //side ... anti
			obj->setColor(color);
			obj->setCoefficients(coefficient[0], coefficient[1], coefficient[2], coefficient[3]);
			obj->setShine(shine);
			objects.push_back(obj);

			obj = new Triangle(p2, p3, p5); //side ... anti
			obj->setColor(color);
			obj->setCoefficients(coefficient[0], coefficient[1], coefficient[2], coefficient[3]);
			obj->setShine(shine);
			objects.push_back(obj);

			// base of pyramid

			obj = new Triangle(p1, p2, p3); //base ... anti
			obj->setColor(color);
			obj->setCoefficients(coefficient[0], coefficient[1], coefficient[2], coefficient[3]);
			obj->setShine(shine);
			objects.push_back(obj);

			obj = new Triangle(p1, p3, p4);
			obj->setColor(color);
			obj->setCoefficients(coefficient[0], coefficient[1], coefficient[2], coefficient[3]);
			obj->setShine(shine);
			objects.push_back(obj);

		}
		else if(objectType == "cube")
		{
			point lowestPoint; //lowest point co-ordinate
			double length;
			Color color;
			vector<double> coefficient(4);
			int shine;
			in >> lowestPoint.x >> lowestPoint.y >> lowestPoint.z;
			in >> length;
			in >> color.red >> color.green >> color.blue;
			for (int i = 0; i < 4; i++) {
				in >> coefficient[i];
			}
			in >> shine;

			// Define the vertices of the cube
			point vertices[8];

			// 4 points of base

			vertices[0] = point(lowestPoint.x, lowestPoint.y, lowestPoint.z); // point p1 = point(lowestPoint.x, lowestPoint.y, lowestPoint.z);
			vertices[1] = point(lowestPoint.x + length, lowestPoint.y, lowestPoint.z);// point p2 = point(lowestPoint.x + length, lowestPoint.y, lowestPoint.z);
			vertices[2] = point(lowestPoint.x + length, lowestPoint.y + length, lowestPoint.z);// point p3 = point(lowestPoint.x + length, lowestPoint.y + length, lowestPoint.z);
			vertices[3] = point(lowestPoint.x, lowestPoint.y + length, lowestPoint.z);// point p4 = point(lowestPoint.x, lowestPoint.y + length, lowestPoint.z);
			
			// 4 points of top
			
			vertices[4] = point(lowestPoint.x, lowestPoint.y, lowestPoint.z + length);// point p5 = point(lowestPoint.x, lowestPoint.y, lowestPoint.z + length);
			vertices[5] = point(lowestPoint.x + length, lowestPoint.y, lowestPoint.z + length);// point p6 = point(lowestPoint.x + length, lowestPoint.y, lowestPoint.z + length);
			vertices[6] = point(lowestPoint.x + length, lowestPoint.y + length, lowestPoint.z + length);// point p7 = point(lowestPoint.x + length, lowestPoint.y + length, lowestPoint.z + length);
			vertices[7] = point(lowestPoint.x, lowestPoint.y + length, lowestPoint.z + length);// point p8 = point(lowestPoint.x, lowestPoint.y + length, lowestPoint.z + length);

			// Define the indices for the cube's faces
			int faceIndices[6][4] = {
				{0, 1, 2, 3}, // Base face --> Triangle(p1, p2, p3) and Triangle(p1, p3, p4)
				{4, 5, 6, 7}, // Top face --> Triangle(p5, p6, p7) and Triangle(p5, p7, p8)
				{0, 1, 5, 4}, // Front face --> Triangle(p1, p2, p6) and Triangle(p1, p6, p5)
				{2, 3, 7, 6}, // Back face --> Triangle(p3, p8, p4) and Triangle(p3, p7, p8)
				{0, 3, 7, 4}, // Left face --> Triangle(p1, p8, p4) and Triangle(p1, p5, p8)
				{1, 2, 6, 5}  // Right face --> Triangle(p2, p3, p7) and Triangle(p2, p7, p6)
			};

			// Create triangles for each face using loops
			for (int i = 0; i < 6; i++) 
			{
				obj = new Triangle(vertices[faceIndices[i][0]], vertices[faceIndices[i][1]], vertices[faceIndices[i][2]]);
				obj->setColor(color);
				obj->setCoefficients(coefficient[0], coefficient[1], coefficient[2], coefficient[3]);
				obj->setShine(shine);
				objects.push_back(obj);

				obj = new Triangle(vertices[faceIndices[i][0]], vertices[faceIndices[i][2]], vertices[faceIndices[i][3]]);
				obj->setColor(color);
				obj->setCoefficients(coefficient[0], coefficient[1], coefficient[2], coefficient[3]);
				obj->setShine(shine);
				objects.push_back(obj);
			}

		}
	}

	int lightCount;
	in >> lightCount;

	for(int i=0;i<lightCount;i++){
		Light *light = new Light();
		in >> light->position.x >> light->position.y >> light->position.z >> light->fallOffParameter;
		lights.push_back(light);
	}

	int spotlightCount;
	in >> spotlightCount;

	for(int i=0;i<spotlightCount;i++)
	{
		SpotLight *spotlight = new SpotLight();

		in >> spotlight->pointLight.position.x >> spotlight->pointLight.position.y >> spotlight->pointLight.position.z >> spotlight->fallOffParameter;
		in >> spotlight->direction.x >> spotlight->direction.y >> spotlight->direction.z;
		in >> spotlight->cutoffAngle;

		spotlights.push_back(spotlight);
	}

	cout<<"Done taking input"<<endl;
}

int main(int argc, char **argv){

	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	numSegments = 36;
	drawaxes=1;

	glutInit(&argc,argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Ray Tracing");

	takeInput();
	image = bitmap_image(imageWidth, imageHeight);
	glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovY, aspectRatio, nearDistance, farDistance);

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing
	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);
	glutMainLoop();

	objects.clear();
	objects.shrink_to_fit();

	lights.clear();
	lights.shrink_to_fit();

	spotlights.clear();
	spotlights.shrink_to_fit();

	return 0;
}
