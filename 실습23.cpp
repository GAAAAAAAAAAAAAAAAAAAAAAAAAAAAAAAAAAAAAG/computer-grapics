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
//GLfloat rectShape5[4][3] = {	//뚜껑면
//	{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1}
//}; //--- 사각형 위치 값
GLfloat rectShape6[4][3] = {	//앞면
	{-1,-1,0},{1,-1,0},{1,-1,1},{-1,-1,1}
}; //--- 사각형 위치 값

GLfloat rectColors[4][3] = { //--- 삼각형 꼭지점 색상
	{0.2,0.2,0.2 },
	{0.2,0.2,0.2 },
	{0.2,0.2,0.2 },
	{0.2,0.2,0.2 }
};
GLfloat rectColors2[4][3] = { //--- 삼각형 꼭지점 색상
	{ 0.3,0.3,0.3 },
	{ 0.3,0.3,0.3 },
	{ 0.3,0.3,0.3 },
	{ 0.3,0.3,0.3 }
};
GLfloat rectColors3[4][3] = { //--- 삼각형 꼭지점 색상
	{0.4,0.4,0.4 },
	{0.4,0.4,0.4 },
	{0.4,0.4,0.4 },
	{0.4,0.4,0.4 }
};
GLfloat rectColors4[4][3] = { //--- 삼각형 꼭지점 색상
	{ 0.5,0.5,0.5 },
	{ 0.5,0.5,0.5 },
	{ 0.5,0.5,0.5 },
	{ 0.5,0.5,0.5 }
};
GLfloat rectColors5[4][3] = { //--- 삼각형 꼭지점 색상
	{ 0.6,0.6,0.6 },
	{ 0.6,0.6,0.6 },
	{ 0.6,0.6,0.6 },
	{ 0.6,0.6,0.6 }
};
GLfloat rectColors6[4][3] = { //--- 삼각형 꼭지점 색상
	{ 0.7,0.7,0.7 },
	{ 0.7,0.7,0.7 },
	{ 0.7,0.7,0.7 },
	{ 0.7,0.7,0.7 }
};

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



GLfloat triShape[10][3][3] = {};//--- 삼각형 위치 값

GLfloat rectShape[10][4][3] = {}; //--- 사각형 위치 값

GLfloat lineShape[10][2][3] = {};	//--- 선분 위치 값

GLfloat dotShape[10][3] = {};	//--- 점 위치 값

//GLfloat colors[4][3] = { //--- 삼각형 꼭지점 색상
//	{ 1.0, 0.0, 0.0 },
//	{ 0.0, 1.0, 0.0 },
//	{ 0.0, 0.0, 1.0 },
//	{ 1.0, 1.0, 1.0 }
//};

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

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.0f); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

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

// 변수 및 상태
float angleX = 0.0f;
float angleY = 0.0f;

int ZSelection = 0;
int ZSelectionCnt = 0;
float Zcnt = 0.0;

float gravity = 0.0;
float movingX = 0.0;
float movingX2 = 0.0;
float movingX3 = 0.0;
float movingX4 = 0.0;
float movingX5 = 0.0;
float movingY = 0.0;
float movingY2 = 0.0;
float movingY3 = 0.0;
float movingY4 = 0.0;
float movingY5 = 0.0;

float moving9X[5];
float moving9Y[5];
int create9 = 0;
int direction9[5]{};

