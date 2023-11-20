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
void ReadObj(FILE* path);

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

	glutTimerFunc(1000, TimerFunction, 1);
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

	//s r t p 코드 작성시에는 반대 방향으로.
	model = glm::mat4(1.0f);

	// x축으로 30도 회전
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// y축으로 30도 회전
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// z축으로 0도 회전
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	//회전
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	//이동
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

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
	glGenBuffers(3, vbo); //--- 2개의 VBO를 지정하고 할당하기
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
	vertexSource = filetobuf("vertex3.glsl");
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
	fragmentSource = filetobuf("fragment3.glsl");
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

GLvoid SpecialKeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
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
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(1000, TimerFunction, 1);
}

void ReadObj(FILE* path) {
	char count[128];
	int vertexnum = 0;
	int facenum = 0;
	int normalnum = 0; // vn 개수 카운트 추가
	int uvnum = 0;

	while (!feof(path)) {
		fscanf(path, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexnum++;
		else if (count[0] == 'f' && count[1] == '\0')
			facenum++;
		else if (count[0] == 'v' && count[1] == 'n' && count[2] == '\0') // vn 개수 카운트
			normalnum++;
		else if (count[0] == 'v' && count[1] == 't' && count[2] == '\0')
			uvnum++;
		memset(count, '\0', sizeof(count));
	}
	rewind(path);

	int vertIndex = 0;
	int faceIndex = 0;
	int normalIndex = 0; // vn 인덱스 추가
	int uvIndex = 0;

	glm::vec3* vertex = new glm::vec3[vertexnum];
	glm::vec3* face = new glm::vec3[facenum];
	glm::vec3* normals = new glm::vec3[normalnum]; // vn 저장 배열 추가
	glm::vec3* uvdata = new glm::vec3[facenum]; // uvdata 배열 크기 수정
	glm::vec2* uv = new glm::vec2[uvnum];

	char bind[128];

	while (!feof(path)) {
		fscanf(path, "%s", bind);
		if (bind[0] == 'v' && bind[1] == '\0') {
			fscanf(path, "%f %f %f\n",
				&vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
			vertIndex++;
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			unsigned int temp_face[3], temp_uv[3], temp_normal[3];
			fscanf(path, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&temp_face[0], &temp_uv[0], &temp_normal[0],
				&temp_face[1], &temp_uv[1], &temp_normal[1],
				&temp_face[2], &temp_uv[2], &temp_normal[2]);
			face[faceIndex].x = temp_face[0];
			face[faceIndex].y = temp_face[1];
			face[faceIndex].z = temp_face[2];
			uvdata[faceIndex].x = temp_uv[0];
			uvdata[faceIndex].y = temp_uv[1];
			uvdata[faceIndex].z = temp_uv[2];
			// vn 정보 저장
			normals[faceIndex].x = temp_normal[0];
			normals[faceIndex].y = temp_normal[1];
			normals[faceIndex].z = temp_normal[2];
			faceIndex++;
		}
		else if (bind[0] == 'v' && bind[1] == 'n' && bind[2] == '\0') {
			fscanf(path, "%f %f %f\n", &normals[normalIndex].x, &normals[normalIndex].y, &normals[normalIndex].z);
			normalIndex++;
		}
		else if (bind[0] == 'v' && bind[1] == 't' && bind[2] == '\0') {
			fscanf(path, "%f %f\n", &uv[uvIndex].x, &uv[uvIndex].y);
			uvIndex++;
		}
	}
	// 나머지 코드는 필요한 경우에 맞게 활용
}