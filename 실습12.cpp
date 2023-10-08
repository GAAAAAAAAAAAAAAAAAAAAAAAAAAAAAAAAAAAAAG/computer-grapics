#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <math.h>

using namespace std;
random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> dis(-1.0,1.0);


struct TRISHAPE
{
	GLfloat triShape[3][3];
	GLfloat cX, cY;
	bool alive = false;
	bool animation = false;
};
TRISHAPE t[10];
int Tcnt = 3;

struct RECTSHAPE
{
	GLfloat rectShape[4][3];
	GLfloat cX, cY;
	bool alive = false;
	bool animation = false;
};
RECTSHAPE r[10];
int Rcnt = 3;

struct PENTASHAPE
{
	GLfloat pentaShape[5][3];
	GLfloat cX, cY;
	bool alive = false;
	bool animation = false;
};
PENTASHAPE p[10];
int Pcnt = 3;

struct LINESHAPE
{
	GLfloat lineShape[2][3];
	GLfloat cX, cY;
	bool alive = false;
	bool animation = false;
};
LINESHAPE l[10];
int Lcnt = 3;

struct DOTSHAPE
{
	GLfloat dotShape[3];
	GLfloat cX, cY;
	bool alive = false;
	bool animation = false;
};
DOTSHAPE d[10];
int Dcnt = 3;


GLfloat crashLine[2][3] = {};
GLfloat testLine[2][3] = {};

//GLfloat triShape[10][3][3] = {};//--- 삼각형 위치 값
//int triCnt = 0;
//
//GLfloat rectShape[10][4][3] = {}; //--- 사각형 위치 값
//int rectShapeCnt = 0;
//
//GLfloat pentaShape[10][5][3] = {};	//--- 오각형 위치 값
//int pentaCnt = 0;
//
//GLfloat lineShape[10][2][3] = {};	//--- 선분 위치 값
//int lineCnt = 0;
//
//GLfloat dotShape[10][3] = {};	//--- 점 위치 값
//int dotCnt = 0;

GLfloat colors[4][3] = { //--- 삼각형 꼭지점 색상
	{ 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 0.0 }
};

GLuint vao, vbo[2];
GLuint TriPosVbo, TriColorVbo;

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

int windowWidth = 800;
int windowHeight = 800;

float openGLX, openGLY;
int movingRectangle = -1;

bool start = true;
double Size = 0.075;
double deviation = 0.05;

double cx1, cy1, cx2, cy2;
double ra[5], rb[5];
double abDistance;
double Distance;
int vertex1, vertex2;

int select1, select2;

void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void InitBuffer();
char* filetobuf(const char*);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y);
GLvoid Motion(int x, int y);
GLvoid TimerFunction(int value);
int checkCollision();
BOOL Collision(float a_x1, float a_x2, float a_y1, float a_y2, float b_x1, float b_x2, float b_y1, float b_y2);

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Example1");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	//--- 세이더 읽어와서 세이더 프로그램 만들기
	make_shaderProgram(); //--- 세이더 프로그램 만들기
	InitBuffer();

	glutTimerFunc(1000, TimerFunction, 1);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	glutMainLoop();
}

