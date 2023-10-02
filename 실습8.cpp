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
uniform_real_distribution<double> dis(0.15, 0.25);
uniform_real_distribution<double> colorDis(0.0, 1.0);

GLfloat triShape[4][3][3] = {};//--- �ﰢ�� ��ġ ��

bool a = true;
bool b = false;

GLfloat XYShape[2][2][3] = {
	{{-1.0,0.0,0.0},{1.0,0.0,0.0}},
	{{0.0,1.0,0.0},{0.0,-1.0,0.0}} };
int AllCnt = 0;

GLfloat colors[4][3][3] = {}; //--- �ﰢ�� ������ ����

GLuint vao, vbo[2];
GLuint TriPosVbo, TriColorVbo;

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

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

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);

	glutMainLoop();
}

bool start = true;

GLvoid drawScene()
{
	//--- ����� ���� ����
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);
	//--- ����� VAO �ҷ�����
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

	//�� �׸���
	for (int i = 0; i < 2; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), XYShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glLineWidth(10.0);
		glDrawArrays(GL_LINES, 0, 2);
	}

	//�ﰢ�� �׸���
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
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
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
