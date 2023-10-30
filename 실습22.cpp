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
uniform_real_distribution<float> color(0.0, 1.0);

glm::vec3 cube1[12][3]{
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
glm::vec3 cube2[12][3]{
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
glm::vec3 cube3[12][3]{
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
glm::vec3 cube4[12][3]{
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
glm::vec3 cube5[12][3]{
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
glm::vec3 cube6[12][3]{
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
glm::vec3 cube7[12][3]{
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

GLfloat rectShape1[4][3] = {	//밑면
	{-1,1,0},{-1,-1,0},{1,-1,0},{1,1,0}
}; //--- 사각형 위치 값
GLfloat rectShape2[4][3] = {	//왼쪽면
	{-1,-1,0},{-1,-1,1},{-1,1,1},{-1,1,0}
}; //--- 사각형 위치 값
GLfloat rectShape3[4][3] = {	//뒷면
	{-1,1,0},{-1,1,1},{1,1,1},{1,1,0}
}; //--- 사각형 위치 값
GLfloat rectShape4[4][3] = {	//오른쪽면
	{1,-1,0},{1,1,0},{1,1,1},{1,-1,1}
}; //--- 사각형 위치 값
GLfloat rectShape5[4][3] = {	//뚜껑면
	{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1}
}; //--- 사각형 위치 값
GLfloat rectShape6[4][3] = {	//앞면
	{-1,-1,0},{0,-1,0},{0,-1,1},{-1,-1,1}
}; //--- 사각형 위치 값
GLfloat rectShape7[4][3] = {	//앞면
	{0,-1,0},{1,-1,0},{1,-1,1},{0,-1,1}
}; //--- 사각형 위치 값

GLfloat lineShape[10][2][3] = {};	//--- 선분 위치 값

GLfloat dotShape[10][3] = {};	//--- 점 위치 값

GLfloat rectColors[4][3] = { //--- 삼각형 꼭지점 색상
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0 }
};
GLfloat rectColors2[4][3] = { //--- 삼각형 꼭지점 색상
	{ 1.0, 0.0, 0.0 },
	{ 1.0, 0.0, 0.0 },
	{ 1.0, 0.0, 0.0 },
	{ 1.0, 0.0, 0.0 }
};
GLfloat rectColors3[4][3] = { //--- 삼각형 꼭지점 색상
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 1.0, 0.0 }
};
GLfloat rectColors4[4][3] = { //--- 삼각형 꼭지점 색상
    { 0.0, 0.0, 1.0 },
	{ 0.0, 0.0, 1.0 },
	{ 0.0, 0.0, 1.0 },
	{ 0.0, 0.0, 1.0 }
};
GLfloat rectColors5[4][3] = { //--- 삼각형 꼭지점 색상
	{ 1.0, 1.0, 0.0 },
	{ 1.0, 1.0, 0.0 },
	{ 1.0, 1.0, 0.0 },
	{ 1.0, 1.0, 0.0 }
};
GLfloat rectColors6[4][3] = { //--- 삼각형 꼭지점 색상
	{ 0.0, 1.0, 1.0 },
	{ 0.0, 1.0, 1.0 },
	{ 0.0, 1.0, 1.0 },
	{ 0.0, 1.0, 1.0 }
};

glm::vec3 colors[12][3] = {};
glm::vec3 colors2[12][3] = {};
glm::vec3 colors3[12][3] = {};
glm::vec3 colors4[12][3] = {};
glm::vec3 colors5[12][3] = {};
glm::vec3 colors6[12][3] = {};

GLfloat XYZShape[3][2][3] = {
	{{-1.0,0.0,0.0},{1.0,0.0,0.0}},
	{{0.0,-1.0,0.0},{0.0,1.0,0.0}},
	{{0.0,0.0,-1.0},{0.0,0.0,1.0}} };

GLfloat XYZcolors[6][3] = { //--- 축 색상
	{ 1.0, 0.0, 0.0 },	   	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },	   	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },	   	{ 0.0, 0.0, 1.0 }
};

glm::vec3 cameraPos = glm::vec3(0.0f, -3.0f, 0.5f); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.5f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f); //--- 카메라 위쪽 방향

glm::vec3 D_P = glm::vec3(0.0f, 0.0f, 0.0f);

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

glm::mat4 tmodel = glm::mat4(1.0f);

GLuint vao, vbo[2];
GLuint TriPosVbo, TriColorVbo;

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

float Rfront = 0;
bool frontSelection = false;

float w, a, s, d;
float speed = 0.04;
int wasdDirection = 3;

float J = 0;
bool Jcount = true;
bool JSelection = false;
bool doingJ = false;

float wave = 0;
bool waveCount = true;

float B = 0;
int BSelection = 0;

float M = 0;
int MSelection = 0;

float E = 0;

float F = 0;
int FSelection = 0;

float T = 0;
int TSelection = 0;

float Z = 0;

float Y = 0;
int YSelection = 0;

float R = 0;
int RSelection = 0;

float O = 0;
int OSelection = 0;

int windowWidth = 800;
int windowHeight = 600;

float openGLX, openGLY;
int movingRectangle = -1;

bool start = true;


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
	//glEnable(GL_CULL_FACE); //--- 상태 설정은 필요한 곳에서 하면 된다.
	//glDisable(GL_DEPTH_TEST | GL_CULL_FACE);	//해제
	glFrontFace(GL_CW);	//시계방향을 앞면으로

	glutTimerFunc(100, TimerFunction, 1);
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
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //--- 깊이 버퍼를 클리어한다.

	glBindVertexArray(vao);

	// 색상 바꾸기
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), XYZcolors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	int modelLocation = glGetUniformLocation(shaderProgramID, "model"); //--- 버텍스 세이더에서 모델링 변환 행렬 변수값을 받아온다.
	int viewLocation = glGetUniformLocation(shaderProgramID, "view"); //--- 버텍스 세이더에서 뷰잉 변환 행렬 변수값을 받아온다.
	int projLocation = glGetUniformLocation(shaderProgramID, "projection"); //--- 버텍스 세이더에서 투영 변환 행렬 변수값을 받아온다.

	//투영 변환
	glm::mat4 pTransform = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f); //--- 투영 공간 설정: fovy, aspect, near, far
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0)); //--- 공간을 z축 이동
	pTransform = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight, 0.1f, 200.0f);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &pTransform[0][0]);
	//카메라 위치만큼 뺀다음 다시 제자리로
	//뷰 변환
	if (YSelection == 1)
	{
		cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	if (RSelection == 1)
	{
		cameraPos = glm::vec3(0.0f, -0.5f, 0.5f);
	}

	if (YSelection == 1)
	{
		cameraDirection -= cameraPos;
		cameraDirection = glm::rotate(glm::mat4(1.0f), glm::radians(Y), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(cameraDirection, 1.0);
		cameraDirection += cameraPos;
	}

	if (RSelection == 1)
	{
		cameraPos = glm::rotate(glm::mat4(1.0f), glm::radians(R), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(cameraPos, 1.0);
	}
	if (OSelection == 1)
	{
		cameraPos = glm::rotate(glm::mat4(1.0f), glm::radians(O), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(cameraPos, 1.0);
		OSelection = 0;
	}

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	if (start)
	{
		double m = 0.0;
		double cx, cy, cz;
		float randomcolor1 = 0;
		float randomcolor2= 0.25;
		float randomcolor3= 1.0;
		float randomcolor4= 0.75;
		float randomcolor5= 0.5;
		for (int i = 0; i < 12; i++)
		{
			cx = 0.3 + m;
			cy = 0.0 + m;
			cz = 0.3 + m;

			m += 0.05;

			colors[i][0].x = cx;
			colors[i][1].x = cx;
			colors[i][2].x = cx;

			colors[i][0].y = cy;
			colors[i][1].y = cy;
			colors[i][2].y = cy;

			colors[i][0].z = cz;
			colors[i][1].z = cz;
			colors[i][2].z = cz;

			colors2[i][0].x = randomcolor1;
			colors2[i][1].x = randomcolor1;
			colors2[i][2].x = randomcolor1;
							  			 
			colors2[i][0].y = randomcolor1;
			colors2[i][1].y = randomcolor1;
			colors2[i][2].y = randomcolor1;
							 			
			colors2[i][0].z = randomcolor1;
			colors2[i][1].z = randomcolor1;
			colors2[i][2].z = randomcolor1;


			colors3[i][0].x = randomcolor2;
			colors3[i][1].x = randomcolor2;
			colors3[i][2].x = randomcolor2;
			
			colors3[i][0].y = randomcolor2;
			colors3[i][1].y = randomcolor2;
			colors3[i][2].y = randomcolor2;
				
			colors3[i][0].z = randomcolor2;
			colors3[i][1].z = randomcolor2;
			colors3[i][2].z = randomcolor2;


			colors4[i][0].x = randomcolor3;
			colors4[i][1].x = randomcolor3;
			colors4[i][2].x = randomcolor3;
				
			colors4[i][0].y = randomcolor3;
			colors4[i][1].y = randomcolor3;
			colors4[i][2].y = randomcolor3;
				  
			colors4[i][0].z = randomcolor3;
			colors4[i][1].z = randomcolor3;
			colors4[i][2].z = randomcolor3;

			colors5[i][0].x = randomcolor4;
			colors5[i][1].x = randomcolor4;
			colors5[i][2].x = randomcolor4;
			
			colors5[i][0].y = randomcolor4;
			colors5[i][1].y = randomcolor4;
			colors5[i][2].y = randomcolor4;
				
			colors5[i][0].z = randomcolor4;
			colors5[i][1].z = randomcolor4;
			colors5[i][2].z = randomcolor4;

		

			colors6[i][0].x = randomcolor5;
			colors6[i][1].x = randomcolor5;
			colors6[i][2].x = randomcolor5;
				
			colors6[i][0].y = randomcolor5;
			colors6[i][1].y = randomcolor5;
			colors6[i][2].y = randomcolor5;
				
			colors6[i][0].z = randomcolor5;
			colors6[i][1].z = randomcolor5;
			colors6[i][2].z = randomcolor5;


			Rfront = 180;	//무대 열기 타임 줄이기 위한 줄 -> 삭제하시오.
		}
		
		start = false;
	}

	model = glm::mat4(1.0f);

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

		glLineWidth(1.0);
		glDrawArrays(GL_LINES, 0, 2);
	}

	//면 그리기
	for (int i = 0; i < 4; i++)
	{
		// 색상 바꾸기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), rectColors[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// 사각형 그리기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectShape1, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_QUADS, 0, 4);
	}
	for (int i = 0; i < 4; i++)
	{
		// 색상 바꾸기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), rectColors2[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// 사각형 그리기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectShape2, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_QUADS, 0, 4);
	}
	for (int i = 0; i < 4; i++)
	{
		// 색상 바꾸기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), rectColors3[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// 사각형 그리기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectShape3, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_QUADS, 0, 4);
	}
	for (int i = 0; i < 4; i++)
	{
		// 색상 바꾸기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), rectColors4[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// 사각형 그리기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectShape4, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_QUADS, 0, 4);
	}
	for (int i = 0; i < 4; i++)
	{
		// 색상 바꾸기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), rectColors5[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// 사각형 그리기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectShape5, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_QUADS, 0, 4);
	}

	//앞면 l
	model = glm::mat4(1.0f);
	//이동
	model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 0.0f));
	//M회전
	model = glm::rotate(model, glm::radians(-Rfront), glm::vec3(0.0f, 0.0f, 1.0f));
	//이동
	model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f));
	
	for (int i = 0; i < 4; i++)
	{
		// 색상 바꾸기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), rectColors6[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		

		// 사각형 그리기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectShape6, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_QUADS, 0, 4);
	}

	//앞면 r
	model = glm::mat4(1.0f);
	//이동
	model = glm::translate(model, glm::vec3(1.0f, -1.0f, 0.0f));
	//M회전
	model = glm::rotate(model, glm::radians(Rfront), glm::vec3(0.0f, 0.0f, 1.0f));
	//이동
	model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 0.0f));

	for (int i = 0; i < 4; i++)
	{
		// 색상 바꾸기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), rectColors6[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		// 사각형 그리기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectShape7, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_QUADS, 0, 4);
	}
	
	//-------------------------------------------------
	//s r t p 코드 작성시에는 반대 방향으로.
	model = glm::mat4(1.0f);
	//M회전
	model = glm::rotate(model, glm::radians(M), glm::vec3(0.0f, 0.0f, 1.0f));
	//B이동
	model = glm::translate(model, glm::vec3(B, 0, 0));
	//점프
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, J));
	//상하좌우 이동
	model = glm::translate(model, glm::vec3(d-a, w-s, 0.0f));

	//이동
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.45f));
	//축소
	model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));

	//머리통
	for (int i = 0; i < 12; i++) {
		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		// 색상 바꾸기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &colors[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &cube1[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	//-------------------------------------------------
	//s r t p 코드 작성시에는 반대 방향으로.
	model = glm::mat4(1.0f);
	//M회전
	model = glm::rotate(model, glm::radians(M), glm::vec3(0.0f, 0.0f, 1.0f));

	//B이동
	model = glm::translate(model, glm::vec3(B, 0, 0));
	//점프
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, J));
	//상하좌우 이동
	model = glm::translate(model, glm::vec3(d - a, w - s, 0.0f));
	//이동
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.31f));
	//축소
	model = glm::scale(model, glm::vec3(0.12, 0.12, 0.185));

	//몸통
	for (int i = 0; i < 12; i++) {
		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &colors[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &cube2[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	//------------------------------------------------------
	//s r t p 코드 작성시에는 반대 방향으로.
	model = glm::mat4(1.0f);
	//M회전
	model = glm::rotate(model, glm::radians(M), glm::vec3(0.0f, 0.0f, 1.0f));
	
	//B이동
	model = glm::translate(model, glm::vec3(B, 0, 0));
	//점프
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, J));
	//상하좌우 이동
	model = glm::translate(model, glm::vec3(d - a, w - s, 0.0f));
	//이동
	model = glm::translate(model, glm::vec3(0.04f, 0.0f, 0.15f));
	//반동
	model = glm::translate(model, glm::vec3(0.04f, 0.0f, 0.15f));
	model = glm::rotate(model, glm::radians(wave), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-0.04f, 0.0f, -0.15f));
	//축소
	model = glm::scale(model, glm::vec3(0.05, 0.05, 0.17));

	//왼다리
	for (int i = 0; i < 12; i++) {
		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &colors2[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &cube3[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	//--------------------------------------------------------
	//s r t p 코드 작성시에는 반대 방향으로.
	model = glm::mat4(1.0f);
	//M회전
	model = glm::rotate(model, glm::radians(M), glm::vec3(0.0f, 0.0f, 1.0f));
	
	//B이동
	model = glm::translate(model, glm::vec3(B, 0, 0));
	//점프
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, J));
	//상하좌우 이동
	model = glm::translate(model, glm::vec3(d - a, w - s, 0.0f));
	//이동
	model = glm::translate(model, glm::vec3(-0.04f, 0.0f, 0.15f));
	//반동
	model = glm::translate(model, glm::vec3(-0.04f, 0.0f, 0.15f));
	model = glm::rotate(model, glm::radians(-wave), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.04f, 0.0f, -0.15f));
	//축소
	model = glm::scale(model, glm::vec3(0.05, 0.05, 0.17));

	//오른다리
	for (int i = 0; i < 12; i++) {
		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &colors5[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &cube4[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	//------------------------------------------------------
	//s r t p 코드 작성시에는 반대 방향으로.
	model = glm::mat4(1.0f);
	//M회전
	model = glm::rotate(model, glm::radians(M), glm::vec3(0.0f, 0.0f, 1.0f));
	
	//B이동
	model = glm::translate(model, glm::vec3(B, 0, 0));
	//점프
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, J));
	switch (wasdDirection)
	{
	case 1:
		model = glm::translate(model, glm::vec3(-0.15f, 0.0f, 0.0f));
		break;
	case 2:
		model = glm::translate(model, glm::vec3(-0.075f, 0.075f, 0.0f));
		break;
	case 3:
		break;
	case 4:
		model = glm::translate(model, glm::vec3(-0.075f, -0.075f, 0.0f));
		break;
	}
	//model = glm::translate(model, glm::vec3(-0.075f, 0.0f, 0.32f));
	//상하좌우 이동
	model = glm::translate(model, glm::vec3(d - a, w - s, 0.0f));
	//이동
	model = glm::translate(model, glm::vec3(0.075f, 0.0f, 0.32f));
	//반동
	if (wasdDirection == 1 || wasdDirection == 3)
	{
		model = glm::translate(model, glm::vec3(0.075f, 0.0f, 0.32f));
		model = glm::rotate(model, glm::radians(wave), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.075f, 0.0f, -0.32f));
	}
	if (wasdDirection == 2 || wasdDirection == 4)
	{
		model = glm::translate(model, glm::vec3(0.075f, 0.0f, 0.32f));
		model = glm::rotate(model, glm::radians(wave), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.075f, 0.0f, -0.32f));
	}
	//축소
	model = glm::scale(model, glm::vec3(0.05, 0.05, 0.15));

	//왼팔
	for (int i = 0; i < 12; i++) {
		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &colors3[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &cube5[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	//--------------------------------------------------------
	//s r t p 코드 작성시에는 반대 방향으로.
	model = glm::mat4(1.0f);
	//M회전
	model = glm::rotate(model, glm::radians(M), glm::vec3(0.0f, 0.0f, 1.0f));
	//B이동
	model = glm::translate(model, glm::vec3(B, 0, 0));
	//점프
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, J));
	switch (wasdDirection)
	{
	case 1:
		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		break;
	case 2:
		model = glm::translate(model, glm::vec3(0.075f, 0.075f, 0.0f));
		break;
	case 3:
		break;
	case 4:
		model = glm::translate(model, glm::vec3(0.075f, -0.075f, 0.0f));
		break;
	}
	//상하좌우 이동
	model = glm::translate(model, glm::vec3(d - a, w - s, 0.0f));
	//이동
	model = glm::translate(model, glm::vec3(-0.075f, 0.0f, 0.32f));
	//반동
	if (wasdDirection == 1 || wasdDirection == 3)
	{
		model = glm::translate(model, glm::vec3(-0.075f, 0.0f, 0.32f));
		model = glm::rotate(model, glm::radians(-wave), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.075f, 0.0f, -0.32f));
	}
	if (wasdDirection == 2 || wasdDirection == 4)
	{
		model = glm::translate(model, glm::vec3(-0.075f, 0.0f, 0.32f));
		model = glm::rotate(model, glm::radians(-wave), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.075f, 0.0f, -0.32f));
	}
	//축소
	model = glm::scale(model, glm::vec3(0.05, 0.05, 0.15));

	//오른팔
	for (int i = 0; i < 12; i++) {
		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &colors6[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &cube6[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	//--------------------------------------------------------
	//s r t p 코드 작성시에는 반대 방향으로.
	model = glm::mat4(1.0f);
	//M회전
	model = glm::rotate(model, glm::radians(M), glm::vec3(0.0f, 0.0f, 1.0f));
	//B이동
	model = glm::translate(model, glm::vec3(B, 0, 0));
	//점프
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, J));
	switch (wasdDirection)
	{
	case 1:
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.0f));
		break;
	case 2:
		model = glm::translate(model, glm::vec3(-0.025f, 0.025f, 0.0));
		break;
	case 3:
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		break;
	case 4:
		model = glm::translate(model, glm::vec3(0.025f, 0.025f, 0.0));
		break;
	}
	//상하좌우 이동
	model = glm::translate(model, glm::vec3(d - a, w - s, 0.0f));
	//이동
	model = glm::translate(model, glm::vec3(0.0f, -0.025f, 0.45f));
	//축소
	model = glm::scale(model, glm::vec3(0.075, 0.075, 0.075));

	//얼굴
	for (int i = 0; i < 12; i++) {
		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &colors4[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &cube7[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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
	vertexSource = filetobuf("vertex2.glsl");
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

bool cdx1 = true;
bool bcnt = true, Bcnt = true, mcnt = true, Mcnt = true, Ecnt = true, fcnt = true, Fcnt = true;
bool tcnt = true, Tcnt = true, rcnt = true, ycnt = true;

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		frontSelection = true;
		break;
	case '2':
		
		cameraDirection.y += 0.1;
		break;
	case '3':
		
		cameraDirection.x -= 0.1;
		break;
	case '4':
		
		cameraDirection.y -= 0.1;
		break;
	case '5':
		
		cameraDirection.x += 0.1;
		break;
	case '6':
		cameraPos.z += 0.1;
		break;
	case '7':
		cameraPos.z -= 0.1;
		break;
	case '-':
		speed -= 0.005;
		if (speed < 0.005)
		{
			speed = 0.005;
		}
		break;
	case '=':
		speed += 0.005;
		if (speed > 0.1)
		{
			speed = 0.1;
		}
		break;
	case 'w':
		//d-a, w-s
		w += speed;
		wasdDirection = 1;
		break;
	case 'a':
		a += speed;
		wasdDirection = 2;
		break;
	case 's':
		s += speed;
		wasdDirection = 3;
		break;
	case 'd':
		d += speed;
		wasdDirection = 4;
		break;
	case 'j':
		if (!doingJ)
		{
			JSelection = true;
			doingJ = true;
		}
		break;
	case 'b':
		if (Bcnt)
		{
			BSelection = 1;
			Bcnt = false;
		}
		else
		{
			BSelection = 0;
			Bcnt = true;
		}
		break;
	case 'B':
		if (Bcnt)
		{
			BSelection = 2;
			Bcnt = false;
		}
		else
		{
			BSelection = 0;
			Bcnt = true;
		}
		break;
	case 'm':
		if (mcnt)
		{
			MSelection = 1;
			mcnt = false;
		}
		else
		{
			MSelection = 0;
			mcnt = true;
		}
		break;
	case 'M':
		if (Mcnt)
		{
			MSelection = 2;
			Mcnt = false;
		}
		else
		{
			MSelection = 0;
			Mcnt = true;
		}
		break;
	case 'e':
		if (Ecnt)
		{
			E += 0.01;
			if (E > 0.05)
			{
				Ecnt = false;
			}
		}
		else
		{
			E -= 0.01;
			if (E < -0.05)
			{
				Ecnt = true;
			}
		}
		break;
	case 'E':
		break;
	case 'f':
		if (fcnt)
		{
			FSelection = 1;
			fcnt = false;
		}
		else
		{
			FSelection = 0;
			fcnt = true;
		}
		break;
	case 'F':
		if (Fcnt)
		{
			FSelection = 2;
			Fcnt = false;
		}
		else
		{
			FSelection = 0;
			Fcnt = true;
		}
		break;
	case 't':
		if (tcnt)
		{
			TSelection = 1;
			tcnt = false;
		}
		else
		{
			TSelection = 0;
			tcnt = true;
		}
		break;
	case 'T':
		if (Tcnt)
		{
			TSelection = 2;
			Tcnt = false;
		}
		else
		{
			TSelection = 0;
			Tcnt = true;
		}
		break;
	case 'z':
		Z += 1;
		break;
	case 'Z':
		Z -= 1;
		break;
	case 'y':
		if (ycnt)
		{
			YSelection = 1;
			ycnt = false;
		}
		else
		{
			YSelection = 0;
			ycnt = true;
		}
		break;
	case 'o':
		cameraPos = glm::vec3(0.0f, -0.5f, 0.5f);
		OSelection = 1;
		O += 1;
		break;
	case 'r':
		cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		if (rcnt)
		{
			RSelection = 1;
			rcnt = false;
		}
		else
		{
			RSelection = 0;
			rcnt = true;
		}
		break;
	case 'x':
		BSelection = 0;
		MSelection = 0;
		FSelection = 0;
		TSelection = 0;
		RSelection = 0;
		YSelection = 0;
		break;
	case 'c':
		cameraPos = glm::vec3(0.0f, -0.5f, 0.5f); //--- 카메라 위치
		cameraDirection = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 바라보는 방향
		cameraUp = glm::vec3(0.0f, 0.0f, 1.0f); //--- 카메라 위쪽 방향
		B = 0;
		BSelection = 0;
		M = 0;
		MSelection = 0;
		E = 0;
		F = 0;
		FSelection = 0;
		T = 0;
		TSelection = 0;
		RSelection = 0;
		YSelection = 0;
		cdx1 = true;
		bcnt = true, Bcnt = true, mcnt = true, Mcnt = true, Ecnt = true, fcnt = true, Fcnt = true;
		tcnt = true, Tcnt = true, rcnt = true, ycnt = true;
		R = 0;
		Y = 0;
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
		cameraPos.y += 0.1;
		cameraDirection.y += 0.1;
		break;
	case GLUT_KEY_DOWN:
		cameraPos.y -= 0.1;
		cameraDirection.y -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		cameraPos.x -= 0.1;
		cameraDirection.x -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		cameraPos.x += 0.1;
		cameraDirection.x += 0.1;
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
		if (BSelection == 1)
		{
			B -= 0.05;
		}
		if (BSelection == 2)
		{
			B += 0.05;
		}
		if (MSelection == 1)
		{
			M -= 2;
		}
		if (MSelection == 2)
		{
			M += 2;
		}
		if (FSelection == 1)
		{
			F += 2;
			if (F > 90)
			{
				FSelection = 2;
			}
		}
		if (FSelection == 2)
		{
			F -= 2;
			if (F < 0)
			{
				FSelection = 1;
			}
		}

		if (TSelection == 1)
		{
			T += 8;
			if (T > 90)
			{
				TSelection = 2;
			}
		}
		if (TSelection == 2)
		{
			T -= 8;
			if (T < -90)
			{
				TSelection = 1;
			}
		}
		if (RSelection == 1)
		{
			R += 3;
		}
		if (YSelection == 1)
		{
			Y += 5;
		}
		if (frontSelection)
		{
			Rfront += 2;
			if (Rfront > 180)
			{
				frontSelection = false;
			}
		}
		//점프
		if (JSelection)
		{
			if (Jcount)
			{
				J += 0.05;
				if (J > 0.35)
				{
					Jcount = false;
				}
			}
			else if (!Jcount)
			{
				J -= 0.05;
				if (J < 0.0)
				{
					Jcount = true;
					JSelection = false;
					doingJ = false;
				}
			}
			
		}
		//흔들기
		if (waveCount)
		{
			wave += 2;
			if (wave > 15)
			{
				waveCount = false;
			}
		}
		else if (!waveCount)
		{
			wave -= 2;
			if (wave < -15)
			{
				waveCount = true;
			}
		}


		break;
	}
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}