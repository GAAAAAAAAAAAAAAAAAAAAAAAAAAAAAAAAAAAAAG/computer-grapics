#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLfloat triShape[8][3][3] = {};//--- 삼각형 위치 값

GLfloat rectShape[1][4][3] = {}; //--- 사각형 위치 값

GLfloat lineShape[10][2][3] = {
	{{0.0,0.75,0.0},{-0.25,0.25,0.0} },
	{{-0.25,0.25,0.0},{-0.75,0.25,0.0}},
	{{-0.75,0.25,0.0},{-0.5,-0.25,0.0}},
	{{-0.5,-0.25,0.0},{-0.5,-0.75,0.0}},
	{{-0.5,-0.75,0.0},{0.0,-0.5,0.0} },
	{{0.0,-0.5,0.0},{0.5,-0.75,0.0}},
	{{0.5,-0.75,0.0}, {0.5,-0.25,0.0}},
	{{0.5,-0.25,0.0},{0.75,0.25,0.0}},
	{{0.75,0.25,0.0},{0.25,0.25,0.0}},
	{{0.25,0.25,0.0},{0.0,0.75,0.0}}
};

GLfloat dotShape[2][3] = {
	{-0.05,0.5,0.0},{0.05,0.5,0.0 } };

GLfloat colors[4][3] = { //--- 삼각형 꼭지점 색상
	{ 0.0, 0.0, 0.0 },
	{ 1.0, 1.0, 0.0 },
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
int clickNum1 = -1;
int clickNum2 = -1;

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
	glutMotionFunc(Motion);

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
	for (int i = 0; i <2; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), dotShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glPointSize(10.0);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	//선 그리기
	for (int i = 0; i <10; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), lineShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glLineWidth(1.0);
		glDrawArrays(GL_LINES, 0, 2);
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
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid Mouse(int button, int state, int x, int y)
{
	float openGLX, openGLY;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			WindowToOpenGL(x, y, openGLX, openGLY);

			//꼭짓점 이동
			for (int i = 0; i < 10; i++)
			{
				if ((lineShape[i][0][0] + 0.05) > openGLX && (lineShape[i][0][0] - 0.05) < openGLX && (lineShape[i][0][1] + 0.05) > openGLY && (lineShape[i][0][1] - 0.05) < openGLY)
				{
					movingRectangle = 0;
					clickNum1 = i;
					clickNum2 = i-1;

					if (clickNum1 == 0)
					{
						clickNum2 = 9;
					}

					movingRectangle = 0;
					break;
				}
			}

			//도형 이동
			
			//삼각형 값 대입
			/*triShape[0][0][0] = lineShape[0][0][0];
			triShape[0][0][1] = lineShape[0][0][1];
			triShape[0][0][2] = 0.0;
			triShape[0][1][0] = lineShape[0][1][0];
			triShape[0][1][1] = lineShape[0][1][1];
			triShape[0][1][2] = 0.0;
			triShape[0][2][0] = lineShape[9][0][0];
			triShape[0][2][1] = lineShape[9][0][1];
			triShape[0][2][2] = 0.0;*/

			triShape[0][0][0] = lineShape[0][0][0];
			triShape[0][0][1] = lineShape[0][0][1];
			triShape[0][0][2] = 0.0;
			triShape[0][1][0] = lineShape[0][1][0];
			triShape[0][1][1] = lineShape[0][1][1];
			triShape[0][1][2] = 0.0;
			triShape[0][2][0] = lineShape[9][0][0];
			triShape[0][2][1] = lineShape[9][0][1];
			triShape[0][2][2] = 0.0;


		}
		else if (state == GLUT_UP)
		{
			movingRectangle = -1;  // 마우스 버튼을 놓으면 이동 모드를 해제합니다.
			/*clickNum1 = -2;
			clickNum2 = -1;*/
		}
	}
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}

GLvoid Motion(int x, int y)
{
	if (movingRectangle == 0)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);


		// 이동 중인 사각형의 중심점을 계산
		float centerX1 = lineShape[clickNum1][0][0];
		float centerY1 = lineShape[clickNum1][0][1];

		float centerX2 = lineShape[clickNum2][1][0];
		float centerY2 = lineShape[clickNum2][1][1];

		// 이동한 거리 계산
		float deltaX1 = openGLX - centerX1;
		float deltaY1 = openGLY - centerY1;

		float deltaX2 = openGLX - centerX2;
		float deltaY2 = openGLY - centerY2;

		// 사각형 위치 이동
		lineShape[clickNum1][0][0] += deltaX1;
		lineShape[clickNum1][0][1] += deltaY1;

		lineShape[clickNum2][1][0] += deltaX2;
		lineShape[clickNum2][1][1] += deltaY2;

		glutPostRedisplay();  // 화면을 다시 그립니다.
	}
	else if (movingRectangle == 1)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);

		glutPostRedisplay();  // 화면을 다시 그립니다.
	}
}