bool bottomDelete = false;
float bottomDown = 0.0;

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

	glutTimerFunc(10, TimerFunction, 1);
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

	int modelLocation = glGetUniformLocation(shaderProgramID, "model"); //--- 버텍스 세이더에서 모델링 변환 행렬 변수값을 받아온다.
	int viewLocation = glGetUniformLocation(shaderProgramID, "view"); //--- 버텍스 세이더에서 뷰잉 변환 행렬 변수값을 받아온다.
	int projLocation = glGetUniformLocation(shaderProgramID, "projection"); //--- 버텍스 세이더에서 투영 변환 행렬 변수값을 받아온다.

	//투영 변환
	glm::mat4 pTransform = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f); //--- 투영 공간 설정: fovy, aspect, near, far
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0)); //--- 공간을 z축 이동
	pTransform = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight, 0.1f, 200.0f);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &pTransform[0][0]);

	//뷰 변환
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

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
		printf("zZ : Z축기준으로 돌기\n");
		printf("aA : Z축으로 위아래 움직이기\n");
		start = false;
	}


	//축 그리기
	model = glm::mat4(1.0f);
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
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-angleX), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(Zcnt), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 0.0f, 1.0f));
	
	for (int i = 0; i < 4; i++)
	{
		// 색상 바꾸기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectColors[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

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
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectColors2[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

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
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectColors3[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

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
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectColors4[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		// 사각형 그리기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectShape4, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_QUADS, 0, 4);
	}
	if (!bottomDelete)
	{
		for (int i = 0; i < 4; i++)
		{
			// 색상 바꾸기
			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectColors6[i], GL_STATIC_DRAW);
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
	}

	//상자 1--------------------------------------------------------------------------------------
	//s r t p 코드 작성시에는 반대 방향으로.
	model = glm::mat4(1.0f);
	//이동
	model = glm::translate(model, glm::vec3(0.0f, -0.82f, 0.3f));

	//이동
	//model = glm::translate(model, glm::vec3(0.0f, -angleX / 100, 0.0f));
	//바닥열리기
	if (bottomDelete)
	{
		model = glm::translate(model, glm::vec3(0.0f, bottomDown, 0.0f));
	}
	//중력
	model = glm::translate(model, glm::vec3(0.0f, gravity, 0.0f));
	
	//회전
	model = glm::rotate(model, glm::radians(-angleX), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(Zcnt), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::translate(model, glm::vec3(movingX, movingY, 0.0f));

	//축소
	model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));

	//정육면체
	for (int i = 0; i < 12; i++) {
		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

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
	//상자2--------------------------------------------------------------------------------------
	//s r t p 코드 작성시에는 반대 방향으로.
	model = glm::mat4(1.0f);
	//이동
	model = glm::translate(model, glm::vec3(0.0f, -0.87f, 0.6f));
	//바닥열리기
	if (bottomDelete)
	{
		model = glm::translate(model, glm::vec3(0.0f, bottomDown, 0.0f));
	}
	
	//회전
	model = glm::rotate(model, glm::radians(-angleX), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(Zcnt), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(movingX2, movingY2, 0.0f));
	//축소
	model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));

	//정육면체
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
	//상자 3--------------------------------------------------------------------------------------
	//s r t p 코드 작성시에는 반대 방향으로.
	model = glm::mat4(1.0f);
	//이동
	model = glm::translate(model, glm::vec3(0.0f, -0.95f, 0.75f));
	//바닥열리기
	if (bottomDelete)
	{
		model = glm::translate(model, glm::vec3(0.0f, bottomDown, 0.0f));
	}
	
	//회전
	model = glm::rotate(model, glm::radians(-angleX), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(Zcnt), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::translate(model, glm::vec3(movingX3, movingY3, 0.0f));
	//축소
	model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));

	//정육면체
	for (int i = 0; i < 12; i++) {
		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &colors[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &cube3[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	//--------------------------------------------------------------------------------------------
	//구1
	if (create9 > 0)
	{
		model = glm::mat4(1.0f);
		//바닥열리기
		if (bottomDelete)
		{
			model = glm::translate(model, glm::vec3(0.0f, bottomDown, 0.0f));
		}
		model = glm::translate(model, glm::vec3(moving9X[0], moving9Y[0], 0.2f));
		model = glm::rotate(model, glm::radians(-angleX), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(Zcnt), glm::vec3(0.0f, 0.0f, 1.0f));
		//축소
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		qobj = gluNewQuadric(); // 객체 생성하기
		gluQuadricDrawStyle(qobj, GLU_FILL); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH); // 생략 가능
		gluQuadricOrientation(qobj, GLU_OUTSIDE); // 생략 가능
		gluSphere(qobj, 1.0, 20, 20); // 객체 만들기
	}
	if (create9 > 1)
	{
		//구2
		model = glm::mat4(1.0f);
		//바닥열리기
		if (bottomDelete)
		{
			model = glm::translate(model, glm::vec3(0.0f, bottomDown, 0.0f));
		}
		model = glm::translate(model, glm::vec3(moving9X[1], moving9Y[1], 0.2f));
		model = glm::rotate(model, glm::radians(-angleX), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(Zcnt), glm::vec3(0.0f, 0.0f, 1.0f));
		//축소
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		qobj = gluNewQuadric(); // 객체 생성하기
		gluQuadricDrawStyle(qobj, GLU_FILL); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH); // 생략 가능
		gluQuadricOrientation(qobj, GLU_OUTSIDE); // 생략 가능
		gluSphere(qobj, 1.0, 20, 20); // 객체 만들기
	}
	if (create9 > 2)
	{
		//구3
		//바닥열리기
		model = glm::mat4(1.0f);
		if (bottomDelete)
		{
			model = glm::translate(model, glm::vec3(0.0f, bottomDown, 0.0f));
		}
		model = glm::translate(model, glm::vec3(moving9X[2], moving9Y[2], 0.2f));
		model = glm::rotate(model, glm::radians(-angleX), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(Zcnt), glm::vec3(0.0f, 0.0f, 1.0f));
		//축소
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		qobj = gluNewQuadric(); // 객체 생성하기
		gluQuadricDrawStyle(qobj, GLU_FILL); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH); // 생략 가능
		gluQuadricOrientation(qobj, GLU_OUTSIDE); // 생략 가능
		gluSphere(qobj, 1.0, 20, 20); // 객체 만들기
	}
	
	if (create9 > 3)
	{
		//구4
		model = glm::mat4(1.0f);
		//바닥열리기
		if (bottomDelete)
		{
			model = glm::translate(model, glm::vec3(0.0f, bottomDown, 0.0f));
		}
		model = glm::translate(model, glm::vec3(moving9X[3], moving9Y[3], 0.2f));
		model = glm::rotate(model, glm::radians(-angleX), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(Zcnt), glm::vec3(0.0f, 0.0f, 1.0f));
		//축소
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		qobj = gluNewQuadric(); // 객체 생성하기
		gluQuadricDrawStyle(qobj, GLU_FILL); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH); // 생략 가능
		gluQuadricOrientation(qobj, GLU_OUTSIDE); // 생략 가능
		gluSphere(qobj, 1.0, 20, 20); // 객체 만들기
	}
	
	if (create9 > 4)
	{
		//구5
		model = glm::mat4(1.0f);
		//바닥열리기
		if (bottomDelete)
		{
			model = glm::translate(model, glm::vec3(0.0f, bottomDown, 0.0f));
		}
		model = glm::translate(model, glm::vec3(moving9X[4], moving9Y[4], 0.2f));
		model = glm::rotate(model, glm::radians(-angleX), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(Zcnt), glm::vec3(0.0f, 0.0f, 1.0f));
		//축소
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		qobj = gluNewQuadric(); // 객체 생성하기
		gluQuadricDrawStyle(qobj, GLU_FILL); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH); // 생략 가능
		gluQuadricOrientation(qobj, GLU_OUTSIDE); // 생략 가능
		gluSphere(qobj, 1.0, 20, 20); // 객체 만들기
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

