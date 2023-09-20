#include <iostream>
#include <gl/glew.h> // 필요한 헤더파일 include
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

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(windowWidth, windowHeight); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);

	glutMainLoop(); // 이벤트 처리 시작
}

struct NEMO
{
	GLfloat x1, y1, x2, y2, r, g, b;
	bool create;
};
NEMO nemo[5];

bool start = true;
double randomX, randomY;





GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현
	
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

	//--- 그리기 관련 부분이 여기에 포함된다.
	for (int i = 0; i < 5; i++)
	{
		glColor3f(nemo[i].r, nemo[i].g, nemo[i].b);
		glRectf(nemo[i].x1, nemo[i].y1, nemo[i].x2, nemo[i].y2);
	}

	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
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
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}