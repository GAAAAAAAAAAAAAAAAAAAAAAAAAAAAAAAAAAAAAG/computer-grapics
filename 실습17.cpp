﻿#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <random>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> XYdis(-1, 1);
uniform_real_distribution<double> dis(0, 1);

glm::vec3 cube[12][3]{
	// 앞면
	{ {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5} },
	{ {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5} },

	// 뒷면
	{ {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5} },
	{ {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5} },

	// 상단면
	{ {-0.5, 0.5, -0.5}, {0.5, 0.5, -0.5}, {0.5, 0.5, 0.5} },
	{ {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, 0.5, -0.5} },

	// 하단면
	{ {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, -0.5, 0.5} },
	{ {0.5, -0.5, 0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5} },

	// 왼쪽면
	{ {-0.5, -0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5} },
	{ {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5} },

	// 오른쪽면
	{ {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {0.5, 0.5, 0.5} },
	{ {0.5, 0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, -0.5, -0.5} },
};

glm::vec3 pyramid[6][3]{
	// 밑면 (사각형)
	{ {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5} },
	{ {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5} },

	// 앞면 (삼각형)
	{ {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.0, 0.0, 0.5} },

	// 오른쪽면 (삼각형)
	{ {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {0.0, 0.0, 0.5} },

	// 왼쪽면 (삼각형)
	{ {-0.5, -0.5, -0.5}, {0.0, 0.0, 0.5}, {-0.5, 0.5, -0.5} },

	// 뒷면 (삼각형)
	{ {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.0, 0.0, 0.5} },
};

GLfloat triShape[10][3][3] = {};//--- 삼각형 위치 값

GLfloat rectShape[10][4][3] = {}; //--- 사각형 위치 값

GLfloat lineShape[10][2][3] = {};	//--- 선분 위치 값

GLfloat dotShape[250][3] = {};

glm::vec3 colors[12][3] = {};

GLfloat XYZShape[3][2][3] = {
	{{-1.0,0.0,0.0},{1.0,0.0,0.0}},
	{{0.0,-1.0,0.0},{0.0,1.0,0.0}},
	{{0.0,0.0,-1.0},{0.0,0.0,1.0}} };

GLfloat XYZcolors[6][3] = { //--- 축 색상
	{ 1.0, 0.0, 0.0 },	   	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },	   	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },	   	{ 0.0, 0.0, 1.0 }
};

GLuint vao, vbo[2];
GLuint TriPosVbo, TriColorVbo;

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

int windowWidth = 800;
int windowHeight = 600;

float openGLX, openGLY;
int movingRectangle = -1;

bool start = true;
bool depth_test = true;
bool Wselect = false;

float xRotation = 0;
float yRotation = 0;
int xRotateSelect = 0;
int yRotateSelect = 0;

//방향키 전체 도형 이동
float xTransform = 0;
float yTransform = 0;
float zTransform = 0;

//각 도형 이동
float xTransformL = 0;
float yTransformL = 0;
float zTransformL = 0;
//각 도형 이동
float xTransformR = 0;
float yTransformR = 0;
float zTransformR = 0;

//키보드 제자리 신축
float hereScale = 1;
float hereScaleL = 1;
float hereScaleR = 1;
//키보드 원점 기준 신축
float Scale0 = 1;
float Scale0L = 1;
float Scale0R = 1;

bool a = false;
bool b = false;

bool c = true;

// 도형 선택
bool objectL = true;
bool objectR = true;

bool revolution = false;
float revolutionAngle = 0;
bool revolutionCnt = false;

//스파이럴
double theta = 0, r = 0;
int cnt = 0;
double thetaL = 0, rL = 0;
int cntL = 0;
double thetaR = 0, rR = 0;
int cntR = 0;
bool rSelection = false;

// 두 도형 원점기준으로 왔다리 갔다리
bool Move0 = false;
float Move0Size = 0;

float Move0SizeXL = 0;
float Move0SizeYL = 0;
float Move0SizeZL = 0;

float Move0SizeXR = 0;
float Move0SizeYR = 0;
float Move0SizeZR = 0;