int ASelection = 0;
int ASelectionCnt = 0;

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		cameraDirection.x += 0.1;
		break;
	case '2':
		cameraDirection.x -= 0.1;
		break;
	case '3':
		cameraDirection.y += 0.1;
		break;
	case '4':
		cameraDirection.y -= 0.1;
		break;
	case '5':
		cameraDirection.z += 0.1;
		break;
	case '6':
		cameraDirection.z -= 0.1;
		break;
	case '-':
		cameraPos.z += 0.1;
		break;
	case '=':
		cameraPos.z -= 0.1;
		break;
	case 'z':
		if (ZSelectionCnt % 2 == 0)
		{
			ZSelection = 1;
			ZSelectionCnt++;
		}
		else
		{
			ZSelection = 0;
			ZSelectionCnt++;
		}
		break;
	case 'Z':
		if (ZSelectionCnt % 2 == 0)
		{
			ZSelection = 2;
			ZSelectionCnt++;
		}
		else
		{
			ZSelection = 0;
			ZSelectionCnt++;
		}
		break;
	case 'a':
		if (ASelectionCnt % 2 == 0)
		{
			ASelection = 1;
			ASelectionCnt++;
		}
		else
		{
			ASelection = 0;
			ASelectionCnt++;
		}
		break;
	case 'A':
		if (ASelectionCnt % 2 == 0)
		{
			ASelection = 2;
			ASelectionCnt++;
		}
		else
		{
			ASelection = 0;
			ASelectionCnt++;
		}
		break;
	case 'b':
		if (create9 < 5)
		{
			create9++;
		}
		break;
	case 't':
		bottomDelete = true;
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
	float sensitivity = 100.0f;
	if (movingMouse >= 0)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);

		float deltaX = openGLX - beforeX;
		float deltaY = openGLY - beforeY;

		//이동
		angleX += deltaX * sensitivity;
		angleY += deltaY * sensitivity;

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
		if (angleX < -10 && angleX > -90)
		{
			if (movingX > -0.6)
			{
				movingX -= 0.005;
			}
			if (movingX2 > -0.645)
			{
				movingX2 -= 0.005;
			}
			if (movingX3 > -0.675)
			{
				movingX3 -= 0.005;
			}
		}
		if (angleX > 10 && angleX < 90)
		{
			if (movingX < 0.6)
			{
				movingX += 0.005;
			}
			if (movingX2 < 0.645)
			{
				movingX2 += 0.005;
			}
			if (movingX3 < 0.675)
			{
				movingX3 += 0.005;
			}
		}

		//구 이동
		for (int i = 0; i < create9; i++)
		{
			if (direction9[i] == 0)
			{
				
				if (!bottomDelete)
				{
					moving9X[i] += 0.01;
					moving9Y[i] += 0.01;
				}
				if (moving9Y[i] > 1 || moving9Y[i] < -1 || moving9X[i]>1 || moving9X[i]<-1)
				{
					direction9[i] = 1;
				}
			}
			if (direction9[i] == 1)
			{
				if (!bottomDelete)
				{
					moving9X[i] -= 0.01;
					moving9Y[i] -= 0.01;
				}
				if (moving9Y[i] > 1 || moving9Y[i] < -1 || moving9X[i]>1 || moving9X[i] < -1)
				{
					direction9[i] = 0;
				}
			}
		}

		//바닥 열리기
		if (bottomDelete)
		{
			bottomDown -= 0.01;
		}

		//z축 회전
		if (ZSelection == 1)
		{
			Zcnt += 1;
		}
		else if (ZSelection == 2)
		{
			Zcnt -= 1;
		}
		//z축 앞 뒤로
		if (ASelection == 1)
		{
			cameraPos.z += 0.05;
		}
		if (ASelection == 2)
		{
			cameraPos.z -= 0.05;
		}
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}