GLvoid drawScene()
{
	//--- 변경된 배경색 설정
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);
	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);

	if (start)
	{
		int cnt = 0;

		while (cnt < 3)
		{
			double centerX, centerY;
			centerX = dis(gen);
			centerY = dis(gen);

			d[cnt].dotShape[0] = centerX;
			d[cnt].dotShape[1] = centerY;
			d[cnt].cX = centerX;
			d[cnt].cY = centerY;

			centerX = dis(gen);
			centerY = dis(gen);
			l[cnt].lineShape[0][0] = centerX-Size;
			l[cnt].lineShape[0][1] = centerY;
			l[cnt].lineShape[1][0] = centerX + Size;
			l[cnt].lineShape[1][1] = centerY;
			l[cnt].cX = centerX;
			l[cnt].cY = centerY;

			centerX = dis(gen);
			centerY = dis(gen);
			t[cnt].triShape[0][0] = centerX - Size;
			t[cnt].triShape[0][1] = centerY - Size;
			t[cnt].triShape[1][0] = centerX + Size;
			t[cnt].triShape[1][1] = centerY - Size;
			t[cnt].triShape[2][0] = centerX;
			t[cnt].triShape[2][1] = centerY + Size;
			t[cnt].cX = centerX;
			t[cnt].cY = centerY;

			centerX = dis(gen);
			centerY = dis(gen);
			r[cnt].rectShape[0][0] = centerX - Size;
			r[cnt].rectShape[0][1] = centerY - Size;
			r[cnt].rectShape[1][0] = centerX + Size;
			r[cnt].rectShape[1][1] = centerY - Size;
			r[cnt].rectShape[2][0] = centerX + Size;
			r[cnt].rectShape[2][1] = centerY + Size;
			r[cnt].rectShape[3][0] = centerX - Size;
			r[cnt].rectShape[3][1] = centerY + Size;
			r[cnt].cX = centerX;
			r[cnt].cY = centerY;

			centerX = dis(gen);
			centerY = dis(gen);
			p[cnt].pentaShape[0][0] = centerX - Size;
			p[cnt].pentaShape[0][1] = centerY - Size;
			p[cnt].pentaShape[1][0] = centerX + Size;
			p[cnt].pentaShape[1][1] = centerY - Size;
			p[cnt].pentaShape[2][0] = centerX + Size +Size/2;
			p[cnt].pentaShape[2][1] = centerY + Size/2;
			p[cnt].pentaShape[3][0] = centerX;
			p[cnt].pentaShape[3][1] = centerY + Size + Size/2;
			p[cnt].pentaShape[4][0] = centerX - Size - Size/2;
			p[cnt].pentaShape[4][1] = centerY + Size/2;
			p[cnt].cX = centerX;
			p[cnt].cY = centerY;

			cnt++;
		}
		start = false;
	}

	// 색상 바꾸기
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 점 찍기
	for (int i = 0; i < Dcnt; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), d[i].dotShape, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glPointSize(10.0);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	

	// 선 그리기
	for (int i = 0; i < Lcnt; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), l[i].lineShape, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glLineWidth(10.0);
		glDrawArrays(GL_LINES, 0, 2);
	}
	

	// 삼각형 그리기
	for (int i = 0; i < Tcnt; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), t[i].triShape, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	

	// 사각형 그리기
	for (int i = 0; i < Rcnt; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), r[i].rectShape, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_QUADS, 0, 4);
	}

	// 오각형 그리기
	for (int i = 0; i < Pcnt; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), p[i].pentaShape, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_POLYGON, 0, 5);
	}

	glutSwapBuffers(); //--- 화면에 출력하기
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
}

void make_shaderProgram()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}

void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cout << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cout << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0; // Null terminator 
	return buf; // Return the buffer 
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

int movingMouse = -1;
float beforeX, beforeY;
int crash = 0;
bool crashStart = false;
int vertex = 0;
int selectNum = 0;

GLvoid Mouse(int button, int state, int x, int y)
{
	float openGLX, openGLY;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			WindowToOpenGL(x, y, openGLX, openGLY);

			for (int i = 0; i < 10; i++)
			{
				if (t[i].cX - 0.07 < openGLX && t[i].cX + 0.07 > openGLX && t[i].cY - 0.07 < openGLY && t[i].cY + 0.07 > openGLY)
				{
					vertex = 3;
					selectNum = i;
					movingMouse = 0;
					beforeX = openGLX;
					beforeY = openGLY;
					break;
				}
				else if (r[i].cX - 0.07 < openGLX && r[i].cX + 0.07 > openGLX && r[i].cY - 0.07 < openGLY && r[i].cY + 0.07 > openGLY)
				{
					vertex = 4;
					selectNum = i;
					movingMouse = 0;
					beforeX = openGLX;
					beforeY = openGLY;
					break;
				}
				else if (p[i].cX - 0.07 < openGLX && p[i].cX + 0.07 > openGLX && p[i].cY - 0.07 < openGLY && p[i].cY + 0.07 > openGLY)
				{
					vertex = 5;
					selectNum = i;
					movingMouse = 0;
					beforeX = openGLX;
					beforeY = openGLY;
					break;
				}
				else if (d[i].cX - 0.07 < openGLX && d[i].cX + 0.07 > openGLX && d[i].cY - 0.07 < openGLY && d[i].cY + 0.07 > openGLY)
				{
					vertex = 1;
					selectNum = i;
					movingMouse = 0;
					beforeX = openGLX;
					beforeY = openGLY;
					break;
				}
				else if (l[i].cX - 0.07 < openGLX && l[i].cX + 0.07 > openGLX && l[i].cY - 0.07 < openGLY && l[i].cY + 0.07 > openGLY)
				{
					vertex = 2;
					selectNum = i;
					movingMouse = 0;
					beforeX = openGLX;
					beforeY = openGLY;
					break;
				}
			}
		}
		else if (state == GLUT_UP)
		{
			movingMouse = -1;
		}
	}
	
}

