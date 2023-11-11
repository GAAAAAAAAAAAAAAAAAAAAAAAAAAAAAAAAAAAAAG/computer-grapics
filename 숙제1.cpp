#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
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

uniform_real_distribution<double> Tdis(-1.0, 1.0);
uniform_real_distribution<double> colorDis(0.0, 1.0);
uniform_int_distribution<int> polygonrandom(0, 2);
uniform_int_distribution<int> directionrandom(0, 1);
uniform_real_distribution<double> randomY(-0.5, 0.5);
uniform_real_distribution<double> randomM(0.25, 0.70);
uniform_real_distribution<double> randomSpeed(0.007, 0.02);

struct TRISHAPE
{
	GLfloat triShape[3][3];
	GLfloat cX, cY;
	bool alive = false;
	double speed = 0.005;
	double m = 0.03;
	int direction = 0; 
	GLfloat C;
	glm::vec3 colors[3] = {};
};

TRISHAPE t[100];
int Tcnt = 0;

struct RECTSHAPE
{
	GLfloat rectShape[4][3];
	GLfloat cX, cY;
	bool alive = false;
	double speed = 0.005;
	double m = 0.03;
	int direction = 0;
	GLfloat C;
	glm::vec3 colors[4] = {};
};
RECTSHAPE r[100];
int Rcnt = 0;

struct PENTASHAPE
{
	GLfloat pentaShape[5][3];
	GLfloat cX, cY;
	bool alive = false;
	double speed = 0.005;
	double m = 0.03;
	int direction = 0;
	GLfloat C;
	glm::vec3 colors[5] = {};
};
PENTASHAPE p[100];
int Pcnt = 0;

struct NEWTRISHAPE
{
	GLfloat triShape[3][3];
	GLfloat cX, cY;
	bool alive = false;
	double speed = 0.005;
	double m = 0.03;
	int direction = 0;
	bool animation = true;
	GLfloat C;
	int timeout = 1;
	glm::vec3 colors[3] = {};
};
NEWTRISHAPE nt[10000];
int NTcnt = 0;


GLfloat TdotShape[100][1000][3] = {};
GLfloat RdotShape[100][1000][3] = {};
GLfloat PdotShape[100][1000][3] = {};

GLfloat dotShape[3] = {};

int TdotCnt[100]{};
int RdotCnt[100]{};
int PdotCnt[100]{};

//direction : (1,왼->오) (2,오->왼)

//박스
GLfloat box[4][3];
bool boxDirection = true;

GLfloat lineShape[2][3] = {};	//--- 선분 위치 값

GLfloat colors[5][3] = { //--- 삼각형 꼭지점 색상
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 0.0, 0.0, 0.0 }
};
GLfloat dotColors[3] = { 0.7, 0.7, 1.0 };
glm::vec3 pathColors {};
glm::vec3 boxColors[4];

GLuint vao, vbo[2];
GLuint TriPosVbo, TriColorVbo;

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

glm::mat4 model = glm::mat4(1.0f);

int windowWidth = 800;
int windowHeight = 600;

float openGLX, openGLY;
int movingRectangle = -1;
int movingMouse = -1;
float beforeX, beforeY;

bool start = true;
double Size = 0.1;
int createPolygon = 0;
int createPolygonDirection = 0;
double Allspeed = 0.0;
int modelLocation;
double centerX, centerY;
int cnt = 0;
bool path = false;

float polygonRotate = 0.0;

bool LineFillMode = true;	//t : l, f : f

