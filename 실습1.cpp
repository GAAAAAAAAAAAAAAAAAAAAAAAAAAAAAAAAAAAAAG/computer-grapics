#include <iostream>
#include <gl/glew.h> // 필요한 헤더파일 include
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
GLvoid TimerFunction(int value);

bool C = false;
bool M = false;
bool Y = false;
bool A = false;
bool W = false;
bool K = false;
bool T = false;
bool S = false;
bool Q = false;
float r, g, b;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
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
	glutDisplayFunc(drawScene); // 출력 콜백함수의 지정
	glutTimerFunc(1000, TimerFunction, 1);
	glutReshapeFunc(Reshape); // 다시 그리기 콜백함수 지정
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백함수 지정
	
	glutMainLoop(); // 이벤트 처리 시작
	
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

GLvoid drawScene ( ) //--- 콜백 함수: 그리기 콜백 함수
{ 
	//--- 변경된 배경색 설정
	//glClearColor (0.0f,0.0f,1.0f,1.0f); //--- 바탕색을 변경
	if (C)
	{
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (M)
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	}
	else if (Y)
	{
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (W)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (K)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (A)
	{
		r = dis(gen);
		g = dis(gen);
		b = dis(gen);

		glClearColor(r, g, b, 1.0f);
	}
	else if (T)
	{
		r = dis(gen);
		g = dis(gen);
		b = dis(gen);

		glClearColor(r, g, b, 1.0f);
	}

	glClear ( GL_COLOR_BUFFER_BIT ); //--- 설정된 색으로 전체를 칠하기
	glutSwapBuffers ( ); //--- 화면에 출력하기
}
GLvoid Reshape ( int w, int h ) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport ( 0, 0, w, h );
}
GLvoid Keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
	case 'C':
		C = true;
		M = false;
		Y = false;
		A = false;
		W = false;
		K = false;
		T = false;
		S = false;
		Q = false;
		break; //--- 배경색을 청록색으로 설정
	case 'm':
		C = false;
		M = true;
		Y = false;
		A = false;
		W = false;
		K = false;
		T = false;
		S = false;
		Q = false;
		break; //--- 배경색을 자홍색으로 설정
	case 'y' :
		C = false;
		M = false;
		Y = true;
		A = false;
		W = false;
		K = false;
		T = false;
		S = false;
		Q = false;
		break; //--- 배경색을 노랑색으로 설정
	case 'a':
		C = false;
		M = false;
		Y = false;
		A = true;
		W = false;
		K = false;
		T = false;
		S = false;
		Q = false;
		break;
	case 'w':
		C = false;
		M = false;
		Y = false;
		A = false;
		W = true;
		K = false;
		T = false;
		S = false;
		Q = false;
		break;
	case 'k':
		C = false;
		M = false;
		Y = false;
		A = false;
		W = false;
		K = true;
		T = false;
		S = false;
		Q = false;
		break;
	case 't':
		C = false;
		M = false;
		Y = false;
		A = false;
		W = false;
		K = false;
		T = true;
		S = false;
		Q = false;
		break;
	case 's':
		C = false;
		M = false;
		Y = false;
		A = false;
		W = false;
		K = false;
		T = false;
		S = true;
		Q = false;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay (); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}