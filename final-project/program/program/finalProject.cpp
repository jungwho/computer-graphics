#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;
typedef struct { float x; float y; float z; } Point;
typedef struct { unsigned int ip[3]; } Face;
int pnum; int fnum;
Point* mpoint = NULL;
Face* mface = NULL;
float x_position = 0;
float z_position = 0;
int status = 0;
int view = 0;
int projection = 0;
int y_view = 150;


void InitLight() {
	GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_position[] = { 500, 0, -100, 0.0 };
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

Point cnormal(Point a, Point b, Point c) {
	Point p, q, r;
	double val;
	p.x = a.x - b.x; p.y = a.y - b.y; p.z = a.z - b.z;
	q.x = c.x - b.x; q.y = c.y - b.y; q.z = c.z - b.z;
	r.x = p.y * q.z - p.z * q.y;
	r.y = p.z * q.x - p.x * q.z;
	r.z = p.x * q.y - p.y * q.x;
	val = sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
	r.x = r.x / val; r.y = r.y / val; r.z = r.z / val;
	return r;
}

void ReadModel(string fname)
{
	FILE* f1; char s[80]; int i;
	if (mpoint != NULL) delete mpoint;
	if (mface != NULL) delete mface;
	if ((f1 = fopen(fname.c_str(), "rt")) == NULL) { printf("No file\n"); exit(0); }
	fscanf(f1, "%s", s);  fscanf(f1, "%s", s);
	fscanf(f1, "%d", &pnum);
	mpoint = new Point[pnum];
	for (i = 0; i < pnum; i++) {
		fscanf(f1, "%f", &mpoint[i].x); fscanf(f1, "%f", &mpoint[i].y); fscanf(f1, "%f", &mpoint[i].z);
	}
	fscanf(f1, "%s", s);
	fscanf(f1, "%s", s);
	fscanf(f1, "%d", &fnum);
	mface = new Face[fnum];
	for (i = 0; i < fnum; i++) {
		fscanf(f1, "%d", &mface[i].ip[0]); fscanf(f1, "%d", &mface[i].ip[1]); fscanf(f1, "%d", &mface[i].ip[2]);
	}
	fclose(f1);
}

void MakeGL_Model(void)
{
	glShadeModel(GL_SMOOTH);
	if (glIsList(1)) glDeleteLists(1, 1);
	glNewList(1, GL_COMPILE);
	glScalef(0.6, 0.6, 0.6);
	if (view == 0) glTranslatef(x_position, 0, z_position);
	if (view == 1) glTranslatef(z_position, 0, -x_position);
	if (view == 2) glTranslatef(-x_position, 0, -z_position);
	if (view == 3) glTranslatef(-z_position, 0, x_position);

	for (int i = 0; i < fnum; i++) {
		Point norm = cnormal(mpoint[mface[i].ip[2]], mpoint[mface[i].ip[1]], mpoint[mface[i].ip[0]]);
		glBegin(GL_TRIANGLES);
		glNormal3f(norm.x, norm.y, norm.z);
		glVertex3f(mpoint[mface[i].ip[0]].x, mpoint[mface[i].ip[0]].y, mpoint[mface[i].ip[0]].z);
		glNormal3f(norm.x, norm.y, norm.z);
		glVertex3f(mpoint[mface[i].ip[1]].x, mpoint[mface[i].ip[1]].y, mpoint[mface[i].ip[1]].z);
		glNormal3f(norm.x, norm.y, norm.z);
		glVertex3f(mpoint[mface[i].ip[2]].x, mpoint[mface[i].ip[2]].y, mpoint[mface[i].ip[2]].z);
		glEnd();
	}
	glEndList();
}

void display(void)
{
	glClearColor(0, 0.1, 0.15, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (projection == 0) gluPerspective(40.0, 1.0, 1.0, 2500.0); //원근 투영
	else glOrtho(-350, 350, -350, 350, -1000, 1000); //직교 투영

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (view == 0) gluLookAt(0, y_view, 1000, 0.0, 0.0, 0.0, 0.0, 1.0, 0); //front view
	if (view == 1) gluLookAt(1000, y_view, 0, 0.0, 0.0, 0.0, 0.0, 1.0, 0); //right view
	if (view == 2) gluLookAt(0, y_view, -1000, 0.0, 0.0, 0.0, 0.0, 1.0, 0); //back view
	if (view == 3) gluLookAt(-1000, y_view, 0, 0.0, 0.0, 0.0, 0.0, 1.0, 0); //left view

	if (status == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wire
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //shading

	glPushMatrix();
	glPushMatrix();
	ReadModel("c:\\data\\pawn.dat");
	glColor3f(0.9, 0.8, 0.6);
	MakeGL_Model();
	glCallList(1);
	glPopMatrix();

	glPushMatrix();
	ReadModel("c:\\data\\pawn2.dat");
	glTranslatef(-300, -40, 0);
	glColor3f(0.9, 0.7, 0.7);
	MakeGL_Model();
	glCallList(1);
	glPopMatrix();

	glPushMatrix();
	ReadModel("c:\\data\\pawn.dat");
	glTranslatef(300, 0, 0);
	glColor3f(0.8, 0.8, 0.9);
	MakeGL_Model();
	glCallList(1);
	glPopMatrix();

	glPushMatrix();
	ReadModel("c:\\data\\bishop.dat");
	glTranslatef(-150, 0, -300);
	glColor3f(0.7, 0.8, 0.8);
	MakeGL_Model();
	glCallList(1);
	glPopMatrix();

	glPushMatrix();
	ReadModel("c:\\data\\rook.dat");
	glTranslatef(150, 0, -300);
	glColor3f(0.9, 0.7, 0.7);
	MakeGL_Model();
	glCallList(1);
	glPopMatrix();

	glPushMatrix();
	ReadModel("c:\\data\\king.dat");
	glTranslatef(-150, 0, 300);
	glColor3f(0.8, 0.8, 0.9);
	MakeGL_Model();
	glCallList(1);
	glPopMatrix();

	glPushMatrix();
	ReadModel("c:\\data\\queen.dat");
	glTranslatef(150, 0, 300);
	glColor3f(0.8, 0.9, 1.0);
	MakeGL_Model();
	glCallList(1);
	glPopMatrix();

	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex3f(1000, -130, 1000);
	glVertex3f(-1000, -130, 1000);
	glColor3f(0.2, 0.3, 0.4);
	glVertex3f(-1000, -130, -1000);
	glVertex3f(1000, -130, -1000);
	glEnd();
	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		status = 0; glutPostRedisplay(); break;
	case 's':
		status = 1; glutPostRedisplay(); break;
	case 'p':
		if (projection == 0) { projection = 1; y_view = 0;  glutPostRedisplay(); break; }
		else { projection = 0; y_view = 150; glutPostRedisplay(); break; }
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (view == 3) view = 0;
		else view += 1;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		if (view == 0) view = 3;
		else view -= 1;
	}
}

void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT: 
		x_position += 10;
		glutPostRedisplay(); break;
	case GLUT_KEY_RIGHT: 
		x_position -= 10;
		glutPostRedisplay(); break;
	case GLUT_KEY_UP: 
		z_position += 10;
		glutPostRedisplay(); break;
	case GLUT_KEY_DOWN:
		z_position -= 10;
		glutPostRedisplay(); break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); 
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(100, 10);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Final Project");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(special);
	InitLight();
	glutMainLoop();
	return 0;
}
