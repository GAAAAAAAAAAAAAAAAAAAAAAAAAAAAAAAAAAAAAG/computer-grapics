#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <math.h>

using namespace std;
random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> dis(-1.0,1.0);


struct TRISHAPE
{
	GLfloat triShape[3][3];
	GLfloat cX, cY;
	bool alive = false;
	bool animation = false;
	int direction = 0;
};
TRISHAPE t[10];
int Tcnt = 3;

struct RECTSHAPE
{
	GLfloat rectShape[4][3];
	GLfloat cX, cY;
	bool alive = false;
	bool animation = false;
	int direction = 0;
};
RECTSHAPE r[10];
int Rcnt = 3;

struct PENTASHAPE
{
	GLfloat pentaShape[5][3];
	GLfloat cX, cY;
	bool alive = false;
	bool animation = false;
	int direction = 0;
};
PENTASHAPE p[10];
int Pcnt = 3;

struct LINESHAPE
{
	GLfloat lineShape[2][3];
	GLfloat cX, cY;
	bool alive = false;
	bool animation = false;
	int direction = 0;
};
LINESHAPE l[10];
int Lcnt = 3;

struct DOTSHAPE
{
	GLfloat dotShape[3];
	GLfloat cX, cY;
	bool alive = false;
	bool animation = false;
	int direction = 0;
};
DOTSHAPE d[10];
int Dcnt = 3;


GLfloat crashLine[2][3] = {};
GLfloat testLine[2][3] = {};

//GLfloat triShape[10][3][3] = {};//--- �ﰢ�� ��ġ ��
//int triCnt = 0;
//
//GLfloat rectShape[10][4][3] = {}; //--- �簢�� ��ġ ��
//int rectShapeCnt = 0;
//
//GLfloat pentaShape[10][5][3] = {};	//--- ������ ��ġ ��
//int pentaCnt = 0;
//
//GLfloat lineShape[10][2][3] = {};	//--- ���� ��ġ ��
//int lineCnt = 0;
//
//GLfloat dotShape[10][3] = {};	//--- �� ��ġ ��
//int dotCnt = 0;

GLfloat colors[4][3] = { //--- �ﰢ�� ������ ����
	{ 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 0.0 }
};

GLuint vao, vbo[2];
GLuint TriPosVbo, TriColorVbo;

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

int windowWidth = 800;
int windowHeight = 800;

float openGLX, openGLY;
int movingRectangle = -1;

bool start = true;
double Size = 0.075;
double deviation = 0.05;
double moveSize = 0.01;

double cx1, cy1, cx2, cy2;
double ra[5], rb[5];
double abDistance;
double Distance;
int vertex1, vertex2;

int select1, select2;

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
int checkCollision();
BOOL Collision(float a_x1, float a_x2, float a_y1, float a_y2, float b_x1, float b_x2, float b_y1, float b_y2);
int collision(int vertex1, int selectNum1, int vertex2, int selectNum2);

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Example1");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();

	//--- ���̴� �о�ͼ� ���̴� ���α׷� �����
	make_shaderProgram(); //--- ���̴� ���α׷� �����
	InitBuffer();

	glutTimerFunc(10, TimerFunction, 1);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	glutMainLoop();
}

GLvoid drawScene()
{
	//--- ����� ���� ����
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);
	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao);

	if (start)
	{
		int cnt = 0;

		while (cnt < 3)
		{
			double centerX, centerY;
			centerX = dis(gen);
			centerY = dis(gen);

			d[cnt].dotShape[0] = centerX;
			d[cnt].dotShape[1] = centerY;
			d[cnt].cX = centerX;
			d[cnt].cY = centerY;
			d[cnt].alive = true;

			centerX = dis(gen);
			centerY = dis(gen);
			l[cnt].lineShape[0][0] = centerX-Size;
			l[cnt].lineShape[0][1] = centerY;
			l[cnt].lineShape[1][0] = centerX + Size;
			l[cnt].lineShape[1][1] = centerY;
			l[cnt].cX = centerX;
			l[cnt].cY = centerY;
			l[cnt].alive = true;

			centerX = dis(gen);
			centerY = dis(gen);
			t[cnt].triShape[0][0] = centerX - Size;
			t[cnt].triShape[0][1] = centerY - Size;
			t[cnt].triShape[1][0] = centerX + Size;
			t[cnt].triShape[1][1] = centerY - Size;
			t[cnt].triShape[2][0] = centerX;
			t[cnt].triShape[2][1] = centerY + Size;
			t[cnt].cX = centerX;
			t[cnt].cY = centerY;
			t[cnt].alive = true;

			centerX = dis(gen);
			centerY = dis(gen);
			r[cnt].rectShape[0][0] = centerX - Size;
			r[cnt].rectShape[0][1] = centerY - Size;
			r[cnt].rectShape[1][0] = centerX + Size;
			r[cnt].rectShape[1][1] = centerY - Size;
			r[cnt].rectShape[2][0] = centerX + Size;
			r[cnt].rectShape[2][1] = centerY + Size;
			r[cnt].rectShape[3][0] = centerX - Size;
			r[cnt].rectShape[3][1] = centerY + Size;
			r[cnt].cX = centerX;
			r[cnt].cY = centerY;
			r[cnt].alive = true;

			centerX = dis(gen);
			centerY = dis(gen);
			p[cnt].pentaShape[0][0] = centerX - Size;
			p[cnt].pentaShape[0][1] = centerY - Size;
			p[cnt].pentaShape[1][0] = centerX + Size;
			p[cnt].pentaShape[1][1] = centerY - Size;
			p[cnt].pentaShape[2][0] = centerX + Size +Size/2;
			p[cnt].pentaShape[2][1] = centerY + Size/2;
			p[cnt].pentaShape[3][0] = centerX;
			p[cnt].pentaShape[3][1] = centerY + Size + Size/2;
			p[cnt].pentaShape[4][0] = centerX - Size - Size/2;
			p[cnt].pentaShape[4][1] = centerY + Size/2;
			p[cnt].cX = centerX;
			p[cnt].cY = centerY;
			p[cnt].alive = true;

			cnt++;
		}
		start = false;
	}

	// ���� �ٲٱ�
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// �� ���
	for (int i = 0; i < Dcnt; i++)
	{
		if (d[i].alive)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), d[i].dotShape, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glPointSize(10.0);
			glDrawArrays(GL_POINTS, 0, 1);
		}
	}
	

	// �� �׸���
	for (int i = 0; i < Lcnt; i++)
	{
		if (l[i].alive)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), l[i].lineShape, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glLineWidth(10.0);
			glDrawArrays(GL_LINES, 0, 2);
		}
	}
	

	// �ﰢ�� �׸���
	for (int i = 0; i < Tcnt; i++)
	{
		if (t[i].alive)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), t[i].triShape, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
	

	// �簢�� �׸���
	for (int i = 0; i < Rcnt; i++)
	{
		if (r[i].alive)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), r[i].rectShape, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_QUADS, 0, 4);
		}
	}

	// ������ �׸���
	for (int i = 0; i < Pcnt; i++)
	{
		if (p[i].alive)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), p[i].pentaShape, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_POLYGON, 0, 5);
		}
	}

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
}

void make_shaderProgram()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(shaderProgramID);
}

void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cout << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cout << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
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
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

int movingMouse = -1;
float beforeX, beforeY;
int crash = 0;
bool crashStart = false;
int vertex = 0;
int selectNum = 0;

