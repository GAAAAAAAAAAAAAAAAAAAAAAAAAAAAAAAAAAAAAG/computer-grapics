#include <iostream>
#include <gl/glew.h> // 필요한 헤더파일 include
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
GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현
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

	//--- 그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
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
			movingRectangle = -1;  // 마우스 버튼을 놓으면 이동 모드를 해제합니다.
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
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}