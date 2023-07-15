#include <GL\glut.h>
#include <math.h>
#include <random>
#include <iostream>  
using namespace std;

// Global Variables
bool STOP = false;
GLfloat c = 0;
float rotateX = 0, rotateY = 0, rotateZ = 0;
float rotateAll_x = 0, rotateAll_y = 0, rotateAll_z = 0;	
int upper_points[7];
int lower_points[7];
GLfloat PI = 22 / 7;
GLubyte green[] = { 20, 118, 4 };
GLubyte dark_green[] = { 8, 74, 0 };

int PointsPositions[][7][2] = {
			{ {-1,-1}},                                                              // 0 points
			{ {0,45},{-1,-1}},                                                       // 1 points
			{ {20,65},{-20,25}, {-1,-1},{-20,-65},{20,-25},{-1,-1}},                 // 2 points (has special case)
			{ {-20,25},{20,65},{0,45},{-1,-1}, {0,-45},{-20,-65},{20,-25}},          // 3 points (has special case)
			{ {-20,25},{-20,65},{20,25}, {20,65},{-1,-1} },                          // 4 points
			{ {-20,25},{-20,65}, {0,45},{20,25}, {20,65},{-1,-1} },                  // 5 points
			{ {-20,25}, {-20,50}, {-20,75}, {20,25}, {20,50}, {20,75},{-1,-1}}       // 6 points                     
};


// used for debuging
void printArray(int arr[5])
{
	cout << "Printing array elements:" << endl;
	for (int i = 0; i < 7; i++)
	{
		cout << arr[i] << "\t";
	}
	cout << "done" << "\n";
}


// generate a square 1 * 1 
void square(GLubyte color[]) {
	glBegin(GL_QUADS);
	glColor3ub(color[0], color[1], color[2]);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glEnd();
}


// Generate one domino
void cube() {

	// Face 1  -- front 
	glPushMatrix();
	glTranslatef(-200, 150, 1);

	glScalef(35, 95, 12);
	square(green);
	//glPopMatrix();

	// Face 2  -- left side
	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	square(dark_green);
	glPopMatrix();

	// Face 3  -- down 
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	square(dark_green);
	glPopMatrix();

	// Face 4  -- up
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	square(dark_green);
	glPopMatrix();

	// Face 5  -- right side
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	square(dark_green);
	glPopMatrix();

	// Face 6  -- back
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	square(green);
	glPopMatrix();

	glPopMatrix();

}


// The  line that divide the domino
void line(GLfloat offset) {
	glPushMatrix();
	glTranslatef(-200, 150, 1);

	// increasing the line width
	glScalef(25, 1, 1);  
	
	// used to locate the line
	glTranslatef(0, 0, offset);  
	
	glBegin(GL_LINES);
	glColor3ub(255, 255, 255);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glEnd();
	glPopMatrix();

}


// Generate the points painted in the domino
void point(GLfloat x, GLfloat y, GLfloat z) {
	// the radius of the point
	GLfloat r = 8;
	glPushMatrix();
	// used to locate the point
	glTranslatef(x - 200, y + 150 , z + 1);

	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);

	for (int i = 0; i < 360; i ++) {
		glVertex3f(r * sin(i * PI / 180), r * cos(i * PI / 180), 1);
	}
	glEnd();
	glPopMatrix();

}


// Generate random number from 0 to 6 to determine th epoints in each domino
void random()
{

	for (int i = 0; i <= 6; i++)
	{
		// generating random int between 0 : 6 to use as an index for choosing points
		upper_points[i] = rand() % ((6 - 0) + 1) ;
		
		lower_points[i] = rand() % ((6 - 0) + 1) ;
	}

	printArray(upper_points);
	printArray(lower_points);
}


void drawPoints(int positions[][7][2], int index, int half)
{	
	// iterartor
	int k = 0;

	/*  
	   special case in 2 & 3 case the points will be mirrored when multipling by - 1(the lower half)
	   so the points location for the lower 2 & 3 is different from the upper
	*/ 
	if (half == -1 && index == 2) {
		k = 3;
		half = 1;
	}
	if (half == -1 && index == 3) {
		k = 4;
		half = 1;
	}
		
	while (k < 7)
	{
		// check if we reached the end of the array
		if (positions[index][k][0] == -1) break;

		else  point(positions[index][k][0], half * positions[index][k][1], 12);
		
		k++;
	}
}


// Useed to display the shapes
void display() {

	// setting the background of the window
	glClearColor(c, c, c, 0);  
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0, 0.5, 1);
	glMatrixMode(GL_MODELVIEW);
	glLineWidth(2);
	// default values used for translate each piece in order
	int x = 55, y = -25, z = 25;  
	 // check if 'space' is pressed to generate new random stones
	if (!STOP) { 
		random();
		STOP = true;
	}

	glPushMatrix();
	// Control the ratation of the whole block of dominoes
	glRotatef(rotateAll_y, 0, 1, 0);
	glRotatef(rotateAll_x, 1, 0, 0);
	glRotatef(rotateAll_z, 0, 0, 1);
	for (int i = 0; i < 7; i++)
	{
		glPushMatrix();
		// Control the first position of each block of domino
		glTranslatef(x * i, y * i, z * i);
		glRotatef(30, 1, 1, 0);
		glRotatef(5, 0, 0, 1);
		// Control the ratation of each block of domino
		glRotatef(rotateY, 0, 1, 0);
		glRotatef(rotateX, 1, 0, 0);
		glRotatef(rotateZ, 0, 0, 1);
		// draw stone
		cube();  
		// draw line
		line(12);  

		drawPoints(PointsPositions, upper_points[i], 1);   // draw upper half of points

		drawPoints(PointsPositions, lower_points[i], -1);  // draw lower half of points

		glPopMatrix(); 
	}
	glPopMatrix();

	glFlush(); 
	glutSwapBuffers();
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {
	// Use exc to exit the program
	if (key == 27) {
		exit(0);
		cout << "the esc is pressed" << "\n";
	}
	// Use space to regenerate the numbers on the dominoes
	else if (key == 32) {
		STOP = false;
		cout << "the space is pressed" << "\n";
	}

	// Control the ratation of each block of domino
	else if (key == 'x') rotateAll_x++;
	else if (key == 'y') rotateAll_y++;
	else if (key == 'z') rotateAll_z++;

	// Use 'c' to change the color of the background
	else if (key == 'c') c += 0.1;

	// Use 'r' to reset setting
	else if (key == 'r') {
		c = 0;
		rotateAll_x = 0;
		rotateAll_y = 0;
		rotateAll_z = 0;
		rotateX = 0;
		rotateY = 0;
		rotateZ = 0;
	}
	glutPostRedisplay();
}


void specFunc(int key, int x1, int y1) {
	
	// Control the ratation of the whole block of dominoes
	switch (key)
	{
	case GLUT_KEY_UP: rotateX -= 2; break;
	case GLUT_KEY_DOWN: rotateX += 2; break;
	case GLUT_KEY_RIGHT: rotateY += 2; break;
	case GLUT_KEY_LEFT: rotateY -= 2; break;
	case GLUT_KEY_PAGE_UP: rotateZ += 2; break;
	case GLUT_KEY_PAGE_DOWN: rotateZ -= 2; break;
	}
	glutPostRedisplay();
}


int main(int argc, char* argv[]) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 700);
	glutCreateWindow("Dominoes");
	glOrtho(-500, 500, -500.0, 500, -500.0, 500);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specFunc);
	glutDisplayFunc(display);
	glutMainLoop();

}