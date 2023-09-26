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
uniform_real_distribution<double> ANIMATIONdis(0, 3);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y);
GLvoid Motion(int x, int y);
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
	glutTimerFunc(500, TimerFunction, 1);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	glutMainLoop(); // �̺�Ʈ ó�� ����
}

struct MININEMO
{
	GLfloat x1, y1, x2, y2, r, g, b;
	int reduceCNT = 0;
	bool create = false;
};

struct NEMO
{
	GLfloat x1, y1, x2, y2, r, g, b;
	bool create;
	int animation = 0;	//0 �¿����, 1 �밢��, 2 �¿���ϴ밢��
	bool selection = false;
	GLfloat halfW, halfH;
	MININEMO mininemo[8];
	bool animationstart = false;
};

NEMO nemo[5];



bool start = true;

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����
	if (start)
	{
		int cnt = 0;
		double randomX, randomY;

		while (cnt < 5)
		{
			randomX = XYdis(gen);
			randomY = XYdis(gen);

			nemo[cnt].x1 = randomX - 0.1;
			nemo[cnt].x2 = randomX + 0.1;
			nemo[cnt].y1 = randomY - 0.1;
			nemo[cnt].y2 = randomY + 0.1;

			nemo[cnt].r = dis(gen);
			nemo[cnt].g = dis(gen);
			nemo[cnt].b = dis(gen);

			nemo[cnt].animation = ANIMATIONdis(gen);
			//nemo[cnt].animation = 2;

			nemo[cnt].halfW = nemo[cnt].x1 + (nemo[cnt].x2 - nemo[cnt].x1)/2;
			nemo[cnt].halfH = nemo[cnt].y1 + (nemo[cnt].y2 - nemo[cnt].y1)/2;

			nemo[cnt].create = true;

			cnt++;
		}
		start = false;
	}
	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	for (int i = 0; i < 5; i++)
	{
		if (nemo[i].create && !nemo[i].selection)
		{
			glColor3f(nemo[i].r, nemo[i].g, nemo[i].b);
			glRectf(nemo[i].x1, nemo[i].y1, nemo[i].x2, nemo[i].y2);
		}
	}

	//�̴ϳ׸� �׸���!
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (nemo[i].mininemo[j].create)
			{
				glColor3f(nemo[i].mininemo[j].r, nemo[i].mininemo[j].g, nemo[i].mininemo[j].b);
				glRectf(nemo[i].mininemo[j].x1, nemo[i].mininemo[j].y1, nemo[i].mininemo[j].x2, nemo[i].mininemo[j].y2);
			}
		}
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}



GLvoid Mouse(int button, int state, int x, int y)
{
	float openGLX, openGLY;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);

		for (int i = 0; i < 5; i++)
		{
			if ((nemo[i].x1 < openGLX && nemo[i].x2 > openGLX) && (nemo[i].y1 < openGLY && nemo[i].y2 > openGLY))
			{
				nemo[i].selection = true;
			}
		}
	}
}

float openGLX, openGLY;
int movingRectangle = -1;

GLvoid Motion(int x, int y)
{
	if (movingRectangle >= 0)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);

		// �̵� ���� �簢���� �߽����� ���
		//float centerX = (nemo[movingRectangle].x1 + nemo[movingRectangle].x2) / 2;
		//float centerY = (nemo[movingRectangle].y1 + nemo[movingRectangle].y2) / 2;

		// �̵��� �Ÿ� ���
		//float deltaX = openGLX - centerX;
		//float deltaY = openGLY - centerY;

		// �簢�� ��ġ �̵�
		//nemo[movingRectangle].x1 += deltaX;
		//nemo[movingRectangle].x2 += deltaX;
		//nemo[movingRectangle].y1 += deltaY;
		//nemo[movingRectangle].y2 += deltaY;

		glutPostRedisplay();  // ȭ���� �ٽ� �׸��ϴ�.
	}
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}

