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
GLvoid Mouse(int button, int state, int x, int y);
GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y);

struct NEMO
{
	GLfloat x1, y1, x2, y2, r, g, b;
};
NEMO nemo[4];
NEMO Bnemo[4];

bool start = true;
bool leftSelection = false;
bool rightSelection = false;

int windowWidth = 800;
int windowHeight = 600;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(windowWidth, windowHeight); // �������� ũ�� ����
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

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);

	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	//��� �׸�
	glColor3f(Bnemo[0].r, Bnemo[0].g, Bnemo[0].b);
	glRectf(Bnemo[0].x1, Bnemo[0].y1, Bnemo[0].x2, Bnemo[0].y2);

	//2
	glColor3f(Bnemo[1].r, Bnemo[1].g, Bnemo[1].b);
	glRectf(Bnemo[1].x1, Bnemo[1].y1, Bnemo[1].x2, Bnemo[1].y2);

	//3
	glColor3f(Bnemo[2].r, Bnemo[2].g, Bnemo[2].b);
	glRectf(Bnemo[2].x1, Bnemo[2].y1, Bnemo[2].x2, Bnemo[2].y2);

	//4
	glColor3f(Bnemo[3].r, Bnemo[3].g, Bnemo[3].b);
	glRectf(Bnemo[3].x1, Bnemo[3].y1, Bnemo[3].x2, Bnemo[3].y2);


	//�׸�
	//1
	glColor3f(nemo[0].r, nemo[0].g, nemo[0].b);
	glRectf(nemo[0].x1, nemo[0].y1, nemo[0].x2, nemo[0].y2);

	//2
	glColor3f(nemo[1].r, nemo[1].g, nemo[1].b);
	glRectf(nemo[1].x1, nemo[1].y1, nemo[1].x2, nemo[1].y2);

	//3
	glColor3f(nemo[2].r, nemo[2].g, nemo[2].b);
	glRectf(nemo[2].x1, nemo[2].y1, nemo[2].x2, nemo[2].y2);

	//4
	glColor3f(nemo[3].r, nemo[3].g, nemo[3].b);
	glRectf(nemo[3].x1, nemo[3].y1, nemo[3].x2, nemo[3].y2);

	// �׸��� �κ� ����
	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	if (start)
	{
		nemo[0] = { 0,0,1,1,1,0,0 };
		nemo[1] = { -1,0,0,1,1,1,0 };
		nemo[2] = { -1,-1,0,0,1,0,1 };
		nemo[3] = { 0,-1,1,0,0,1,1 };

		Bnemo[0] = { 0,0,1,1,0,0,0 };
		Bnemo[1] = { -1,0,0,1,0,0,0 };
		Bnemo[2] = { -1,-1,0,0,0,0,0 };
		Bnemo[3] = { 0,-1,1,0,0,0,0 };

		start = !start;
	}
	glViewport(0, 0, w, h);
}