GLvoid Mouse(int button, int state, int x, int y)
{
	float openGLX, openGLY;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			WindowToOpenGL(x, y, openGLX, openGLY);

			for (int i = 0; i < 10; i++)
			{
				if (t[i].cX - 0.07 < openGLX && t[i].cX + 0.07 > openGLX && t[i].cY - 0.07 < openGLY && t[i].cY + 0.07 > openGLY && t[i].alive)
				{
					vertex = 3;
					selectNum = i;
					movingMouse = 0;
					beforeX = openGLX;
					beforeY = openGLY;
					break;
				}
				else if (r[i].cX - 0.07 < openGLX && r[i].cX + 0.07 > openGLX && r[i].cY - 0.07 < openGLY && r[i].cY + 0.07 > openGLY && r[i].alive)
				{
					vertex = 4;
					selectNum = i;
					movingMouse = 0;
					beforeX = openGLX;
					beforeY = openGLY;
					break;
				}
				else if (p[i].cX - 0.07 < openGLX && p[i].cX + 0.07 > openGLX && p[i].cY - 0.07 < openGLY && p[i].cY + 0.07 > openGLY && p[i].alive)
				{
					vertex = 5;
					selectNum = i;
					movingMouse = 0;
					beforeX = openGLX;
					beforeY = openGLY;
					break;
				}
				else if (d[i].cX - 0.07 < openGLX && d[i].cX + 0.07 > openGLX && d[i].cY - 0.07 < openGLY && d[i].cY + 0.07 > openGLY && d[i].alive)
				{
					vertex = 1;
					selectNum = i;
					movingMouse = 0;
					beforeX = openGLX;
					beforeY = openGLY;
					break;
				}
				else if (l[i].cX - 0.07 < openGLX && l[i].cX + 0.07 > openGLX && l[i].cY - 0.07 < openGLY && l[i].cY + 0.07 > openGLY && l[i].alive)
				{
					vertex = 2;
					selectNum = i;
					movingMouse = 0;
					beforeX = openGLX;
					beforeY = openGLY;
					break;
				}
			}

			//��ġ��
			switch (vertex)
			{
			case 1:
				for (int i = 1; i <= 5; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						switch (i)
						{
						case 1:
							if (d[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									d[selectNum].alive = false;
									d[j].alive = false;

									l[Lcnt].alive = true;
									l[Lcnt].animation = true;
									l[Lcnt].cX = d[selectNum].cX;
									l[Lcnt].cY = d[selectNum].cY;
									
									l[Lcnt].lineShape[0][0] = l[Lcnt].cX - Size;
									l[Lcnt].lineShape[0][1] = l[Lcnt].cY;
									l[Lcnt].lineShape[1][0] = l[Lcnt].cX + Size;
									l[Lcnt].lineShape[1][1] = l[Lcnt].cY;

									Lcnt++;
								}
							}
							break;
						case 2:
							if (l[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									d[selectNum].alive = false;
									l[j].alive = false;

									t[Tcnt].cX = d[selectNum].cX;
									t[Tcnt].cY = d[selectNum].cY;

									t[Tcnt].triShape[0][0] = t[Tcnt].cX - Size;
									t[Tcnt].triShape[0][1] = t[Tcnt].cY - Size;
									t[Tcnt].triShape[1][0] = t[Tcnt].cX + Size;
									t[Tcnt].triShape[1][1] = t[Tcnt].cY - Size;
									t[Tcnt].triShape[2][0] = t[Tcnt].cX;
									t[Tcnt].triShape[2][1] = t[Tcnt].cY + Size;
									
									t[Tcnt].alive = true;
									t[Tcnt].animation = true;

									Tcnt++;
								}
							}
							break;
						case 3:
							if (t[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									d[selectNum].alive = false;
									t[j].alive = false;

									r[Rcnt].cX = d[selectNum].cX;
									r[Rcnt].cY = d[selectNum].cY;

									r[Rcnt].rectShape[0][0] = r[Rcnt].cX - Size;
									r[Rcnt].rectShape[0][1] = r[Rcnt].cY - Size;
									r[Rcnt].rectShape[1][0] = r[Rcnt].cX + Size;
									r[Rcnt].rectShape[1][1] = r[Rcnt].cY - Size;
									r[Rcnt].rectShape[2][0] = r[Rcnt].cX + Size;
									r[Rcnt].rectShape[2][1] = r[Rcnt].cY + Size;
									r[Rcnt].rectShape[3][0] = r[Rcnt].cX - Size;
									r[Rcnt].rectShape[3][1] = r[Rcnt].cY + Size;
									
									r[Rcnt].alive = true;
									r[Rcnt].animation = true;

									Rcnt++;
								}
							}
							break;
						case 4:
							if (r[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									d[selectNum].alive = false;
									r[j].alive = false;

									p[Pcnt].cX = d[selectNum].cX;
									p[Pcnt].cY = d[selectNum].cY;

									p[Pcnt].pentaShape[0][0] = p[Pcnt].cX - Size;
									p[Pcnt].pentaShape[0][1] = p[Pcnt].cY - Size;
									p[Pcnt].pentaShape[1][0] = p[Pcnt].cX + Size;
									p[Pcnt].pentaShape[1][1] = p[Pcnt].cY - Size;
									p[Pcnt].pentaShape[2][0] = p[Pcnt].cX + Size + Size / 2;
									p[Pcnt].pentaShape[2][1] = p[Pcnt].cY + Size / 2;
									p[Pcnt].pentaShape[3][0] = p[Pcnt].cX;
									p[Pcnt].pentaShape[3][1] = p[Pcnt].cY + Size + Size / 2;
									p[Pcnt].pentaShape[4][0] = p[Pcnt].cX - Size - Size / 2;
									p[Pcnt].pentaShape[4][1] = p[Pcnt].cY + Size / 2;
									
									p[Pcnt].alive = true;
									p[Pcnt].animation = true;

									Pcnt++;
								}
							}
							break;
						case 5:
							if (p[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									d[selectNum].alive = false;
									p[j].alive = false;

									d[Dcnt].cX = d[selectNum].cX;
									d[Dcnt].cY = d[selectNum].cY;

									d[Dcnt].dotShape[0] = d[Dcnt].cX;
									d[Dcnt].dotShape[1] = d[Dcnt].cY;
									
									d[Dcnt].alive = true;
									d[Dcnt].animation = true;

									Dcnt++;
								}
							}
						}

					}
				}
				break;
			case 2:
				for (int i = 1; i <= 5; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						switch (i)
						{
						case 1:
							if (d[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									l[selectNum].alive = false;
									d[j].alive = false;

									t[Tcnt].cX = l[selectNum].cX;
									t[Tcnt].cY = l[selectNum].cY;

									t[Tcnt].triShape[0][0] = t[Tcnt].cX - Size;
									t[Tcnt].triShape[0][1] = t[Tcnt].cY - Size;
									t[Tcnt].triShape[1][0] = t[Tcnt].cX + Size;
									t[Tcnt].triShape[1][1] = t[Tcnt].cY - Size;
									t[Tcnt].triShape[2][0] = t[Tcnt].cX;
									t[Tcnt].triShape[2][1] = t[Tcnt].cY + Size;

									t[Tcnt].alive = true;
									t[Tcnt].animation = true;

									Tcnt++;
								}
							}
							break;
						case 2:
							if (l[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									l[selectNum].alive = false;
									l[j].alive = false;

									r[Rcnt].cX = l[selectNum].cX;
									r[Rcnt].cY = l[selectNum].cY;

									r[Rcnt].rectShape[0][0] = r[Rcnt].cX - Size;
									r[Rcnt].rectShape[0][1] = r[Rcnt].cY - Size;
									r[Rcnt].rectShape[1][0] = r[Rcnt].cX + Size;
									r[Rcnt].rectShape[1][1] = r[Rcnt].cY - Size;
									r[Rcnt].rectShape[2][0] = r[Rcnt].cX + Size;
									r[Rcnt].rectShape[2][1] = r[Rcnt].cY + Size;
									r[Rcnt].rectShape[3][0] = r[Rcnt].cX - Size;
									r[Rcnt].rectShape[3][1] = r[Rcnt].cY + Size;

									r[Rcnt].alive = true;
									r[Rcnt].animation = true;

									Rcnt++;
								}
							}
							break;
						case 3:
							if (t[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									l[selectNum].alive = false;
									t[j].alive = false;

									p[Pcnt].cX = l[selectNum].cX;
									p[Pcnt].cY = l[selectNum].cY;

									p[Pcnt].pentaShape[0][0] = p[Pcnt].cX - Size;
									p[Pcnt].pentaShape[0][1] = p[Pcnt].cY - Size;
									p[Pcnt].pentaShape[1][0] = p[Pcnt].cX + Size;
									p[Pcnt].pentaShape[1][1] = p[Pcnt].cY - Size;
									p[Pcnt].pentaShape[2][0] = p[Pcnt].cX + Size + Size / 2;
									p[Pcnt].pentaShape[2][1] = p[Pcnt].cY + Size / 2;
									p[Pcnt].pentaShape[3][0] = p[Pcnt].cX;
									p[Pcnt].pentaShape[3][1] = p[Pcnt].cY + Size + Size / 2;
									p[Pcnt].pentaShape[4][0] = p[Pcnt].cX - Size - Size / 2;
									p[Pcnt].pentaShape[4][1] = p[Pcnt].cY + Size / 2;

									p[Pcnt].alive = true;
									p[Pcnt].animation = true;

									Pcnt++;
								}
							}
							break;
						case 4:
							if (r[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									l[selectNum].alive = false;
									r[j].alive = false;

									d[Dcnt].cX = l[selectNum].cX;
									d[Dcnt].cY = l[selectNum].cY;

									d[Dcnt].dotShape[0] = d[Dcnt].cX;
									d[Dcnt].dotShape[1] = d[Dcnt].cY;

									d[Dcnt].alive = true;
									d[Dcnt].animation = true;

									Dcnt++;
								}
							}
							break;
						case 5:
							if (p[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									l[selectNum].alive = false;
									p[j].alive = false;

									l[Lcnt].alive = true;
									l[Lcnt].animation = true;
									l[Lcnt].cX = l[selectNum].cX;
									l[Lcnt].cY = l[selectNum].cY;

									l[Lcnt].lineShape[0][0] = l[Lcnt].cX - Size;
									l[Lcnt].lineShape[0][1] = l[Lcnt].cY;
									l[Lcnt].lineShape[1][0] = l[Lcnt].cX + Size;
									l[Lcnt].lineShape[1][1] = l[Lcnt].cY;

									Lcnt++;
								}
							}
						}

					}
				}
				break;
			case 3:
				for (int i = 1; i <= 5; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						switch (i)
						{
						case 1:
							if (d[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									t[selectNum].alive = false;
									d[j].alive = false;

									r[Rcnt].cX = t[selectNum].cX;
									r[Rcnt].cY = t[selectNum].cY;

									r[Rcnt].rectShape[0][0] = r[Rcnt].cX - Size;
									r[Rcnt].rectShape[0][1] = r[Rcnt].cY - Size;
									r[Rcnt].rectShape[1][0] = r[Rcnt].cX + Size;
									r[Rcnt].rectShape[1][1] = r[Rcnt].cY - Size;
									r[Rcnt].rectShape[2][0] = r[Rcnt].cX + Size;
									r[Rcnt].rectShape[2][1] = r[Rcnt].cY + Size;
									r[Rcnt].rectShape[3][0] = r[Rcnt].cX - Size;
									r[Rcnt].rectShape[3][1] = r[Rcnt].cY + Size;

									r[Rcnt].alive = true;
									r[Rcnt].animation = true;

									Rcnt++;

									
								}
							}
							break;
						case 2:
							if (l[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									t[selectNum].alive = false;
									l[j].alive = false;

									p[Pcnt].cX = t[selectNum].cX;
									p[Pcnt].cY = t[selectNum].cY;

									p[Pcnt].pentaShape[0][0] = p[Pcnt].cX - Size;
									p[Pcnt].pentaShape[0][1] = p[Pcnt].cY - Size;
									p[Pcnt].pentaShape[1][0] = p[Pcnt].cX + Size;
									p[Pcnt].pentaShape[1][1] = p[Pcnt].cY - Size;
									p[Pcnt].pentaShape[2][0] = p[Pcnt].cX + Size + Size / 2;
									p[Pcnt].pentaShape[2][1] = p[Pcnt].cY + Size / 2;
									p[Pcnt].pentaShape[3][0] = p[Pcnt].cX;
									p[Pcnt].pentaShape[3][1] = p[Pcnt].cY + Size + Size / 2;
									p[Pcnt].pentaShape[4][0] = p[Pcnt].cX - Size - Size / 2;
									p[Pcnt].pentaShape[4][1] = p[Pcnt].cY + Size / 2;

									p[Pcnt].alive = true;
									p[Pcnt].animation = true;

									Pcnt++;
								}
							}
							break;
						case 3:
							if (t[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									t[selectNum].alive = false;
									t[j].alive = false;

									d[Dcnt].cX = t[selectNum].cX;
									d[Dcnt].cY = t[selectNum].cY;

									d[Dcnt].dotShape[0] = d[Dcnt].cX;
									d[Dcnt].dotShape[1] = d[Dcnt].cY;

									d[Dcnt].alive = true;
									d[Dcnt].animation = true;

									Dcnt++;
								}
							}
							break;
						case 4:
							if (r[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									t[selectNum].alive = false;
									r[j].alive = false;

									l[Lcnt].alive = true;
									l[Lcnt].animation = true;
									l[Lcnt].cX = t[selectNum].cX;
									l[Lcnt].cY = t[selectNum].cY;

									l[Lcnt].lineShape[0][0] = l[Lcnt].cX - Size;
									l[Lcnt].lineShape[0][1] = l[Lcnt].cY;
									l[Lcnt].lineShape[1][0] = l[Lcnt].cX + Size;
									l[Lcnt].lineShape[1][1] = l[Lcnt].cY;

									Lcnt++;
								}
							}
							break;
						case 5:
							if (p[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									t[selectNum].alive = false;
									p[j].alive = false;

									t[Tcnt].cX = t[selectNum].cX;
									t[Tcnt].cY = t[selectNum].cY;

									t[Tcnt].triShape[0][0] = t[Tcnt].cX - Size;
									t[Tcnt].triShape[0][1] = t[Tcnt].cY - Size;
									t[Tcnt].triShape[1][0] = t[Tcnt].cX + Size;
									t[Tcnt].triShape[1][1] = t[Tcnt].cY - Size;
									t[Tcnt].triShape[2][0] = t[Tcnt].cX;
									t[Tcnt].triShape[2][1] = t[Tcnt].cY + Size;

									t[Tcnt].alive = true;
									t[Tcnt].animation = true;

									Tcnt++;
								}
							}
						}

					}
				}
				break;
			case 4:
				for (int i = 1; i <= 5; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						switch (i)
						{
						case 1:
							if (d[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									r[selectNum].alive = false;
									d[j].alive = false;

									p[Pcnt].cX = r[selectNum].cX;
									p[Pcnt].cY = r[selectNum].cY;

									p[Pcnt].pentaShape[0][0] = p[Pcnt].cX - Size;
									p[Pcnt].pentaShape[0][1] = p[Pcnt].cY - Size;
									p[Pcnt].pentaShape[1][0] = p[Pcnt].cX + Size;
									p[Pcnt].pentaShape[1][1] = p[Pcnt].cY - Size;
									p[Pcnt].pentaShape[2][0] = p[Pcnt].cX + Size + Size / 2;
									p[Pcnt].pentaShape[2][1] = p[Pcnt].cY + Size / 2;
									p[Pcnt].pentaShape[3][0] = p[Pcnt].cX;
									p[Pcnt].pentaShape[3][1] = p[Pcnt].cY + Size + Size / 2;
									p[Pcnt].pentaShape[4][0] = p[Pcnt].cX - Size - Size / 2;
									p[Pcnt].pentaShape[4][1] = p[Pcnt].cY + Size / 2;

									p[Pcnt].alive = true;
									p[Pcnt].animation = true;

									Pcnt++;

								}
							}
							break;
						case 2:
							if (l[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									r[selectNum].alive = false;
									l[j].alive = false;

									d[Dcnt].cX = r[selectNum].cX;
									d[Dcnt].cY = r[selectNum].cY;

									d[Dcnt].dotShape[0] = d[Dcnt].cX;
									d[Dcnt].dotShape[1] = d[Dcnt].cY;

									d[Dcnt].alive = true;
									d[Dcnt].animation = true;

									Dcnt++;
								}
							}
							break;
						case 3:
							if (t[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									r[selectNum].alive = false;
									t[j].alive = false;

									l[Lcnt].alive = true;
									l[Lcnt].animation = true;
									l[Lcnt].cX = r[selectNum].cX;
									l[Lcnt].cY = r[selectNum].cY;

									l[Lcnt].lineShape[0][0] = l[Lcnt].cX - Size;
									l[Lcnt].lineShape[0][1] = l[Lcnt].cY;
									l[Lcnt].lineShape[1][0] = l[Lcnt].cX + Size;
									l[Lcnt].lineShape[1][1] = l[Lcnt].cY;

									Lcnt++;
								}
							}
							break;
						case 4:
							if (r[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									r[selectNum].alive = false;
									r[j].alive = false;

									t[Tcnt].cX = r[selectNum].cX;
									t[Tcnt].cY = r[selectNum].cY;

									t[Tcnt].triShape[0][0] = t[Tcnt].cX - Size;
									t[Tcnt].triShape[0][1] = t[Tcnt].cY - Size;
									t[Tcnt].triShape[1][0] = t[Tcnt].cX + Size;
									t[Tcnt].triShape[1][1] = t[Tcnt].cY - Size;
									t[Tcnt].triShape[2][0] = t[Tcnt].cX;
									t[Tcnt].triShape[2][1] = t[Tcnt].cY + Size;

									t[Tcnt].alive = true;
									t[Tcnt].animation = true;

									Tcnt++;
								}
							}
							break;
						case 5:
							if (p[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									r[selectNum].alive = false;
									p[j].alive = false;

									r[Rcnt].cX = r[selectNum].cX;
									r[Rcnt].cY = r[selectNum].cY;

									r[Rcnt].rectShape[0][0] = r[Rcnt].cX - Size;
									r[Rcnt].rectShape[0][1] = r[Rcnt].cY - Size;
									r[Rcnt].rectShape[1][0] = r[Rcnt].cX + Size;
									r[Rcnt].rectShape[1][1] = r[Rcnt].cY - Size;
									r[Rcnt].rectShape[2][0] = r[Rcnt].cX + Size;
									r[Rcnt].rectShape[2][1] = r[Rcnt].cY + Size;
									r[Rcnt].rectShape[3][0] = r[Rcnt].cX - Size;
									r[Rcnt].rectShape[3][1] = r[Rcnt].cY + Size;

									r[Rcnt].alive = true;
									r[Rcnt].animation = true;

									Rcnt++;
								}
							}
						}

					}
				}
				break;
			case 5:
				for (int i = 1; i <= 5; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						switch (i)
						{
						case 1:
							if (d[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									p[selectNum].alive = false;
									d[j].alive = false;

									d[Dcnt].cX = p[selectNum].cX;
									d[Dcnt].cY = p[selectNum].cY;

									d[Dcnt].dotShape[0] = d[Dcnt].cX;
									d[Dcnt].dotShape[1] = d[Dcnt].cY;

									d[Dcnt].alive = true;
									d[Dcnt].animation = true;

									Dcnt++;

									

								}
							}
							break;
						case 2:
							if (l[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									p[selectNum].alive = false;
									l[j].alive = false;

									l[Lcnt].alive = true;
									l[Lcnt].animation = true;
									l[Lcnt].cX = p[selectNum].cX;
									l[Lcnt].cY = p[selectNum].cY;

									l[Lcnt].lineShape[0][0] = l[Lcnt].cX - Size;
									l[Lcnt].lineShape[0][1] = l[Lcnt].cY;
									l[Lcnt].lineShape[1][0] = l[Lcnt].cX + Size;
									l[Lcnt].lineShape[1][1] = l[Lcnt].cY;

									Lcnt++;
								}
							}
							break;
						case 3:
							if (t[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									p[selectNum].alive = false;
									t[j].alive = false;

									t[Tcnt].cX = p[selectNum].cX;
									t[Tcnt].cY = p[selectNum].cY;

									t[Tcnt].triShape[0][0] = t[Tcnt].cX - Size;
									t[Tcnt].triShape[0][1] = t[Tcnt].cY - Size;
									t[Tcnt].triShape[1][0] = t[Tcnt].cX + Size;
									t[Tcnt].triShape[1][1] = t[Tcnt].cY - Size;
									t[Tcnt].triShape[2][0] = t[Tcnt].cX;
									t[Tcnt].triShape[2][1] = t[Tcnt].cY + Size;

									t[Tcnt].alive = true;
									t[Tcnt].animation = true;

									Tcnt++;
								}
							}
							break;
						case 4:
							if (r[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									p[selectNum].alive = false;
									r[j].alive = false;

									r[Rcnt].cX = p[selectNum].cX;
									r[Rcnt].cY = p[selectNum].cY;

									r[Rcnt].rectShape[0][0] = r[Rcnt].cX - Size;
									r[Rcnt].rectShape[0][1] = r[Rcnt].cY - Size;
									r[Rcnt].rectShape[1][0] = r[Rcnt].cX + Size;
									r[Rcnt].rectShape[1][1] = r[Rcnt].cY - Size;
									r[Rcnt].rectShape[2][0] = r[Rcnt].cX + Size;
									r[Rcnt].rectShape[2][1] = r[Rcnt].cY + Size;
									r[Rcnt].rectShape[3][0] = r[Rcnt].cX - Size;
									r[Rcnt].rectShape[3][1] = r[Rcnt].cY + Size;

									r[Rcnt].alive = true;
									r[Rcnt].animation = true;

									Rcnt++;
								}
							}
							break;
						case 5:
							if (p[j].alive)
							{
								if (collision(vertex, selectNum, i, j))
								{
									p[selectNum].alive = false;
									p[j].alive = false;

									p[Pcnt].cX = p[selectNum].cX;
									p[Pcnt].cY = p[selectNum].cY;

									p[Pcnt].pentaShape[0][0] = p[Pcnt].cX - Size;
									p[Pcnt].pentaShape[0][1] = p[Pcnt].cY - Size;
									p[Pcnt].pentaShape[1][0] = p[Pcnt].cX + Size;
									p[Pcnt].pentaShape[1][1] = p[Pcnt].cY - Size;
									p[Pcnt].pentaShape[2][0] = p[Pcnt].cX + Size + Size / 2;
									p[Pcnt].pentaShape[2][1] = p[Pcnt].cY + Size / 2;
									p[Pcnt].pentaShape[3][0] = p[Pcnt].cX;
									p[Pcnt].pentaShape[3][1] = p[Pcnt].cY + Size + Size / 2;
									p[Pcnt].pentaShape[4][0] = p[Pcnt].cX - Size - Size / 2;
									p[Pcnt].pentaShape[4][1] = p[Pcnt].cY + Size / 2;

									p[Pcnt].alive = true;
									p[Pcnt].animation = true;

									Pcnt++;
								}
							}
						}
					}
				}
				break;
			}
		}
		else if (state == GLUT_UP)
		{
			movingMouse = -1;
		}
	}
	
}

