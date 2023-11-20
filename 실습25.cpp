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
uniform_int_distribution<int> dis6(1, 6);
uniform_int_distribution<int> dis4(1, 4);
uniform_real_distribution<double> color(0, 1);

// cube[2][0].x

glm::vec3 cube[12][3]{
	// �ո�
	{ {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5} },
	{ {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5} },

	// �޸�
	{ {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5} },
	{ {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5} },

	// ��ܸ�
	{ {-0.5, 0.5, -0.5}, {0.5, 0.5, -0.5}, {0.5, 0.5, 0.5} },
	{ {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, 0.5, -0.5} },

	// �ϴܸ�
	{ {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, -0.5, 0.5} },
	{ {0.5, -0.5, 0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5} },

	// ���ʸ�
	{ {-0.5, -0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5} },
	{ {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5} },

	// �����ʸ�
	{ {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {0.5, 0.5, 0.5} },
	{ {0.5, 0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, -0.5, -0.5} },
};

glm::vec3 pyramid[6][3]{
	// �ظ� (�簢��)
	{ {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5} },
	{ {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5} },

	// �ո� (�ﰢ��)
	{ {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.0, 0.0, 0.5} },

	// �����ʸ� (�ﰢ��)
	{ {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {0.0, 0.0, 0.5} },

	// ���ʸ� (�ﰢ��)
	{ {-0.5, -0.5, -0.5}, {0.0, 0.0, 0.5}, {-0.5, 0.5, -0.5} },

	// �޸� (�ﰢ��)
	{ {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.0, 0.0, 0.5} },
};

glm::vec3 colors[12][3] = {};

GLfloat XYShape[3][2][3] = {
	{{-1.0,0.0,0.0},{1.0,0.0,0.0}},
	{{0.0,1.0,0.0},{0.0,-1.0,0.0}},
	{{0.0,0.0,-1.0},{0.0,0.0,1.0}} };

GLfloat XYcolors[4][3] = { //--- �� ����
	{ 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0 }
};

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

glm::vec3 cameraPos = glm::vec3(0.0f, -3.0f, 0.5f); //--- ī�޶� ��ġ
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.5f); //--- ī�޶� �ٶ󺸴� ����
glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f); //--- ī�޶� ���� ����

GLuint vao, vbo[2];
GLuint TriPosVbo, TriColorVbo;

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

int windowWidth = 800;
int windowHeight = 600;

float openGLX, openGLY;
int movingRectangle = -1;

bool c = true;
bool p = false;

bool start = true;
bool depth_test = true;
bool Wselect = false;

float xRotation = 0;
float yRotation = 0;
int xRotateSelect = 0;
int yRotateSelect = 0;

float xTransform = 0;
float yTransform = 0;
int xTranslateSelect = -1;
int yTranslateSelect = -1;

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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	//--- ���� �˻� ��嵵 ���� ����
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
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeys); // ����Ű �ݹ� �Լ� ���
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	glutMainLoop();
}

