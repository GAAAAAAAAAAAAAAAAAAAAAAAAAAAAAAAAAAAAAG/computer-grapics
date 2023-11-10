#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
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
uniform_real_distribution<double> randomM(0.25, 0.75);
uniform_real_distribution<double> randomSpeed(0.007, 0.02);

struct TRISHAPE
{
	GLfloat triShape[3][3];
	GLfloat cX, cY;
	bool alive = false;
	double speed = 0.005;
	double m = 0.03;
	int direction = 0;
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
};
PENTASHAPE p[100];
int Pcnt = 0;


GLfloat TdotShape[100][1000][3] = {};
GLfloat RdotShape[100][1000][3] = {};
GLfloat PdotShape[100][1000][3] = {};

GLfloat dotShape[3] = {};

int TdotCnt[100]{};
int RdotCnt[100]{};
int PdotCnt[100]{};

//direction : (1,��->��) (2,��->��)

//�ڽ�
GLfloat box[4][3];
bool boxDirection = true;

GLfloat lineShape[2][3] = {};	//--- ���� ��ġ ��

GLfloat colors[5][3] = { //--- �ﰢ�� ������ ����
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 0.0, 0.0, 0.0 }
};
GLfloat dotColors[3] = { 0.7, 0.7, 1.0 };

GLuint vao, vbo[2];
GLuint TriPosVbo, TriColorVbo;

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

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
	float x, y;
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
void CrossDot();

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
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE); //--- ���� ������ �ʿ��� ������ �ϸ� �ȴ�.
	//glDisable(GL_DEPTH_TEST | GL_CULL_FACE);	//����

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
	//--- ����� ���� ����
	glClearColor(0.7f, 0.7f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);
	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao);

	modelLocation = glGetUniformLocation(shaderProgramID, "model"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��� �������� �޾ƿ´�.
	

	if (start)
	{

		box[0][0] = -0.3;
		box[0][1] = -0.8;
		box[1][0] = 0.3;
		box[1][1] = -0.8;
		box[2][0] = 0.3;
		box[2][1] = -0.7;
		box[3][0] = -0.3;
		box[3][1] = -0.7;

		start = false;
	}

	//��� �� �����
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

	//����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	
	//�ﰢ�� �׸���
	for (int i = 0; i < Tcnt; i++)
	{
		if (t[i].alive)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(t[i].cX, t[i].cY, 0.0f));
			model = glm::rotate(model, glm::radians(polygonRotate), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-t[i].cX, -t[i].cY, 0.0f));

			// modelTransform ������ ��ȯ �� �����ϱ�
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
	//�ﰢ�� ��
	if (path)
	{
		for (int i = 0; i < Tcnt; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				if (t[i].alive)
				{
					model = glm::mat4(1.0f);// modelTransform ������ ��ȯ �� �����ϱ�
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

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

	// �簢�� �׸���
	for (int i = 0; i < Rcnt; i++)
	{
		if (r[i].alive)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(r[i].cX, r[i].cY, 0.0f));
			model = glm::rotate(model, glm::radians(polygonRotate), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-r[i].cX, -r[i].cY, 0.0f));

			// modelTransform ������ ��ȯ �� �����ϱ�
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
		//�簢�� ��
		for (int i = 0; i < Rcnt; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				if (r[i].alive)
				{
					model = glm::mat4(1.0f);// modelTransform ������ ��ȯ �� �����ϱ�
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

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
	

	// ������ �׸���
	for (int i = 0; i < Pcnt; i++)
	{
		if (p[i].alive)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(p[i].cX, p[i].cY, 0.0f));
			model = glm::rotate(model, glm::radians(polygonRotate), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-p[i].cX, -p[i].cY, 0.0f));

			// modelTransform ������ ��ȯ �� �����ϱ�
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
		//������ ��
		for (int i = 0; i < Pcnt; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				if (p[i].alive)
				{
					model = glm::mat4(1.0f);// modelTransform ������ ��ȯ �� �����ϱ�
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

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
	// modelTransform ������ ��ȯ �� �����ϱ�
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	//�ڽ� �׸���
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

	// �� �׸���
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), lineShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_LINES, 0, 2);

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
	vertexSource = filetobuf("vertexh.glsl");
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
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
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

		//�̵�
		lineShape[1][0] = openGLX;
		lineShape[1][1] = openGLY;

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
	switch (value)
	{
	case 1:
		polygonRotate += 1;

		//������
		//�ﰢ��
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
				t[i].cY = -t[i].m * (t[i].cX - 1.0) * (t[i].cX + 1.0);

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

		//�簢��
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
				r[i].cY = -r[i].m * (r[i].cX - 1.0) * (r[i].cX + 1.0);

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
		//������
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
				p[i].cY = -p[i].m * (p[i].cX - 1.0) * (p[i].cX + 1.0);

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

		//�ڽ�
		if (boxDirection)
		{
			box[0][0] -= 0.005;
			box[1][0] -= 0.005;
			box[2][0] -= 0.005;
			box[3][0] -= 0.005;
		}
		if (!boxDirection)
		{
			box[0][0] += 0.005;
			box[1][0] += 0.005;
			box[2][0] += 0.005;
			box[3][0] += 0.005;
		}
		if (box[0][0] < -1.0)
		{
			boxDirection = false;
			box[0][0] += 0.005;
			box[1][0] += 0.005;
			box[2][0] += 0.005;
			box[3][0] += 0.005;
		}
		if (box[1][0] > 1.0)
		{
			boxDirection = true;
			box[0][0] -= 0.005;
			box[1][0] -= 0.005;
			box[2][0] -= 0.005;
			box[3][0] -= 0.005;
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
			t[Tcnt].alive = true;

			t[Tcnt].direction = createPolygonDirection;
			t[Tcnt].m = randomM(gen);
			t[Tcnt].speed = randomSpeed(gen);

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
			r[Rcnt].alive = true;

			r[Rcnt].direction = createPolygonDirection;
			r[Rcnt].m = randomM(gen);
			r[Rcnt].speed = randomSpeed(gen);

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
			p[Pcnt].alive = true;

			p[Pcnt].direction = createPolygonDirection;
			p[Pcnt].m = randomM(gen);
			p[Pcnt].speed = randomSpeed(gen);

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

Point CrossDot(Point a, Point b, Point c, Point d)
{
	// �� ������ ��ǥ�� ������ ����
	double x1 = c.x;
	double x2 = d.x;
	double x3 = a.x;
	double x4 = b.x;

	double y1 = c.y;
	double y2 = d.y;
	double y3 = a.y;
	double y4 = b.y;

	// �� ������ ������ ���
	double a1 = y2 - y1;
	double b1 = x1 - x2;
	double c1 = x2 * y1 - x1 * y2;

	double a2 = y4 - y3;
	double b2 = x3 - x4;
	double c2 = x4 * y3 - x3 * y4;

	Point intersection;
	Point xx;
	xx.x = -1.0;
	xx.y = -1.0;

	// �� ������ ������ ���� ��ǥ
	intersection.x = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
	intersection.y = (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1);

	// ���� �� ���� ���̿� �ִ��� Ȯ��
	if (
		(intersection.x >= fmin(x1, x2) && intersection.x <= fmax(x1, x2)) &&
		(intersection.y >= fmin(y1, y2) && intersection.y <= fmax(y1, y2)) &&
		(intersection.x >= fmin(x3, x4) && intersection.x <= fmax(x3, x4)) &&
		(intersection.y >= fmin(y3, y4) && intersection.y <= fmax(y3, y4))
		) {
		return intersection; // �� ������ ������
	}
	else {
		return xx; // �� ������ �������� ����
	}
}