#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

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

	//�� ���
	for (int i = 0; i < dotCnt; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), dotShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glPointSize(10.0);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	//�� �׸���
	for (int i = 0; i < lineCnt; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), lineShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glLineWidth(10.0);
		glDrawArrays(GL_LINES, 0, 2);
	}
	//�ﰢ�� �׸���
	for (int i = 0; i < triCnt; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	//�簢�� �׸���
	for (int i = 0; i < rectCnt; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), rectShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_QUADS, 0, 4);
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
		D = true;
		L = false;
		T = false;
		R = false;
		break;
	case 'l':
		D = false;
		L = true;
		T = false;
		R = false;
		break;
	case 't':
		D = false;
		L = false;
		T = true;
		R = false;
		break;
	case 'r':
		D = false;
		L = false;
		T = false;
		R = true;
		break;
	case 'w':
		for (int i = 0; i < AllCnt; i++)
		{
			dotShape[i][1] += 0.05;

			lineShape[i][0][1] += 0.05;
			lineShape[i][1][1] += 0.05;

			triShape[i][0][1] += 0.05;
			triShape[i][1][1] += 0.05;
			triShape[i][2][1] += 0.05;

			rectShape[i][0][1] += 0.05;
			rectShape[i][1][1] += 0.05;
			rectShape[i][2][1] += 0.05;
			rectShape[i][3][1] += 0.05;
		}
		break;
	case 'a':
		for (int i = 0; i < AllCnt; i++)
		{
			dotShape[i][0] -= 0.05;

			lineShape[i][0][0] -= 0.05;
			lineShape[i][1][0] -= 0.05;

			triShape[i][0][0] -= 0.05;
			triShape[i][1][0] -= 0.05;
			triShape[i][2][0] -= 0.05;

			rectShape[i][0][0] -= 0.05;
			rectShape[i][1][0] -= 0.05;
			rectShape[i][2][0] -= 0.05;
			rectShape[i][3][0] -= 0.05;
		}
		break;
	case 's':
		for (int i = 0; i < AllCnt; i++)
		{
			dotShape[i][1] -= 0.05;

			lineShape[i][0][1] -= 0.05;
			lineShape[i][1][1] -= 0.05;

			triShape[i][0][1] -= 0.05;
			triShape[i][1][1] -= 0.05;
			triShape[i][2][1] -= 0.05;

			rectShape[i][0][1] -= 0.05;
			rectShape[i][1][1] -= 0.05;
			rectShape[i][2][1] -= 0.05;
			rectShape[i][3][1] -= 0.05;
		}
		break;
	case 'd':
		for (int i = 0; i < AllCnt; i++)
		{
			dotShape[i][0] += 0.05;

			lineShape[i][0][0] += 0.05;
			lineShape[i][1][0] += 0.05;

			triShape[i][0][0] += 0.05;
			triShape[i][1][0] += 0.05;
			triShape[i][2][0] += 0.05;

			rectShape[i][0][0] += 0.05;
			rectShape[i][1][0] += 0.05;
			rectShape[i][2][0] += 0.05;
			rectShape[i][3][0] += 0.05;
		}
		break;
	case 'c':
		AllCnt = 0;
		dotCnt = 0;
		lineCnt = 0;
		triCnt = 0;
		rectCnt = 0;
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

		if (AllCnt < 10)
		{
			if (D && dotCnt < 10)
			{
				dotShape[dotCnt][0] = openGLX;
				dotShape[dotCnt][1] = openGLY;
				dotShape[dotCnt][2] = 0.0;

				dotCnt++;
			}
			else if (L && lineCnt < 10)
			{
				lineShape[lineCnt][0][0] = openGLX-0.25;
				lineShape[lineCnt][0][1] = openGLY;
				lineShape[lineCnt][0][2] = 0.0;
				lineShape[lineCnt][1][0] = openGLX + 0.25;
				lineShape[lineCnt][1][1] = openGLY;
				lineShape[lineCnt][1][2] = 0.0;

				lineCnt++;
			}
			else if (T && triCnt < 10)
			{
				triShape[triCnt][0][0] = openGLX - 0.25;
				triShape[triCnt][0][1] = openGLY - 0.25;
				triShape[triCnt][0][2] = 0.0;
				triShape[triCnt][1][0] = openGLX + 0.25;
				triShape[triCnt][1][1] = openGLY - 0.25;
				triShape[triCnt][1][2] = 0.0;
				triShape[triCnt][2][0] = openGLX;
				triShape[triCnt][2][1] = openGLY+0.5;
				triShape[triCnt][2][2] = 0.0;

				triCnt++;
			}
			else if (R && rectCnt < 10)
			{
				rectShape[rectCnt][0][0] = openGLX - 0.25;
				rectShape[rectCnt][0][1] = openGLY - 0.25;
				rectShape[rectCnt][0][2] = 0.0;
				rectShape[rectCnt][1][0] = openGLX + 0.25;
				rectShape[rectCnt][1][1] = openGLY - 0.25;
				rectShape[rectCnt][1][2] = 0.0;
				rectShape[rectCnt][2][0] = openGLX + 0.25;
				rectShape[rectCnt][2][1] = openGLY + 0.25;
				rectShape[rectCnt][2][2] = 0.0;
				rectShape[rectCnt][3][0] = openGLX - 0.25;
				rectShape[rectCnt][3][1] = openGLY + 0.25;
				rectShape[rectCnt][3][2] = 0.0;

				rectCnt++;
			}
			AllCnt++;
		}
		WindowToOpenGL(x, y, openGLX, openGLY);
	}
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}
