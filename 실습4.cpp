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

	glutMainLoop(); // �̺�Ʈ ó�� ����
}

struct NEMO
{
	GLfloat x1, y1, x2, y2, r, g, b;
	bool create;
};
NEMO nemo[5];

bool start = true;
double randomX, randomY;





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
			/*randomX = XYdis(gen);
			randomY = XYdis(gen);

			nemo[cnt].x1 = randomX;
			nemo[cnt].x2 = randomX + 0.1;
			nemo[cnt].y1 = randomY;
			nemo[cnt].y2 = randomY + 0.1;*/

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
		glColor3f(nemo[i].r, nemo[i].g, nemo[i].b);
		glRectf(nemo[i].x1, nemo[i].y1, nemo[i].x2, nemo[i].y2);
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

int cnt = 0;

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

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}