GLvoid drawScene()
{
	GLUquadricObj * qobj;

	glUseProgram(shaderProgramID);
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //--- ���� ���۸� Ŭ�����Ѵ�.

	glBindVertexArray(vao);

	// ���� �ٲٱ�
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), XYcolors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);


	// ���̴� ���α׷����� modelTransform ���� ��ġ ��������
	int modelLocation = glGetUniformLocation(shaderProgramID, "model"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��� �������� �޾ƿ´�.
	int viewLocation = glGetUniformLocation(shaderProgramID, "view"); //--- ���ؽ� ���̴����� ���� ��ȯ ��� �������� �޾ƿ´�.
	int projLocation = glGetUniformLocation(shaderProgramID, "projection"); //--- ���ؽ� ���̴����� ���� ��ȯ ��� �������� �޾ƿ´�.

	if (start)
	{
		double m = 0.0;
		double cx, cy, cz;
		for (int i = 0; i < 12; i++)
		{
			cx = 0.3 + m;
			cy = 0.0 + m;
			cz = 0.3 + m;

			m += 0.05;

			//glm::vec3 colors[10][3] = {};

			colors[i][0].x = cx;
			//cx = color(gen);
			colors[i][1].x = cx;
			//cx = color(gen);
			colors[i][2].x = cx;

			colors[i][0].y = cy;
			//cy = color(gen);
			colors[i][1].y = cy;
			//cy = color(gen);
			colors[i][2].y = cy;

			colors[i][0].z = cz;
			//cz = color(gen);
			colors[i][1].z = cz;
			//cz = color(gen);
			colors[i][2].z = cz;
		}
		start = false;
	}

	//�� ��ȯ
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	//���� ��ȯ
	glm::mat4 pTransform = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f); //--- ���� ���� ����: fovy, aspect, near, far
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0)); //--- ������ z�� �̵�
	pTransform = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight, 0.1f, 200.0f);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &pTransform[0][0]);
	model = glm::mat4(1.0f);

	//�� �׸���
	for (int i = 0; i < 3; i++)
	{
		model = glm::mat4(1.0f);

		// modelTransform ������ ��ȯ �� �����ϱ�
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), XYShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glLineWidth(1.0);
		glDrawArrays(GL_LINES, 0, 2);
	}

	if (c)
	{
		//�̵�
		if (xTranslateSelect > 0)
		{
			model = glm::translate(model, glm::vec3(xTransform, 0.0f, 0.0f));
		}
		if (yTranslateSelect > 0)
		{
			model = glm::translate(model, glm::vec3(0.0f, yTransform, 0.0f));
		}
		//���� �ִϸ��̼�
		if (xRotateSelect > 0)
		{
			model = glm::rotate(model, glm::radians(xRotation), glm::vec3(1.0f, 0.0f, 0.0f));

		}
		if (yRotateSelect > 0)
		{
			model = glm::rotate(model, glm::radians(yRotation), glm::vec3(0.0f, 1.0f, 0.0f));

		}

		// x������ 10�� ȸ��
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// y������ 10�� ȸ��
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//������ü
		for (int i = 0; i < 12; i++) {
			// modelTransform ������ ��ȯ �� �����ϱ�
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &colors[i][0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &cube[i][0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			if (Wselect)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		// modelTransform ������ ��ȯ �� �����ϱ�
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	}

	if (p)
	{
		//�̵�
		if (xTranslateSelect > 0)
		{
			model = glm::translate(model, glm::vec3(xTransform, 0.0f, 0.0f));
		}
		if (yTranslateSelect > 0)
		{
			model = glm::translate(model, glm::vec3(0.0f, yTransform, 0.0f));
		}
		//���� �ִϸ��̼�
		if (xRotateSelect > 0)
		{
			model = glm::rotate(model, glm::radians(xRotation), glm::vec3(1.0f, 0.0f, 0.0f));

		}
		if (yRotateSelect > 0)
		{
			model = glm::rotate(model, glm::radians(yRotation), glm::vec3(0.0f, 1.0f, 0.0f));

		}
		// x������ 10�� ȸ��
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// y������ 10�� ȸ��
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//�簢��
		for (int i = 0; i < 6; i++) {
			// modelTransform ������ ��ȯ �� �����ϱ�
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &colors[i][0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &pyramid[i][0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			if (Wselect)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		// modelTransform ������ ��ȯ �� �����ϱ�
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	}

	glutSwapBuffers();
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
	glGenBuffers(3, vbo); //--- 3���� VBO�� �����ϰ� �Ҵ��ϱ�
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
	vertexSource = filetobuf("vertex3.glsl");
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
	fragmentSource = filetobuf("fragment3.glsl");
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

bool a = false;
bool b = false;

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		c = true;
		p = false;

		model = glm::mat4(1.0f);
		xRotateSelect = 0;
		yRotateSelect = 0;
		xRotation = 0;
		yRotation = 0;
		xTranslateSelect = -1;
		yTranslateSelect = -1;
		xTransform = 0.0;
		yTransform = 0.0;
		break;
	case 'p':
		c = false;
		p = true;

		model = glm::mat4(1.0f);
		xRotateSelect = 0;
		yRotateSelect = 0;
		xRotation = 0;
		yRotation = 0;
		xTranslateSelect = -1;
		yTranslateSelect = -1;
		xTransform = 0.0;
		yTransform = 0.0;
		break;
	case 'h':
		depth_test = !depth_test;
		if (depth_test)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
		break;
	case 'w':
		Wselect = !Wselect;
		break;
	case 'x':
		a = !a;
		if (a)
		{
			xRotateSelect = 1;
		}
		else
		{
			xRotateSelect = 2;
		}
		break;
	case 'y':
		b = !b;
		if (b)
		{
			yRotateSelect = 1;
		}
		else
		{
			yRotateSelect = 2;
		}
		break;
	case 's':
		model = glm::mat4(1.0f);
		xRotateSelect = 0;
		yRotateSelect = 0;
		xRotation = 0;
		yRotation = 0;
		xTranslateSelect = -1;
		yTranslateSelect = -1;
		xTransform = 0.0;
		yTransform = 0.0;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid SpecialKeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		yTranslateSelect = 1;
		yTransform += 0.05;
		break;
	case GLUT_KEY_DOWN:
		yTranslateSelect = 2;
		yTransform -= 0.05;
		break;
	case GLUT_KEY_LEFT:
		xTranslateSelect = 1;
		xTransform -= 0.05;
		break;
	case GLUT_KEY_RIGHT:
		xTranslateSelect = 2;
		xTransform += 0.05;
		break;
	}
	glutPostRedisplay(); // ȭ�� ����
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
		if (xRotateSelect == 2)
		{
			xRotation += 1;
		}
		if (yRotateSelect == 2)
		{
			yRotation += 1;
		}
		if (xRotateSelect == 1)
		{
			xRotation -= 1;
		}
		if (yRotateSelect == 1)
		{
			yRotation -= 1;
		}
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

//void ReadObj(FILE* path) {
//	char count[128];
//	int vertexnum = 0;
//	int facenum = 0;
//	int normalnum = 0; // vn ���� ī��Ʈ �߰�
//	int uvnum = 0;
//
//	while (!feof(path)) {
//		fscanf(path, "%s", count);
//		if (count[0] == 'v' && count[1] == '\0')
//			vertexnum++;
//		else if (count[0] == 'f' && count[1] == '\0')
//			facenum++;
//		else if (count[0] == 'v' && count[1] == 'n' && count[2] == '\0') // vn ���� ī��Ʈ
//			normalnum++;
//		else if (count[0] == 'v' && count[1] == 't' && count[2] == '\0')
//			uvnum++;
//		memset(count, '\0', sizeof(count));
//	}
//	rewind(path);
//
//	int vertIndex = 0;
//	int faceIndex = 0;
//	int normalIndex = 0; // vn �ε��� �߰�
//	int uvIndex = 0;
//
//	glm::vec3* vertex = new glm::vec3[vertexnum];
//	glm::vec3* face = new glm::vec3[facenum];
//	glm::vec3* normals = new glm::vec3[normalnum]; // vn ���� �迭 �߰�
//	glm::vec3* uvdata = new glm::vec3[facenum]; // uvdata �迭 ũ�� ����
//	glm::vec2* uv = new glm::vec2[uvnum];
//
//	char bind[128];
//
//	while (!feof(path)) {
//		fscanf(path, "%s", bind);
//		if (bind[0] == 'v' && bind[1] == '\0') {
//			fscanf(path, "%f %f %f\n",
//				&vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
//			vertIndex++;
//		}
//		else if (bind[0] == 'f' && bind[1] == '\0') {
//			unsigned int temp_face[3], temp_uv[3], temp_normal[3];
//			fscanf(path, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
//				&temp_face[0], &temp_uv[0], &temp_normal[0],
//				&temp_face[1], &temp_uv[1], &temp_normal[1],
//				&temp_face[2], &temp_uv[2], &temp_normal[2]);
//			face[faceIndex].x = temp_face[0];
//			face[faceIndex].y = temp_face[1];
//			face[faceIndex].z = temp_face[2];
//			uvdata[faceIndex].x = temp_uv[0];
//			uvdata[faceIndex].y = temp_uv[1];
//			uvdata[faceIndex].z = temp_uv[2];
//			// vn ���� ����
//			normals[faceIndex].x = temp_normal[0];
//			normals[faceIndex].y = temp_normal[1];
//			normals[faceIndex].z = temp_normal[2];
//			faceIndex++;
//		}
//		else if (bind[0] == 'v' && bind[1] == 'n' && bind[2] == '\0') {
//			fscanf(path, "%f %f %f\n", &normals[normalIndex].x, &normals[normalIndex].y, &normals[normalIndex].z);
//			normalIndex++;
//		}
//		else if (bind[0] == 'v' && bind[1] == 't' && bind[2] == '\0') {
//			fscanf(path, "%f %f\n", &uv[uvIndex].x, &uv[uvIndex].y);
//			uvIndex++;
//		}
//	}
//	// ������ �ڵ�� �ʿ��� ��쿡 �°� Ȱ��
//}