float Move0XL = 0;
float Move0YL = 0;
float Move0ZL = 0;
float Move0XR = 0;
float Move0YR = 0;
float Move0ZR = 0;

int Move0Cnt = 0;

// 두 도형 원점 지나치고 왔다리 갔다리
bool Move0Pass = false;
float Move0PassSize = 0;
int Move0PassCnt = 0;

// 두 도형이 한개는 위로, 다른 한 개는 아래로 이동하면서 서로 자리 탐하기
bool UpDownMove = false;
float UpDownSizeX = 0;
float UpDownSizeY = 0;
int UpDownCnt = 0;

glm::mat4 model = glm::mat4(1.0f);

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
GLvoid SpecialKeys(int key, int x, int y);


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
	glEnable(GL_DEPTH_TEST);

	glutTimerFunc(10, TimerFunction, 1);
	glutTimerFunc(20, TimerFunction, 2);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeys); // 방향키 콜백 함수 등록
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	glutMainLoop();
}


GLvoid drawScene()
{

	GLUquadricObj* qobj;

	glUseProgram(shaderProgramID);
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //--- 깊이 버퍼를 클리어한다.

	glBindVertexArray(vao);

	// 색상 바꾸기
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), XYZcolors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);


	// 세이더 프로그램에서 modelTransform 변수 위치 가져오기
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform");

	if (start)
	{
		double m = 0.0;
		double cx, cy, cz;
		for (int i = 0; i < 12; i++)
		{
			cx = 0.3 + m;
			cy = 0.0 + m;
			cz = 0.3 + m;

			m += 0.05;

			//glm::vec3 colors[10][3] = {};

			colors[i][0].x = cx;
			//cx = color(gen);
			colors[i][1].x = cx;
			//cx = color(gen);
			colors[i][2].x = cx;

			colors[i][0].y = cy;
			//cy = color(gen);
			colors[i][1].y = cy;
			//cy = color(gen);
			colors[i][2].y = cy;

			colors[i][0].z = cz;
			//cz = color(gen);
			colors[i][1].z = cz;
			//cz = color(gen);
			colors[i][2].z = cz;
		}

		for (int i = 0; i < 250; i++)
		{
			theta = cnt * 0.1;
			r = 0.01 + 0.03 * theta;

			dotShape[cnt][0] = r * cos(theta) + 0;
			dotShape[cnt][1] = r * sin(theta) + 0;
			dotShape[cnt][2] = 0.0;

			cnt++;
		}

		start = false;
	}

	model = glm::mat4(1.0f);

	// x축으로 10도 회전
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// y축으로 10도 회전
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// z축으로 10도 회전
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	//축 그리기
	for (int i = 0; i < 3; i++)
	{
		// 색상 바꾸기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), XYZcolors[i * 2], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), XYZShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glLineWidth(2.0);
		glDrawArrays(GL_LINES, 0, 2);
	}

	if (rSelection)
	{
		//스파이럴 그리기
		model = glm::mat4(1.0f);
		// x축으로 10도 회전
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// y축으로 10도 회전
		//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		for (int j = 0; j < 250; j++)
		{
			if (j > 1 && j < 249)
			{
				glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
				glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), dotShape[j], GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(0);
				glLineWidth(1.0);
				glDrawArrays(GL_LINES, 0, 2);
			}
		}
	}
	
	//s r t p
	model = glm::mat4(1.0f);

	// x축으로 30도 회전
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// y축으로 30도 회전
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// z축으로 0도 회전
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	//공전
	if (revolution > 0)
	{
		model = glm::rotate(model, glm::radians(revolutionAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	//원점에서 원래 자리까지 왔다리 갔다리
	if (Move0)
	{
		//model = glm::translate(model, glm::vec3(Move0Size, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(Move0XL, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, Move0YL, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, Move0ZL));
	}

	//원점 지나서 원래 자리까지 왔다리 갔다리
	if (Move0Pass)
	{
		model = glm::translate(model, glm::vec3(Move0PassSize, 0.0f, 0.0f));
	}

	//위 아래로 이동하면서 자리 찾기
	if (UpDownMove)
	{
		model = glm::translate(model, glm::vec3(UpDownSizeX, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, UpDownSizeY ,0.0f));
	}

	//스파이럴 이동
	if (rSelection)
	{
		model = glm::translate(model, glm::vec3(rL * cos(thetaL), 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, rL * sin(thetaL), 0.0f));
	}

	//방향키 도형 이동

	model = glm::translate(model, glm::vec3(xTransformL, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, yTransformL, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, zTransformL));

	
	
	
	//이동
	if (!rSelection)
	{
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
	}
	
	//키보드 제자리 신축
	if (!rSelection)
	{
		model = glm::scale(model, glm::vec3(hereScaleL, hereScaleL, hereScaleL));
	}
	
	//원점 기준으로 신축
	if (!rSelection)
	{
		model = glm::scale(glm::mat4(1.0f), glm::vec3(Scale0L, Scale0L, Scale0L)) * model;
	}
	
	//축소
	model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25));

	//정육면체
	for (int i = 0; i < 12; i++) {
		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &colors[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &cube[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);


		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	//s r t p
	model = glm::mat4(1.0f);

	// x축으로 30도 회전
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// y축으로 30도 회전
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// z축으로 0도 회전
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	//공전
	if (revolution > 0)
	{
		model = glm::rotate(model, glm::radians(revolutionAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	}

	//원점에서 원래 자리까지 왔다리 갔다리
	if (Move0)
	{
		//model = glm::translate(model, glm::vec3(Move0Size, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(Move0XR, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, Move0YR, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, Move0ZR));
	}

	//원점 지나서 원래 자리까지 왔다리 갔다리
	if (Move0Pass)
	{
		model = glm::translate(model, glm::vec3(-Move0PassSize, 0.0f, 0.0f));
	}

	//위 아래로 이동하면서 자리 찾기
	if (UpDownMove)
	{
		model = glm::translate(model, glm::vec3(-UpDownSizeX, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -UpDownSizeY, 0.0f));
	}

	//스파이럴 이동
	if (rSelection)
	{
		model = glm::translate(model, glm::vec3(rR * cos(thetaR), 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, rR * sin(thetaR), 0.0f));
	}

	//방향키 도형 이동
	model = glm::translate(model, glm::vec3(xTransformR, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, yTransformR, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, zTransformR));
	
	//이동
	if (!rSelection)
	{
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
	}

	//키보드 제자리 신축
	if (!rSelection)
	{
		model = glm::scale(model, glm::vec3(hereScaleR, hereScaleR, hereScaleR));

	}
	
	
	//원점 기준으로 신축
	if (!rSelection)
	{
		model = glm::scale(glm::mat4(1.0f), glm::vec3(Scale0R, Scale0R, Scale0R)) * model;

	}
	
	
	//축소
	model = glm::scale(model, glm::vec3(0.15, 0.15, 0.15));

	
	//구
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	qobj = gluNewQuadric(); // 객체 생성하기
	gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
	gluQuadricNormals(qobj, GLU_SMOOTH); // 생략 가능
	gluQuadricOrientation(qobj, GLU_OUTSIDE); // 생략 가능
	gluSphere(qobj, 1.0, 20, 20); // 객체 만들기

	// modelTransform 변수에 변환 값 적용하기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

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
	//z축 이동
	case '-':
		//zTransform -= 0.05;
		if (objectL)
		{
			zTransformL -= 0.05;
		}
		if (objectR)
		{
			zTransformR -= 0.05;
		}
		
		break;
	case '=':
		//zTransform += 0.05;
		if (objectL)
		{
			zTransformL += 0.05;
		}
		if (objectR)
		{
			zTransformR += 0.05;
		}
		break;
	//제자리 신축
	case 'o':
		//hereScale -= 0.05;
		if (objectL)
		{
			hereScaleL -= 0.05;

		}
		if (objectR)
		{
			hereScaleR -= 0.05;

		}
		break;
	case 'p':
		//hereScale += 0.05;
		if (objectL)
		{
			hereScaleL += 0.05;

		}
		if (objectR)
		{
			hereScaleR += 0.05;

		}
		break;
	//원점에 대하여 신축
	case 'u':
		//Scale0 -= 0.05;
		if (objectL)
		{

			Scale0L -= 0.05;
		}
		if (objectR)
		{

			Scale0R -= 0.05;
		}
		break;
	case 'i':
		//Scale0 += 0.05;
		if (objectL)
		{

			Scale0L += 0.05;
		}
		if (objectR)
		{

			Scale0R += 0.05;
		}
		break;
	//스파이럴 회전
	case'r':
		rSelection = !rSelection;
		if (!rSelection)
		{
			cntL=0;
			cntR=0;
		}
		break;
	//각 도형 고르기
	case 'j':
		objectL = true;
		objectR = false;
		break;
	case 'k':
		objectL = false;
		objectR = true;
		break;
	//전체 도형 고르기
	case 'l':
		objectL = true;
		objectR = true;
		break;
	case 'c':
		c = !c;
		break;
	case 's':
		//-----------
		model = glm::mat4(1.0f);

		xRotation = 0;
		yRotation = 0;
		xRotateSelect = 0;
		yRotateSelect = 0;

		//방향키 전체 도형 이동
		xTransformL = 0;
		yTransformL = 0;
		zTransformL = 0;
		xTransformR = 0;
		yTransformR = 0;
		zTransformR = 0;

		//키보드 제자리 신축
		//hereScale = 1;
		hereScaleL = 1;
		hereScaleR = 1;
		//키보드 원점 기준 신축
		//Scale0 = 1;
		Scale0L = 1;
		Scale0R = 1;


		a = false;
		b = false;

		c = true;

		// 도형 선택
		objectL = true;
		objectR = true;

		revolution = false;
		revolutionAngle = 0;

		//스파이럴
		theta = 0, r = 0;
		cnt = 0;
		thetaL = 0, rL = 0;
		cntL = 0;
		thetaR = 0, rR = 0;
		cntR = 0;
		rSelection = false;

		// 두 도형 원점기준으로 왔다리 갔다리
		Move0 = false;
		Move0Size = 0;
		Move0Cnt = 0;
		Move0XL = 0;
		Move0YL = 0;
		Move0ZL = 0;
		Move0XR = 0;
		Move0YR = 0;
		Move0ZR = 0;

		// 두 도형 원점 지나치고 왔다리 갔다리
		Move0Pass = false;
		Move0PassSize = 0;
		Move0PassCnt = 0;

		revolutionCnt = false;

		UpDownMove = false;
		UpDownSizeX = 0;
		UpDownSizeY = 0;
		UpDownCnt = 0;
		//-----------
		break;
		//원점에서 원래 자리까지 왔다리 갔다리
	case 't':
		Move0 = !Move0;
		if (Move0)
		{
			Move0SizeXL = (xTransformL- 0.5)/10;
			Move0SizeYL = (yTransformL)/10;
			Move0SizeZL = (zTransformL)/10;
			Move0SizeXR = (xTransformR + 0.5)/10;
			Move0SizeYR = (yTransformR)/10;
			Move0SizeZR = (zTransformR)/10;
		}

		if (!Move0)
		{
			Move0Size = 0;
			Move0Cnt = 0;
			Move0XL=0;
			Move0YL=0;
			Move0ZL=0;
			Move0XR=0;
			Move0YR=0;
			Move0ZR=0;
		}
		break;
		//원점 지나치고 상대 자리에서 원래 자리까지 왔다리 갔다리
	case'1':
		Move0Pass = !Move0Pass;

		if (!Move0Pass)
		{
			Move0PassSize = 0;
			Move0PassCnt = 0;
		}
		break;
	case'2':
		revolutionCnt = !revolutionCnt;
		if (revolutionCnt)
		{
			revolution = 1;
		}
		else
		{
			revolution = 0;
		}
		break;
	case'3':
		UpDownMove = !UpDownMove;
		if (!UpDownMove)
		{
			UpDownSizeX = 0;
			UpDownSizeY = 0;
			UpDownCnt = 0;
		}
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid SpecialKeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		//yTransform += 0.05;
		if (objectL)
		{
			yTransformL += 0.05;
		}
		if (objectR)
		{
			yTransformR += 0.05;
		}
		break;
	case GLUT_KEY_DOWN:
		//yTransform -= 0.05;
		if (objectL)
		{
			yTransformL -= 0.05;
		}
		if (objectR)
		{
			yTransformR -= 0.05;
		}
		break;
	case GLUT_KEY_LEFT:
		//xTransform -= 0.05;
		if (objectL)
		{
			xTransformL -= 0.05;
		}
		if (objectR)
		{
			xTransformR -= 0.05;
		}
		
		break;
	case GLUT_KEY_RIGHT:
		//xTransform += 0.05;
		if (objectL)
		{
			xTransformL += 0.05;
		}
		if (objectR)
		{
			xTransformR += 0.05;
		}
		break;
	}
	glutPostRedisplay(); // 화면 갱신
}

int movingMouse = -1;
float beforeX, beforeY;

GLvoid Mouse(int button, int state, int x, int y)
{
	float openGLX, openGLY;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);

		movingMouse = 0;

		beforeX = openGLX;
		beforeY = openGLY;
	}
	else if (state == GLUT_UP)
	{
		movingMouse = -1;
	}
}

GLvoid Motion(int x, int y)
{
	if (movingMouse >= 0)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);

		float deltaX = openGLX - beforeX;
		float deltaY = openGLY - beforeY;

		//이동

		//마우스값 넣기
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
		if (revolution == 2)
		{
			revolutionAngle += 1;
		}
		if (revolution == 1)
		{
			revolutionAngle -= 1;
		}

		if (rSelection)
		{
			if (cntL < 250)
			{
				thetaL = cntL * 0.1;
				rL = 0.01 + 0.03 * thetaL;

				cntL++;
			}
		}

		glutTimerFunc(10, TimerFunction, 1);
		break;
	case 2:
		if (rSelection)
		{
			if (cntR < 250)
			{
				thetaR = cntR * 0.1;
				rR = 0.01 + 0.03 * thetaR;

				cntR++;
			}
		}
		if (Move0)
		{
			if (Move0Cnt < 10)
			{
				Move0XL -= Move0SizeXL;
				Move0YL -= Move0SizeYL;
				Move0ZL -= Move0SizeZL;
				Move0XR -= Move0SizeXR;
				Move0YR -= Move0SizeYR;
				Move0ZR -= Move0SizeZR;
			}
			else if (Move0Cnt > 10 && Move0Cnt < 20)
			{
				Move0XL += Move0SizeXL;
				Move0YL += Move0SizeYL;
				Move0ZL += Move0SizeZL;
				Move0XR += Move0SizeXR;
				Move0YR += Move0SizeYR;
				Move0ZR += Move0SizeZR;

				//Move0Size -= 0.05;
			}
			else if (Move0Cnt == 20)
			{
				Move0Cnt = 0;
			}
			Move0Cnt++;
		}

		if (Move0Pass)
		{
			if (Move0PassCnt < 20)
			{
				Move0PassSize += 0.05;
			}
			else if (Move0PassCnt > 20 && Move0PassCnt < 40)
			{
				Move0PassSize -= 0.05;
			}
			else if (Move0PassCnt == 40)
			{
				Move0PassCnt = 0;
			}
			Move0PassCnt++;
		}

		if (UpDownMove)
		{
			if (UpDownCnt < 10)
			{
				UpDownSizeX += 0.05;
				UpDownSizeY += 0.05;
			}
			else if (UpDownCnt > 10 && UpDownCnt < 20)
			{
				UpDownSizeX += 0.05;
				UpDownSizeY -= 0.05;
			}
			else if (UpDownCnt > 20 && UpDownCnt < 30)
			{
				UpDownSizeX -= 0.05;
				UpDownSizeY -= 0.05;
			}
			else if (UpDownCnt > 30 && UpDownCnt < 40)
			{
				UpDownSizeX -= 0.05;
				UpDownSizeY += 0.05;
			}
			else if (UpDownCnt == 40)
			{
				UpDownCnt = 0;
			}

			UpDownCnt++;
		}
		glutTimerFunc(20, TimerFunction, 2);
		break;
	}
	glutPostRedisplay();
}