GLvoid TimerFunction(int value)
{
	switch (value)
	{
	case 1:
		for (int i = 0; i < 5; i++)
		{
			if (nemo[i].selection)
			{
				switch (nemo[i].animation)
				{
				case 0:
					//�ʱ⼳��
					if (!nemo[i].animationstart)
					{
						nemo[i].mininemo[0].x1 = nemo[i].x1;
						nemo[i].mininemo[0].y1 = nemo[i].y1;
						nemo[i].mininemo[0].x2 = nemo[i].halfW;
						nemo[i].mininemo[0].y2 = nemo[i].halfH;
	
						nemo[i].mininemo[1].x1 = nemo[i].halfW;
						nemo[i].mininemo[1].y1 = nemo[i].y1;
						nemo[i].mininemo[1].x2 = nemo[i].x2;
						nemo[i].mininemo[1].y2 = nemo[i].halfH;

						nemo[i].mininemo[2].x1 = nemo[i].halfW;
						nemo[i].mininemo[2].y1 = nemo[i].halfH;
						nemo[i].mininemo[2].x2 = nemo[i].x2;
						nemo[i].mininemo[2].y2 = nemo[i].y2;
					
						nemo[i].mininemo[3].x1 = nemo[i].x1;
						nemo[i].mininemo[3].y1 = nemo[i].halfH;
						nemo[i].mininemo[3].x2 = nemo[i].halfW;
						nemo[i].mininemo[3].y2 = nemo[i].y2;
					
						for (int j = 0; j < 4; j++)
						{
							nemo[i].mininemo[j].create = true;
							nemo[i].mininemo[j].r = nemo[i].r;
							nemo[i].mininemo[j].g = nemo[i].g;
							nemo[i].mininemo[j].b = nemo[i].b;
						}
						
						nemo[i].animationstart = true;
					}
					//��ġ ����
					nemo[i].mininemo[0].y1 -= 0.05;
					nemo[i].mininemo[0].y2 -= 0.05;

					nemo[i].mininemo[1].x1 += 0.05;
					nemo[i].mininemo[1].x2 += 0.05;

					nemo[i].mininemo[2].y1 += 0.05;
					nemo[i].mininemo[2].y2 += 0.05;

					nemo[i].mininemo[3].x1 -= 0.05;
					nemo[i].mininemo[3].x2 -= 0.05;

					//ũ�� ����
					for (int j = 0; j < 4; j++)
					{
						nemo[i].mininemo[j].x1 += 0.01;
						nemo[i].mininemo[j].y1 += 0.01;
						nemo[i].mininemo[j].x2 -= 0.01;
						nemo[i].mininemo[j].y2 -= 0.01;

						nemo[i].mininemo[j].reduceCNT += 1;

						if (nemo[i].mininemo[j].reduceCNT > 4)
						{
							nemo[i].mininemo[j].create = false;
						}
					}
					break;
				case 1:
					//�ʱ⼳��
					if (!nemo[i].animationstart)
					{
						nemo[i].mininemo[0].x1 = nemo[i].x1;
						nemo[i].mininemo[0].y1 = nemo[i].y1;
						nemo[i].mininemo[0].x2 = nemo[i].halfW;
						nemo[i].mininemo[0].y2 = nemo[i].halfH;

						nemo[i].mininemo[1].x1 = nemo[i].halfW;
						nemo[i].mininemo[1].y1 = nemo[i].y1;
						nemo[i].mininemo[1].x2 = nemo[i].x2;
						nemo[i].mininemo[1].y2 = nemo[i].halfH;

						nemo[i].mininemo[2].x1 = nemo[i].halfW;
						nemo[i].mininemo[2].y1 = nemo[i].halfH;
						nemo[i].mininemo[2].x2 = nemo[i].x2;
						nemo[i].mininemo[2].y2 = nemo[i].y2;

						nemo[i].mininemo[3].x1 = nemo[i].x1;
						nemo[i].mininemo[3].y1 = nemo[i].halfH;
						nemo[i].mininemo[3].x2 = nemo[i].halfW;
						nemo[i].mininemo[3].y2 = nemo[i].y2;

						for (int j = 0; j < 4; j++)
						{
							nemo[i].mininemo[j].create = true;
							nemo[i].mininemo[j].r = nemo[i].r;
							nemo[i].mininemo[j].g = nemo[i].g;
							nemo[i].mininemo[j].b = nemo[i].b;
						}

						nemo[i].animationstart = true;
					}
					//��ġ ����
					nemo[i].mininemo[0].x1 -= 0.05;
					nemo[i].mininemo[0].y1 -= 0.05;
					nemo[i].mininemo[0].x2 -= 0.05;
					nemo[i].mininemo[0].y2 -= 0.05;

					nemo[i].mininemo[1].x1 += 0.05;
					nemo[i].mininemo[1].y1 -= 0.05;
					nemo[i].mininemo[1].x2 += 0.05;
					nemo[i].mininemo[1].y2 -= 0.05;

					nemo[i].mininemo[2].x1 += 0.05;
					nemo[i].mininemo[2].y1 += 0.05;
					nemo[i].mininemo[2].x2 += 0.05;
					nemo[i].mininemo[2].y2 += 0.05;

					nemo[i].mininemo[3].x1 -= 0.05;
					nemo[i].mininemo[3].y1 += 0.05;
					nemo[i].mininemo[3].x2 -= 0.05;
					nemo[i].mininemo[3].y2 += 0.05;

					//ũ�� ����
					for (int j = 0; j < 4; j++)
					{
						nemo[i].mininemo[j].x1 += 0.01;
						nemo[i].mininemo[j].y1 += 0.01;
						nemo[i].mininemo[j].x2 -= 0.01;
						nemo[i].mininemo[j].y2 -= 0.01;

						nemo[i].mininemo[j].reduceCNT += 1;

						if (nemo[i].mininemo[j].reduceCNT > 4)
						{
							nemo[i].mininemo[j].create = false;
						}
					}
					break;
				case 2:
					//�ʱ⼳��
					if (!nemo[i].animationstart)
					{
						double X1, Y1, X2, Y2, X3, Y3, X4, Y4;
						//nemo[cnt].halfW = nemo[cnt].x1 + (nemo[cnt].x2 - nemo[cnt].x1)/2;
						X1 = (nemo[i].halfW - nemo[i].x1) / 2 + nemo[i].x1;
						Y1 = (nemo[i].halfH - nemo[i].y1) / 2 + nemo[i].y1;

						X2 = (nemo[i].x2 - nemo[i].halfW) / 2 + nemo[i].halfW;
						Y2 = (nemo[i].halfH - nemo[i].y1) / 2 + nemo[i].y1;

						X3 = (nemo[i].x2 - nemo[i].halfW) / 2 + nemo[i].halfW;
						Y3 = (nemo[i].y2 - nemo[i].halfH) / 2 + nemo[i].halfH;

						X4 = (nemo[i].halfW - nemo[i].x1) / 2 + nemo[i].x1;
						Y4 = (nemo[i].y2 - nemo[i].halfH) / 2 + nemo[i].halfH;

						nemo[i].mininemo[0].x1 = nemo[i].x1;
						nemo[i].mininemo[0].y1 = nemo[i].y1;
						nemo[i].mininemo[0].x2 = X1;
						nemo[i].mininemo[0].y2 = Y1;

						nemo[i].mininemo[1].x1 = X1;
						nemo[i].mininemo[1].y1 = nemo[i].y1;
						nemo[i].mininemo[1].x2 = X2;
						nemo[i].mininemo[1].y2 = Y2;

						nemo[i].mininemo[2].x1 = X2;
						nemo[i].mininemo[2].y1 = nemo[i].y1;
						nemo[i].mininemo[2].x2 = nemo[i].x2;
						nemo[i].mininemo[2].y2 = Y2;

						nemo[i].mininemo[3].x1 = X2;
						nemo[i].mininemo[3].y1 = Y2;
						nemo[i].mininemo[3].x2 = nemo[i].x2;
						nemo[i].mininemo[3].y2 = Y3;

						nemo[i].mininemo[4].x1 = X3;
						nemo[i].mininemo[4].y1 = Y3;
						nemo[i].mininemo[4].x2 = nemo[i].x2;
						nemo[i].mininemo[4].y2 = nemo[i].y2;

						nemo[i].mininemo[5].x1 = X4;
						nemo[i].mininemo[5].y1 = Y4;
						nemo[i].mininemo[5].x2 = X3;
						nemo[i].mininemo[5].y2 = nemo[i].y2;

						nemo[i].mininemo[6].x1 = nemo[i].x1;
						nemo[i].mininemo[6].y1 = Y4;
						nemo[i].mininemo[6].x2 = X4;
						nemo[i].mininemo[6].y2 = nemo[i].y2;

						nemo[i].mininemo[7].x1 = nemo[i].x1;
						nemo[i].mininemo[7].y1 = Y1;
						nemo[i].mininemo[7].x2 = X4;
						nemo[i].mininemo[7].y2 = Y4;

						for (int j = 0; j < 8; j++)
						{
							nemo[i].mininemo[j].create = true;
							nemo[i].mininemo[j].r = nemo[i].r;
							nemo[i].mininemo[j].g = nemo[i].g;
							nemo[i].mininemo[j].b = nemo[i].b;
						}

						nemo[i].animationstart = true;
					}

					//��ġ ����(�¿����)
					nemo[i].mininemo[1].y1 -= 0.05;
					nemo[i].mininemo[1].y2 -= 0.05;

					nemo[i].mininemo[3].x1 += 0.05;
					nemo[i].mininemo[3].x2 += 0.05;

					nemo[i].mininemo[5].y1 += 0.05;
					nemo[i].mininemo[5].y2 += 0.05;

					nemo[i].mininemo[7].x1 -= 0.05;
					nemo[i].mininemo[7].x2 -= 0.05;

					//��ġ ����(�밢��)
					nemo[i].mininemo[0].x1 -= 0.05;
					nemo[i].mininemo[0].y1 -= 0.05;
					nemo[i].mininemo[0].x2 -= 0.05;
					nemo[i].mininemo[0].y2 -= 0.05;

					nemo[i].mininemo[2].x1 += 0.05;
					nemo[i].mininemo[2].y1 -= 0.05;
					nemo[i].mininemo[2].x2 += 0.05;
					nemo[i].mininemo[2].y2 -= 0.05;

					nemo[i].mininemo[4].x1 += 0.05;
					nemo[i].mininemo[4].y1 += 0.05;
					nemo[i].mininemo[4].x2 += 0.05;
					nemo[i].mininemo[4].y2 += 0.05;

					nemo[i].mininemo[6].x1 -= 0.05;
					nemo[i].mininemo[6].y1 += 0.05;
					nemo[i].mininemo[6].x2 -= 0.05;
					nemo[i].mininemo[6].y2 += 0.05;

					//ũ�� ����
					for (int j = 0; j < 8; j++)
					{
						nemo[i].mininemo[j].x1 += 0.005;
						nemo[i].mininemo[j].y1 += 0.005;
						nemo[i].mininemo[j].x2 -= 0.005;
						nemo[i].mininemo[j].y2 -= 0.005;

						nemo[i].mininemo[j].reduceCNT += 1;

						if (nemo[i].mininemo[j].reduceCNT > 5)
						{
							nemo[i].mininemo[j].create = false;
						}
					}

					break;
				}
			}
		}
		glutPostRedisplay();
		glutTimerFunc(500, TimerFunction, 1);
		break;

	}
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