//���콺�� ������ ��ǥ��(������� 0,0)
GLvoid Mouse(int button, int state, int x, int y)
{
	float openGLX, openGLY;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);

		leftSelection = false;

		//�׸�	
		if (openGLX > nemo[0].x1 && openGLX< nemo[0].x2 && openGLY > nemo[0].y1 && openGLY < nemo[0].y2)			//1��
		{
			nemo[0].r = dis(gen);
			nemo[0].g = dis(gen);
			nemo[0].b = dis(gen);

			leftSelection = true;
		}
		else if (openGLX > nemo[1].x1 && openGLX < nemo[1].x2 && openGLY > nemo[1].y1 && openGLY < nemo[1].y2)		//2��
		{
			nemo[1].r = dis(gen);
			nemo[1].g = dis(gen);
			nemo[1].b = dis(gen);

			leftSelection = true;
		}
		else if (openGLX > nemo[2].x1 && openGLX < nemo[2].x2 && openGLY > nemo[2].y1 && openGLY < nemo[2].y2)	//3��
		{
			nemo[2].r = dis(gen);
			nemo[2].g = dis(gen);
			nemo[2].b = dis(gen);

			leftSelection = true;
		}
		else if (openGLX > nemo[3].x1 && openGLX < nemo[3].x2 && openGLY > nemo[3].y1 && openGLY < nemo[3].y2)	//4��
		{
			nemo[3].r = dis(gen);
			nemo[3].g = dis(gen);
			nemo[3].b = dis(gen);

			leftSelection = true;
		}

		//���� �׸�
		if (!leftSelection)
		{
			if (openGLX > Bnemo[0].x1 && openGLX< Bnemo[0].x2 && openGLY > Bnemo[0].y1 && openGLY < Bnemo[0].y2)			//1��
			{
				Bnemo[0].r = dis(gen);
				Bnemo[0].g = dis(gen);
				Bnemo[0].b = dis(gen);
			}
			else if (openGLX > Bnemo[1].x1 && openGLX < Bnemo[1].x2 && openGLY > Bnemo[1].y1 && openGLY < Bnemo[1].y2)		//2��
			{
				Bnemo[1].r = dis(gen);
				Bnemo[1].g = dis(gen);
				Bnemo[1].b = dis(gen);
			}
			else if (openGLX > Bnemo[2].x1 && openGLX < Bnemo[2].x2 && openGLY > Bnemo[2].y1 && openGLY < Bnemo[2].y2)	//3��
			{
				Bnemo[2].r = dis(gen);
				Bnemo[2].g = dis(gen);
				Bnemo[2].b = dis(gen);
			}
			else if (openGLX > Bnemo[3].x1 && openGLX < Bnemo[3].x2 && openGLY > Bnemo[3].y1 && openGLY < Bnemo[3].y2)	//4��
			{
				Bnemo[3].r = dis(gen);
				Bnemo[3].g = dis(gen);
				Bnemo[3].b = dis(gen);
			}
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);

		rightSelection = false;

		//�׸�	
		if (openGLX > nemo[0].x1 && openGLX< nemo[0].x2 && openGLY > nemo[0].y1 && openGLY < nemo[0].y2)			//1��
		{
			nemo[0].x1 += 0.1;
			nemo[0].x2 -= 0.1;
			nemo[0].y1 += 0.1;
			nemo[0].y2 -= 0.1;

			rightSelection = true;
		}
		else if (openGLX > nemo[1].x1 && openGLX < nemo[1].x2 && openGLY > nemo[1].y1 && openGLY < nemo[1].y2)		//2��
		{
			nemo[1].x1 += 0.1;
			nemo[1].x2 -= 0.1;
			nemo[1].y1 += 0.1;
			nemo[1].y2 -= 0.1;

			rightSelection = true;
		}
		else if (openGLX > nemo[2].x1 && openGLX < nemo[2].x2 && openGLY > nemo[2].y1 && openGLY < nemo[2].y2)	//3��
		{
			nemo[2].x1 += 0.1;
			nemo[2].x2 -= 0.1;
			nemo[2].y1 += 0.1;
			nemo[2].y2 -= 0.1;

			rightSelection = true;
		}
		else if (openGLX > nemo[3].x1 && openGLX < nemo[3].x2 && openGLY > nemo[3].y1 && openGLY < nemo[3].y2)	//4��
		{
			nemo[3].x1 += 0.1;
			nemo[3].x2 -= 0.1;
			nemo[3].y1 += 0.1;
			nemo[3].y2 -= 0.1;

			rightSelection = true;
		}

		//���� �׸�
		if (!rightSelection)
		{
			if (openGLX > Bnemo[0].x1 && openGLX< Bnemo[0].x2 && openGLY > Bnemo[0].y1 && openGLY < Bnemo[0].y2)			//1��
			{
				nemo[0].x1 -= 0.1;
				nemo[0].x2 += 0.1;
				nemo[0].y1 -= 0.1;
				nemo[0].y2 += 0.1;
			}
			else if (openGLX > Bnemo[1].x1 && openGLX < Bnemo[1].x2 && openGLY > Bnemo[1].y1 && openGLY < Bnemo[1].y2)		//2��
			{
				nemo[1].x1 -= 0.1;
				nemo[1].x2 += 0.1;
				nemo[1].y1 -= 0.1;
				nemo[1].y2 += 0.1;
			}
			else if (openGLX > Bnemo[2].x1 && openGLX < Bnemo[2].x2 && openGLY > Bnemo[2].y1 && openGLY < Bnemo[2].y2)	//3��
			{
				nemo[2].x1 -= 0.1;
				nemo[2].x2 += 0.1;
				nemo[2].y1 -= 0.1;
				nemo[2].y2 += 0.1;
			}
			else if (openGLX > Bnemo[3].x1 && openGLX < Bnemo[3].x2 && openGLY > Bnemo[3].y1 && openGLY < Bnemo[3].y2)	//4��
			{
				nemo[3].x1 -= 0.1;
				nemo[3].x2 += 0.1;
				nemo[3].y1 -= 0.1;
				nemo[3].y2 += 0.1;
			}
		}
	}
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}