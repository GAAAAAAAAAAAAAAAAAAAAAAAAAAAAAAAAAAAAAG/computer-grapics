#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLfloat triShape[8][3][3] = {};//--- �ﰢ�� ��ġ ��

GLfloat rectShape[1][4][3] = {}; //--- �簢�� ��ġ ��

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

GLfloat colors[4][3] = { //--- �ﰢ�� ������ ����
	{ 0.0, 0.0, 0.0 },
	{ 1.0, 1.0, 0.0 },
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

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//�� ���
	for (int i = 0; i <2; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), dotShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glPointSize(10.0);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	//�� �׸���
	for (int i = 0; i <10; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), lineShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glLineWidth(1.0);
		glDrawArrays(GL_LINES, 0, 2);
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

GLvoid Mouse(int button, int state, int x, int y)
{
	float openGLX, openGLY;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			WindowToOpenGL(x, y, openGLX, openGLY);

			//������ �̵�
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

			//���� �̵�
			
			//�ﰢ�� �� ����
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
			movingRectangle = -1;  // ���콺 ��ư�� ������ �̵� ��带 �����մϴ�.
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


		// �̵� ���� �簢���� �߽����� ���
		float centerX1 = lineShape[clickNum1][0][0];
		float centerY1 = lineShape[clickNum1][0][1];

		float centerX2 = lineShape[clickNum2][1][0];
		float centerY2 = lineShape[clickNum2][1][1];

		// �̵��� �Ÿ� ���
		float deltaX1 = openGLX - centerX1;
		float deltaY1 = openGLY - centerY1;

		float deltaX2 = openGLX - centerX2;
		float deltaY2 = openGLY - centerY2;

		// �簢�� ��ġ �̵�
		lineShape[clickNum1][0][0] += deltaX1;
		lineShape[clickNum1][0][1] += deltaY1;

		lineShape[clickNum2][1][0] += deltaX2;
		lineShape[clickNum2][1][1] += deltaY2;

		glutPostRedisplay();  // ȭ���� �ٽ� �׸��ϴ�.
	}
	else if (movingRectangle == 1)
	{
		WindowToOpenGL(x, y, openGLX, openGLY);

		glutPostRedisplay();  // ȭ���� �ٽ� �׸��ϴ�.
	}
}