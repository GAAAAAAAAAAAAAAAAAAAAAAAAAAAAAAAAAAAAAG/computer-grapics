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
	glutTimerFunc(1000, TimerFunction, 1);
	glutTimerFunc(100, TimerFunction, 2);
	glutTimerFunc(100, TimerFunction, 3);
	glutTimerFunc(100, TimerFunction, 4);

	glutMainLoop(); // 이벤트 처리 시작
}

struct NEMO
{
	GLfloat x1, y1, x2, y2, r, g, b;
	bool create;
	int direction = 0;	//0 우상, 1 좌상, 2 좌하, 3 우하
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
		if (nemo[i].create)
		{
			glColor3f(nemo[i].r, nemo[i].g, nemo[i].b);
			glRectf(nemo[i].x1, nemo[i].y1, nemo[i].x2, nemo[i].y2);
		}
	}

	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
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

int sizeChange = 0;	//50 좌우확대, 100 상하확대, 150 좌우축소, 200 상하축소
int zigzag = 0;	//50 우상, 100 우하

GLvoid TimerFunction(int value)
{
	if (!S)
	{
		switch (value)
		{
		case 1:
			if (O)	//색상 변화 O
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
			if (C)	//크기 변화 C
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
			if (I)	//지그재그 I
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
			if (A)	//대각선 A
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
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

//타이머 함수는 한 번만 실행되기 때문에 재귀함수로 다시 불러줘야한다.
//if문 안에 타이머 함수를 넣었을 시에는 키를 받는 곳에 함수를 다시 불러준다.