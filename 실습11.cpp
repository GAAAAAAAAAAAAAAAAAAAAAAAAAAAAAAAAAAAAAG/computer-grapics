#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLfloat triShape[9][3][3] = {};//--- �ﰢ�� ��ġ ��
int triCnt = 0;

GLfloat BigTriShape[9][3][3] = {};//--- �ﰢ�� ��ġ ��

GLfloat XYShape[2][2][3] = {
	{{-1.0,0.0,0.0},{1.0,0.0,0.0}},
	{{0.0,1.0,0.0},{0.0,-1.0,0.0}} };

GLfloat colors[4][3] = { //--- �ﰢ�� ������ ����
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
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

bool animating = false;
int aniCnt = 0;

bool Lanimation = false;
bool Tanimation = false;
bool Ranimation = false;
bool Panimation = false;

int Lcnt = 0;
int Tcnt = 0;
int Rcnt = 0;
int Pcnt = 0;

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
	glutTimerFunc(100, TimerFunction, 1);

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

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	if (start)
	{
		/*triShape[0][0][0] = -0.5;
		triShape[0][0][1] = -0.5;
		triShape[0][0][2] =  0.0;
		triShape[0][1][0]  = -0.5;
		triShape[0][1][1]  = -0.5;
		triShape[0][1][2]  = 0.0;
		triShape[0][2][0]  = -0.5;
		triShape[0][2][1]  = -0.5;
		triShape[0][2][2]  =  0.0;*/

		//���� -> �ﰢ�� 
		triShape[0][0][0] = -0.5 - 0.2;
		triShape[0][0][1] = 0.3;
		triShape[0][0][2] = 0.0;
		triShape[0][1][0] = -0.5;
		triShape[0][1][1] = 0.3;	// + 0.4
		triShape[0][1][2] = 0.0;
		triShape[0][2][0] = -0.5 + 0.2;
		triShape[0][2][1] = 0.3;
		triShape[0][2][2] = 0.0;

		//�ﰢ�� -> �簢��
		triShape[1][0][0] = 0.5 - 0.2;
		triShape[1][0][1] = 0.3;
		triShape[1][0][2] =  0.0;
		triShape[1][1][0] = 0.5;	// - 0.2
		triShape[1][1][1] = 0.3 + 0.4;
		triShape[1][1][2] = 0.0;
		triShape[1][2][0] = 0.5 + 0.2;
		triShape[1][2][1] = 0.3;
		triShape[1][2][2] =  0.0;

		triShape[2][0][0] = 0.5;	// - 0.2
		triShape[2][0][1] = 0.3 + 0.4;
		triShape[2][0][2] = 0.0;
		triShape[2][1][0] = 0.5;	// + 0.2
		triShape[2][1][1] = 0.3 + 0.4;
		triShape[2][1][2] = 0.0;
		triShape[2][2][0] = 0.5 + 0.2;
		triShape[2][2][1] = 0.3;
		triShape[2][2][2] = 0.0;

		//�簢�� -> ������
		triShape[3][0][0] = -0.5 - 0.2;
		triShape[3][0][1] = -0.5 - 0.2;	// + 0.1
		triShape[3][0][2] =  0.0;
		triShape[3][1][0]  = -0.5;
		triShape[3][1][1]  = -0.5 - 0.2;
		triShape[3][1][2]  = 0.0;
		triShape[3][2][0]  = -0.5 - 0.2;	//+ 0.1
		triShape[3][2][1]  = -0.5 + 0.2;
		triShape[3][2][2]  =  0.0;

		triShape[4][0][0] = -0.5 - 0.2;	//+0.1
		triShape[4][0][1] = -0.5 + 0.2;
		triShape[4][0][2] =  0.0;
		triShape[4][1][0]  = -0.5;
		triShape[4][1][1]  = -0.5 - 0.2;
		triShape[4][1][2]  = 0.0;
		triShape[4][2][0]  = -0.5 + 0.2;	//-0.1
		triShape[4][2][1]  = -0.5 + 0.2;
		triShape[4][2][2]  =  0.0;

		triShape[5][0][0] = -0.5;
		triShape[5][0][1] = -0.5 - 0.2;
		triShape[5][0][2] = 0.0;
		triShape[5][1][0] = -0.5 + 0.2;
		triShape[5][1][1] = -0.5 - 0.2;	//+0.1
		triShape[5][1][2] = 0.0;
		triShape[5][2][0] = -0.5 + 0.2;	//-0.1
		triShape[5][2][1] = -0.5 + 0.2;
		triShape[5][2][2] = 0.0;

		//������ -> ��
		triShape[6][0][0] = +0.5 - 0.2;			//0.3	+0.2
		triShape[6][0][1] = -0.5 - 0.2 + 0.1;	//-0.6	+0.1
		triShape[6][0][2] = 0.0;
		triShape[6][1][0] = +0.5;				//0.5	
		triShape[6][1][1] = -0.5 - 0.2;			//-0.7	+0.2
		triShape[6][1][2] = 0.0;
		triShape[6][2][0] = +0.5 - 0.2 + 0.1;	//0.4	+0.1
		triShape[6][2][1] = -0.5 + 0.2;			//-0.3	-0.2
		triShape[6][2][2] = 0.0;

		triShape[7][0][0] = +0.5 - 0.2 + 0.1;	//-0.1	+0.1
		triShape[7][0][1] = -0.5 + 0.2;			//		-0.2
		triShape[7][0][2] = 0.0;
		triShape[7][1][0] = +0.5;
		triShape[7][1][1] = -0.5 - 0.2;			//		+0.2
		triShape[7][1][2] = 0.0;
		triShape[7][2][0] = +0.5 + 0.2 - 0.1;	//		-0.1
		triShape[7][2][1] = -0.5 + 0.2;			//		-0.2
		triShape[7][2][2] = 0.0;

		triShape[8][0][0] = +0.5;
		triShape[8][0][1] = -0.5 - 0.2;			//		+0.2
		triShape[8][0][2] = 0.0;
		triShape[8][1][0] = +0.5 + 0.2;			//		-0.2
		triShape[8][1][1] = -0.5 - 0.2 + 0.1;	//		+0.1
		triShape[8][1][2] = 0.0;
		triShape[8][2][0] = +0.5 + 0.2 - 0.1;	//		-0.1
		triShape[8][2][1] = -0.5 + 0.2;			//		-0.2
		triShape[8][2][2] = 0.0;

		// ū ȭ�鿡 ������ ������
		//���� -> �ﰢ�� 
		BigTriShape[0][0][0] = -0.5;
		BigTriShape[0][0][1] = 0.0;
		BigTriShape[0][0][2] = 0.0;
		BigTriShape[0][1][0] = 0.0;
		BigTriShape[0][1][1] = 0.0;	// + 0.8
		BigTriShape[0][1][2] = 0.0;
		BigTriShape[0][2][0] = 0.5;
		BigTriShape[0][2][1] = 0.0;
		BigTriShape[0][2][2] = 0.0;

		//�ﰢ�� -> �簢��
		BigTriShape[1][0][0] = 0.0 - 0.4;
		BigTriShape[1][0][1] = 0.0;
		BigTriShape[1][0][2] = 0.0;
		BigTriShape[1][1][0] = 0.0;	// - 0.4
		BigTriShape[1][1][1] = 0.0 + 0.8;
		BigTriShape[1][1][2] = 0.0;
		BigTriShape[1][2][0] = 0.0 + 0.4;
		BigTriShape[1][2][1] = 0.0;
		BigTriShape[1][2][2] = 0.0;

		BigTriShape[2][0][0] = 0.0;	// - 0.4
		BigTriShape[2][0][1] = 0.0 + 0.8;
		BigTriShape[2][0][2] = 0.0;
		BigTriShape[2][1][0] = 0.0;	// + 0.4
		BigTriShape[2][1][1] = 0.0 + 0.8;
		BigTriShape[2][1][2] = 0.0;
		BigTriShape[2][2][0] = 0.0 + 0.4;
		BigTriShape[2][2][1] = 0.0;
		BigTriShape[2][2][2] = 0.0;

		//�簢�� -> ������
		BigTriShape[3][0][0] = -0.0 - 0.4;
		BigTriShape[3][0][1] = -0.0 - 0.4;	// + 0.2
		BigTriShape[3][0][2] = 0.0;
		BigTriShape[3][1][0] = -0.0;
		BigTriShape[3][1][1] = -0.0 - 0.4;
		BigTriShape[3][1][2] = 0.0;
		BigTriShape[3][2][0] = -0.0 - 0.4;	//+ 0.2
		BigTriShape[3][2][1] = -0.0 + 0.4;
		BigTriShape[3][2][2] = 0.0;

		BigTriShape[4][0][0] = -0.0 - 0.4;	//+0.2
		BigTriShape[4][0][1] = -0.0 + 0.4;
		BigTriShape[4][0][2] = 0.0;
		BigTriShape[4][1][0] = -0.0;
		BigTriShape[4][1][1] = -0.0 - 0.4;
		BigTriShape[4][1][2] = 0.0;
		BigTriShape[4][2][0] = -0.0 + 0.4;	//-0.2
		BigTriShape[4][2][1] = -0.0 + 0.4;
		BigTriShape[4][2][2] = 0.0;

		BigTriShape[5][0][0] = -0.0;
		BigTriShape[5][0][1] = -0.0 - 0.4;
		BigTriShape[5][0][2] = 0.0;
		BigTriShape[5][1][0] = -0.0 + 0.4;
		BigTriShape[5][1][1] = -0.0 - 0.4;	//+0.2
		BigTriShape[5][1][2] = 0.0;
		BigTriShape[5][2][0] = -0.0 + 0.4;	//-0.2
		BigTriShape[5][2][1] = -0.0 + 0.4;
		BigTriShape[5][2][2] = 0.0;

		//������ -> ��
		BigTriShape[6][0][0] = +0.0 - 0.4;			//0.3	+0.2
		BigTriShape[6][0][1] = -0.0 - 0.4 + 0.2;	//-0.6	+0.1
		BigTriShape[6][0][2] = 0.0;
		BigTriShape[6][1][0] = +0.0;				//0.5	
		BigTriShape[6][1][1] = -0.0 - 0.4;			//-0.7	+0.2
		BigTriShape[6][1][2] = 0.0;
		BigTriShape[6][2][0] = +0.0 - 0.4 + 0.2;	//0.4	+0.1
		BigTriShape[6][2][1] = -0.0 + 0.4;			//-0.3	-0.2
		BigTriShape[6][2][2] = 0.0;
	
		BigTriShape[7][0][0] = +0.0 - 0.4 + 0.2;	//-0.1	+0.1
		BigTriShape[7][0][1] = -0.0 + 0.4;			//		-0.2
		BigTriShape[7][0][2] = 0.0;
		BigTriShape[7][1][0] = +0.0;
		BigTriShape[7][1][1] = -0.0 - 0.4;			//		+0.2
		BigTriShape[7][1][2] = 0.0;
		BigTriShape[7][2][0] = +0.0 + 0.4 - 0.2;	//		-0.1
		BigTriShape[7][2][1] = -0.0 + 0.4;			//		-0.2
		BigTriShape[7][2][2] = 0.0;
		
		BigTriShape[8][0][0] = +0.0;
		BigTriShape[8][0][1] = -0.0 - 0.4;			//		+0.2
		BigTriShape[8][0][2] = 0.0;
		BigTriShape[8][1][0] = +0.0 + 0.4;			//		-0.2
		BigTriShape[8][1][1] = -0.0 - 0.4 + 0.2;	//		+0.1
		BigTriShape[8][1][2] = 0.0;
		BigTriShape[8][2][0] = +0.0 + 0.4 - 0.2;	//		-0.1
		BigTriShape[8][2][1] = -0.0 + 0.4;			//		-0.2
		BigTriShape[8][2][2] = 0.0;

		start = false;
	}

	if (!Lanimation && !Tanimation && !Ranimation && !Panimation)
	{
		//�� �׸���
		for (int i = 0; i < 2; i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), XYShape[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glLineWidth(1.0);
			glDrawArrays(GL_LINES, 0, 2);
		}

		//�ﰢ�� �׸���
		for (int i = 0; i < 9; i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	//ū ȭ�� ������ �׸���
	if (Lanimation)
	{
		for (int i = 0; i < 1; i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), BigTriShape[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
	if (Tanimation)
	{
		for (int i = 1; i < 3; i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), BigTriShape[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
	if (Ranimation)
	{
		for (int i = 3; i < 6; i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), BigTriShape[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
	if (Panimation)
	{
		for (int i = 6; i < 9; i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), BigTriShape[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_TRIANGLES, 0, 3);
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
	case 'a':
		Lanimation = false;
		Tanimation = false;
		Ranimation = false;
		Panimation = false;

		//���� -> �ﰢ�� 
		triShape[0][0][0] = -0.5 - 0.2;
		triShape[0][0][1] = 0.3;
		triShape[0][0][2] = 0.0;
		triShape[0][1][0] = -0.5;
		triShape[0][1][1] = 0.3;	// + 0.4
		triShape[0][1][2] = 0.0;
		triShape[0][2][0] = -0.5 + 0.2;
		triShape[0][2][1] = 0.3;
		triShape[0][2][2] = 0.0;

		//�ﰢ�� -> �簢��
		triShape[1][0][0] = 0.5 - 0.2;
		triShape[1][0][1] = 0.3;
		triShape[1][0][2] = 0.0;
		triShape[1][1][0] = 0.5;	// - 0.2
		triShape[1][1][1] = 0.3 + 0.4;
		triShape[1][1][2] = 0.0;
		triShape[1][2][0] = 0.5 + 0.2;
		triShape[1][2][1] = 0.3;
		triShape[1][2][2] = 0.0;

		triShape[2][0][0] = 0.5;	// - 0.2
		triShape[2][0][1] = 0.3 + 0.4;
		triShape[2][0][2] = 0.0;
		triShape[2][1][0] = 0.5;	// + 0.2
		triShape[2][1][1] = 0.3 + 0.4;
		triShape[2][1][2] = 0.0;
		triShape[2][2][0] = 0.5 + 0.2;
		triShape[2][2][1] = 0.3;
		triShape[2][2][2] = 0.0;

		//�簢�� -> ������
		triShape[3][0][0] = -0.5 - 0.2;
		triShape[3][0][1] = -0.5 - 0.2;	// + 0.1
		triShape[3][0][2] = 0.0;
		triShape[3][1][0] = -0.5;
		triShape[3][1][1] = -0.5 - 0.2;
		triShape[3][1][2] = 0.0;
		triShape[3][2][0] = -0.5 - 0.2;	//+ 0.1
		triShape[3][2][1] = -0.5 + 0.2;
		triShape[3][2][2] = 0.0;

		triShape[4][0][0] = -0.5 - 0.2;	//+0.1
		triShape[4][0][1] = -0.5 + 0.2;
		triShape[4][0][2] = 0.0;
		triShape[4][1][0] = -0.5;
		triShape[4][1][1] = -0.5 - 0.2;
		triShape[4][1][2] = 0.0;
		triShape[4][2][0] = -0.5 + 0.2;	//-0.1
		triShape[4][2][1] = -0.5 + 0.2;
		triShape[4][2][2] = 0.0;

		triShape[5][0][0] = -0.5;
		triShape[5][0][1] = -0.5 - 0.2;
		triShape[5][0][2] = 0.0;
		triShape[5][1][0] = -0.5 + 0.2;
		triShape[5][1][1] = -0.5 - 0.2;	//+0.1
		triShape[5][1][2] = 0.0;
		triShape[5][2][0] = -0.5 + 0.2;	//-0.1
		triShape[5][2][1] = -0.5 + 0.2;
		triShape[5][2][2] = 0.0;

		//������ -> ��
		triShape[6][0][0] = +0.5 - 0.2;			//0.3	+0.2
		triShape[6][0][1] = -0.5 - 0.2 + 0.1;	//-0.6	+0.1
		triShape[6][0][2] = 0.0;
		triShape[6][1][0] = +0.5;				//0.5	
		triShape[6][1][1] = -0.5 - 0.2;			//-0.7	+0.2
		triShape[6][1][2] = 0.0;
		triShape[6][2][0] = +0.5 - 0.2 + 0.1;	//0.4	+0.1
		triShape[6][2][1] = -0.5 + 0.2;			//-0.3	-0.2
		triShape[6][2][2] = 0.0;

		triShape[7][0][0] = +0.5 - 0.2 + 0.1;	//-0.1	+0.1
		triShape[7][0][1] = -0.5 + 0.2;			//		-0.2
		triShape[7][0][2] = 0.0;
		triShape[7][1][0] = +0.5;
		triShape[7][1][1] = -0.5 - 0.2;			//		+0.2
		triShape[7][1][2] = 0.0;
		triShape[7][2][0] = +0.5 + 0.2 - 0.1;	//		-0.1
		triShape[7][2][1] = -0.5 + 0.2;			//		-0.2
		triShape[7][2][2] = 0.0;

		triShape[8][0][0] = +0.5;
		triShape[8][0][1] = -0.5 - 0.2;			//		+0.2
		triShape[8][0][2] = 0.0;
		triShape[8][1][0] = +0.5 + 0.2;			//		-0.2
		triShape[8][1][1] = -0.5 - 0.2 + 0.1;	//		+0.1
		triShape[8][1][2] = 0.0;
		triShape[8][2][0] = +0.5 + 0.2 - 0.1;	//		-0.1
		triShape[8][2][1] = -0.5 + 0.2;			//		-0.2
		triShape[8][2][2] = 0.0;

		animating = true;
		break;
	case 'l':
		BigTriShape[0][0][0] = -0.5;
		BigTriShape[0][0][1] = 0.0;
		BigTriShape[0][0][2] = 0.0;
		BigTriShape[0][1][0] = 0.0;
		BigTriShape[0][1][1] = 0.0;	// + 0.8
		BigTriShape[0][1][2] = 0.0;
		BigTriShape[0][2][0] = 0.5;
		BigTriShape[0][2][1] = 0.0;
		BigTriShape[0][2][2] = 0.0;

		Lanimation = true;
		Tanimation = false;
		Ranimation = false;
		Panimation = false;
		break;
	case 't':
		//�ﰢ�� -> �簢��
		BigTriShape[1][0][0] = 0.0 - 0.4;
		BigTriShape[1][0][1] = 0.0;
		BigTriShape[1][0][2] = 0.0;
		BigTriShape[1][1][0] = 0.0;	// - 0.4
		BigTriShape[1][1][1] = 0.0 + 0.8;
		BigTriShape[1][1][2] = 0.0;
		BigTriShape[1][2][0] = 0.0 + 0.4;
		BigTriShape[1][2][1] = 0.0;
		BigTriShape[1][2][2] = 0.0;

		BigTriShape[2][0][0] = 0.0;	// - 0.4
		BigTriShape[2][0][1] = 0.0 + 0.8;
		BigTriShape[2][0][2] = 0.0;
		BigTriShape[2][1][0] = 0.0;	// + 0.4
		BigTriShape[2][1][1] = 0.0 + 0.8;
		BigTriShape[2][1][2] = 0.0;
		BigTriShape[2][2][0] = 0.0 + 0.4;
		BigTriShape[2][2][1] = 0.0;
		BigTriShape[2][2][2] = 0.0;

		Lanimation = false;
		Tanimation = true;
		Ranimation = false;
		Panimation = false;
		break;
	case 'r':
		//�簢�� -> ������
		BigTriShape[3][0][0] = -0.0 - 0.4;
		BigTriShape[3][0][1] = -0.0 - 0.4;	// + 0.2
		BigTriShape[3][0][2] = 0.0;
		BigTriShape[3][1][0] = -0.0;
		BigTriShape[3][1][1] = -0.0 - 0.4;
		BigTriShape[3][1][2] = 0.0;
		BigTriShape[3][2][0] = -0.0 - 0.4;	//+ 0.2
		BigTriShape[3][2][1] = -0.0 + 0.4;
		BigTriShape[3][2][2] = 0.0;

		BigTriShape[4][0][0] = -0.0 - 0.4;	//+0.2
		BigTriShape[4][0][1] = -0.0 + 0.4;
		BigTriShape[4][0][2] = 0.0;
		BigTriShape[4][1][0] = -0.0;
		BigTriShape[4][1][1] = -0.0 - 0.4;
		BigTriShape[4][1][2] = 0.0;
		BigTriShape[4][2][0] = -0.0 + 0.4;	//-0.2
		BigTriShape[4][2][1] = -0.0 + 0.4;
		BigTriShape[4][2][2] = 0.0;

		BigTriShape[5][0][0] = -0.0;
		BigTriShape[5][0][1] = -0.0 - 0.4;
		BigTriShape[5][0][2] = 0.0;
		BigTriShape[5][1][0] = -0.0 + 0.4;
		BigTriShape[5][1][1] = -0.0 - 0.4;	//+0.2
		BigTriShape[5][1][2] = 0.0;
		BigTriShape[5][2][0] = -0.0 + 0.4;	//-0.2
		BigTriShape[5][2][1] = -0.0 + 0.4;
		BigTriShape[5][2][2] = 0.0;

		Lanimation = false;
		Tanimation = false;
		Ranimation = true;
		Panimation = false;
		break;
	case 'p':
		//������ -> ��
		BigTriShape[6][0][0] = +0.0 - 0.4;			//0.3	+0.2
		BigTriShape[6][0][1] = -0.0 - 0.4 + 0.2;	//-0.6	+0.1
		BigTriShape[6][0][2] = 0.0;
		BigTriShape[6][1][0] = +0.0;				//0.5	
		BigTriShape[6][1][1] = -0.0 - 0.4;			//-0.7	+0.2
		BigTriShape[6][1][2] = 0.0;
		BigTriShape[6][2][0] = +0.0 - 0.4 + 0.2;	//0.4	+0.1
		BigTriShape[6][2][1] = -0.0 + 0.4;			//-0.3	-0.2
		BigTriShape[6][2][2] = 0.0;

		BigTriShape[7][0][0] = +0.0 - 0.4 + 0.2;	//-0.1	+0.1
		BigTriShape[7][0][1] = -0.0 + 0.4;			//		-0.2
		BigTriShape[7][0][2] = 0.0;
		BigTriShape[7][1][0] = +0.0;
		BigTriShape[7][1][1] = -0.0 - 0.4;			//		+0.2
		BigTriShape[7][1][2] = 0.0;
		BigTriShape[7][2][0] = +0.0 + 0.4 - 0.2;	//		-0.1
		BigTriShape[7][2][1] = -0.0 + 0.4;			//		-0.2
		BigTriShape[7][2][2] = 0.0;

		BigTriShape[8][0][0] = +0.0;
		BigTriShape[8][0][1] = -0.0 - 0.4;			//		+0.2
		BigTriShape[8][0][2] = 0.0;
		BigTriShape[8][1][0] = +0.0 + 0.4;			//		-0.2
		BigTriShape[8][1][1] = -0.0 - 0.4 + 0.2;	//		+0.1
		BigTriShape[8][1][2] = 0.0;
		BigTriShape[8][2][0] = +0.0 + 0.4 - 0.2;	//		-0.1
		BigTriShape[8][2][1] = -0.0 + 0.4;			//		-0.2
		BigTriShape[8][2][2] = 0.0;

		Lanimation = false;
		Tanimation = false;
		Ranimation = false;
		Panimation = true;
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
	}
}

GLvoid WindowToOpenGL(int mouseX, int mouseY, float& x, float& y)
{
	x = (2.0f * mouseX) / windowWidth - 1.0f;
	y = 1.0f - (2.0f * mouseY) / windowHeight;
}

GLvoid TimerFunction(int value)
{
	if (animating)
	{
		//1
		triShape[0][1][1] += 0.02;

		//2
		triShape[1][1][0] -= 0.01;	// - 0.2
		triShape[2][0][0] -= 0.01;	// - 0.2
		triShape[2][1][0] += 0.01;	// + 0.2
	
		//3
		triShape[3][0][1] += 0.005;	//+0.1
		triShape[3][2][0] += 0.005;	//+0.1
		triShape[4][0][0] += 0.005;	//+0.1
		triShape[4][2][0] -= 0.005;	//-0.1
		triShape[5][1][1] += 0.005;	//+0.1
		triShape[5][2][0] -= 0.005;	//-0.1
		
		//4
		triShape[6][0][0] += 0.01;	//	+0.2
		triShape[6][0][1] += 0.005;	//	+0.1
		triShape[6][1][1] += 0.01;	//	+0.2
		triShape[6][2][0] += 0.005;	//	+0.1
		triShape[6][2][1] -= 0.01;	//	-0.2

		triShape[7][0][0] += 0.005;	//  +0.1
		triShape[7][0][1] -= 0.01;	//	-0.2
		triShape[7][1][1] += 0.01;	//	+0.2
		triShape[7][2][0] -= 0.005;	//	-0.1
		triShape[7][2][1] -= 0.01;	//	-0.2

		triShape[8][0][1] += 0.01;	//	+0.2
		triShape[8][1][0] -= 0.01;	//	-0.2
		triShape[8][1][1] += 0.005;	//	+0.1
		triShape[8][2][0] -= 0.005;	//	-0.1
		triShape[8][2][1] -= 0.01;	//	-0.2

		aniCnt++;
		if (aniCnt > 19)
		{
			triShape[6][0][0] -= 0.01;	//	+0.2
			triShape[6][0][1] -= 0.005;	//	+0.1
			triShape[6][1][1] -= 0.01;	//	+0.2
			triShape[6][2][0] -= 0.005;	//	+0.1
			triShape[6][2][1] += 0.01;	//	-0.2

			triShape[7][0][0] -= 0.005;	//  +0.1
			triShape[7][0][1] += 0.01;	//	-0.2
			triShape[7][1][1] -= 0.01;	//	+0.2
			triShape[7][2][0] += 0.005;	//	-0.1
			triShape[7][2][1]+= 0.01;	//	-0.2

			triShape[8][0][1] -= 0.01;	//	+0.2
			triShape[8][1][0] += 0.01;	//	-0.2
			triShape[8][1][1] -= 0.005;	//	+0.1
			triShape[8][2][0] += 0.005;	//	-0.1
			triShape[8][2][1] += 0.01;	//	-0.2

			animating = false;
			aniCnt = 0;
		}
	}
	if (Lanimation)
	{
		BigTriShape[0][1][1] += 0.02 * 2;
		Lcnt++;
		if (Lcnt > 19)
		{
			Lanimation = false;
			Lcnt = 0;
		}
	}
	if (Tanimation)
	{
		//2
		BigTriShape[1][1][0] -= 0.01*2;	// - 0.2
		BigTriShape[2][0][0] -= 0.01*2;	// - 0.2
		BigTriShape[2][1][0] += 0.01*2;	// + 0.2
		Tcnt++;
		if (Tcnt > 19)
		{
			Tanimation = false;
			Tcnt = 0;
		}
	}
	if (Ranimation)
	{
		//3
		BigTriShape[3][0][1] += 0.005*2;	//+0.1
		BigTriShape[3][2][0] += 0.005 * 2;	//+0.1
		BigTriShape[4][0][0] += 0.005 * 2;	//+0.1
		BigTriShape[4][2][0] -= 0.005 * 2;	//-0.1
		BigTriShape[5][1][1] += 0.005 * 2;	//+0.1
		BigTriShape[5][2][0] -= 0.005 * 2;	//-0.1
		Rcnt++;
		if (Rcnt > 19)
		{
			Ranimation = false;
			Rcnt = 0;
		}
	}
	if (Panimation)
	{
		//4
		BigTriShape[6][0][0] += 0.01*2;	//	+0.2
		BigTriShape[6][0][1] += 0.005 * 2;	//	+0.1
		BigTriShape[6][1][1] += 0.01 * 2;	//	+0.2
		BigTriShape[6][2][0] += 0.005 * 2;	//	+0.1
		BigTriShape[6][2][1] -= 0.01 * 2;	//	-0.2
		
		BigTriShape[7][0][0] += 0.005 * 2;	//  +0.1
		BigTriShape[7][0][1] -= 0.01 * 2;	//	-0.2
		BigTriShape[7][1][1] += 0.01 * 2;	//	+0.2
		BigTriShape[7][2][0] -= 0.005 * 2;	//	-0.1
		BigTriShape[7][2][1] -= 0.01 * 2;	//	-0.2
		
		BigTriShape[8][0][1] += 0.01 * 2;	//	+0.2
		BigTriShape[8][1][0] -= 0.01 * 2;	//	-0.2
		BigTriShape[8][1][1] += 0.005 * 2;	//	+0.1
		BigTriShape[8][2][0] -= 0.005 * 2;	//	-0.1
		BigTriShape[8][2][1] -= 0.01 * 2;	//	-0.2

		Pcnt++;
		if (Pcnt > 19)
		{
			BigTriShape[6][0][0] -= 0.01 * 2;	//	+0.2
			BigTriShape[6][0][1] -= 0.005 * 2;	//	+0.1
			BigTriShape[6][1][1] -= 0.01 * 2;	//	+0.2
			BigTriShape[6][2][0] -= 0.005 * 2;	//	+0.1
			BigTriShape[6][2][1] += 0.01 * 2;	//	-0.2
			
			BigTriShape[7][0][0] -= 0.005 * 2;	//  +0.1
			BigTriShape[7][0][1] += 0.01 * 2;	//	-0.2
			BigTriShape[7][1][1] -= 0.01 * 2;	//	+0.2
			BigTriShape[7][2][0] += 0.005 * 2;	//	-0.1
			BigTriShape[7][2][1] += 0.01 * 2;	//	-0.2
			
			BigTriShape[8][0][1] -= 0.01 * 2;	//	+0.2
			BigTriShape[8][1][0] += 0.01 * 2;	//	-0.2
			BigTriShape[8][1][1] -= 0.005 * 2;	//	+0.1
			BigTriShape[8][2][0] += 0.005 * 2;	//	-0.1
			BigTriShape[8][2][1] += 0.01 * 2;	//	-0.2

			Panimation = false;
			Pcnt = 0;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}