struct Point {
	GLfloat x, y;
};

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
int checkCollision(GLfloat a1, GLfloat b1, GLfloat a2, GLfloat b2);
void NewTriangleCreate(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3);

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
	glutTimerFunc(1000, TimerFunction, 2);
	glutTimerFunc(100, TimerFunction, 3);
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
	glClearColor(0.7f, 0.7f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);
	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);

	modelLocation = glGetUniformLocation(shaderProgramID, "model"); //--- 버텍스 세이더에서 모델링 변환 행렬 변수값을 받아온다.
	
	if (start)
	{
		box[0][0] = -0.3;
		box[0][1] = -0.9;
		box[1][0] = 0.3;
		box[1][1] = -0.9;
		box[2][0] = 0.3;
		box[2][1] = -0.7;
		box[3][0] = -0.3;
		box[3][1] = -0.7;

		pathColors.x = 0.0;
		pathColors.y = 0.0;
		pathColors.z = 0.0;

		boxColors[0].x = 0.0;
		boxColors[0].y = 0.0;
		boxColors[0].z = 0.0;
		boxColors[1].x = 0.0;
		boxColors[1].y = 0.0;
		boxColors[1].z = 0.0;
		boxColors[2].x = 0.0;
		boxColors[2].y = 0.0;
		boxColors[2].z = 0.0;
		boxColors[3].x = 0.0;
		boxColors[3].y = 0.0;
		boxColors[3].z = 0.0;

		start = false;
	}

	//가운데 점 지우기
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), dotColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), dotShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glPointSize(2.0);
	glDrawArrays(GL_POINTS, 0, 1);

	//색깔
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	
	//new 삼각형 그리기
	for (int i = 0; i < NTcnt; i++)
	{
		if (nt[i].alive)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), &nt[i].colors, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			// modelTransform 변수에 변환 값 적용하기
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), nt[i].triShape, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			if (LineFillMode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			if (!LineFillMode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	//삼각형 그리기
	for (int i = 0; i < Tcnt; i++)
	{
		if (t[i].alive)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(t[i].cX, t[i].cY, 0.0f));
			model = glm::rotate(model, glm::radians(polygonRotate), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-t[i].cX, -t[i].cY, 0.0f));

			//색상
			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), &t[i].colors, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			// modelTransform 변수에 변환 값 적용하기
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), t[i].triShape, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			if (LineFillMode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			if (!LineFillMode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
	//삼각형 똥
	if (path)
	{
		for (int i = 0; i < Tcnt; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				if (t[i].alive)
				{
					model = glm::mat4(1.0f);// modelTransform 변수에 변환 값 적용하기
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

					glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
					glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), &pathColors, GL_STATIC_DRAW);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(1);

					glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
					glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), TdotShape[i][j], GL_STATIC_DRAW);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(0);
					glPointSize(2.0);
					glDrawArrays(GL_POINTS, 0, 1);
				}
				if (!t[i].alive)
				{
					break;
				}
			}
		}
	}

	// 사각형 그리기
	for (int i = 0; i < Rcnt; i++)
	{
		if (r[i].alive)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(r[i].cX, r[i].cY, 0.0f));
			model = glm::rotate(model, glm::radians(polygonRotate), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-r[i].cX, -r[i].cY, 0.0f));

			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), &r[i].colors, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			// modelTransform 변수에 변환 값 적용하기
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), r[i].rectShape, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			if (LineFillMode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			if (!LineFillMode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
	}
	if (path)
	{
		//사각형 똥
		for (int i = 0; i < Rcnt; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				if (r[i].alive)
				{
					model = glm::mat4(1.0f);// modelTransform 변수에 변환 값 적용하기
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

					glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
					glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), &pathColors, GL_STATIC_DRAW);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(1);

					glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
					glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), RdotShape[i][j], GL_STATIC_DRAW);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(0);
					glPointSize(2.0);
					glDrawArrays(GL_POINTS, 0, 1);
				}
				if (!r[i].alive)
				{
					break;
				}
			}
		}
	}
	
	// 오각형 그리기
	for (int i = 0; i < Pcnt; i++)
	{
		if (p[i].alive)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(p[i].cX, p[i].cY, 0.0f));
			model = glm::rotate(model, glm::radians(polygonRotate), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-p[i].cX, -p[i].cY, 0.0f));

			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), &p[i].colors, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			// modelTransform 변수에 변환 값 적용하기
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), p[i].pentaShape, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			if (LineFillMode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			if (!LineFillMode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
		}
	}
	if (path)
	{
		//오각형 똥
		for (int i = 0; i < Pcnt; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				if (p[i].alive)
				{
					model = glm::mat4(1.0f);// modelTransform 변수에 변환 값 적용하기
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

					glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
					glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), &pathColors, GL_STATIC_DRAW);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(1);

					glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
					glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), PdotShape[i][j], GL_STATIC_DRAW);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(0);
					glPointSize(2.0);
					glDrawArrays(GL_POINTS, 0, 1);
				}
				if (!p[i].alive)
				{
					break;
				}
			}
		}
	}

	model = glm::mat4(1.0f);
	// modelTransform 변수에 변환 값 적용하기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	
	//박스 그리기
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), &boxColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), box, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	if (LineFillMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (!LineFillMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	// 선 그리기
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), lineShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_LINES, 0, 2);

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
	vertexSource = filetobuf("vertexh.glsl");
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
	case 'p':
		path = !path;
		break;
	case 'l':
		LineFillMode = true;
		break;
	case 'f':
		LineFillMode = false;
		break;
	case '=':
	case '+':
		Allspeed += 0.001;
		break;
	case '-':
		if (Allspeed > -0.02)
		{
			Allspeed -= 0.001;
		}
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid Mouse(int button, int state, int x, int y)
{
	float openGLX, openGLY;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);

		movingMouse = 0;

		lineShape[0][0] = openGLX;
		lineShape[0][1] = openGLY;
		lineShape[1][0] = openGLX;
		lineShape[1][1] = openGLY;
	}
	else if (state == GLUT_UP)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);
		movingMouse = -1;
		//새로운 삼각형 시간차 두기
		for (int i = 0; i < NTcnt; i++)
		{
			if (nt[i].alive)
			{
				if (nt[i].timeout > 0)
				{
					nt[i].timeout -= 1;
				}
			}
		}
		//새로운 삼각형 자르기
		for (int i = 0; i < NTcnt; i++)
		{
			if (nt[i].alive && nt[i].timeout == 0)
			{
				if (checkCollision(nt[i].triShape[0][0], nt[i].triShape[0][1], nt[i].triShape[1][0], nt[i].triShape[1][1]))
				{
					nt[i].alive = false;
					NewTriangleCreate(nt[i].triShape[0][0], nt[i].triShape[0][1], (nt[i].triShape[0][0] + nt[i].triShape[1][0]) / 2, (nt[i].triShape[0][1] + nt[i].triShape[1][1]) / 2, nt[i].triShape[2][0], nt[i].triShape[2][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate((nt[i].triShape[0][0] + nt[i].triShape[1][0]) / 2, (nt[i].triShape[0][1] + nt[i].triShape[1][1]) / 2, nt[i].triShape[1][0], nt[i].triShape[1][1], nt[i].triShape[2][0], nt[i].triShape[2][1]);
					nt[NTcnt - 1].direction = 3;
				}
				else if (checkCollision(nt[i].triShape[1][0], nt[i].triShape[1][1], nt[i].triShape[2][0], nt[i].triShape[2][1]))
				{
					nt[i].alive = false;
					NewTriangleCreate(nt[i].triShape[0][0], nt[i].triShape[0][1], (nt[i].triShape[0][0] + nt[i].triShape[1][0]) / 2, (nt[i].triShape[0][1] + nt[i].triShape[1][1]) / 2, nt[i].triShape[2][0], nt[i].triShape[2][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate((nt[i].triShape[0][0] + nt[i].triShape[1][0]) / 2, (nt[i].triShape[0][1] + nt[i].triShape[1][1]) / 2, nt[i].triShape[1][0], nt[i].triShape[1][1], nt[i].triShape[2][0], nt[i].triShape[2][1]);
					nt[NTcnt - 1].direction = 3;
				}
				else if (checkCollision(nt[i].triShape[2][0], nt[i].triShape[2][1], nt[i].triShape[0][0], nt[i].triShape[0][1]))
				{
					nt[i].alive = false;
					NewTriangleCreate(nt[i].triShape[0][0], nt[i].triShape[0][1], (nt[i].triShape[0][0] + nt[i].triShape[1][0]) / 2, (nt[i].triShape[0][1] + nt[i].triShape[1][1]) / 2, nt[i].triShape[2][0], nt[i].triShape[2][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate((nt[i].triShape[0][0] + nt[i].triShape[1][0]) / 2, (nt[i].triShape[0][1] + nt[i].triShape[1][1]) / 2, nt[i].triShape[1][0], nt[i].triShape[1][1], nt[i].triShape[2][0], nt[i].triShape[2][1]);
					nt[NTcnt - 1].direction = 3;
				}
			}
		}

		//삼각형 자르기
		for (int i = 0; i < Tcnt; i++)
		{
			if (t[i].alive)
			{
				if (checkCollision(t[i].triShape[0][0], t[i].triShape[0][1], t[i].triShape[1][0], t[i].triShape[1][1]))
				{
					t[i].alive = false;
					NewTriangleCreate(t[i].triShape[0][0], t[i].triShape[0][1], (t[i].triShape[0][0] + t[i].triShape[1][0]) / 2, (t[i].triShape[0][1] + t[i].triShape[1][1]) / 2, t[i].triShape[2][0], t[i].triShape[2][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate((t[i].triShape[0][0] + t[i].triShape[1][0]) / 2, (t[i].triShape[0][1] + t[i].triShape[1][1]) / 2, t[i].triShape[1][0], t[i].triShape[1][1], t[i].triShape[2][0], t[i].triShape[2][1]);
					nt[NTcnt - 1].direction = 3;
				}
				else if (checkCollision(t[i].triShape[1][0], t[i].triShape[1][1], t[i].triShape[2][0], t[i].triShape[2][1]))
				{
					t[i].alive = false;
					NewTriangleCreate(t[i].triShape[0][0], t[i].triShape[0][1], (t[i].triShape[0][0] + t[i].triShape[1][0]) / 2, (t[i].triShape[0][1] + t[i].triShape[1][1]) / 2, t[i].triShape[2][0], t[i].triShape[2][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate((t[i].triShape[0][0] + t[i].triShape[1][0]) / 2, (t[i].triShape[0][1] + t[i].triShape[1][1]) / 2, t[i].triShape[1][0], t[i].triShape[1][1], t[i].triShape[2][0], t[i].triShape[2][1]);
					nt[NTcnt - 1].direction = 3;
				}
				else if (checkCollision(t[i].triShape[2][0], t[i].triShape[2][1], t[i].triShape[0][0], t[i].triShape[0][1]))
				{
					t[i].alive = false;
					NewTriangleCreate(t[i].triShape[0][0], t[i].triShape[0][1], (t[i].triShape[0][0] + t[i].triShape[1][0]) / 2, (t[i].triShape[0][1] + t[i].triShape[1][1]) / 2, t[i].triShape[2][0], t[i].triShape[2][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate((t[i].triShape[0][0] + t[i].triShape[1][0]) / 2, (t[i].triShape[0][1] + t[i].triShape[1][1]) / 2, t[i].triShape[1][0], t[i].triShape[1][1], t[i].triShape[2][0], t[i].triShape[2][1]);
					nt[NTcnt - 1].direction = 3;
				}
			}
		}
		//사각형 자르기
		for (int i = 0; i < Rcnt; i++)
		{
			if (r[i].alive)
			{
				if (checkCollision(r[i].rectShape[0][0], r[i].rectShape[0][1], r[i].rectShape[1][0], r[i].rectShape[1][1]))
				{
					r[i].alive = false;
					NewTriangleCreate(r[i].rectShape[0][0], r[i].rectShape[0][1], r[i].rectShape[1][0], r[i].rectShape[1][1], r[i].rectShape[3][0], r[i].rectShape[3][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate(r[i].rectShape[1][0], r[i].rectShape[1][1], r[i].rectShape[2][0], r[i].rectShape[2][1], r[i].rectShape[3][0], r[i].rectShape[3][1]);
					nt[NTcnt - 1].direction = 3;
				}
				else if (checkCollision(r[i].rectShape[1][0], r[i].rectShape[1][1], r[i].rectShape[2][0], r[i].rectShape[2][1]))
				{
					r[i].alive = false;
					NewTriangleCreate(r[i].rectShape[0][0], r[i].rectShape[0][1], r[i].rectShape[1][0], r[i].rectShape[1][1], r[i].rectShape[3][0], r[i].rectShape[3][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate(r[i].rectShape[1][0], r[i].rectShape[1][1], r[i].rectShape[2][0], r[i].rectShape[2][1], r[i].rectShape[3][0], r[i].rectShape[3][1]);
					nt[NTcnt - 1].direction = 3;
				}
				else if (checkCollision(r[i].rectShape[2][0], r[i].rectShape[2][1], r[i].rectShape[3][0], r[i].rectShape[3][1]))
				{
					r[i].alive = false;
					NewTriangleCreate(r[i].rectShape[0][0], r[i].rectShape[0][1], r[i].rectShape[1][0], r[i].rectShape[1][1], r[i].rectShape[3][0], r[i].rectShape[3][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate(r[i].rectShape[1][0], r[i].rectShape[1][1], r[i].rectShape[2][0], r[i].rectShape[2][1], r[i].rectShape[3][0], r[i].rectShape[3][1]);
					nt[NTcnt - 1].direction = 3;
				}
				else if (checkCollision(r[i].rectShape[3][0], r[i].rectShape[3][1], r[i].rectShape[0][0], r[i].rectShape[0][1]))
				{
					r[i].alive = false;
					NewTriangleCreate(r[i].rectShape[0][0], r[i].rectShape[0][1], r[i].rectShape[1][0], r[i].rectShape[1][1], r[i].rectShape[3][0], r[i].rectShape[3][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate(r[i].rectShape[1][0], r[i].rectShape[1][1], r[i].rectShape[2][0], r[i].rectShape[2][1], r[i].rectShape[3][0], r[i].rectShape[3][1]);
					nt[NTcnt - 1].direction = 3;
				}
			}
		}
		//오각형 자르기
		for (int i = 0; i < Pcnt; i++)
		{
			if (p[i].alive)
			{
				if (checkCollision(p[i].pentaShape[0][0], p[i].pentaShape[0][1], p[i].pentaShape[1][0], p[i].pentaShape[1][1]))
				{
					p[i].alive = false;
					NewTriangleCreate(p[i].pentaShape[0][0], p[i].pentaShape[0][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1], p[i].pentaShape[4][0], p[i].pentaShape[4][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate(p[i].pentaShape[0][0], p[i].pentaShape[0][1], p[i].pentaShape[1][0], p[i].pentaShape[1][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1]);
					nt[NTcnt - 1].direction = 3;
					NewTriangleCreate(p[i].pentaShape[1][0], p[i].pentaShape[1][1], p[i].pentaShape[2][0], p[i].pentaShape[2][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1]);
					nt[NTcnt - 1].direction = 2;
				}
				else if (checkCollision(p[i].pentaShape[1][0], p[i].pentaShape[1][1], p[i].pentaShape[2][0], p[i].pentaShape[2][1]))
				{
					p[i].alive = false;
					NewTriangleCreate(p[i].pentaShape[0][0], p[i].pentaShape[0][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1], p[i].pentaShape[4][0], p[i].pentaShape[4][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate(p[i].pentaShape[0][0], p[i].pentaShape[0][1], p[i].pentaShape[1][0], p[i].pentaShape[1][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1]);
					nt[NTcnt - 1].direction = 3;
					NewTriangleCreate(p[i].pentaShape[1][0], p[i].pentaShape[1][1], p[i].pentaShape[2][0], p[i].pentaShape[2][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1]);
					nt[NTcnt - 1].direction = 2;
				}
				else if (checkCollision(p[i].pentaShape[2][0], p[i].pentaShape[2][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1]))
				{
					p[i].alive = false;
					NewTriangleCreate(p[i].pentaShape[0][0], p[i].pentaShape[0][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1], p[i].pentaShape[4][0], p[i].pentaShape[4][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate(p[i].pentaShape[0][0], p[i].pentaShape[0][1], p[i].pentaShape[1][0], p[i].pentaShape[1][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1]);
					nt[NTcnt - 1].direction = 3;
					NewTriangleCreate(p[i].pentaShape[1][0], p[i].pentaShape[1][1], p[i].pentaShape[2][0], p[i].pentaShape[2][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1]);
					nt[NTcnt - 1].direction = 2;
				}
				else if (checkCollision(p[i].pentaShape[3][0], p[i].pentaShape[3][1], p[i].pentaShape[4][0], p[i].pentaShape[4][1]))
				{
					p[i].alive = false;
					NewTriangleCreate(p[i].pentaShape[0][0], p[i].pentaShape[0][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1], p[i].pentaShape[4][0], p[i].pentaShape[4][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate(p[i].pentaShape[0][0], p[i].pentaShape[0][1], p[i].pentaShape[1][0], p[i].pentaShape[1][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1]);
					nt[NTcnt - 1].direction = 3;
					NewTriangleCreate(p[i].pentaShape[1][0], p[i].pentaShape[1][1], p[i].pentaShape[2][0], p[i].pentaShape[2][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1]);
					nt[NTcnt - 1].direction = 2;
				}
				else if (checkCollision(p[i].pentaShape[4][0], p[i].pentaShape[4][1], p[i].pentaShape[0][0], p[i].pentaShape[0][1]))
				{
					p[i].alive = false;
					NewTriangleCreate(p[i].pentaShape[0][0], p[i].pentaShape[0][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1], p[i].pentaShape[4][0], p[i].pentaShape[4][1]);
					nt[NTcnt - 1].direction = 2;
					NewTriangleCreate(p[i].pentaShape[0][0], p[i].pentaShape[0][1], p[i].pentaShape[1][0], p[i].pentaShape[1][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1]);
					nt[NTcnt - 1].direction = 3;
					NewTriangleCreate(p[i].pentaShape[1][0], p[i].pentaShape[1][1], p[i].pentaShape[2][0], p[i].pentaShape[2][1], p[i].pentaShape[3][0], p[i].pentaShape[3][1]);
					nt[NTcnt - 1].direction = 2;
				}
			}
		}

		

		lineShape[0][0] = -1.0;
		lineShape[0][1] = -1.0;
		lineShape[1][0] = -1.0;
		lineShape[1][1] = -1.0;
	}
}

GLvoid Motion(int x, int y)
{
	if (movingMouse >= 0)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);

		//이동
		lineShape[1][0] = openGLX;
		lineShape[1][1] = openGLY;

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
		//도형 회전
		polygonRotate += 1;
		//새로운 삼각형 움직임
		for (int i = 0; i < NTcnt; i++)
		{
			if (nt[i].alive)
			{
				if (nt[i].animation)
				{
					if (nt[i].direction == 2)
					{
						nt[i].triShape[0][0] -= 0.001;
						nt[i].triShape[0][1] -= 0.008;
						nt[i].triShape[1][0] -= 0.001;
						nt[i].triShape[1][1] -= 0.008;
						nt[i].triShape[2][0] -= 0.001;
						nt[i].triShape[2][1] -= 0.008;
						nt[i].cX -= 0.001;
						nt[i].cY -= 0.008;

						/*if (nt[i].speed + Allspeed > 0.001)
						{
							nt[i].cX = nt[i].cX + nt[i].speed + Allspeed;
						}
						else
						{
							nt[i].cX = nt[i].cX + 0.001;
						}*/
					}
					if (nt[i].direction == 3)
					{
						nt[i].triShape[0][0] += 0.001;
						nt[i].triShape[0][1] -= 0.008;
						nt[i].triShape[1][0] += 0.001;
						nt[i].triShape[1][1] -= 0.008;
						nt[i].triShape[2][0] += 0.001;
						nt[i].triShape[2][1] -= 0.008;
						nt[i].cX += 0.001;
						nt[i].cY -= 0.008;
						/*if (nt[i].speed + Allspeed > 0.001)
						{
							nt[i].cX = nt[i].cX - nt[i].speed - Allspeed;
						}
						else
						{
							nt[i].cX = nt[i].cX - 0.001;
						}*/
					}
					/*nt[i].cY = nt[i].cY - 0.001;

					nt[i].triShape[0][0] = nt[i].cX - Size;
					nt[i].triShape[0][1] = nt[i].cY - Size;
					nt[i].triShape[1][0] = nt[i].cX + Size;
					nt[i].triShape[1][1] = nt[i].cY - Size;
					nt[i].triShape[2][0] = nt[i].cX;
					nt[i].triShape[2][1] = nt[i].cY + Size;*/
					if (nt[i].cY < -1.0)
					{
						nt[i].alive = false;
					}
				}
				
				if (nt[i].cX > box[0][0] && nt[i].cX < box[1][0] && nt[i].cY > box[0][1] && nt[i].cY < box[2][1])
				{
					nt[i].animation = false;
				}
			}
		}

		//포물선
		//삼각형
		for (int i = 0; i < Tcnt; i++)
		{
			if (t[i].alive)
			{
				if (t[i].direction == 0)
				{
					if (t[i].speed + Allspeed > 0.001)
					{
						t[i].cX = t[i].cX + t[i].speed + Allspeed;
					}
					else
					{
						t[i].cX = t[i].cX + 0.001;
					}
				}
				if (t[i].direction == 1)
				{
					if (t[i].speed + Allspeed > 0.001)
					{
						t[i].cX = t[i].cX - t[i].speed - Allspeed;
					}
					else
					{
						t[i].cX = t[i].cX - 0.001;
					}
				}
				t[i].cY = -t[i].m * (t[i].cX - 1.0) * (t[i].cX + 1.0) + t[i].C;

				t[i].triShape[0][0] = t[i].cX - Size;
				t[i].triShape[0][1] = t[i].cY - Size;
				t[i].triShape[1][0] = t[i].cX + Size;
				t[i].triShape[1][1] = t[i].cY - Size;
				t[i].triShape[2][0] = t[i].cX;
				t[i].triShape[2][1] = t[i].cY + Size;

				if (t[i].direction == 0 && t[i].cX > 1.0)
				{
					t[i].alive = false;
				}
				if (t[i].direction == 1 && t[i].cX < -1.0)
				{
					t[i].alive = false;
				}
			}
		}

		//사각형
		for (int i = 0; i < Rcnt; i++)
		{
			if (r[i].alive)
			{
				if (r[i].direction == 0)
				{
					if (r[i].speed + Allspeed > 0.001)
					{
						r[i].cX = r[i].cX + r[i].speed + Allspeed;
					}
					else
					{
						r[i].cX = r[i].cX + 0.001;
					}
				}
				if (r[i].direction == 1)
				{
					if (r[i].speed + Allspeed > 0.001)
					{
						r[i].cX = r[i].cX - r[i].speed - Allspeed;
					}
					else
					{
						r[i].cX = r[i].cX - 0.001;
					}
				}
				r[i].cY = -r[i].m * (r[i].cX - 1.0) * (r[i].cX + 1.0)+r[i].C;

				r[i].rectShape[0][0] = r[i].cX - Size;
				r[i].rectShape[0][1] = r[i].cY - Size;
				r[i].rectShape[1][0] = r[i].cX + Size;
				r[i].rectShape[1][1] = r[i].cY - Size;
				r[i].rectShape[2][0] = r[i].cX + Size;
				r[i].rectShape[2][1] = r[i].cY + Size;
				r[i].rectShape[3][0] = r[i].cX - Size;
				r[i].rectShape[3][1] = r[i].cY + Size;

				if (r[i].direction == 0 && r[i].cX > 1.0)
				{
					r[i].alive = false;
				}
				if (r[i].direction == 1 && r[i].cX < -1.0)
				{
					r[i].alive = false;
				}
			}
		}
		//오각형
		for (int i = 0; i < Pcnt; i++)
		{
			if (p[i].alive)
			{
				if (p[i].direction == 0)
				{
					if (p[i].speed + Allspeed > 0.001)
					{
						p[i].cX = p[i].cX + p[i].speed + Allspeed;
					}
					else
					{
						p[i].cX = p[i].cX + 0.001;
					}
				}
				if (p[i].direction == 1)
				{
					if (p[i].speed + Allspeed > 0.001)
					{
						p[i].cX = p[i].cX - p[i].speed - Allspeed;
					}
					else
					{
						p[i].cX = p[i].cX - 0.001;
					}
				}
				p[i].cY = -p[i].m * (p[i].cX - 1.0) * (p[i].cX + 1.0)+p[i].C;

				p[i].pentaShape[0][0] = p[i].cX - Size;
				p[i].pentaShape[0][1] = p[i].cY - Size;
				p[i].pentaShape[1][0] = p[i].cX + Size;
				p[i].pentaShape[1][1] = p[i].cY - Size;
				p[i].pentaShape[2][0] = p[i].cX + Size + Size / 2;
				p[i].pentaShape[2][1] = p[i].cY + Size / 2;
				p[i].pentaShape[3][0] = p[i].cX;
				p[i].pentaShape[3][1] = p[i].cY + Size + Size / 2;
				p[i].pentaShape[4][0] = p[i].cX - Size - Size / 2;
				p[i].pentaShape[4][1] = p[i].cY + Size / 2;

				if (p[i].direction == 0 && p[i].cX > 1.0)
				{
					p[i].alive = false;
				}
				if (p[i].direction == 1 && p[i].cX < -1.0)
				{
					p[i].alive = false;
				}
			}
		}

		//박스
		if (boxDirection)
		{
			box[0][0] -= 0.005;
			box[1][0] -= 0.005;
			box[2][0] -= 0.005;
			box[3][0] -= 0.005;
			for (int i = 0; i < NTcnt; i++)
			{
				if (!nt[i].animation)
				{
					nt[i].triShape[0][0] -= 0.005;
					nt[i].triShape[1][0] -= 0.005;
					nt[i].triShape[2][0] -= 0.005;
					nt[i].cX -= 0.005;
				}
			}
		}
		if (!boxDirection)
		{
			box[0][0] += 0.005;
			box[1][0] += 0.005;
			box[2][0] += 0.005;
			box[3][0] += 0.005;
			for (int i = 0; i < NTcnt; i++)
			{
				if (!nt[i].animation)
				{
					nt[i].triShape[0][0] += 0.005;
					nt[i].triShape[1][0] += 0.005;
					nt[i].triShape[2][0] += 0.005;
					nt[i].cX += 0.005;
				}
			}
		}
		if (box[0][0] < -1.0)
		{
			boxDirection = false;
			box[0][0] += 0.005;
			box[1][0] += 0.005;
			box[2][0] += 0.005;
			box[3][0] += 0.005;
			for (int i = 0; i < NTcnt; i++)
			{
				if (!nt[i].animation)
				{
					nt[i].triShape[0][0] += 0.005;
					nt[i].triShape[1][0] += 0.005;
					nt[i].triShape[2][0] += 0.005;
					nt[i].cX += 0.005;
				}
			}
		}
		if (box[1][0] > 1.0)
		{
			boxDirection = true;
			box[0][0] -= 0.005;
			box[1][0] -= 0.005;
			box[2][0] -= 0.005;
			box[3][0] -= 0.005;
			for (int i = 0; i < NTcnt; i++)
			{
				if (!nt[i].animation)
				{
					nt[i].triShape[0][0] -= 0.005;
					nt[i].triShape[1][0] -= 0.005;
					nt[i].triShape[2][0] -= 0.005;
					nt[i].cX -= 0.005;
				}
			}
		}
		glutTimerFunc(10, TimerFunction, 1);
		break;
	case 2:
		createPolygon = polygonrandom(gen);
		createPolygonDirection = directionrandom(gen);

		if (createPolygonDirection == 0)
		{
			centerX = -1.0;
		}
		if (createPolygonDirection == 1)
		{
			centerX = 1.0;
		}
		
		switch (createPolygon)
		{
		case 0:
			centerY = randomY(gen);
			t[Tcnt].triShape[0][0] = centerX - Size;
			t[Tcnt].triShape[0][1] = centerY - Size;
			t[Tcnt].triShape[1][0] = centerX + Size;
			t[Tcnt].triShape[1][1] = centerY - Size;
			t[Tcnt].triShape[2][0] = centerX;
			t[Tcnt].triShape[2][1] = centerY + Size;
			t[Tcnt].cX = centerX;
			t[Tcnt].cY = centerY;
			t[Tcnt].C = centerY;
			t[Tcnt].alive = true;

			t[Tcnt].direction = createPolygonDirection;
			t[Tcnt].m = randomM(gen);
			t[Tcnt].speed = randomSpeed(gen);
			t[Tcnt].colors[0].x = colorDis(gen);
			t[Tcnt].colors[0].y = colorDis(gen);
			t[Tcnt].colors[0].z = colorDis(gen);
			t[Tcnt].colors[1].x = colorDis(gen);
			t[Tcnt].colors[1].y = colorDis(gen);
			t[Tcnt].colors[1].z = colorDis(gen);
			t[Tcnt].colors[2].x = colorDis(gen);
			t[Tcnt].colors[2].y = colorDis(gen);
			t[Tcnt].colors[2].z = colorDis(gen);

			Tcnt++;
			break;

		case 1:
			centerY = randomY(gen);
			r[Rcnt].rectShape[0][0] = centerX - Size;
			r[Rcnt].rectShape[0][1] = centerY - Size;
			r[Rcnt].rectShape[1][0] = centerX + Size;
			r[Rcnt].rectShape[1][1] = centerY - Size;
			r[Rcnt].rectShape[2][0] = centerX + Size;
			r[Rcnt].rectShape[2][1] = centerY + Size;
			r[Rcnt].rectShape[3][0] = centerX - Size;
			r[Rcnt].rectShape[3][1] = centerY + Size;
			r[Rcnt].cX = centerX;
			r[Rcnt].cY = centerY;
			r[Rcnt].C = centerY;
			r[Rcnt].alive = true;

			r[Rcnt].direction = createPolygonDirection;
			r[Rcnt].m = randomM(gen);
			r[Rcnt].speed = randomSpeed(gen);
			r[Rcnt].colors[0].x = colorDis(gen);
			r[Rcnt].colors[0].y = colorDis(gen);
			r[Rcnt].colors[0].z = colorDis(gen);
			r[Rcnt].colors[1].x = colorDis(gen);
			r[Rcnt].colors[1].y = colorDis(gen);
			r[Rcnt].colors[1].z = colorDis(gen);
			r[Rcnt].colors[2].x = colorDis(gen);
			r[Rcnt].colors[2].y = colorDis(gen);
			r[Rcnt].colors[2].z = colorDis(gen);
			r[Rcnt].colors[3].x = colorDis(gen);
			r[Rcnt].colors[3].y = colorDis(gen);
			r[Rcnt].colors[3].z = colorDis(gen);

			Rcnt++;
			break;
		case 2:
			centerY = randomY(gen);
			p[Pcnt].pentaShape[0][0] = centerX - Size;
			p[Pcnt].pentaShape[0][1] = centerY - Size;
			p[Pcnt].pentaShape[1][0] = centerX + Size;
			p[Pcnt].pentaShape[1][1] = centerY - Size;
			p[Pcnt].pentaShape[2][0] = centerX + Size + Size / 2;
			p[Pcnt].pentaShape[2][1] = centerY + Size / 2;
			p[Pcnt].pentaShape[3][0] = centerX;
			p[Pcnt].pentaShape[3][1] = centerY + Size + Size / 2;
			p[Pcnt].pentaShape[4][0] = centerX - Size - Size / 2;
			p[Pcnt].pentaShape[4][1] = centerY + Size / 2;
			p[Pcnt].cX = centerX;
			p[Pcnt].cY = centerY;
			p[Pcnt].C = centerY;
			p[Pcnt].alive = true;

			p[Pcnt].direction = createPolygonDirection;
			p[Pcnt].m = randomM(gen);
			p[Pcnt].speed = randomSpeed(gen);
			p[Pcnt].colors[0].x = colorDis(gen);
			p[Pcnt].colors[0].y = colorDis(gen);
			p[Pcnt].colors[0].z = colorDis(gen);
			p[Pcnt].colors[1].x = colorDis(gen);
			p[Pcnt].colors[1].y = colorDis(gen);
			p[Pcnt].colors[1].z = colorDis(gen);
			p[Pcnt].colors[2].x = colorDis(gen);
			p[Pcnt].colors[2].y = colorDis(gen);
			p[Pcnt].colors[2].z = colorDis(gen);
			p[Pcnt].colors[3].x = colorDis(gen);
			p[Pcnt].colors[3].y = colorDis(gen);
			p[Pcnt].colors[3].z = colorDis(gen);
			p[Pcnt].colors[4].x = colorDis(gen);
			p[Pcnt].colors[4].y = colorDis(gen);
			p[Pcnt].colors[4].z = colorDis(gen);

			Pcnt++;
			break;
		}
		glutTimerFunc(1000, TimerFunction, 2);
		break;
	case 3:
		if (path)
		{
			for (int i = 0; i < Tcnt; i++)
			{
				if (t[i].alive)
				{
					TdotShape[i][TdotCnt[i]][0] = t[i].cX;
					TdotShape[i][TdotCnt[i]][1] = t[i].cY;
					TdotCnt[i]++;
				}
			}
			for (int i = 0; i < Rcnt; i++)
			{
				if (r[i].alive)
				{
					RdotShape[i][RdotCnt[i]][0] = r[i].cX;
					RdotShape[i][RdotCnt[i]][1] = r[i].cY;
					RdotCnt[i]++;
				}
			}
			for (int i = 0; i < Pcnt; i++)
			{
				if (p[i].alive)
				{
					PdotShape[i][PdotCnt[i]][0] = p[i].cX;
					PdotShape[i][PdotCnt[i]][1] = p[i].cY;
					PdotCnt[i]++;
				}
			}
		}
		glutTimerFunc(100, TimerFunction, 3);
		break;
	}
	glutPostRedisplay();
}

int checkCollision(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2)
{
	// 두 선분의 좌표를 변수에 저장
	double x1 = lineShape[0][0];
	double x2 = lineShape[1][0];
	double x3 = X1;
	double x4 = X2;

	double y1 = lineShape[0][1];
	double y2 = lineShape[1][1];
	double y3 = Y1;
	double y4 = Y2;

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

void NewTriangleCreate(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3)
{
	//centerY = randomY(gen);
	nt[NTcnt].triShape[0][0] = x1;
	nt[NTcnt].triShape[0][1] = y1;
	nt[NTcnt].triShape[1][0] = x2;
	nt[NTcnt].triShape[1][1] = y2;
	nt[NTcnt].triShape[2][0] = x3;
	nt[NTcnt].triShape[2][1] = y3;
	nt[NTcnt].cX = (x1+x2+x3)/3;
	nt[NTcnt].cY = (y1+y2+y3)/3;

	//nt[NTcnt].C = centerY;
	nt[NTcnt].alive = true;
	 
	//nt[NTcnt].direction = createPolygonDirection;
	//nt[NTcnt].m = randomM(gen);

	//방향은 따로 추가

	//nt[NTcnt].speed = randomSpeed(gen);
	nt[NTcnt].speed = 0.007;

	nt[NTcnt].colors[0].x = 0.5;
	nt[NTcnt].colors[1].x = 0.5;
	nt[NTcnt].colors[2].x = 0.5;
	nt[NTcnt].colors[0].y = 0.5;
	nt[NTcnt].colors[1].y = 0.5;
	nt[NTcnt].colors[2].y = 0.5;
	nt[NTcnt].colors[0].z = 0.5;
	nt[NTcnt].colors[1].z = 0.5;
	nt[NTcnt].colors[2].z = 0.5;

	NTcnt++;
}