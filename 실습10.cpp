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

double R, G, B;
double theta[5], r[5], centerX[5], centerY[5], lastX[5], lastY[5], distanceX[5], distanceY[5], centerX2[5], centerY2[5], lastTheta[5], lastR[5];
bool RGBchange = false;

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
	glutTimerFunc(10, TimerFunction, 1);
	glutMouseFunc(Mouse);

	glutMainLoop();
}

GLvoid drawScene()
{
	//--- 변경된 배경색 설정
	if (RGBchange)
	{
		R = colorDis(gen);
		G = colorDis(gen);
		B = colorDis(gen);

		RGBchange = false;
	}
	
	glClearColor(R, G, B, 1.0f);
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
				if (j>1 && j < 499)
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
			centerX[i] = dis(gen);
			centerY[i] = dis(gen);
		}
		RGBchange = true;
		dotCnt = 1;
		break;
	case '2':
		for (int i = 0; i < 2; i++)
		{
			centerX[i] = dis(gen);
			centerY[i] = dis(gen);
		}
		RGBchange = true;
		dotCnt = 2;
		break;
	case '3':
		for (int i = 0; i < 3; i++)
		{
			centerX[i] = dis(gen);
			centerY[i] = dis(gen);
		}
		RGBchange = true;
		dotCnt = 3;
		break;
	case '4':
		for (int i = 0; i < 4; i++)
		{
			centerX[i] = dis(gen);
			centerY[i] = dis(gen);
		}
		RGBchange = true;
		dotCnt = 4;
		break;
	case '5':
		for (int i = 0; i < 5; i++)
		{
			centerX[i] = dis(gen);
			centerY[i] = dis(gen);
		}
		RGBchange = true;
		dotCnt = 5;
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
		if (dotCnt < 5)
		{
			centerX[dotCnt] = openGLX;
			centerY[dotCnt] = openGLY;
			RGBchange = true;
			dotCnt++;
		}
	}
	glutPostRedisplay();
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}

GLvoid TimerFunction(int value)
{
	for (int i = 0; i < dotCnt; i++)
	{
		if (cnt[i] < 500)
		{
			cnt[i]++;
		}

		if (cnt[i] < 250)
		{
			theta[i] = cnt[i] * 0.1;
			r[i] = 0.01 + 0.01 * theta[i];

			dotShape[i][cnt[i]][0] = r[i] * cos(theta[i]) + centerX[i];
			dotShape[i][cnt[i]][1] = r[i] * sin(theta[i]) + centerY[i];
			dotShape[i][cnt[i]][2] = 0.0;

			//cnt[i]++;
		}

		if (cnt[i] == 250)
		{
			lastX[i] = dotShape[i][cnt[i] - 1][0];
			lastY[i] = dotShape[i][cnt[i] - 1][1];

			dotShape[i][cnt[i]][0] = dotShape[i][cnt[i] - 1][0];
			dotShape[i][cnt[i]][1] = dotShape[i][cnt[i] - 1][1];
			dotShape[i][cnt[i]][2] = 0.0;

			//cnt[i]++;

			distanceX[i] = lastX[i] - centerX[i];
			distanceY[i] = lastY[i] - centerY[i];

			centerX2[i] = lastX[i] + distanceX[i];
			centerY2[i] = lastY[i] + distanceY[i];

			lastTheta[i] = theta[i];
			lastR[i] = r[i];
		}

		if (cnt[i] > 250 && cnt[i] < 500)
		{
			theta[i] = lastTheta[i] - (cnt[i] - 250) * 0.1;
			r[i] = 0.01 + (-0.01) * theta[i];

			dotShape[i][cnt[i]][0] = r[i] * cos(theta[i]) + centerX2[i];
			dotShape[i][cnt[i]][1] = r[i] * sin(theta[i]) + centerY2[i];
			dotShape[i][cnt[i]][2] = 0.0;

			//cnt[i]++;
		}
	}
	glutPostRedisplay();  // 화면을 갱신하도록 추가
	glutTimerFunc(10, TimerFunction, 1);
}