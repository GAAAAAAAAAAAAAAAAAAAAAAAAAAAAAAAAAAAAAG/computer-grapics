#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <cmath>
#include <vector>

GLfloat triShape[10][3][3] = {};//--- 삼각형 위치 값
int triCnt = 0;
bool T = false;

GLfloat rectShape[10][4][3] = {}; //--- 사각형 위치 값
int rectCnt = 0;
bool R = false;

GLfloat lineShape[10][2][3] = {};
int lineCnt = 0;
bool L = false;

GLfloat dotShape[10][3] = {};
int dotCnt = 0;
bool D = true;

int AllCnt = 0;

GLfloat colors[4][3] = { //--- 삼각형 꼭지점 색상
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
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

bool drawSpiral = false;
bool drawClockwise = true;
float spiralRadius = 0.0f;
const float spiralIncrement = 0.002f;
bool drawSecondSpiral = false;

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
void draw_Spiral();
void initialize();

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

	initialize(); // 초기화 함수 호출

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);

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

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	if (drawSpiral) {
		draw_Spiral();
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
	case 'p':
		drawSpiral = true;
		drawClockwise = true;
		break;
	case'l':
		drawSpiral = true;
		drawClockwise = false;
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
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

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		WindowToOpenGL(x, y, openGLX, openGLY);

		if (!drawSpiral) {
			// 첫 번째 클릭에서 스파이럴 그리기 시작
			dotShape[0][0] = openGLX;
			dotShape[0][1] = openGLY;
			dotShape[0][2] = 0.0f;

			drawSpiral = true;
			drawClockwise = true; // 예시로 시계방향으로 설정
		}
	}
	glutPostRedisplay();
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}

//void draw_Spiral(float centerX, float centerY) {
//	std::vector<GLfloat> vertices;
//
//	float direction = (drawClockwise) ? 1.0f : -1.0f;
//
//	for (float theta = 0.0f; theta <= 6.0f * 3.14159f; theta += 0.1f) {
//		float x = centerX + spiralRadius * cos(theta) * direction;
//		float y = centerY + spiralRadius * sin(theta) * direction;
//
//		vertices.push_back(x);
//		vertices.push_back(y);
//		vertices.push_back(0.0f);
//
//		spiralRadius += spiralIncrement;
//
//		if (spiralRadius > 0.5f) {
//			drawSpiral = false;
//			spiralRadius = 0.0f;
//
//			// 두 번째 스파이럴 그리기 시작
//			drawClockwise = !drawClockwise;
//			dotShape[1][0] = x + 0.2f;  // x좌표를 현재 위치에서 오른쪽으로 0.2f만큼 이동
//			dotShape[1][1] = y;
//			dotShape[1][2] = 0.0f;
//
//			// 이전 스파이럴의 끝점을 두 번째 스파이럴의 시작점으로 설정
//			dotShape[0][0] = x;
//			dotShape[0][1] = y;
//			dotShape[0][2] = 0.0f;
//
//			drawSpiral = true;
//
//			break;
//		}
//	}
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(0);
//
//	glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
//}

const double radiusChangeRate = 0.05;

// 각도 변화율
const double angleChangeRate = 0.1;

// 초기 반지름
double radius = 0.5;

// 초기 각도
double angle = 0.0;

// 원 스파이럴을 그리는 함수
void draw_Spiral() {
	glBegin(GL_POINTS);

	// 원 스파이럴을 그리는 루프
	for (int i = 0; i < 1000; ++i) {
		double x = radius * cos(angle);
		double y = radius * sin(angle);

		glVertex2d(x, y);

		// 반지름과 각도 갱신
		radius += radiusChangeRate;
		angle += angleChangeRate;
	}

	glEnd();
}

void initialize()
{
	drawSpiral = false;
	drawClockwise = true;
	spiralRadius = 0.0f;
	drawSecondSpiral = false;
}