GLvoid Motion(int x, int y)
{
	if (movingMouse >= 0)
	{
		crashLine[1][0] = 1.0;
		crashLine[1][1] = openGLY;
		WindowToOpenGL(x, y, openGLX, openGLY);

		float deltaX = openGLX - beforeX;
		float deltaY = openGLY - beforeY;

		switch (vertex)
		{
		case 1:
			d[selectNum].dotShape[0] += deltaX;
			d[selectNum].dotShape[1] += deltaY;
			d[selectNum].cX += deltaX;
			d[selectNum].cY += deltaY;
			break;
		case 2:
			l[selectNum].lineShape[0][0] += deltaX;
			l[selectNum].lineShape[0][1] += deltaY;
			l[selectNum].lineShape[1][0] += deltaX;
			l[selectNum].lineShape[1][1] += deltaY;
			l[selectNum].cX += deltaX;
			l[selectNum].cY += deltaY;
			break;
		case 3:
			t[selectNum].triShape[0][0] += deltaX;
			t[selectNum].triShape[0][1] += deltaY;
			t[selectNum].triShape[1][0] += deltaX;
			t[selectNum].triShape[1][1] += deltaY;
			t[selectNum].triShape[2][0] += deltaX;
			t[selectNum].triShape[2][1] += deltaY;
			t[selectNum].cX += deltaX;
			t[selectNum].cY += deltaY;
			break;
		case 4:
			r[selectNum].rectShape[0][0] += deltaX;
			r[selectNum].rectShape[0][1] += deltaY;
			r[selectNum].rectShape[1][0] += deltaX;
			r[selectNum].rectShape[1][1] += deltaY;
			r[selectNum].rectShape[2][0] += deltaX;
			r[selectNum].rectShape[2][1] += deltaY;
			r[selectNum].rectShape[3][0] += deltaX;
			r[selectNum].rectShape[3][1] += deltaY;
			r[selectNum].cX += deltaX;
			r[selectNum].cY += deltaY;
			break;
		case 5:
			p[selectNum].pentaShape[0][0] += deltaX;
			p[selectNum].pentaShape[0][1] += deltaY;
			p[selectNum].pentaShape[1][0] += deltaX;
			p[selectNum].pentaShape[1][1] += deltaY;
			p[selectNum].pentaShape[2][0] += deltaX;
			p[selectNum].pentaShape[2][1] += deltaY;
			p[selectNum].pentaShape[3][0] += deltaX;
			p[selectNum].pentaShape[3][1] += deltaY;
			p[selectNum].pentaShape[4][0] += deltaX;
			p[selectNum].pentaShape[4][1] += deltaY;
			p[selectNum].cX += deltaX;
			p[selectNum].cY += deltaY;
			break;
		}

		beforeX = openGLX;
		beforeY = openGLY;

		glutPostRedisplay();  // 화면을 다시 그립니다.
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

		break;
	}
	glutPostRedisplay();
	glutTimerFunc(1000, TimerFunction, 1);
}