GLvoid Motion(int x, int y)
{
	if (movingMouse >= 0)
	{
		crashLine[1][0] = 1.0;
		crashLine[1][1] = openGLY;
		WindowToOpenGL(x, y, openGLX, openGLY);

		float deltaX = openGLX - beforeX;
		float deltaY = openGLY - beforeY;

		switch (vertex)
		{
		case 1:
			d[selectNum].dotShape[0] += deltaX;
			d[selectNum].dotShape[1] += deltaY;
			d[selectNum].cX += deltaX;
			d[selectNum].cY += deltaY;
			break;
		case 2:
			l[selectNum].lineShape[0][0] += deltaX;
			l[selectNum].lineShape[0][1] += deltaY;
			l[selectNum].lineShape[1][0] += deltaX;
			l[selectNum].lineShape[1][1] += deltaY;
			l[selectNum].cX += deltaX;
			l[selectNum].cY += deltaY;
			break;
		case 3:
			t[selectNum].triShape[0][0] += deltaX;
			t[selectNum].triShape[0][1] += deltaY;
			t[selectNum].triShape[1][0] += deltaX;
			t[selectNum].triShape[1][1] += deltaY;
			t[selectNum].triShape[2][0] += deltaX;
			t[selectNum].triShape[2][1] += deltaY;
			t[selectNum].cX += deltaX;
			t[selectNum].cY += deltaY;
			break;
		case 4:
			r[selectNum].rectShape[0][0] += deltaX;
			r[selectNum].rectShape[0][1] += deltaY;
			r[selectNum].rectShape[1][0] += deltaX;
			r[selectNum].rectShape[1][1] += deltaY;
			r[selectNum].rectShape[2][0] += deltaX;
			r[selectNum].rectShape[2][1] += deltaY;
			r[selectNum].rectShape[3][0] += deltaX;
			r[selectNum].rectShape[3][1] += deltaY;
			r[selectNum].cX += deltaX;
			r[selectNum].cY += deltaY;
			break;
		case 5:
			p[selectNum].pentaShape[0][0] += deltaX;
			p[selectNum].pentaShape[0][1] += deltaY;
			p[selectNum].pentaShape[1][0] += deltaX;
			p[selectNum].pentaShape[1][1] += deltaY;
			p[selectNum].pentaShape[2][0] += deltaX;
			p[selectNum].pentaShape[2][1] += deltaY;
			p[selectNum].pentaShape[3][0] += deltaX;
			p[selectNum].pentaShape[3][1] += deltaY;
			p[selectNum].pentaShape[4][0] += deltaX;
			p[selectNum].pentaShape[4][1] += deltaY;
			p[selectNum].cX += deltaX;
			p[selectNum].cY += deltaY;
			break;
		}

		beforeX = openGLX;
		beforeY = openGLY;

		glutPostRedisplay();  // ȭ���� �ٽ� �׸��ϴ�.
	}
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}

