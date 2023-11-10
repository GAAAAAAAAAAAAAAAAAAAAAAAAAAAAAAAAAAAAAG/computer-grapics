#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
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

uniform_real_distribution<double> Tdis(-1.0, 1.0);
uniform_real_distribution<double> colorDis(0.0, 1.0);
uniform_int_distribution<int> polygonrandom(0, 2);
uniform_int_distribution<int> directionrandom(0, 1);
uniform_real_distribution<double> randomY(-0.5, 0.5);


struct TRISHAPE
{
	GLfloat triShape[3][3];
	GLfloat cX, cY;
	bool alive = false;
	double speed = 0.005;
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
	int direction = 0;
};
PENTASHAPE p[100];
int Pcnt = 0;

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

GLuint vao, vbo[2];
GLuint TriPosVbo, TriColorVbo;

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

int windowWidth = 800;
int windowHeight = 600;

float openGLX, openGLY;
int movingRectangle = -1;

bool start = true;
double Size = 0.1;
int createPolygon = 0;
int createPolygonDirection = 0;

double centerX, centerY;
int cnt = 0;

bool LineFillMode = true;	//t : l, f : f

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

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//�ﰢ�� �׸���
	for (int i = 0; i < Tcnt; i++)
	{
		if (t[i].alive)
		{
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

	// �簢�� �׸���
	for (int i = 0; i < Rcnt; i++)
	{
		if (r[i].alive)
		{
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

	// ������ �׸���
	for (int i = 0; i < Pcnt; i++)
	{
		if (p[i].alive)
		{
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
	case 'l':
		LineFillMode = true;
		break;
	case 'f':
		LineFillMode = false;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
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

		//�̵�

		//���콺�� �ֱ�
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
	switch (value)
	{
	case 1:


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

			Pcnt++;
			break;
		}
		glutTimerFunc(1000, TimerFunction, 2);
		break;
	}
	glutPostRedisplay();
	
}