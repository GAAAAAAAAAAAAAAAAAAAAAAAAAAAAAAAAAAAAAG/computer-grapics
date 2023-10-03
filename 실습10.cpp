#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <cmath>
#include <random>

using namespace std;
random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> dis(-1.0, 1.0);
uniform_real_distribution<double> colorDis(0.0, 1.0);

GLfloat dotShape[5][500][3] = {};
int dotCnt =0;

bool Pselect = true;
bool Lselect = false;
int cnt[5];

GLfloat colors[4][3] = { //--- 삼각형 꼭지점 색상
	{ 0.0, 0.0, 0.0 },
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

bool drawAsPoint = true;
int numSpirals = 3;
bool mouseClicked = false;
float clickX, clickY;
double r, g, b;

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
void drawSpiral(int dotCnt, double centerX, double centerY);
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
	glutTimerFunc(100, TimerFunction, 1);
	glutMouseFunc(Mouse);

	glutMainLoop();
}

GLvoid drawScene()
{
	//--- 변경된 배경색 설정
	r = colorDis(gen);
	g = colorDis(gen);
	b= colorDis(gen);

	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);
	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//점 찍기
	for (int i = 0; i < dotCnt; i++)
	{
		for (int j = 0; j < 500; j++)
		{
			if (Pselect)
			{
				glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
				glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), dotShape[i][j], GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(0);
				glPointSize(2.0);
				glDrawArrays(GL_POINTS, 0, 1);
			}
			else if (Lselect)
			{
				if (j < 499)
				{
					glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
					glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), dotShape[i][j], GL_STATIC_DRAW);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(0);
					glLineWidth(1.0);
					glDrawArrays(GL_LINES, 0, 2);
				}
			}
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
	case 'p':
		Pselect = true;
		Lselect = false;
		break;
	case'l':
		Pselect = false;
		Lselect = true;
		break;
	case '1':
		for (int i = 0; i <1; i++)
		{
			double randomX, randomY;
			randomX = dis(gen);
			randomY = dis(gen);
			drawSpiral(i, randomX, randomY);
			
		}
		dotCnt = 1;
		drawScene();
		break;
	case '2':
		for (int i = 0; i < 2; i++)
		{
			double randomX, randomY;
			randomX = dis(gen);
			randomY = dis(gen);
			drawSpiral(i, randomX, randomY);
		}
		dotCnt = 2;
		drawScene();
		break;
	case '3':
		for (int i = 0; i < 3; i++)
		{
			double randomX, randomY;
			randomX = dis(gen);
			randomY = dis(gen);
			drawSpiral(i, randomX, randomY);
		}
		dotCnt = 3;
		drawScene();
		break;
	case '4':
		for (int i = 0; i < 4; i++)
		{
			double randomX, randomY;
			randomX = dis(gen);
			randomY = dis(gen);
			drawSpiral(i, randomX, randomY);
			 
		}
		dotCnt = 4;
		drawScene();
		break;
	case '5':
		for (int i = 0; i < 5; i++)
		{
			double randomX, randomY;
			randomX = dis(gen);
			randomY = dis(gen);
			drawSpiral(i, randomX, randomY);	 
		}
		dotCnt = 5;
		drawScene();
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
		
		drawSpiral(dotCnt,openGLX, openGLY);
		dotCnt++;
	}
	glutPostRedisplay();
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}

void drawSpiral(int dotCnt, double centerX, double centerY)
{
	GLfloat theta, r;
	cnt[dotCnt] = 0;

	while (cnt[dotCnt] < 250)
	{
		theta = cnt[dotCnt] * 0.1;
		r = 0.01 + 0.01 * theta;

		dotShape[dotCnt][cnt[dotCnt]][0] = r * cos(theta) + centerX;
		dotShape[dotCnt][cnt[dotCnt]][1] = r * sin(theta) + centerY;
		dotShape[dotCnt][cnt[dotCnt]][2] = 0.0;

		cnt[dotCnt]++;
	}

	double lastX, lastY;
	lastX = dotShape[dotCnt][cnt[dotCnt] - 1][0];
	lastY = dotShape[dotCnt][cnt[dotCnt] - 1][1];

	dotShape[dotCnt][cnt[dotCnt]][0] = dotShape[dotCnt][cnt[dotCnt] - 1][0];
	dotShape[dotCnt][cnt[dotCnt]][1] = dotShape[dotCnt][cnt[dotCnt] - 1][1];
	dotShape[dotCnt][cnt[dotCnt]][2] = 0.0;

	cnt[dotCnt]++;

	double distanceX, distanceY;
	distanceX = lastX - centerX;
	distanceY = lastY - centerY;

	double centerX2, centerY2;
	centerX2 = lastX + distanceX;
	centerY2 = lastY + distanceY;

	double lastTheta = theta;
	double lastR = r;

	while (cnt[dotCnt] < 500)
	{
		theta = lastTheta - (cnt[dotCnt]-250) * 0.1;
		r = 0.01 + (- 0.01) * theta;

		dotShape[dotCnt][cnt[dotCnt]][0] = r * cos(theta) + centerX2;
		dotShape[dotCnt][cnt[dotCnt]][1] = r * sin(theta) + centerY2;
		dotShape[dotCnt][cnt[dotCnt]][2] = 0.0;

		cnt[dotCnt]++;
	}
}

GLvoid TimerFunction(int value)
{
	glutTimerFunc(100, TimerFunction, 1);
}