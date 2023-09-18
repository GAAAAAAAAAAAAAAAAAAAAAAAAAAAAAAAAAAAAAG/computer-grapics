#include <iostream>
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <Windows.h>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0, 1);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

bool C = false;
bool M = false;
bool Y = false;
bool A = false;
bool W = false;
bool K = false;
bool T = false;
bool S = false;
bool Q = false;
float r, g, b;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����(������ �̸�)
		//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	glutDisplayFunc(drawScene); // ��� �ݹ��Լ��� ����
	glutTimerFunc(1000, TimerFunction, 1);
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ��Լ� ����
	glutKeyboardFunc(Keyboard); // Ű���� �Է� �ݹ��Լ� ����
	
	glutMainLoop(); // �̺�Ʈ ó�� ����
	
}

GLvoid TimerFunction(int value)
{
	if (T)
	{
		/*r = dis(gen);
		g = dis(gen);
		b = dis(gen);

		glClearColor(r, g, b, 1.0f);
		glutPostRedisplay();*/
		drawScene();
		glutTimerFunc(1000, TimerFunction, 0);
	}
	else
	{
		glutTimerFunc(1000, TimerFunction, 1);
	}
}

GLvoid drawScene ( ) //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{ 
	//--- ����� ���� ����
	//glClearColor (0.0f,0.0f,1.0f,1.0f); //--- �������� ����
	if (C)
	{
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (M)
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	}
	else if (Y)
	{
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (W)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (K)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (A)
	{
		r = dis(gen);
		g = dis(gen);
		b = dis(gen);

		glClearColor(r, g, b, 1.0f);
	}
	else if (T)
	{
		r = dis(gen);
		g = dis(gen);
		b = dis(gen);

		glClearColor(r, g, b, 1.0f);
	}

	glClear ( GL_COLOR_BUFFER_BIT ); //--- ������ ������ ��ü�� ĥ�ϱ�
	glutSwapBuffers ( ); //--- ȭ�鿡 ����ϱ�
}
GLvoid Reshape ( int w, int h ) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport ( 0, 0, w, h );
}
GLvoid Keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
	case 'C':
		C = true;
		M = false;
		Y = false;
		A = false;
		W = false;
		K = false;
		T = false;
		S = false;
		Q = false;
		break; //--- ������ û�ϻ����� ����
	case 'm':
		C = false;
		M = true;
		Y = false;
		A = false;
		W = false;
		K = false;
		T = false;
		S = false;
		Q = false;
		break; //--- ������ ��ȫ������ ����
	case 'y' :
		C = false;
		M = false;
		Y = true;
		A = false;
		W = false;
		K = false;
		T = false;
		S = false;
		Q = false;
		break; //--- ������ ��������� ����
	case 'a':
		C = false;
		M = false;
		Y = false;
		A = true;
		W = false;
		K = false;
		T = false;
		S = false;
		Q = false;
		break;
	case 'w':
		C = false;
		M = false;
		Y = false;
		A = false;
		W = true;
		K = false;
		T = false;
		S = false;
		Q = false;
		break;
	case 'k':
		C = false;
		M = false;
		Y = false;
		A = false;
		W = false;
		K = true;
		T = false;
		S = false;
		Q = false;
		break;
	case 't':
		C = false;
		M = false;
		Y = false;
		A = false;
		W = false;
		K = false;
		T = true;
		S = false;
		Q = false;
		break;
	case 's':
		C = false;
		M = false;
		Y = false;
		A = false;
		W = false;
		K = false;
		T = false;
		S = true;
		Q = false;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay (); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}