GLvoid TimerFunction(int value)
{
	for (int i = 0; i < Dcnt; i++)
	{
		if (d[i].animation && d[i].alive)
		{
			switch (d[i].direction)
			{
			case 0:
				d[i].dotShape[0] += moveSize;
				d[i].dotShape[1] += moveSize;
				d[i].cX += moveSize;
				d[i].cY += moveSize;
				if (d[i].dotShape[0] < -1 || d[i].dotShape[0] > 1 || d[i].dotShape[1] < -1 || d[i].dotShape[1] > 1)
				{
					d[i].direction++;
					if (d[i].direction > 3)
					{
						d[i].direction = 0;
					}
					d[i].dotShape[0] -= moveSize;
					d[i].dotShape[1] -= moveSize;
					d[i].cX -= moveSize;
					d[i].cY -= moveSize;
				}
				break;
			case 1:
				d[i].dotShape[0] -= moveSize;
				d[i].dotShape[1] += moveSize;
				d[i].cX -= moveSize;
				d[i].cY += moveSize;
				if (d[i].dotShape[0] < -1 || d[i].dotShape[0] > 1 || d[i].dotShape[1] < -1 || d[i].dotShape[1] > 1)
				{
					d[i].direction++;
					if (d[i].direction > 3)
					{
						d[i].direction = 0;
					}
					d[i].dotShape[0] += moveSize;
					d[i].dotShape[1] -= moveSize;
					d[i].cX += moveSize;
					d[i].cY -= moveSize;
				}
				break;
			case 2:
				d[i].dotShape[0] -= moveSize;
				d[i].dotShape[1] -= moveSize;
				d[i].cX -= moveSize;
				d[i].cY -= moveSize;
				if (d[i].dotShape[0] < -1 || d[i].dotShape[0] > 1 || d[i].dotShape[1] < -1 || d[i].dotShape[1] > 1)
				{
					d[i].direction++;
					if (d[i].direction > 3)
					{
						d[i].direction = 0;
					}
					d[i].dotShape[0] += moveSize;
					d[i].dotShape[1] += moveSize;
					d[i].cX += moveSize;
					d[i].cY += moveSize;
				}
				break;
			case 3:
				d[i].dotShape[0] += moveSize;
				d[i].dotShape[1] -= moveSize;
				d[i].cX += moveSize;
				d[i].cY -= moveSize;
				if (d[i].dotShape[0] < -1 || d[i].dotShape[0] > 1 || d[i].dotShape[1] < -1 || d[i].dotShape[1] > 1)
				{
					d[i].direction++;
					if (d[i].direction > 3)
					{
						d[i].direction = 0;
					}
					d[i].dotShape[0] -= moveSize;
					d[i].dotShape[1] += moveSize;
					d[i].cX -= moveSize;
					d[i].cY += moveSize;
				}
				break;
			}
		}
	}
	for (int i = 0; i < Lcnt; i++)
	{
		if (l[i].animation && l[i].alive)
		{
			switch (l[i].direction)
			{
			case 0:
				l[i].lineShape[0][0] += moveSize;
				l[i].lineShape[0][1] += moveSize;
				l[i].lineShape[1][0] += moveSize;
				l[i].lineShape[1][1] += moveSize;
				l[i].cX += moveSize;
				l[i].cY += moveSize;
				for (int j = 0; j < 2; j++)
				{
					if (l[i].lineShape[j][0] < -1 || l[i].lineShape[j][0] > 1 || l[i].lineShape[j][1] < -1 || l[i].lineShape[j][1]>1)
					{
						l[i].direction++;
						if (l[i].direction > 3)
						{
							l[i].direction = 0;
						}

						l[i].lineShape[0][0] -= moveSize;
						l[i].lineShape[0][1] -= moveSize;
						l[i].lineShape[1][0] -= moveSize;
						l[i].lineShape[1][1] -= moveSize;
						l[i].cX -= moveSize;
						l[i].cY -= moveSize;
						break;
					}
				}
				break;
			case 1:
				l[i].lineShape[0][0] -= moveSize;
				l[i].lineShape[0][1] += moveSize;
				l[i].lineShape[1][0] -= moveSize;
				l[i].lineShape[1][1] += moveSize;
				l[i].cX -= moveSize;
				l[i].cY += moveSize;
				for (int j = 0; j < 2; j++)
				{
					if (l[i].lineShape[j][0] < -1 || l[i].lineShape[j][0] > 1 || l[i].lineShape[j][1] < -1 || l[i].lineShape[j][1]>1)
					{
						l[i].direction++;
						if (l[i].direction > 3)
						{
							l[i].direction = 0;
						}

						l[i].lineShape[0][0] += moveSize;
						l[i].lineShape[0][1] -= moveSize;
						l[i].lineShape[1][0] += moveSize;
						l[i].lineShape[1][1] -= moveSize;
						l[i].cX += moveSize;
						l[i].cY -= moveSize;
						break;
					}
				}
				break;
			case 2:
				l[i].lineShape[0][0] -= moveSize;
				l[i].lineShape[0][1] -= moveSize;
				l[i].lineShape[1][0] -= moveSize;
				l[i].lineShape[1][1] -= moveSize;
				l[i].cX -= moveSize;
				l[i].cY -= moveSize;
				for (int j = 0; j < 2; j++)
				{
					if (l[i].lineShape[j][0] < -1 || l[i].lineShape[j][0] > 1 || l[i].lineShape[j][1] < -1 || l[i].lineShape[j][1]>1)
					{
						l[i].direction++;
						if (l[i].direction > 3)
						{
							l[i].direction = 0;
						}

						l[i].lineShape[0][0] += moveSize;
						l[i].lineShape[0][1] += moveSize;
						l[i].lineShape[1][0] += moveSize;
						l[i].lineShape[1][1] += moveSize;
						l[i].cX += moveSize;
						l[i].cY += moveSize;
						break;
					}
				}
				break;
			case 3:
				l[i].lineShape[0][0] += moveSize;
				l[i].lineShape[0][1] -= moveSize;
				l[i].lineShape[1][0] += moveSize;
				l[i].lineShape[1][1] -= moveSize;
				l[i].cX += moveSize;
				l[i].cY -= moveSize;
				for (int j = 0; j < 2; j++)
				{
					if (l[i].lineShape[j][0] < -1 || l[i].lineShape[j][0] > 1 || l[i].lineShape[j][1] < -1 || l[i].lineShape[j][1]>1)
					{
						l[i].direction++;
						if (l[i].direction > 3)
						{
							l[i].direction = 0;
						}

						l[i].lineShape[0][0] -= moveSize;
						l[i].lineShape[0][1] += moveSize;
						l[i].lineShape[1][0] -= moveSize;
						l[i].lineShape[1][1] += moveSize;
						l[i].cX -= moveSize;
						l[i].cY += moveSize;
						break;
					}
				}
				break;
			}
		}
	}
	for (int i = 0; i < Tcnt; i++)
	{
		if (t[i].animation && t[i].alive)
		{
			switch (t[i].direction)
			{
			case 0:
				t[i].triShape[0][0] += moveSize;
				t[i].triShape[0][1] += moveSize;
				t[i].triShape[1][0] += moveSize;
				t[i].triShape[1][1] += moveSize;
				t[i].triShape[2][0] += moveSize;
				t[i].triShape[2][1] += moveSize;
				t[i].cX += moveSize;
				t[i].cY += moveSize;
				for (int j = 0; j < 3; j++)
				{
					if (t[i].triShape[j][0] < -1 || t[i].triShape[j][0] > 1 || t[i].triShape[j][1] < -1 || t[i].triShape[j][1]>1)
					{
						t[i].direction++;
						if (t[i].direction > 3)
						{
							t[i].direction = 0;
						}

						t[i].triShape[0][0] -= moveSize;
						t[i].triShape[0][1] -= moveSize;
						t[i].triShape[1][0] -= moveSize;
						t[i].triShape[1][1] -= moveSize;
						t[i].triShape[2][0] -= moveSize;
						t[i].triShape[2][1] -= moveSize;
						t[i].cX -= moveSize;
						t[i].cY -= moveSize;
						break;
					}
				}
				break;
			case 1:
				t[i].triShape[0][0] -= moveSize;
				t[i].triShape[0][1] += moveSize;
				t[i].triShape[1][0] -= moveSize;
				t[i].triShape[1][1] += moveSize;
				t[i].triShape[2][0] -= moveSize;
				t[i].triShape[2][1] += moveSize;
				t[i].cX -= moveSize;
				t[i].cY += moveSize;
				for (int j = 0; j < 3; j++)
				{
					if (t[i].triShape[j][0] < -1 || t[i].triShape[j][0] > 1 || t[i].triShape[j][1] < -1 || t[i].triShape[j][1]>1)
					{
						t[i].direction++;
						if (t[i].direction > 3)
						{
							t[i].direction = 0;
						}

						t[i].triShape[0][0] += moveSize;
						t[i].triShape[0][1] -= moveSize;
						t[i].triShape[1][0] += moveSize;
						t[i].triShape[1][1] -= moveSize;
						t[i].triShape[2][0] += moveSize;
						t[i].triShape[2][1] -= moveSize;
						t[i].cX += moveSize;
						t[i].cY -= moveSize;
						break;
					}
				}
				break;
			case 2:
				t[i].triShape[0][0] -= moveSize;
				t[i].triShape[0][1] -= moveSize;
				t[i].triShape[1][0] -= moveSize;
				t[i].triShape[1][1] -= moveSize;
				t[i].triShape[2][0] -= moveSize;
				t[i].triShape[2][1] -= moveSize;
				t[i].cX -= moveSize;
				t[i].cY -= moveSize;
				for (int j = 0; j < 3; j++)
				{
					if (t[i].triShape[j][0] < -1 || t[i].triShape[j][0] > 1 || t[i].triShape[j][1] < -1 || t[i].triShape[j][1]>1)
					{
						t[i].direction++;
						if (t[i].direction > 3)
						{
							t[i].direction = 0;
						}

						t[i].triShape[0][0] += moveSize;
						t[i].triShape[0][1] += moveSize;
						t[i].triShape[1][0] += moveSize;
						t[i].triShape[1][1] += moveSize;
						t[i].triShape[2][0] += moveSize;
						t[i].triShape[2][1] += moveSize;
						t[i].cX += moveSize;
						t[i].cY += moveSize;
						break;
					}
				}
				break;
			case 3:
				t[i].triShape[0][0] += moveSize;
				t[i].triShape[0][1] -= moveSize;
				t[i].triShape[1][0] += moveSize;
				t[i].triShape[1][1] -= moveSize;
				t[i].triShape[2][0] += moveSize;
				t[i].triShape[2][1] -= moveSize;
				t[i].cX += moveSize;
				t[i].cY -= moveSize;
				for (int j = 0; j < 3; j++)
				{
					if (t[i].triShape[j][0] < -1 || t[i].triShape[j][0] > 1 || t[i].triShape[j][1] < -1 || t[i].triShape[j][1]>1)
					{
						t[i].direction++;
						if (t[i].direction > 3)
						{
							t[i].direction = 0;
						}

						t[i].triShape[0][0] -= moveSize;
						t[i].triShape[0][1] += moveSize;
						t[i].triShape[1][0] -= moveSize;
						t[i].triShape[1][1] += moveSize;
						t[i].triShape[2][0] -= moveSize;
						t[i].triShape[2][1] += moveSize;
						t[i].cX -= moveSize;
						t[i].cY += moveSize;
						break;
					}
				}
				break;
			}
		}
	}
	for (int i = 0; i < Rcnt; i++)
	{
		if (r[i].animation && r[i].alive)
		{
			switch (r[i].direction)
			{
			case 0:
				r[i].rectShape[0][0] += moveSize;
				r[i].rectShape[0][1] += moveSize;
				r[i].rectShape[1][0] += moveSize;
				r[i].rectShape[1][1] += moveSize;
				r[i].rectShape[2][0] += moveSize;
				r[i].rectShape[2][1] += moveSize;
				r[i].rectShape[3][0] += moveSize;
				r[i].rectShape[3][1] += moveSize;
				r[i].cX += moveSize;
				r[i].cY += moveSize;
				for (int j = 0; j < 4; j++)
				{
					if (r[i].rectShape[j][0] < -1 || r[i].rectShape[j][0] > 1 || r[i].rectShape[j][1] < -1 || r[i].rectShape[j][1]>1)
					{
						r[i].direction++;
						if (r[i].direction > 3)
						{
							r[i].direction = 0;
						}

						r[i].rectShape[0][0] -= moveSize;
						r[i].rectShape[0][1] -= moveSize;
						r[i].rectShape[1][0] -= moveSize;
						r[i].rectShape[1][1] -= moveSize;
						r[i].rectShape[2][0] -= moveSize;
						r[i].rectShape[2][1] -= moveSize;
						r[i].rectShape[3][0] -= moveSize;
						r[i].rectShape[3][1] -= moveSize;
						r[i].cX -= moveSize;
						r[i].cY -= moveSize;
						break;
					}
				}
				break;
			case 1:
				r[i].rectShape[0][0] -= moveSize;
				r[i].rectShape[0][1] += moveSize;
				r[i].rectShape[1][0] -= moveSize;
				r[i].rectShape[1][1] += moveSize;
				r[i].rectShape[2][0] -= moveSize;
				r[i].rectShape[2][1] += moveSize;
				r[i].rectShape[3][0] -= moveSize;
				r[i].rectShape[3][1] += moveSize;
				r[i].cX -= moveSize;
				r[i].cY += moveSize;
				for (int j = 0; j < 4; j++)
				{
					if (r[i].rectShape[j][0] < -1 || r[i].rectShape[j][0] > 1 || r[i].rectShape[j][1] < -1 || r[i].rectShape[j][1]>1)
					{
						r[i].direction++;
						if (r[i].direction > 3)
						{
							r[i].direction = 0;
						}

						r[i].rectShape[0][0] += moveSize;
						r[i].rectShape[0][1] -= moveSize;
						r[i].rectShape[1][0] += moveSize;
						r[i].rectShape[1][1] -= moveSize;
						r[i].rectShape[2][0] += moveSize;
						r[i].rectShape[2][1] -= moveSize;
						r[i].rectShape[3][0] += moveSize;
						r[i].rectShape[3][1] -= moveSize;
						r[i].cX += moveSize;
						r[i].cY -= moveSize;
						break;
					}
				}
				break;
			case 2:
				r[i].rectShape[0][0] -= moveSize;
				r[i].rectShape[0][1] -= moveSize;
				r[i].rectShape[1][0] -= moveSize;
				r[i].rectShape[1][1] -= moveSize;
				r[i].rectShape[2][0] -= moveSize;
				r[i].rectShape[2][1] -= moveSize;
				r[i].rectShape[3][0] -= moveSize;
				r[i].rectShape[3][1] -= moveSize;
				r[i].cX -= moveSize;
				r[i].cY -= moveSize;
				for (int j = 0; j < 4; j++)
				{
					if (r[i].rectShape[j][0] < -1 || r[i].rectShape[j][0] > 1 || r[i].rectShape[j][1] < -1 || r[i].rectShape[j][1]>1)
					{
						r[i].direction++;
						if (r[i].direction > 3)
						{
							r[i].direction = 0;
						}

						r[i].rectShape[0][0] += moveSize;
						r[i].rectShape[0][1] += moveSize;
						r[i].rectShape[1][0] += moveSize;
						r[i].rectShape[1][1] += moveSize;
						r[i].rectShape[2][0] += moveSize;
						r[i].rectShape[2][1] += moveSize;
						r[i].rectShape[3][0] += moveSize;
						r[i].rectShape[3][1] += moveSize;
						r[i].cX += moveSize;
						r[i].cY += moveSize;
						break;
					}
				}
				break;
			case 3:
				r[i].rectShape[0][0] += moveSize;
				r[i].rectShape[0][1] -= moveSize;
				r[i].rectShape[1][0] += moveSize;
				r[i].rectShape[1][1] -= moveSize;
				r[i].rectShape[2][0] += moveSize;
				r[i].rectShape[2][1] -= moveSize;
				r[i].rectShape[3][0] += moveSize;
				r[i].rectShape[3][1] -= moveSize;
				r[i].cX += moveSize;
				r[i].cY -= moveSize;
				for (int j = 0; j < 4; j++)
				{
					if (r[i].rectShape[j][0] < -1 || r[i].rectShape[j][0] > 1 || r[i].rectShape[j][1] < -1 || r[i].rectShape[j][1]>1)
					{
						r[i].direction++;
						if (r[i].direction > 3)
						{
							r[i].direction = 0;
						}

						r[i].rectShape[0][0] -= moveSize;
						r[i].rectShape[0][1] += moveSize;
						r[i].rectShape[1][0] -= moveSize;
						r[i].rectShape[1][1] += moveSize;
						r[i].rectShape[2][0] -= moveSize;
						r[i].rectShape[2][1] += moveSize;
						r[i].rectShape[3][0] -= moveSize;
						r[i].rectShape[3][1] += moveSize;
						r[i].cX -= moveSize;
						r[i].cY += moveSize;
						break;
					}
				}
				break;
			}
		}
	}
	for (int i = 0; i < Pcnt; i++)
	{
		if (p[i].animation && p[i].alive)
		{
			switch (p[i].direction)
			{
			case 0:
				p[i].pentaShape[0][0] += moveSize;
				p[i].pentaShape[0][1] += moveSize;
				p[i].pentaShape[1][0] += moveSize;
				p[i].pentaShape[1][1] += moveSize;
				p[i].pentaShape[2][0] += moveSize;
				p[i].pentaShape[2][1] += moveSize;
				p[i].pentaShape[3][0] += moveSize;
				p[i].pentaShape[3][1] += moveSize;
				p[i].pentaShape[4][0] += moveSize;
				p[i].pentaShape[4][1] += moveSize;
				p[i].cX += moveSize;
				p[i].cY += moveSize;
				for (int j = 0; j < 5; j++)
				{
					if (p[i].pentaShape[j][0] < -1 || p[i].pentaShape[j][0] > 1 || p[i].pentaShape[j][1] < -1 || p[i].pentaShape[j][1]>1)
					{
						p[i].direction++;
						if (p[i].direction > 3)
						{
							p[i].direction = 0;
						}

						p[i].pentaShape[0][0] -= moveSize;
						p[i].pentaShape[0][1] -= moveSize;
						p[i].pentaShape[1][0] -= moveSize;
						p[i].pentaShape[1][1] -= moveSize;
						p[i].pentaShape[2][0] -= moveSize;
						p[i].pentaShape[2][1] -= moveSize;
						p[i].pentaShape[3][0] -= moveSize;
						p[i].pentaShape[3][1] -= moveSize;
						p[i].pentaShape[4][0] -= moveSize;
						p[i].pentaShape[4][1] -= moveSize;
						p[i].cX -= moveSize;
						p[i].cY -= moveSize;
						break;
					}
				}
				break;
			case 1:
				p[i].pentaShape[0][0] -= moveSize;
				p[i].pentaShape[0][1] += moveSize;
				p[i].pentaShape[1][0] -= moveSize;
				p[i].pentaShape[1][1] += moveSize;
				p[i].pentaShape[2][0] -= moveSize;
				p[i].pentaShape[2][1] += moveSize;
				p[i].pentaShape[3][0] -= moveSize;
				p[i].pentaShape[3][1] += moveSize;
				p[i].pentaShape[4][0] -= moveSize;
				p[i].pentaShape[4][1] += moveSize;
				p[i].cX -= moveSize;
				p[i].cY += moveSize;
				for (int j = 0; j < 5; j++)
				{
					if (p[i].pentaShape[j][0] < -1 || p[i].pentaShape[j][0] > 1 || p[i].pentaShape[j][1] < -1 || p[i].pentaShape[j][1]>1)
					{
						p[i].direction++;
						if (p[i].direction > 3)
						{
							p[i].direction = 0;
						}

						p[i].pentaShape[0][0] += moveSize;
						p[i].pentaShape[0][1] -= moveSize;
						p[i].pentaShape[1][0] += moveSize;
						p[i].pentaShape[1][1] -= moveSize;
						p[i].pentaShape[2][0] += moveSize;
						p[i].pentaShape[2][1] -= moveSize;
						p[i].pentaShape[3][0] += moveSize;
						p[i].pentaShape[3][1] -= moveSize;
						p[i].pentaShape[4][0] += moveSize;
						p[i].pentaShape[4][1] -= moveSize;
						p[i].cX += moveSize;
						p[i].cY -= moveSize;
						break;
					}
				}
				break;
			case 2:
				p[i].pentaShape[0][0] -= moveSize;
				p[i].pentaShape[0][1] -= moveSize;
				p[i].pentaShape[1][0] -= moveSize;
				p[i].pentaShape[1][1] -= moveSize;
				p[i].pentaShape[2][0] -= moveSize;
				p[i].pentaShape[2][1] -= moveSize;
				p[i].pentaShape[3][0] -= moveSize;
				p[i].pentaShape[3][1] -= moveSize;
				p[i].pentaShape[4][0] -= moveSize;
				p[i].pentaShape[4][1] -= moveSize;
				p[i].cX -= moveSize;
				p[i].cY -= moveSize;
				for (int j = 0; j < 5; j++)
				{
					if (p[i].pentaShape[j][0] < -1 || p[i].pentaShape[j][0] > 1 || p[i].pentaShape[j][1] < -1 || p[i].pentaShape[j][1]>1)
					{
						p[i].direction++;
						if (p[i].direction > 3)
						{
							p[i].direction = 0;
						}

						p[i].pentaShape[0][0] += moveSize;
						p[i].pentaShape[0][1] += moveSize;
						p[i].pentaShape[1][0] += moveSize;
						p[i].pentaShape[1][1] += moveSize;
						p[i].pentaShape[2][0] += moveSize;
						p[i].pentaShape[2][1] += moveSize;
						p[i].pentaShape[3][0] += moveSize;
						p[i].pentaShape[3][1] += moveSize;
						p[i].pentaShape[4][0] += moveSize;
						p[i].pentaShape[4][1] += moveSize;
						p[i].cX += moveSize;
						p[i].cY += moveSize;
						break;
					}
				}
				break;
			case 3:
				p[i].pentaShape[0][0] += moveSize;
				p[i].pentaShape[0][1] -= moveSize;
				p[i].pentaShape[1][0] += moveSize;
				p[i].pentaShape[1][1] -= moveSize;
				p[i].pentaShape[2][0] += moveSize;
				p[i].pentaShape[2][1] -= moveSize;
				p[i].pentaShape[3][0] += moveSize;
				p[i].pentaShape[3][1] -= moveSize;
				p[i].pentaShape[4][0] += moveSize;
				p[i].pentaShape[4][1] -= moveSize;
				p[i].cX += moveSize;
				p[i].cY -= moveSize;
				for (int j = 0; j < 5; j++)
				{
					if (p[i].pentaShape[j][0] < -1 || p[i].pentaShape[j][0] > 1 || p[i].pentaShape[j][1] < -1 || p[i].pentaShape[j][1]>1)
					{
						p[i].direction++;
						if (p[i].direction > 3)
						{
							p[i].direction = 0;
						}

						p[i].pentaShape[0][0] -= moveSize;
						p[i].pentaShape[0][1] += moveSize;
						p[i].pentaShape[1][0] -= moveSize;
						p[i].pentaShape[1][1] += moveSize;
						p[i].pentaShape[2][0] -= moveSize;
						p[i].pentaShape[2][1] += moveSize;
						p[i].pentaShape[3][0] -= moveSize;
						p[i].pentaShape[3][1] += moveSize;
						p[i].pentaShape[4][0] -= moveSize;
						p[i].pentaShape[4][1] += moveSize;
						p[i].cX -= moveSize;
						p[i].cY += moveSize;
						break;
					}
				}
				break;
			}
		}
	}
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

int checkCollision()
{
	// �� ������ ��ǥ�� ������ ����
	double x1 = testLine[0][0];
	double x2 = testLine[1][0];
	double x3 = crashLine[0][0];
	double x4 = crashLine[1][0];

	double y1 = testLine[0][1];
	double y2 = testLine[1][1];
	double y3 = crashLine[0][1];
	double y4 = crashLine[1][1];

	// �� ������ ������ ���
	double a1 = y2 - y1;
	double b1 = x1 - x2;
	double c1 = x2 * y1 - x1 * y2;

	double a2 = y4 - y3;
	double b2 = x3 - x4;
	double c2 = x4 * y3 - x3 * y4;

	// �� ������ ������ ���� ��ǥ
	double intersectionX = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
	double intersectionY = (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1);

	// ���� �� ���� ���̿� �ִ��� Ȯ��
	if (
		(intersectionX >= fmin(x1, x2) && intersectionX <= fmax(x1, x2)) &&
		(intersectionY >= fmin(y1, y2) && intersectionY <= fmax(y1, y2)) &&
		(intersectionX >= fmin(x3, x4) && intersectionX <= fmax(x3, x4)) &&
		(intersectionY >= fmin(y3, y4) && intersectionY <= fmax(y3, y4))
		) {
		return 1; // �� ������ ������
	}
	else {
		return 0; // �� ������ �������� ����
	}
}

int collision(int vertex1, int selectNum1, int vertex2, int selectNum2)
{
	double cx1=0, cy1=0, cx2=0, cy2=0;

	switch (vertex1)
	{
	case 1:
		cx1 = d[selectNum1].cX;
		cy1 = d[selectNum1].cY;
		break;
	case 2:
		cx1 = l[selectNum1].cX;
		cy1 = l[selectNum1].cY;
		break;
	case 3:
		cx1 = t[selectNum1].cX;
		cy1 = t[selectNum1].cY;
		break;
	case 4:
		cx1 = r[selectNum1].cX;
		cy1 = r[selectNum1].cY;
		break;
	case 5:
		cx1 = p[selectNum1].cX;
		cy1 = p[selectNum1].cY;
		break;
	}
	switch (vertex2)
	{
	case 1:
		cx2 = d[selectNum2].cX;
		cy2 = d[selectNum2].cY;
		break;
	case 2:
		cx2 = l[selectNum2].cX;
		cy2 = l[selectNum2].cY;
		break;
	case 3:
		cx2 = t[selectNum2].cX;
		cy2 = t[selectNum2].cY;
		break;
	case 4:
		cx2 = r[selectNum2].cX;
		cy2 = r[selectNum2].cY;
		break;
	case 5:
		cx2 = p[selectNum2].cX;
		cy2 = p[selectNum2].cY;
		break;
	}

	return Collision(cx1 - 0.07, cx1 + 0.07, cy1 - 0.07, cy1 + 0.07, cx2 - 0.07, cx2 + 0.07, cy2 - 0.07, cy2 + 0.07);
}

BOOL Collision(float a_x1, float a_x2, float a_y1, float a_y2, float b_x1, float b_x2, float b_y1, float b_y2)
{
	if (((b_x1 < a_x1 && a_x1 < b_x2) || (b_x1 < a_x2 && a_x2 < b_x2)) &&
		((b_y1 < a_y2 && a_y2 < b_y2) ||  (b_y1 < a_y1 && a_y1 < b_y2)))
	{
		return true;
	}
	return false;
}