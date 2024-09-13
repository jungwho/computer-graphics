#include<GL/glut.h>
#include<stdlib.h>
#include<vector>

GLsizei winWidth = 1000, winHeight = 600;

class point3d
{
    public:
	float x, y, z;
};
std::vector<point3d> clickpt; //클릭한 점 저장
std::vector<point3d> newpt; //회전하며 도는 점 저장

void init(void)
{
	glClearColor(0.85, 0.9, 0.95, 1); //하늘색
	glMatrixMode(GL_PROJECTION);
}

void winReshapeFcn(int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));
	winWidth = newWidth;
	winHeight = newHeight;
}

void displayLine(void) //x축, y축 그리기
{
	glColor3f(0.5, 0.5, 0.8); //파란색
	glBegin(GL_LINES);
	glVertex2i(500, 0); glVertex2i(500, 600);
	glVertex2i(0, 300); glVertex2i(1000, 300); glEnd();
	glFlush();
}

void plotPoint(GLint x, GLint y, GLint z) //점 찍는 함수
{
	glPointSize(4);
	glColor3f(0.95, 0.1, 0.3); //핑크색
	glBegin(GL_POINTS);
	glVertex3i(x, y, z); glEnd();
}

void drawLine(point3d a, point3d b) //선 그리는 함수
{
	glBegin(GL_LINES);
	glColor3f(0.5, 0.5, 0.5); //회색
	glVertex3i(a.x, a.y, a.z);
	glVertex3i(b.x, b.y, b.z);
	glEnd();
}

void clickPoint(GLint button, GLint action, GLint xMouse, GLint yMouse) //클릭한 곳에 점 찍고 clickpt에 저장
{
	GLint x = xMouse;
	GLint y = winHeight - yMouse;

	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		plotPoint(x, y, 0);
		point3d pt;
		pt.x = x; pt.y = y; pt.z = 0;
		clickpt.push_back(pt);
	}
	glFlush();
}

void wireFrame(int n) //wire frame 그리기
{
	for (int i = 0; i < clickpt.size(); i++) {
		drawLine(clickpt[i], newpt[n * i]);
		drawLine(clickpt[i], newpt[(i + 1) * n - 1]);
		if (i != clickpt.size() - 1) {
			drawLine(clickpt[i], clickpt[i + 1]);
		}
		for (int j = 0; j < newpt.size(); j++) {
			if (j % n != n - 1) {
				drawLine(newpt[j], newpt[j + 1]);
			}
			if (j < newpt.size() - n) {
				drawLine(newpt[j], newpt[j + n]);
			}
		}
	}
}

GLboolean Xrotation = true;

void newPoint(GLint angle) //angle만큼 회전하면서 점 찍고 newpt에 저장
{
	int angleSize = 360 / angle - 1;
	double radian = angle * (3.141592653589793 / 180);
	point3d pt;
	for (int i = 0; i < clickpt.size(); i++) {
		for (int j = 1; j < angleSize + 1; j++) {
			if (Xrotation) 
			{
				float radius = 300 - clickpt[i].y; //X_rotation
				pt.x = clickpt[i].x;
				pt.y = 300 - radius * cos(radian * j);
				pt.z = radius * sin(radian * j);
			}
			else 
			{
				float radius = 500 - clickpt[i].x; //Y_rotation
				pt.x = 500 - radius * cos(radian * j);
				pt.y = clickpt[i].y;
				pt.z = radius * sin(radian * j);
			}
			plotPoint(pt.x, pt.y, pt.z);
			newpt.push_back(pt);
		}
	}
	wireFrame(angleSize);
	glFlush();
}

void Mmenu(int entryID) //main menu
{
	if (entryID == 0) exit(0);
}

void Xmenu(int entryID) //sub menu: X_rotation
{
	Xrotation = true;
	newPoint(entryID);
	clickpt.clear(); newpt.clear(); glClear(GL_COLOR_BUFFER_BIT);
}

void Ymenu(int entryID) //sub menu: Y_rotation
{
	Xrotation = false;
	newPoint(entryID);
	clickpt.clear(); newpt.clear(); glClear(GL_COLOR_BUFFER_BIT);
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("20203180_SongJunghu");
	glutReshapeFunc(winReshapeFcn);
	glClearColor(0.85, 0.9, 0.95, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glOrtho(-1, 1, -1, 1, 1000, -1000);
	init();
	GLint XmenuID = glutCreateMenu(Xmenu); //sub menu: X_rotation
	glutAddMenuEntry("5", 5);
	glutAddMenuEntry("10", 10);
	glutAddMenuEntry("30", 30);
	glutAddMenuEntry("50", 50);
	glutAddMenuEntry("60", 60);
	glutAddMenuEntry("90", 90);
	glutAddMenuEntry("120", 120);
	GLint YmenuID = glutCreateMenu(Ymenu); //sub_menu: Y_rotation
	glutAddMenuEntry("5", 5);
	glutAddMenuEntry("10", 10);
	glutAddMenuEntry("30", 30);
	glutAddMenuEntry("50", 50);
	glutAddMenuEntry("60", 60);
	glutAddMenuEntry("90", 90);
	glutAddMenuEntry("120", 120);
	GLint MyMainMenuID = glutCreateMenu(Mmenu); //main menu
	glutAddSubMenu("X_rotation", XmenuID);
	glutAddSubMenu("Y_rotation", YmenuID);
	glutAddMenuEntry("Exit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(displayLine);
	glutMouseFunc(clickPoint);
	glutMainLoop();
}

