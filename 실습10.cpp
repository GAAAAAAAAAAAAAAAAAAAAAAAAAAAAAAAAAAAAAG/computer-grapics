#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <cmath>
#include <vector>

GLfloat triShape[10][3][3] = {};//--- �ﰢ�� ��ġ ��
int triCnt = 0;
bool T = false;

GLfloat rectShape[10][4][3] = {}; //--- �簢�� ��ġ ��
int rectCnt = 0;
bool R = false;

GLfloat lineShape[10][2][3] = {};
int lineCnt = 0;
bool L = false;

GLfloat dotShape[10][3] = {};
int dotCnt = 0;
bool D = true;

int AllCnt = 0;

GLfloat colors[4][3] = { //--- �ﰢ�� ������ ����
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0 }
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

bool drawSpiral = false;
bool drawClockwise = true;
float spiralRadius = 0.0f;
const float spiralIncrement = 0.002f;
bool drawSecondSpiral = false;

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
void draw_Spiral();
void initialize();

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

	initialize(); // �ʱ�ȭ �Լ� ȣ��

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);

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

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	if (drawSpiral) {
		draw_Spiral();
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
	case 'p':
		drawSpiral = true;
		drawClockwise = true;
		break;
	case'l':
		drawSpiral = true;
		drawClockwise = false;
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
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

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		WindowToOpenGL(x, y, openGLX, openGLY);

		if (!drawSpiral) {
			// ù ��° Ŭ������ �����̷� �׸��� ����
			dotShape[0][0] = openGLX;
			dotShape[0][1] = openGLY;
			dotShape[0][2] = 0.0f;

			drawSpiral = true;
			drawClockwise = true; // ���÷� �ð�������� ����
		}
	}
	glutPostRedisplay();
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}

//void draw_Spiral(float centerX, float centerY) {
//	std::vector<GLfloat> vertices;
//
//	float direction = (drawClockwise) ? 1.0f : -1.0f;
//
//	for (float theta = 0.0f; theta <= 6.0f * 3.14159f; theta += 0.1f) {
//		float x = centerX + spiralRadius * cos(theta) * direction;
//		float y = centerY + spiralRadius * sin(theta) * direction;
//
//		vertices.push_back(x);
//		vertices.push_back(y);
//		vertices.push_back(0.0f);
//
//		spiralRadius += spiralIncrement;
//
//		if (spiralRadius > 0.5f) {
//			drawSpiral = false;
//			spiralRadius = 0.0f;
//
//			// �� ��° �����̷� �׸��� ����
//			drawClockwise = !drawClockwise;
//			dotShape[1][0] = x + 0.2f;  // x��ǥ�� ���� ��ġ���� ���������� 0.2f��ŭ �̵�
//			dotShape[1][1] = y;
//			dotShape[1][2] = 0.0f;
//
//			// ���� �����̷��� ������ �� ��° �����̷��� ���������� ����
//			dotShape[0][0] = x;
//			dotShape[0][1] = y;
//			dotShape[0][2] = 0.0f;
//
//			drawSpiral = true;
//
//			break;
//		}
//	}
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(0);
//
//	glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
//}

const double radiusChangeRate = 0.05;

// ���� ��ȭ��
const double angleChangeRate = 0.1;

// �ʱ� ������
double radius = 0.5;

// �ʱ� ����
double angle = 0.0;

// �� �����̷��� �׸��� �Լ�
void draw_Spiral() {
	glBegin(GL_POINTS);

	// �� �����̷��� �׸��� ����
	for (int i = 0; i < 1000; ++i) {
		double x = radius * cos(angle);
		double y = radius * sin(angle);

		glVertex2d(x, y);

		// �������� ���� ����
		radius += radiusChangeRate;
		angle += angleChangeRate;
	}

	glEnd();
}

void initialize()
{
	drawSpiral = false;
	drawClockwise = true;
	spiralRadius = 0.0f;
	drawSecondSpiral = false;
}