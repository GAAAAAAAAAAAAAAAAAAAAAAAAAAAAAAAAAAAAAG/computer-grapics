#include <iostream>
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <Windows.h>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution < double > dis(0, 1);

//ex) random = floor(random * 100) / 100;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y);
GLvoid Motion(int x, int y);

int windowWidth = 800;
int windowHeight = 600;

bool left_button = false;

struct NEMO
{
	GLfloat x1, y1, x2, y2, r, g, b;
	bool create; 
};
NEMO nemo[5];

bool start = true;

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
	glutMotionFunc(Motion);

	glutMainLoop(); // �̺�Ʈ ó�� ����
}
GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����
	if (start)
	{
		int cnt = 0;
		while (cnt < 5)
		{
			nemo[cnt].x1 = dis(gen);
			nemo[cnt].x2 = dis(gen);
			nemo[cnt].y1 = dis(gen);
			nemo[cnt].y2 = dis(gen);

			nemo[cnt].r = dis(gen);
			nemo[cnt].g = dis(gen);
			nemo[cnt].b = dis(gen);

			if ((nemo[cnt].x2 - nemo[cnt].x1) > 0.15 && (nemo[cnt].x2 - nemo[cnt].x1) < 0.3)
			{
				if ((nemo[cnt].y2 - nemo[cnt].y1) > 0.15 && (nemo[cnt].y2 - nemo[cnt].y1) < 0.3)
				{
					cnt++;
				}
			}
		}

		nemo[0].create = true;

		start = false;
	}
	
	for (int i = 0; i < 5; i++)
	{
		if (nemo[i].create)
		{
			glColor3f(nemo[i].r, nemo[i].g, nemo[i].b);
			glRectf(nemo[i].x1, nemo[i].y1, nemo[i].x2, nemo[i].y2);
		}
	}

	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

float openGLX, openGLY;
int movingRectangle = -1;

GLvoid Mouse(int button, int state, int x, int y)
{
	
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			WindowToOpenGL(x, y, openGLX, openGLY);
			for (int i = 4; i >=0; i--)
			{
				if (openGLX >= nemo[i].x1 && openGLX <= nemo[i].x2 && openGLY >= nemo[i].y1 && openGLY <= nemo[i].y2)
				{
					movingRectangle = i;
					break;
				}
			}
		}
		else if (state == GLUT_UP)
		{
			movingRectangle = -1;  // ���콺 ��ư�� ������ �̵� ��带 �����մϴ�.
		}
	}
}

GLvoid Motion(int x, int y)
{
	if (movingRectangle >= 0)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);

		// �̵� ���� �簢���� �߽����� ���
		float centerX = (nemo[movingRectangle].x1 + nemo[movingRectangle].x2) / 2;
		float centerY = (nemo[movingRectangle].y1 + nemo[movingRectangle].y2) / 2;

		// �̵��� �Ÿ� ���
		float deltaX = openGLX - centerX;
		float deltaY = openGLY - centerY;

		// �簢�� ��ġ �̵�
		nemo[movingRectangle].x1 += deltaX;
		nemo[movingRectangle].x2 += deltaX;
		nemo[movingRectangle].y1 += deltaY;
		nemo[movingRectangle].y2 += deltaY;

		glutPostRedisplay();  // ȭ���� �ٽ� �׸��ϴ�.
	}
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}

int cnt = 0;

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		if (cnt < 4)
		{
			cnt++;
			nemo[cnt].create = true;
		}
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}