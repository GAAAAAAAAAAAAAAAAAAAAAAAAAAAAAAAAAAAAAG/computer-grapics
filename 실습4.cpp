#include <iostream>
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <Windows.h>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> XYdis(-1, 1);
uniform_real_distribution<double> dis(0, 1);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y);

GLvoid TimerFunction(int value);

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
	glutTimerFunc(1000, TimerFunction, 1);
	glutTimerFunc(100, TimerFunction, 2);
	glutTimerFunc(100, TimerFunction, 3);
	glutTimerFunc(100, TimerFunction, 4);

	glutMainLoop(); // �̺�Ʈ ó�� ����
}

struct NEMO
{
	GLfloat x1, y1, x2, y2, r, g, b;
	bool create;
	int direction = 0;	//0 ���, 1 �»�, 2 ����, 3 ����
};
NEMO nemo[5];

bool start = true;
double randomX, randomY;

bool A = false;
bool I = false;
bool C = false;
bool O = false;
bool S = false;
bool M = false;

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����
	
	if (start)
	{
		int cnt = 0;
		while (cnt < 5)
		{
			
			nemo[cnt].r = dis(gen);
			nemo[cnt].g = dis(gen);
			nemo[cnt].b = dis(gen);

			nemo[cnt].create = false;

			cnt++;
		}

		start = false;
	}

	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	for (int i = 0; i < 5; i++)
	{
		if (nemo[i].create)
		{
			glColor3f(nemo[i].r, nemo[i].g, nemo[i].b);
			glRectf(nemo[i].x1, nemo[i].y1, nemo[i].x2, nemo[i].y2);
		}
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

int cnt = 0;

float XY[5][4];

GLvoid Mouse(int button, int state, int x, int y)
{
	float openGLX, openGLY;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (cnt < 5)
		{
			WindowToOpenGL(x, y, openGLX, openGLY);

			nemo[cnt].x1 = openGLX - 0.05;
			nemo[cnt].x2 = openGLX + 0.05;
			nemo[cnt].y1 = openGLY - 0.05;
			nemo[cnt].y2 = openGLY + 0.05;

			XY[cnt][0] = nemo[cnt].x1;
			XY[cnt][1] = nemo[cnt].y1;
			XY[cnt][2] = nemo[cnt].x2;
			XY[cnt][3] = nemo[cnt].y2;

			nemo[cnt].create = true;

			cnt++;
		}
	}
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}

int sizeChange = 0;	//50 �¿�Ȯ��, 100 ����Ȯ��, 150 �¿����, 200 �������
int zigzag = 0;	//50 ���, 100 ����

GLvoid TimerFunction(int value)
{
	if (!S)
	{
		switch (value)
		{
		case 1:
			if (O)	//���� ��ȭ O
			{
				for (int i = 0; i < 5; i++)
				{
					nemo[i].r = dis(gen);
					nemo[i].g = dis(gen);
					nemo[i].b = dis(gen);
				}
			}
			glutPostRedisplay();
			glutTimerFunc(1000, TimerFunction, 1);
			break;
		case 2:
			if (C)	//ũ�� ��ȭ C
			{
				if (sizeChange >= 200)
				{
					sizeChange = 0;
				}
				if (sizeChange >= 0 && sizeChange < 50)
				{
					for (int i = 0; i < 5; i++)
					{
						nemo[i].x1 -= 0.05;
						nemo[i].x2 += 0.05;
						sizeChange++;
					}
				}
				else if (sizeChange >= 50 && sizeChange < 100)
				{
					for (int i = 0; i < 5; i++)
					{
						nemo[i].y1 -= 0.05;
						nemo[i].y2 += 0.05;
						sizeChange++;
					}
				}
				else if (sizeChange >= 100 && sizeChange < 150)
				{
					for (int i = 0; i < 5; i++)
					{
						nemo[i].x1 += 0.05;
						nemo[i].x2 -= 0.05;
						sizeChange++;
					}
				}
				else if (sizeChange >= 150 && sizeChange < 200)
				{
					for (int i = 0; i < 5; i++)
					{
						nemo[i].y1 += 0.05;
						nemo[i].y2 -= 0.05;
						sizeChange++;
					}
				}
			}
			glutPostRedisplay();
			glutTimerFunc(100, TimerFunction, 2);
			break;
		case 3:
			if (I)	//������� I
			{
				if (zigzag >= 100)
				{
					zigzag = 0;
				}
				if (zigzag >= 0 && zigzag < 50)
				{
					for (int i = 0; i < 5; i++)
					{
						nemo[i].x1 += 0.05;
						nemo[i].y1 += 0.05;
						nemo[i].x2 += 0.05;
						nemo[i].y2 += 0.05;
						zigzag++;
					}
				}
				else if (zigzag >= 50 && zigzag < 100)
				{
					for (int i = 0; i < 5; i++)
					{
						nemo[i].x1 += 0.05;
						nemo[i].y1 -= 0.05;
						nemo[i].x2 += 0.05;
						nemo[i].y2 -= 0.05;
						zigzag++;
					}
				}

				for (int i = 0; i < 5; i++)
				{
					if (((nemo[i].x1 + nemo[i].x2) / 2) > 1)
					{
						nemo[i].x1 -= 2;
						nemo[i].x2 -= 2;
					}
				}
			}
			glutPostRedisplay();
			glutTimerFunc(100, TimerFunction, 3);
			break;
		case 4:
			if (A)	//�밢�� A
			{
				for (int i = 0; i < 5; i++)
				{
					switch (nemo[i].direction)
					{
					case 0:
						nemo[i].x1 += 0.05;
						nemo[i].y1 += 0.05;
						nemo[i].x2 += 0.05;
						nemo[i].y2 += 0.05;
						break;
					case 1:
						nemo[i].x1 -= 0.05;
						nemo[i].y1 += 0.05;
						nemo[i].x2 -= 0.05;
						nemo[i].y2 += 0.05;
						break;
					case 2:
						nemo[i].x1 -= 0.05;
						nemo[i].y1 -= 0.05;
						nemo[i].x2 -= 0.05;
						nemo[i].y2 -= 0.05;
						break;
					case 3:
						nemo[i].x1 += 0.05;
						nemo[i].y1 -= 0.05;
						nemo[i].x2 += 0.05;
						nemo[i].y2 -= 0.05;
						break;
					}
				}

				for (int i = 0; i < 5; i++)
				{
					if (nemo[i].x1 < -1 || nemo[i].x2 > 1 || nemo[i].y1 < -1 || nemo[i].y2 > 1)
					{
						nemo[i].direction++;
						if (nemo[i].direction > 3)
						{
							nemo[i].direction = 0;
						}
					}
				}
			}
			glutPostRedisplay();
			glutTimerFunc(100, TimerFunction, 4);
			break;
		}
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		A = !A;
		break;
	case 'i':
		I = !I;
		break;
	case 'c':
		C = !C;
		break;
	case 'o':
		O = !O;
		break;
	case 's':
		S = !S;
		if (!S)
		{
			glutTimerFunc(1000, TimerFunction, 1);
			glutTimerFunc(100, TimerFunction, 2);
			glutTimerFunc(100, TimerFunction, 3);
			glutTimerFunc(100, TimerFunction, 4);
		}
		break;
	case 'm':
		M = !M;
		for (int i = 0; i < 5; i++)
		{
			nemo[i].x1 = XY[i][0];
			nemo[i].y1 = XY[i][1];
			nemo[i].x2 = XY[i][2];
			nemo[i].y2 = XY[i][3];
		}
		break;
	case 'r':
		for (int i = 0; i < 5; i++)
		{
			nemo[i].r = dis(gen);
			nemo[i].g = dis(gen);
			nemo[i].b = dis(gen);
			nemo[i].create = false;
		}
		cnt = 0;
		drawScene();
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

//Ÿ�̸� �Լ��� �� ���� ����Ǳ� ������ ����Լ��� �ٽ� �ҷ�����Ѵ�.
//if�� �ȿ� Ÿ�̸� �Լ��� �־��� �ÿ��� Ű�� �޴� ���� �Լ��� �ٽ� �ҷ��ش�.