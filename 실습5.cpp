#include <iostream>
#include <gl/glew.h> // 필요한 헤더파일 include
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
	glutMotionFunc(Motion);

	glutMainLoop(); // 이벤트 처리 시작
}

double randomX, randomY;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현
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

	//--- 그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
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
			movingRectangle = -1;  // 마우스 버튼을 놓으면 이동 모드를 해제합니다.

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

		// 이동 중인 사각형의 중심점을 계산
		float centerX = (nemo[movingRectangle].x1 + nemo[movingRectangle].x2) / 2;
		float centerY = (nemo[movingRectangle].y1 + nemo[movingRectangle].y2) / 2;

		// 이동한 거리 계산
		float deltaX = openGLX - centerX;
		float deltaY = openGLY - centerY;

		// 사각형 위치 이동
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

		glutPostRedisplay();  // 화면을 다시 그립니다.
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
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
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
