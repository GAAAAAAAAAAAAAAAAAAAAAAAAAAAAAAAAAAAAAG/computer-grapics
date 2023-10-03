#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLfloat triShape[9][3][3] = {};//--- 삼각형 위치 값
int triCnt = 0;

GLfloat BigTriShape[9][3][3] = {};//--- 삼각형 위치 값

GLfloat XYShape[2][2][3] = {
	{{-1.0,0.0,0.0},{1.0,0.0,0.0}},
	{{0.0,1.0,0.0},{0.0,-1.0,0.0}} };

GLfloat colors[4][3] = { //--- 삼각형 꼭지점 색상
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
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

bool animating = false;
int aniCnt = 0;

bool Lanimation = false;
bool Tanimation = false;
bool Ranimation = false;
bool Panimation = false;

int Lcnt = 0;
int Tcnt = 0;
int Rcnt = 0;
int Pcnt = 0;

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
GLvoid TimerFunction(int value);

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
	glutTimerFunc(100, TimerFunction, 1);

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

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	if (start)
	{
		/*triShape[0][0][0] = -0.5;
		triShape[0][0][1] = -0.5;
		triShape[0][0][2] =  0.0;
		triShape[0][1][0]  = -0.5;
		triShape[0][1][1]  = -0.5;
		triShape[0][1][2]  = 0.0;
		triShape[0][2][0]  = -0.5;
		triShape[0][2][1]  = -0.5;
		triShape[0][2][2]  =  0.0;*/

		//직선 -> 삼각형 
		triShape[0][0][0] = -0.5 - 0.2;
		triShape[0][0][1] = 0.3;
		triShape[0][0][2] = 0.0;
		triShape[0][1][0] = -0.5;
		triShape[0][1][1] = 0.3;	// + 0.4
		triShape[0][1][2] = 0.0;
		triShape[0][2][0] = -0.5 + 0.2;
		triShape[0][2][1] = 0.3;
		triShape[0][2][2] = 0.0;

		//삼각형 -> 사각형
		triShape[1][0][0] = 0.5 - 0.2;
		triShape[1][0][1] = 0.3;
		triShape[1][0][2] =  0.0;
		triShape[1][1][0] = 0.5;	// - 0.2
		triShape[1][1][1] = 0.3 + 0.4;
		triShape[1][1][2] = 0.0;
		triShape[1][2][0] = 0.5 + 0.2;
		triShape[1][2][1] = 0.3;
		triShape[1][2][2] =  0.0;

		triShape[2][0][0] = 0.5;	// - 0.2
		triShape[2][0][1] = 0.3 + 0.4;
		triShape[2][0][2] = 0.0;
		triShape[2][1][0] = 0.5;	// + 0.2
		triShape[2][1][1] = 0.3 + 0.4;
		triShape[2][1][2] = 0.0;
		triShape[2][2][0] = 0.5 + 0.2;
		triShape[2][2][1] = 0.3;
		triShape[2][2][2] = 0.0;

		//사각형 -> 오각형
		triShape[3][0][0] = -0.5 - 0.2;
		triShape[3][0][1] = -0.5 - 0.2;	// + 0.1
		triShape[3][0][2] =  0.0;
		triShape[3][1][0]  = -0.5;
		triShape[3][1][1]  = -0.5 - 0.2;
		triShape[3][1][2]  = 0.0;
		triShape[3][2][0]  = -0.5 - 0.2;	//+ 0.1
		triShape[3][2][1]  = -0.5 + 0.2;
		triShape[3][2][2]  =  0.0;

		triShape[4][0][0] = -0.5 - 0.2;	//+0.1
		triShape[4][0][1] = -0.5 + 0.2;
		triShape[4][0][2] =  0.0;
		triShape[4][1][0]  = -0.5;
		triShape[4][1][1]  = -0.5 - 0.2;
		triShape[4][1][2]  = 0.0;
		triShape[4][2][0]  = -0.5 + 0.2;	//-0.1
		triShape[4][2][1]  = -0.5 + 0.2;
		triShape[4][2][2]  =  0.0;

		triShape[5][0][0] = -0.5;
		triShape[5][0][1] = -0.5 - 0.2;
		triShape[5][0][2] = 0.0;
		triShape[5][1][0] = -0.5 + 0.2;
		triShape[5][1][1] = -0.5 - 0.2;	//+0.1
		triShape[5][1][2] = 0.0;
		triShape[5][2][0] = -0.5 + 0.2;	//-0.1
		triShape[5][2][1] = -0.5 + 0.2;
		triShape[5][2][2] = 0.0;

		//오각형 -> 점
		triShape[6][0][0] = +0.5 - 0.2;			//0.3	+0.2
		triShape[6][0][1] = -0.5 - 0.2 + 0.1;	//-0.6	+0.1
		triShape[6][0][2] = 0.0;
		triShape[6][1][0] = +0.5;				//0.5	
		triShape[6][1][1] = -0.5 - 0.2;			//-0.7	+0.2
		triShape[6][1][2] = 0.0;
		triShape[6][2][0] = +0.5 - 0.2 + 0.1;	//0.4	+0.1
		triShape[6][2][1] = -0.5 + 0.2;			//-0.3	-0.2
		triShape[6][2][2] = 0.0;

		triShape[7][0][0] = +0.5 - 0.2 + 0.1;	//-0.1	+0.1
		triShape[7][0][1] = -0.5 + 0.2;			//		-0.2
		triShape[7][0][2] = 0.0;
		triShape[7][1][0] = +0.5;
		triShape[7][1][1] = -0.5 - 0.2;			//		+0.2
		triShape[7][1][2] = 0.0;
		triShape[7][2][0] = +0.5 + 0.2 - 0.1;	//		-0.1
		triShape[7][2][1] = -0.5 + 0.2;			//		-0.2
		triShape[7][2][2] = 0.0;

		triShape[8][0][0] = +0.5;
		triShape[8][0][1] = -0.5 - 0.2;			//		+0.2
		triShape[8][0][2] = 0.0;
		triShape[8][1][0] = +0.5 + 0.2;			//		-0.2
		triShape[8][1][1] = -0.5 - 0.2 + 0.1;	//		+0.1
		triShape[8][1][2] = 0.0;
		triShape[8][2][0] = +0.5 + 0.2 - 0.1;	//		-0.1
		triShape[8][2][1] = -0.5 + 0.2;			//		-0.2
		triShape[8][2][2] = 0.0;

		// 큰 화면에 나오는 도형들
		//직선 -> 삼각형 
		BigTriShape[0][0][0] = -0.5;
		BigTriShape[0][0][1] = 0.0;
		BigTriShape[0][0][2] = 0.0;
		BigTriShape[0][1][0] = 0.0;
		BigTriShape[0][1][1] = 0.0;	// + 0.8
		BigTriShape[0][1][2] = 0.0;
		BigTriShape[0][2][0] = 0.5;
		BigTriShape[0][2][1] = 0.0;
		BigTriShape[0][2][2] = 0.0;

		//삼각형 -> 사각형
		BigTriShape[1][0][0] = 0.0 - 0.4;
		BigTriShape[1][0][1] = 0.0;
		BigTriShape[1][0][2] = 0.0;
		BigTriShape[1][1][0] = 0.0;	// - 0.4
		BigTriShape[1][1][1] = 0.0 + 0.8;
		BigTriShape[1][1][2] = 0.0;
		BigTriShape[1][2][0] = 0.0 + 0.4;
		BigTriShape[1][2][1] = 0.0;
		BigTriShape[1][2][2] = 0.0;

		BigTriShape[2][0][0] = 0.0;	// - 0.4
		BigTriShape[2][0][1] = 0.0 + 0.8;
		BigTriShape[2][0][2] = 0.0;
		BigTriShape[2][1][0] = 0.0;	// + 0.4
		BigTriShape[2][1][1] = 0.0 + 0.8;
		BigTriShape[2][1][2] = 0.0;
		BigTriShape[2][2][0] = 0.0 + 0.4;
		BigTriShape[2][2][1] = 0.0;
		BigTriShape[2][2][2] = 0.0;

		//사각형 -> 오각형
		BigTriShape[3][0][0] = -0.0 - 0.4;
		BigTriShape[3][0][1] = -0.0 - 0.4;	// + 0.2
		BigTriShape[3][0][2] = 0.0;
		BigTriShape[3][1][0] = -0.0;
		BigTriShape[3][1][1] = -0.0 - 0.4;
		BigTriShape[3][1][2] = 0.0;
		BigTriShape[3][2][0] = -0.0 - 0.4;	//+ 0.2
		BigTriShape[3][2][1] = -0.0 + 0.4;
		BigTriShape[3][2][2] = 0.0;

		BigTriShape[4][0][0] = -0.0 - 0.4;	//+0.2
		BigTriShape[4][0][1] = -0.0 + 0.4;
		BigTriShape[4][0][2] = 0.0;
		BigTriShape[4][1][0] = -0.0;
		BigTriShape[4][1][1] = -0.0 - 0.4;
		BigTriShape[4][1][2] = 0.0;
		BigTriShape[4][2][0] = -0.0 + 0.4;	//-0.2
		BigTriShape[4][2][1] = -0.0 + 0.4;
		BigTriShape[4][2][2] = 0.0;

		BigTriShape[5][0][0] = -0.0;
		BigTriShape[5][0][1] = -0.0 - 0.4;
		BigTriShape[5][0][2] = 0.0;
		BigTriShape[5][1][0] = -0.0 + 0.4;
		BigTriShape[5][1][1] = -0.0 - 0.4;	//+0.2
		BigTriShape[5][1][2] = 0.0;
		BigTriShape[5][2][0] = -0.0 + 0.4;	//-0.2
		BigTriShape[5][2][1] = -0.0 + 0.4;
		BigTriShape[5][2][2] = 0.0;

		//오각형 -> 점
		BigTriShape[6][0][0] = +0.0 - 0.4;			//0.3	+0.2
		BigTriShape[6][0][1] = -0.0 - 0.4 + 0.2;	//-0.6	+0.1
		BigTriShape[6][0][2] = 0.0;
		BigTriShape[6][1][0] = +0.0;				//0.5	
		BigTriShape[6][1][1] = -0.0 - 0.4;			//-0.7	+0.2
		BigTriShape[6][1][2] = 0.0;
		BigTriShape[6][2][0] = +0.0 - 0.4 + 0.2;	//0.4	+0.1
		BigTriShape[6][2][1] = -0.0 + 0.4;			//-0.3	-0.2
		BigTriShape[6][2][2] = 0.0;
	
		BigTriShape[7][0][0] = +0.0 - 0.4 + 0.2;	//-0.1	+0.1
		BigTriShape[7][0][1] = -0.0 + 0.4;			//		-0.2
		BigTriShape[7][0][2] = 0.0;
		BigTriShape[7][1][0] = +0.0;
		BigTriShape[7][1][1] = -0.0 - 0.4;			//		+0.2
		BigTriShape[7][1][2] = 0.0;
		BigTriShape[7][2][0] = +0.0 + 0.4 - 0.2;	//		-0.1
		BigTriShape[7][2][1] = -0.0 + 0.4;			//		-0.2
		BigTriShape[7][2][2] = 0.0;
		
		BigTriShape[8][0][0] = +0.0;
		BigTriShape[8][0][1] = -0.0 - 0.4;			//		+0.2
		BigTriShape[8][0][2] = 0.0;
		BigTriShape[8][1][0] = +0.0 + 0.4;			//		-0.2
		BigTriShape[8][1][1] = -0.0 - 0.4 + 0.2;	//		+0.1
		BigTriShape[8][1][2] = 0.0;
		BigTriShape[8][2][0] = +0.0 + 0.4 - 0.2;	//		-0.1
		BigTriShape[8][2][1] = -0.0 + 0.4;			//		-0.2
		BigTriShape[8][2][2] = 0.0;

		start = false;
	}

	if (!Lanimation && !Tanimation && !Ranimation && !Panimation)
	{
		//축 그리기
		for (int i = 0; i < 2; i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), XYShape[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glLineWidth(1.0);
			glDrawArrays(GL_LINES, 0, 2);
		}

		//삼각형 그리기
		for (int i = 0; i < 9; i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	//큰 화면 도형들 그리기
	if (Lanimation)
	{
		for (int i = 0; i < 1; i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), BigTriShape[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
	if (Tanimation)
	{
		for (int i = 1; i < 3; i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), BigTriShape[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
	if (Ranimation)
	{
		for (int i = 3; i < 6; i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), BigTriShape[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
	if (Panimation)
	{
		for (int i = 6; i < 9; i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), BigTriShape[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
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
		Lanimation = false;
		Tanimation = false;
		Ranimation = false;
		Panimation = false;

		//직선 -> 삼각형 
		triShape[0][0][0] = -0.5 - 0.2;
		triShape[0][0][1] = 0.3;
		triShape[0][0][2] = 0.0;
		triShape[0][1][0] = -0.5;
		triShape[0][1][1] = 0.3;	// + 0.4
		triShape[0][1][2] = 0.0;
		triShape[0][2][0] = -0.5 + 0.2;
		triShape[0][2][1] = 0.3;
		triShape[0][2][2] = 0.0;

		//삼각형 -> 사각형
		triShape[1][0][0] = 0.5 - 0.2;
		triShape[1][0][1] = 0.3;
		triShape[1][0][2] = 0.0;
		triShape[1][1][0] = 0.5;	// - 0.2
		triShape[1][1][1] = 0.3 + 0.4;
		triShape[1][1][2] = 0.0;
		triShape[1][2][0] = 0.5 + 0.2;
		triShape[1][2][1] = 0.3;
		triShape[1][2][2] = 0.0;

		triShape[2][0][0] = 0.5;	// - 0.2
		triShape[2][0][1] = 0.3 + 0.4;
		triShape[2][0][2] = 0.0;
		triShape[2][1][0] = 0.5;	// + 0.2
		triShape[2][1][1] = 0.3 + 0.4;
		triShape[2][1][2] = 0.0;
		triShape[2][2][0] = 0.5 + 0.2;
		triShape[2][2][1] = 0.3;
		triShape[2][2][2] = 0.0;

		//사각형 -> 오각형
		triShape[3][0][0] = -0.5 - 0.2;
		triShape[3][0][1] = -0.5 - 0.2;	// + 0.1
		triShape[3][0][2] = 0.0;
		triShape[3][1][0] = -0.5;
		triShape[3][1][1] = -0.5 - 0.2;
		triShape[3][1][2] = 0.0;
		triShape[3][2][0] = -0.5 - 0.2;	//+ 0.1
		triShape[3][2][1] = -0.5 + 0.2;
		triShape[3][2][2] = 0.0;

		triShape[4][0][0] = -0.5 - 0.2;	//+0.1
		triShape[4][0][1] = -0.5 + 0.2;
		triShape[4][0][2] = 0.0;
		triShape[4][1][0] = -0.5;
		triShape[4][1][1] = -0.5 - 0.2;
		triShape[4][1][2] = 0.0;
		triShape[4][2][0] = -0.5 + 0.2;	//-0.1
		triShape[4][2][1] = -0.5 + 0.2;
		triShape[4][2][2] = 0.0;

		triShape[5][0][0] = -0.5;
		triShape[5][0][1] = -0.5 - 0.2;
		triShape[5][0][2] = 0.0;
		triShape[5][1][0] = -0.5 + 0.2;
		triShape[5][1][1] = -0.5 - 0.2;	//+0.1
		triShape[5][1][2] = 0.0;
		triShape[5][2][0] = -0.5 + 0.2;	//-0.1
		triShape[5][2][1] = -0.5 + 0.2;
		triShape[5][2][2] = 0.0;

		//오각형 -> 점
		triShape[6][0][0] = +0.5 - 0.2;			//0.3	+0.2
		triShape[6][0][1] = -0.5 - 0.2 + 0.1;	//-0.6	+0.1
		triShape[6][0][2] = 0.0;
		triShape[6][1][0] = +0.5;				//0.5	
		triShape[6][1][1] = -0.5 - 0.2;			//-0.7	+0.2
		triShape[6][1][2] = 0.0;
		triShape[6][2][0] = +0.5 - 0.2 + 0.1;	//0.4	+0.1
		triShape[6][2][1] = -0.5 + 0.2;			//-0.3	-0.2
		triShape[6][2][2] = 0.0;

		triShape[7][0][0] = +0.5 - 0.2 + 0.1;	//-0.1	+0.1
		triShape[7][0][1] = -0.5 + 0.2;			//		-0.2
		triShape[7][0][2] = 0.0;
		triShape[7][1][0] = +0.5;
		triShape[7][1][1] = -0.5 - 0.2;			//		+0.2
		triShape[7][1][2] = 0.0;
		triShape[7][2][0] = +0.5 + 0.2 - 0.1;	//		-0.1
		triShape[7][2][1] = -0.5 + 0.2;			//		-0.2
		triShape[7][2][2] = 0.0;

		triShape[8][0][0] = +0.5;
		triShape[8][0][1] = -0.5 - 0.2;			//		+0.2
		triShape[8][0][2] = 0.0;
		triShape[8][1][0] = +0.5 + 0.2;			//		-0.2
		triShape[8][1][1] = -0.5 - 0.2 + 0.1;	//		+0.1
		triShape[8][1][2] = 0.0;
		triShape[8][2][0] = +0.5 + 0.2 - 0.1;	//		-0.1
		triShape[8][2][1] = -0.5 + 0.2;			//		-0.2
		triShape[8][2][2] = 0.0;

		animating = true;
		break;
	case 'l':
		BigTriShape[0][0][0] = -0.5;
		BigTriShape[0][0][1] = 0.0;
		BigTriShape[0][0][2] = 0.0;
		BigTriShape[0][1][0] = 0.0;
		BigTriShape[0][1][1] = 0.0;	// + 0.8
		BigTriShape[0][1][2] = 0.0;
		BigTriShape[0][2][0] = 0.5;
		BigTriShape[0][2][1] = 0.0;
		BigTriShape[0][2][2] = 0.0;

		Lanimation = true;
		Tanimation = false;
		Ranimation = false;
		Panimation = false;
		break;
	case 't':
		//삼각형 -> 사각형
		BigTriShape[1][0][0] = 0.0 - 0.4;
		BigTriShape[1][0][1] = 0.0;
		BigTriShape[1][0][2] = 0.0;
		BigTriShape[1][1][0] = 0.0;	// - 0.4
		BigTriShape[1][1][1] = 0.0 + 0.8;
		BigTriShape[1][1][2] = 0.0;
		BigTriShape[1][2][0] = 0.0 + 0.4;
		BigTriShape[1][2][1] = 0.0;
		BigTriShape[1][2][2] = 0.0;

		BigTriShape[2][0][0] = 0.0;	// - 0.4
		BigTriShape[2][0][1] = 0.0 + 0.8;
		BigTriShape[2][0][2] = 0.0;
		BigTriShape[2][1][0] = 0.0;	// + 0.4
		BigTriShape[2][1][1] = 0.0 + 0.8;
		BigTriShape[2][1][2] = 0.0;
		BigTriShape[2][2][0] = 0.0 + 0.4;
		BigTriShape[2][2][1] = 0.0;
		BigTriShape[2][2][2] = 0.0;

		Lanimation = false;
		Tanimation = true;
		Ranimation = false;
		Panimation = false;
		break;
	case 'r':
		//사각형 -> 오각형
		BigTriShape[3][0][0] = -0.0 - 0.4;
		BigTriShape[3][0][1] = -0.0 - 0.4;	// + 0.2
		BigTriShape[3][0][2] = 0.0;
		BigTriShape[3][1][0] = -0.0;
		BigTriShape[3][1][1] = -0.0 - 0.4;
		BigTriShape[3][1][2] = 0.0;
		BigTriShape[3][2][0] = -0.0 - 0.4;	//+ 0.2
		BigTriShape[3][2][1] = -0.0 + 0.4;
		BigTriShape[3][2][2] = 0.0;

		BigTriShape[4][0][0] = -0.0 - 0.4;	//+0.2
		BigTriShape[4][0][1] = -0.0 + 0.4;
		BigTriShape[4][0][2] = 0.0;
		BigTriShape[4][1][0] = -0.0;
		BigTriShape[4][1][1] = -0.0 - 0.4;
		BigTriShape[4][1][2] = 0.0;
		BigTriShape[4][2][0] = -0.0 + 0.4;	//-0.2
		BigTriShape[4][2][1] = -0.0 + 0.4;
		BigTriShape[4][2][2] = 0.0;

		BigTriShape[5][0][0] = -0.0;
		BigTriShape[5][0][1] = -0.0 - 0.4;
		BigTriShape[5][0][2] = 0.0;
		BigTriShape[5][1][0] = -0.0 + 0.4;
		BigTriShape[5][1][1] = -0.0 - 0.4;	//+0.2
		BigTriShape[5][1][2] = 0.0;
		BigTriShape[5][2][0] = -0.0 + 0.4;	//-0.2
		BigTriShape[5][2][1] = -0.0 + 0.4;
		BigTriShape[5][2][2] = 0.0;

		Lanimation = false;
		Tanimation = false;
		Ranimation = true;
		Panimation = false;
		break;
	case 'p':
		//오각형 -> 점
		BigTriShape[6][0][0] = +0.0 - 0.4;			//0.3	+0.2
		BigTriShape[6][0][1] = -0.0 - 0.4 + 0.2;	//-0.6	+0.1
		BigTriShape[6][0][2] = 0.0;
		BigTriShape[6][1][0] = +0.0;				//0.5	
		BigTriShape[6][1][1] = -0.0 - 0.4;			//-0.7	+0.2
		BigTriShape[6][1][2] = 0.0;
		BigTriShape[6][2][0] = +0.0 - 0.4 + 0.2;	//0.4	+0.1
		BigTriShape[6][2][1] = -0.0 + 0.4;			//-0.3	-0.2
		BigTriShape[6][2][2] = 0.0;

		BigTriShape[7][0][0] = +0.0 - 0.4 + 0.2;	//-0.1	+0.1
		BigTriShape[7][0][1] = -0.0 + 0.4;			//		-0.2
		BigTriShape[7][0][2] = 0.0;
		BigTriShape[7][1][0] = +0.0;
		BigTriShape[7][1][1] = -0.0 - 0.4;			//		+0.2
		BigTriShape[7][1][2] = 0.0;
		BigTriShape[7][2][0] = +0.0 + 0.4 - 0.2;	//		-0.1
		BigTriShape[7][2][1] = -0.0 + 0.4;			//		-0.2
		BigTriShape[7][2][2] = 0.0;

		BigTriShape[8][0][0] = +0.0;
		BigTriShape[8][0][1] = -0.0 - 0.4;			//		+0.2
		BigTriShape[8][0][2] = 0.0;
		BigTriShape[8][1][0] = +0.0 + 0.4;			//		-0.2
		BigTriShape[8][1][1] = -0.0 - 0.4 + 0.2;	//		+0.1
		BigTriShape[8][1][2] = 0.0;
		BigTriShape[8][2][0] = +0.0 + 0.4 - 0.2;	//		-0.1
		BigTriShape[8][2][1] = -0.0 + 0.4;			//		-0.2
		BigTriShape[8][2][2] = 0.0;

		Lanimation = false;
		Tanimation = false;
		Ranimation = false;
		Panimation = true;
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
	}
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}

GLvoid TimerFunction(int value)
{
	if (animating)
	{
		//1
		triShape[0][1][1] += 0.02;

		//2
		triShape[1][1][0] -= 0.01;	// - 0.2
		triShape[2][0][0] -= 0.01;	// - 0.2
		triShape[2][1][0] += 0.01;	// + 0.2
	
		//3
		triShape[3][0][1] += 0.005;	//+0.1
		triShape[3][2][0] += 0.005;	//+0.1
		triShape[4][0][0] += 0.005;	//+0.1
		triShape[4][2][0] -= 0.005;	//-0.1
		triShape[5][1][1] += 0.005;	//+0.1
		triShape[5][2][0] -= 0.005;	//-0.1
		
		//4
		triShape[6][0][0] += 0.01;	//	+0.2
		triShape[6][0][1] += 0.005;	//	+0.1
		triShape[6][1][1] += 0.01;	//	+0.2
		triShape[6][2][0] += 0.005;	//	+0.1
		triShape[6][2][1] -= 0.01;	//	-0.2

		triShape[7][0][0] += 0.005;	//  +0.1
		triShape[7][0][1] -= 0.01;	//	-0.2
		triShape[7][1][1] += 0.01;	//	+0.2
		triShape[7][2][0] -= 0.005;	//	-0.1
		triShape[7][2][1] -= 0.01;	//	-0.2

		triShape[8][0][1] += 0.01;	//	+0.2
		triShape[8][1][0] -= 0.01;	//	-0.2
		triShape[8][1][1] += 0.005;	//	+0.1
		triShape[8][2][0] -= 0.005;	//	-0.1
		triShape[8][2][1] -= 0.01;	//	-0.2

		aniCnt++;
		if (aniCnt > 19)
		{
			triShape[6][0][0] -= 0.01;	//	+0.2
			triShape[6][0][1] -= 0.005;	//	+0.1
			triShape[6][1][1] -= 0.01;	//	+0.2
			triShape[6][2][0] -= 0.005;	//	+0.1
			triShape[6][2][1] += 0.01;	//	-0.2

			triShape[7][0][0] -= 0.005;	//  +0.1
			triShape[7][0][1] += 0.01;	//	-0.2
			triShape[7][1][1] -= 0.01;	//	+0.2
			triShape[7][2][0] += 0.005;	//	-0.1
			triShape[7][2][1]+= 0.01;	//	-0.2

			triShape[8][0][1] -= 0.01;	//	+0.2
			triShape[8][1][0] += 0.01;	//	-0.2
			triShape[8][1][1] -= 0.005;	//	+0.1
			triShape[8][2][0] += 0.005;	//	-0.1
			triShape[8][2][1] += 0.01;	//	-0.2

			animating = false;
			aniCnt = 0;
		}
	}
	if (Lanimation)
	{
		BigTriShape[0][1][1] += 0.02 * 2;
		Lcnt++;
		if (Lcnt > 19)
		{
			Lanimation = false;
			Lcnt = 0;
		}
	}
	if (Tanimation)
	{
		//2
		BigTriShape[1][1][0] -= 0.01*2;	// - 0.2
		BigTriShape[2][0][0] -= 0.01*2;	// - 0.2
		BigTriShape[2][1][0] += 0.01*2;	// + 0.2
		Tcnt++;
		if (Tcnt > 19)
		{
			Tanimation = false;
			Tcnt = 0;
		}
	}
	if (Ranimation)
	{
		//3
		BigTriShape[3][0][1] += 0.005*2;	//+0.1
		BigTriShape[3][2][0] += 0.005 * 2;	//+0.1
		BigTriShape[4][0][0] += 0.005 * 2;	//+0.1
		BigTriShape[4][2][0] -= 0.005 * 2;	//-0.1
		BigTriShape[5][1][1] += 0.005 * 2;	//+0.1
		BigTriShape[5][2][0] -= 0.005 * 2;	//-0.1
		Rcnt++;
		if (Rcnt > 19)
		{
			Ranimation = false;
			Rcnt = 0;
		}
	}
	if (Panimation)
	{
		//4
		BigTriShape[6][0][0] += 0.01*2;	//	+0.2
		BigTriShape[6][0][1] += 0.005 * 2;	//	+0.1
		BigTriShape[6][1][1] += 0.01 * 2;	//	+0.2
		BigTriShape[6][2][0] += 0.005 * 2;	//	+0.1
		BigTriShape[6][2][1] -= 0.01 * 2;	//	-0.2
		
		BigTriShape[7][0][0] += 0.005 * 2;	//  +0.1
		BigTriShape[7][0][1] -= 0.01 * 2;	//	-0.2
		BigTriShape[7][1][1] += 0.01 * 2;	//	+0.2
		BigTriShape[7][2][0] -= 0.005 * 2;	//	-0.1
		BigTriShape[7][2][1] -= 0.01 * 2;	//	-0.2
		
		BigTriShape[8][0][1] += 0.01 * 2;	//	+0.2
		BigTriShape[8][1][0] -= 0.01 * 2;	//	-0.2
		BigTriShape[8][1][1] += 0.005 * 2;	//	+0.1
		BigTriShape[8][2][0] -= 0.005 * 2;	//	-0.1
		BigTriShape[8][2][1] -= 0.01 * 2;	//	-0.2

		Pcnt++;
		if (Pcnt > 19)
		{
			BigTriShape[6][0][0] -= 0.01 * 2;	//	+0.2
			BigTriShape[6][0][1] -= 0.005 * 2;	//	+0.1
			BigTriShape[6][1][1] -= 0.01 * 2;	//	+0.2
			BigTriShape[6][2][0] -= 0.005 * 2;	//	+0.1
			BigTriShape[6][2][1] += 0.01 * 2;	//	-0.2
			
			BigTriShape[7][0][0] -= 0.005 * 2;	//  +0.1
			BigTriShape[7][0][1] += 0.01 * 2;	//	-0.2
			BigTriShape[7][1][1] -= 0.01 * 2;	//	+0.2
			BigTriShape[7][2][0] += 0.005 * 2;	//	-0.1
			BigTriShape[7][2][1] += 0.01 * 2;	//	-0.2
			
			BigTriShape[8][0][1] -= 0.01 * 2;	//	+0.2
			BigTriShape[8][1][0] += 0.01 * 2;	//	-0.2
			BigTriShape[8][1][1] -= 0.005 * 2;	//	+0.1
			BigTriShape[8][2][0] += 0.005 * 2;	//	-0.1
			BigTriShape[8][2][1] += 0.01 * 2;	//	-0.2

			Panimation = false;
			Pcnt = 0;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}