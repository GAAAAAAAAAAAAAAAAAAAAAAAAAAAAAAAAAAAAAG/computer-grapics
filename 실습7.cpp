#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

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

	//점 찍기
	for (int i = 0; i < dotCnt; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), dotShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glPointSize(10.0);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	//선 그리기
	for (int i = 0; i < lineCnt; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), lineShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glLineWidth(10.0);
		glDrawArrays(GL_LINES, 0, 2);
	}
	//삼각형 그리기
	for (int i = 0; i < triCnt; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	//사각형 그리기
	for (int i = 0; i < rectCnt; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_QUADS, 0, 4);
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
		D = true;
		L = false;
		T = false;
		R = false;
		break;
	case 'l':
		D = false;
		L = true;
		T = false;
		R = false;
		break;
	case 't':
		D = false;
		L = false;
		T = true;
		R = false;
		break;
	case 'r':
		D = false;
		L = false;
		T = false;
		R = true;
		break;
	case 'w':
		for (int i = 0; i < AllCnt; i++)
		{
			dotShape[i][1] += 0.05;

			lineShape[i][0][1] += 0.05;
			lineShape[i][1][1] += 0.05;

			triShape[i][0][1] += 0.05;
			triShape[i][1][1] += 0.05;
			triShape[i][2][1] += 0.05;

			rectShape[i][0][1] += 0.05;
			rectShape[i][1][1] += 0.05;
			rectShape[i][2][1] += 0.05;
			rectShape[i][3][1] += 0.05;
		}
		break;
	case 'a':
		for (int i = 0; i < AllCnt; i++)
		{
			dotShape[i][0] -= 0.05;

			lineShape[i][0][0] -= 0.05;
			lineShape[i][1][0] -= 0.05;

			triShape[i][0][0] -= 0.05;
			triShape[i][1][0] -= 0.05;
			triShape[i][2][0] -= 0.05;

			rectShape[i][0][0] -= 0.05;
			rectShape[i][1][0] -= 0.05;
			rectShape[i][2][0] -= 0.05;
			rectShape[i][3][0] -= 0.05;
		}
		break;
	case 's':
		for (int i = 0; i < AllCnt; i++)
		{
			dotShape[i][1] -= 0.05;

			lineShape[i][0][1] -= 0.05;
			lineShape[i][1][1] -= 0.05;

			triShape[i][0][1] -= 0.05;
			triShape[i][1][1] -= 0.05;
			triShape[i][2][1] -= 0.05;

			rectShape[i][0][1] -= 0.05;
			rectShape[i][1][1] -= 0.05;
			rectShape[i][2][1] -= 0.05;
			rectShape[i][3][1] -= 0.05;
		}
		break;
	case 'd':
		for (int i = 0; i < AllCnt; i++)
		{
			dotShape[i][0] += 0.05;

			lineShape[i][0][0] += 0.05;
			lineShape[i][1][0] += 0.05;

			triShape[i][0][0] += 0.05;
			triShape[i][1][0] += 0.05;
			triShape[i][2][0] += 0.05;

			rectShape[i][0][0] += 0.05;
			rectShape[i][1][0] += 0.05;
			rectShape[i][2][0] += 0.05;
			rectShape[i][3][0] += 0.05;
		}
		break;
	case 'c':
		AllCnt = 0;
		dotCnt = 0;
		lineCnt = 0;
		triCnt = 0;
		rectCnt = 0;
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

		if (AllCnt < 10)
		{
			if (D && dotCnt < 10)
			{
				dotShape[dotCnt][0] = openGLX;
				dotShape[dotCnt][1] = openGLY;
				dotShape[dotCnt][2] = 0.0;

				dotCnt++;
			}
			else if (L && lineCnt < 10)
			{
				lineShape[lineCnt][0][0] = openGLX-0.25;
				lineShape[lineCnt][0][1] = openGLY;
				lineShape[lineCnt][0][2] = 0.0;
				lineShape[lineCnt][1][0] = openGLX + 0.25;
				lineShape[lineCnt][1][1] = openGLY;
				lineShape[lineCnt][1][2] = 0.0;

				lineCnt++;
			}
			else if (T && triCnt < 10)
			{
				triShape[triCnt][0][0] = openGLX - 0.25;
				triShape[triCnt][0][1] = openGLY - 0.25;
				triShape[triCnt][0][2] = 0.0;
				triShape[triCnt][1][0] = openGLX + 0.25;
				triShape[triCnt][1][1] = openGLY - 0.25;
				triShape[triCnt][1][2] = 0.0;
				triShape[triCnt][2][0] = openGLX;
				triShape[triCnt][2][1] = openGLY+0.5;
				triShape[triCnt][2][2] = 0.0;

				triCnt++;
			}
			else if (R && rectCnt < 10)
			{
				rectShape[rectCnt][0][0] = openGLX - 0.25;
				rectShape[rectCnt][0][1] = openGLY - 0.25;
				rectShape[rectCnt][0][2] = 0.0;
				rectShape[rectCnt][1][0] = openGLX + 0.25;
				rectShape[rectCnt][1][1] = openGLY - 0.25;
				rectShape[rectCnt][1][2] = 0.0;
				rectShape[rectCnt][2][0] = openGLX + 0.25;
				rectShape[rectCnt][2][1] = openGLY + 0.25;
				rectShape[rectCnt][2][2] = 0.0;
				rectShape[rectCnt][3][0] = openGLX - 0.25;
				rectShape[rectCnt][3][1] = openGLY + 0.25;
				rectShape[rectCnt][3][2] = 0.0;

				rectCnt++;
			}
			AllCnt++;
		}
		WindowToOpenGL(x, y, openGLX, openGLY);
	}
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}
