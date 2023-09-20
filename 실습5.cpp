#include <iostream>
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <Windows.h>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution < double > XYdis(-1, 1);
uniform_real_distribution < double > dis(0, 1);

//ex) random = floor(random * 100) / 100;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y);
GLvoid Motion(int x, int y);
BOOL collision(float a_x1, float a_x2, float a_y1, float a_y2, float b_x1, float b_x2, float b_y1, float b_y2);

int windowWidth = 800;
int windowHeight = 600;

bool left_button = false;

struct NEMO
{
	GLfloat x1, y1, x2, y2, r, g, b;
	bool create;
};
NEMO nemo[30];

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

double randomX, randomY;

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����
	if (start)
	{
		int cnt = 0;
		while (cnt < 30)
		{
			randomX = XYdis(gen);
			randomY = XYdis(gen);

			nemo[cnt].x1 = randomX;
			nemo[cnt].x2 = randomX +0.05;
			nemo[cnt].y1 = randomY;
			nemo[cnt].y2 = randomY +0.05;

			nemo[cnt].r = dis(gen);
			nemo[cnt].g = dis(gen);
			nemo[cnt].b = dis(gen);

			nemo[cnt].create = true;

			cnt++;
		}

		nemo[0].r = 0;
		nemo[0].g = 0;
		nemo[0].b = 0;

		nemo[0].create = false;

		start = false;
	}

	for (int i = 0; i < 30; i++)
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
int collisionNUM = 0;


GLvoid Mouse(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			WindowToOpenGL(x, y, openGLX, openGLY);
		
			movingRectangle = 0;
			nemo[0].create = true;
			
		}
		else if (state == GLUT_UP)
		{
			movingRectangle = -1;  // ���콺 ��ư�� ������ �̵� ��带 �����մϴ�.

			nemo[0].r = 0;
			nemo[0].g = 0;
			nemo[0].b = 0;

			nemo[0].x1 += 0.025* (GLfloat)collisionNUM;
			nemo[0].y1 += 0.025 * (GLfloat)collisionNUM;
			nemo[0].x2 -= 0.025 * (GLfloat)collisionNUM;
			nemo[0].y2 -= 0.025 * (GLfloat)collisionNUM;

			nemo[0].create = false;
			collisionNUM = 0;
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

		for (int i = 1; i < 30; i++)
		{
			if (nemo[i].create)
			{
				if (collision(nemo[0].x1, nemo[0].x2, nemo[0].y1, nemo[0].y2, nemo[i].x1, nemo[i].x2, nemo[i].y1, nemo[i].y2))
				{
					nemo[i].create = false;

					nemo[0].r = nemo[i].r;
					nemo[0].g = nemo[i].g;
					nemo[0].b = nemo[i].b;

					nemo[0].x1 -= 0.025;
					nemo[0].y1 -= 0.025;
					nemo[0].x2 += 0.025;
					nemo[0].y2 += 0.025;

					collisionNUM++;
					break;
				}
			}
		}

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
	case 'r':
		start = true;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

BOOL collision(float a_x1, float a_x2, float a_y1, float a_y2, float b_x1, float b_x2, float b_y1, float b_y2)
{
	if (((b_x1 < a_x1 && a_x1 < b_x2) || (b_x1 < a_x2 && a_x2 < b_x2)) &&
		((b_y1 < a_y2 && a_y2 < b_y2) || (b_y1 < a_y1 && a_y1 < b_y2)))
	{
		return true;
	}
	return false;
}
