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
#include <random>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis6(1, 6);
uniform_int_distribution<int> dis4(1, 4);
uniform_real_distribution<double> color(0, 1);

// cube[2][0].x

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

glm::vec3 colors[12][3] = {};

GLfloat XYShape[3][2][3] = {
	{{-1.0,0.0,0.0},{1.0,0.0,0.0}},
	{{0.0,1.0,0.0},{0.0,-1.0,0.0}},
	{{0.0,0.0,-1.0},{0.0,0.0,1.0}} };

GLfloat XYcolors[4][3] = { //--- 축 색상
	{ 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0 }
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

bool c = true;
bool p = false;

bool start = true;
bool depth_test = true;
bool Wselect = false;

float xRotation = 0;
float yRotation = 0;
int xRotateSelect = 0;
int yRotateSelect = 0;

float xTransform = 0;
float yTransform = 0;
int xTranslateSelect = -1;
int yTranslateSelect = -1;

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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	//--- 깊이 검사 모드도 같이 설정
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Example1");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	//--- 세이더 읽어와서 세이더 프로그램 만들기
	make_shaderProgram(); //--- 세이더 프로그램 만들기
	InitBuffer();
	//glEnable(GL_DEPTH_TEST);
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

glm::mat4 model = glm::mat4(1.0f);

GLvoid drawScene()
{
	GLUquadricObj* qobj;

	glUseProgram(shaderProgramID);
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //--- 깊이 버퍼를 클리어한다.

	glBindVertexArray(vao);

	// 색상 바꾸기
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), XYcolors, GL_STATIC_DRAW);
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
		start = false;
	}

	//축 그리기
	for (int i = 0; i < 3; i++)
	{
		model = glm::mat4(1.0f);

		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), XYShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glLineWidth(1.0);
		glDrawArrays(GL_LINES, 0, 2);
	}

	if (c)
	{
		//이동
		if (xTranslateSelect > 0)
		{
			model = glm::translate(model, glm::vec3(xTransform, 0.0f, 0.0f));
		}
		if (yTranslateSelect > 0)
		{
			model = glm::translate(model, glm::vec3(0.0f, yTransform, 0.0f));
		}
		//자전 애니메이션
		if (xRotateSelect > 0)
		{
			model = glm::rotate(model, glm::radians(xRotation), glm::vec3(1.0f, 0.0f, 0.0f));

		}
		if (yRotateSelect > 0)
		{
			model = glm::rotate(model, glm::radians(yRotation), glm::vec3(0.0f, 1.0f, 0.0f));

		}

		// x축으로 10도 회전
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// y축으로 10도 회전
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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

			if (Wselect)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	}

	if (p)
	{
		//이동
		if (xTranslateSelect > 0)
		{
			model = glm::translate(model, glm::vec3(xTransform, 0.0f, 0.0f));
		}
		if (yTranslateSelect > 0)
		{
			model = glm::translate(model, glm::vec3(0.0f, yTransform, 0.0f));
		}
		//자전 애니메이션
		if (xRotateSelect > 0)
		{
			model = glm::rotate(model, glm::radians(xRotation), glm::vec3(1.0f, 0.0f, 0.0f));

		}
		if (yRotateSelect > 0)
		{
			model = glm::rotate(model, glm::radians(yRotation), glm::vec3(0.0f, 1.0f, 0.0f));

		}
		// x축으로 10도 회전
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// y축으로 10도 회전
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//사각뿔
		for (int i = 0; i < 6; i++) {
			// modelTransform 변수에 변환 값 적용하기
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &colors[i][0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &pyramid[i][0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			if (Wselect)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	}

	glutSwapBuffers();
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

bool a = false;
bool b = false;

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		c = true;
		p = false;

		model = glm::mat4(1.0f);
		xRotateSelect = 0;
		yRotateSelect = 0;
		xRotation = 0;
		yRotation = 0;
		xTranslateSelect = -1;
		yTranslateSelect = -1;
		xTransform = 0.0;
		yTransform = 0.0;
		break;
	case 'p':
		c = false;
		p = true;

		model = glm::mat4(1.0f);
		xRotateSelect = 0;
		yRotateSelect = 0;
		xRotation = 0;
		yRotation = 0;
		xTranslateSelect = -1;
		yTranslateSelect = -1;
		xTransform = 0.0;
		yTransform = 0.0;
		break;
	case 'h':
		depth_test = !depth_test;
		if (depth_test)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
		break;
	case 'w':
		Wselect = !Wselect;
		break;
	case 'x':
		a = !a;
		if (a)
		{
			xRotateSelect = 1;
		}
		else
		{
			xRotateSelect = 2;
		}
		break;
	case 'y':
		b = !b;
		if (b)
		{
			yRotateSelect = 1;
		}
		else
		{
			yRotateSelect = 2;
		}
		break;
	case 's':
		model = glm::mat4(1.0f);
		xRotateSelect = 0;
		yRotateSelect = 0;
		xRotation = 0;
		yRotation = 0;
		xTranslateSelect = -1;
		yTranslateSelect = -1;
		xTransform = 0.0;
		yTransform = 0.0;
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
		yTranslateSelect = 1;
		yTransform += 0.05;
		break;
	case GLUT_KEY_DOWN:
		yTranslateSelect = 2;
		yTransform -= 0.05;
		break;
	case GLUT_KEY_LEFT:
		xTranslateSelect = 1;
		xTransform -= 0.05;
		break;
	case GLUT_KEY_RIGHT:
		xTranslateSelect = 2;
		xTransform += 0.05;
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
		if (xRotateSelect == 2)
		{
			xRotation += 1;
		}
		if (yRotateSelect == 2)
		{
			yRotation += 1;
		}
		if (xRotateSelect == 1)
		{
			xRotation -= 1;
		}
		if (yRotateSelect == 1)
		{
			yRotation -= 1;
		}
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}