int checkCollision()
{
	// 두 선분의 좌표를 변수에 저장
	double x1 = testLine[0][0];
	double x2 = testLine[1][0];
	double x3 = crashLine[0][0];
	double x4 = crashLine[1][0];

	double y1 = testLine[0][1];
	double y2 = testLine[1][1];
	double y3 = crashLine[0][1];
	double y4 = crashLine[1][1];

	// 두 선분의 방정식 계수
	double a1 = y2 - y1;
	double b1 = x1 - x2;
	double c1 = x2 * y1 - x1 * y2;

	double a2 = y4 - y3;
	double b2 = x3 - x4;
	double c2 = x4 * y3 - x3 * y4;

	// 두 직선이 만나는 점의 좌표
	double intersectionX = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
	double intersectionY = (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1);

	// 점이 두 선분 사이에 있는지 확인
	if (
		(intersectionX >= fmin(x1, x2) && intersectionX <= fmax(x1, x2)) &&
		(intersectionY >= fmin(y1, y2) && intersectionY <= fmax(y1, y2)) &&
		(intersectionX >= fmin(x3, x4) && intersectionX <= fmax(x3, x4)) &&
		(intersectionY >= fmin(y3, y4) && intersectionY <= fmax(y3, y4))
		) {
		return 1; // 두 선분이 교차함
	}
	else {
		return 0; // 두 선분이 교차하지 않음
	}
}

int collision(int vertex1, int selectNum1, int vertex2, int selectNum2)
{
	double cx1, cy1, cx2, cy2;
	switch (vertex1)
	{
	case 1:
		cx1 = d[selectNum1].cX;
		cy1 = d[selectNum1].cY;
		break;
	case 2:
		cx1 = l[selectNum1].cX;
		cy1 = l[selectNum1].cY;
		break;
	case 3:
		cx1 = t[selectNum1].cX;
		cy1 = t[selectNum1].cY;
		break;
	case 4:
		cx1 = r[selectNum1].cX;
		cy1 = r[selectNum1].cY;
		break;
	case 5:
		cx1 = p[selectNum1].cX;
		cy1 = p[selectNum1].cY;
		break;
	}
	switch (vertex2)
	{
	case 1:
		cx1 = d[selectNum2].cX;
		cy1 = d[selectNum2].cY;
		break;
	case 2:
		cx1 = l[selectNum2].cX;
		cy1 = l[selectNum2].cY;
		break;
	case 3:
		cx1 = t[selectNum2].cX;
		cy1 = t[selectNum2].cY;
		break;
	case 4:
		cx1 = r[selectNum2].cX;
		cy1 = r[selectNum2].cY;
		break;
	case 5:
		cx1 = p[selectNum2].cX;
		cy1 = p[selectNum2].cY;
		break;
	}

	return Collision(cx1 - 0.07, cx1 + 0.07, cy1 - 0.07, cy1 + 0.07, cx2 - 0.07, cx2 + 0.07, cy2 - 0.07, cy2 + 0.07);
}

BOOL Collision(float a_x1, float a_x2, float a_y1, float a_y2, float b_x1, float b_x2, float b_y1, float b_y2)
{
	if (((b_x1 < a_x1 && a_x1 < b_x2) || (b_x1 < a_x2 && a_x2 < b_x2)) &&
		((b_y1 < a_y2 && a_y2 < b_y2) ||  (b_y1 < a_y1 && a_y1 < b_y2)))
	{
		return true;
	}
	return false;
}

//int sat()
//{
//	Distance = cx2 - cx1;
//
//	/*for (int i = 0; i < vertex1; i++)
//	{
//		if (vertex1 == 4)
//		{
//			cx1 = t[select1].cX;
//			cy1 = t[select2].cY;
//		}
//	}*/
//
//	//삼각형1
//	for (int i = 0; i < 3; i++)
//	{
//		ra[i] = t[select1].triShape[i][0] - t[select1].cX;
//		cx1 = t[select1].cX;
//	}
//	//삼각형2
//	for (int i = 0; i < 3; i++)
//	{
//		rb[i] = t[select2].triShape[i][0] - t[select2].cX;
//		cy1 = t[select1].cY;
//	}
//
//	//확인
//	for (int i = 0; i < 3; i++)
//	{
//		if (Distance >= ra[i] + rb[i])
//		{
//			return 1;
//		}
//	}
//
//	return 0;
//}