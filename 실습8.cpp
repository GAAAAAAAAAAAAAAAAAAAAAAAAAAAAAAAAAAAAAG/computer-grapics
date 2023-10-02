#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

using namespace std;
random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> dis(0.15, 0.25);
uniform_real_distribution<double> colorDis(0.0, 1.0);

GLfloat triShape[4][3][3] = {};//--- 삼각형 위치 값

bool a = true;
bool b = false;

GLfloat XYShape[2][2][3] = {
	{{-1.0,0.0,0.0},{1.0,0.0,0.0}},
	{{0.0,1.0,0.0},{0.0,-1.0,0.0}} };
int AllCnt = 0;

GLfloat colors[4][3][3] = {}; //--- 삼각형 꼭지점 색상

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

bool start = true;

GLvoid drawScene()
{
	//--- 변경된 배경색 설정
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);
	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);

	/*glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);*/

	if (start)
	{
		triShape[0][0][0] = 0.5 - 0.15;
		triShape[0][0][1] = 0.5 - 0.15;
		triShape[0][0][2] = 0.0;
		triShape[0][1][0] = 0.5 + 0.15;
		triShape[0][1][1] = 0.5 - 0.15;
		triShape[0][1][2] = 0.0;
		triShape[0][2][0] = 0.5;
		triShape[0][2][1] = 0.5 + 0.25;
		triShape[0][2][2] = 0.0;

		triShape[1][0][0] = -0.5 - 0.15;
		triShape[1][0][1] = 0.5 - 0.15;
		triShape[1][0][2] = 0.0;
		triShape[1][1][0] = -0.5 + 0.15;
		triShape[1][1][1] = 0.5 - 0.15;
		triShape[1][1][2] = 0.0;
		triShape[1][2][0] = -0.5;
		triShape[1][2][1] = 0.5 + 0.25;
		triShape[1][2][2] = 0.0;

		triShape[2][0][0] = -0.5 - 0.15;
		triShape[2][0][1] = -0.5 - 0.15;
		triShape[2][0][2] = 0.0;
		triShape[2][1][0] = -0.5 + 0.15;
		triShape[2][1][1] = -0.5 - 0.15;
		triShape[2][1][2] = 0.0;
		triShape[2][2][0] = -0.5;
		triShape[2][2][1] = -0.5 + 0.25;
		triShape[2][2][2] = 0.0;

		triShape[3][0][0] = 0.5 - 0.15;
		triShape[3][0][1] = -0.5 - 0.15;
		triShape[3][0][2] = 0.0;
		triShape[3][1][0] = 0.5 + 0.15;
		triShape[3][1][1] = -0.5 - 0.15;
		triShape[3][1][2] = 0.0;
		triShape[3][2][0] = 0.5;
		triShape[3][2][1] = -0.5 + 0.25;
		triShape[3][2][2] = 0.0;

		for (int i = 0; i < 4; i++)
		{
			colors[i][0][0] = colorDis(gen);
			colors[i][0][1] = colorDis(gen);
			colors[i][0][2] = colorDis(gen);
			colors[i][1][0] = colorDis(gen);
			colors[i][1][1] = colorDis(gen);
			colors[i][1][2] = colorDis(gen);
			colors[i][2][0] = colorDis(gen);
			colors[i][2][1] = colorDis(gen);
			colors[i][2][2] = colorDis(gen);
		}

		start = false;
	}

	//축 그리기
	for (int i = 0; i < 2; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), XYShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glLineWidth(10.0);
		glDrawArrays(GL_LINES, 0, 2);
	}

	//삼각형 그리기
	for (int i = 0; i < 4; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		if (a)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (b)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		
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
	case 'a':
		a = true;
		b = false;
		break;
	case 'b':
		a = false;
		b = true;
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
	double randX, randY;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);

		if (openGLX > 0 && openGLX < 1 && openGLY > 0 && openGLY < 1)
		{
			randX = dis(gen);
			randY = dis(gen);

			triShape[0][0][0] = openGLX - randX;
			triShape[0][0][1] = openGLY - randY;
			triShape[0][0][2] = 0.0;
			triShape[0][1][0] = openGLX + randX;
			triShape[0][1][1] = openGLY - randY;
			triShape[0][1][2] = 0.0;
			triShape[0][2][0] = openGLX;
			triShape[0][2][1] = openGLY + randY;
			triShape[0][2][2] = 0.0;

			colors[0][0][0] = colorDis(gen);
			colors[0][0][1] = colorDis(gen);
			colors[0][0][2] = colorDis(gen);
			colors[0][1][0] = colorDis(gen);
			colors[0][1][1] = colorDis(gen);
			colors[0][1][2] = colorDis(gen);
			colors[0][2][0] = colorDis(gen);
			colors[0][2][1] = colorDis(gen);
			colors[0][2][2] = colorDis(gen);
		}
		else if (openGLX > -1 && openGLX < 0 && openGLY > 0 && openGLY < 1)
		{
			randX = dis(gen);
			randY = dis(gen);

			triShape[1][0][0] = openGLX - randX;
			triShape[1][0][1] = openGLY - randY;
			triShape[1][0][2] = 0.0;
			triShape[1][1][0] = openGLX + randX;
			triShape[1][1][1] = openGLY - randY;
			triShape[1][1][2] = 0.0;
			triShape[1][2][0] = openGLX;
			triShape[1][2][1] = openGLY + randY;
			triShape[1][2][2] = 0.0;

			colors[1][0][0] = colorDis(gen);
			colors[1][0][1] = colorDis(gen);
			colors[1][0][2] = colorDis(gen);
			colors[1][1][0] = colorDis(gen);
			colors[1][1][1] = colorDis(gen);
			colors[1][1][2] = colorDis(gen);
			colors[1][2][0] = colorDis(gen);
			colors[1][2][1] = colorDis(gen);
			colors[1][2][2] = colorDis(gen);
		}
		else if (openGLX > -1 && openGLX < 0 && openGLY > -1 && openGLY < 0)
		{
			randX = dis(gen);
			randY = dis(gen);

			triShape[2][0][0] = openGLX - randX;
			triShape[2][0][1] = openGLY - randY;
			triShape[2][0][2] = 0.0;
			triShape[2][1][0] = openGLX + randX;
			triShape[2][1][1] = openGLY - randY;
			triShape[2][1][2] = 0.0;
			triShape[2][2][0] = openGLX;
			triShape[2][2][1] = openGLY + randY;
			triShape[2][2][2] = 0.0;

			colors[2][0][0] = colorDis(gen);
			colors[2][0][1] = colorDis(gen);
			colors[2][0][2] = colorDis(gen);
			colors[2][1][0] = colorDis(gen);
			colors[2][1][1] = colorDis(gen);
			colors[2][1][2] = colorDis(gen);
			colors[2][2][0] = colorDis(gen);
			colors[2][2][1] = colorDis(gen);
			colors[2][2][2] = colorDis(gen);
		}
		else if (openGLX > 0 && openGLX < 1 && openGLY > -1 && openGLY < 0)
		{
			randX = dis(gen);
			randY = dis(gen);

			triShape[3][0][0] = openGLX - randX;
			triShape[3][0][1] = openGLY - randY;
			triShape[3][0][2] = 0.0;
			triShape[3][1][0] = openGLX + randX;
			triShape[3][1][1] = openGLY - randY;
			triShape[3][1][2] = 0.0;
			triShape[3][2][0] = openGLX;
			triShape[3][2][1] = openGLY + randY;
			triShape[3][2][2] = 0.0;

			colors[3][0][0] = colorDis(gen);
			colors[3][0][1] = colorDis(gen);
			colors[3][0][2] = colorDis(gen);
			colors[3][1][0] = colorDis(gen);
			colors[3][1][1] = colorDis(gen);
			colors[3][1][2] = colorDis(gen);
			colors[3][2][0] = colorDis(gen);
			colors[3][2][1] = colorDis(gen);
			colors[3][2][2] = colorDis(gen);
		}
		
		WindowToOpenGL(x, y, openGLX, openGLY);
	}
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}
