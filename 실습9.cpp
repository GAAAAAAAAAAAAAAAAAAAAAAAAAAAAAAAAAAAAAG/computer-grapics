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
uniform_real_distribution<double> dis(0.10, 0.15);
uniform_real_distribution<double> colorDis(0.0, 1.0);
uniform_int_distribution<int> directDis(0,3);

GLfloat triShape[4][3][3] = {};//--- �ﰢ�� ��ġ ��
int triCnt = 0;
int triDirection_1[4];

int triDirection_2[4];
int downCnt_2[4]{};

int triDirection_3[4];
bool start_3 = true;
int triCnt_3[4];

int triDirection_4[4];
double theta[4];
double r[4];
int triDirection[4];
int triCnt_4[4];

double width = 0.3, height = 0.4;
GLfloat temp1[4][3][3];
GLfloat temp2[4][3][3];
GLfloat temp3[4][3][3];
GLfloat temp4[4][3][3];

bool start4 = true;

bool a = true;
bool b = false;

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
GLvoid TimerFunction(int value);
void rotation(int i, int direction);

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
	glutTimerFunc(10, TimerFunction, 1);

	glutMainLoop();
}

bool start = true;

int select = 0;

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
	case '1':
		select = 1;
		break;
	case '2':
		select = 2;
		break;
	case '3':
		select = 3;
		break;
	case '4':
		select = 4;
		start4 = true;
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

		if (triCnt < 4)
		{
			//randX = dis(gen);
			//randY = dis(gen);

			triShape[triCnt][0][0] = openGLX - 0.15;
			triShape[triCnt][0][1] = openGLY - 0.15;
			triShape[triCnt][0][2] = 0.0;
			triShape[triCnt][1][0] = openGLX + 0.15;
			triShape[triCnt][1][1] = openGLY - 0.15;
			triShape[triCnt][1][2] = 0.0;
			triShape[triCnt][2][0] = openGLX;
			triShape[triCnt][2][1] = openGLY + 0.25;
			triShape[triCnt][2][2] = 0.0;

			colors[triCnt][0][0] = colorDis(gen);
			colors[triCnt][0][1] = colorDis(gen);
			colors[triCnt][0][2] = colorDis(gen);
			colors[triCnt][1][0] = colorDis(gen);
			colors[triCnt][1][1] = colorDis(gen);
			colors[triCnt][1][2] = colorDis(gen);
			colors[triCnt][2][0] = colorDis(gen);
			colors[triCnt][2][1] = colorDis(gen);
			colors[triCnt][2][2] = colorDis(gen);

			triCnt++;
		}

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
	switch (select)
	{
	case 1:
		for (int i = 0; i < 4; i++)
		{
			switch (triDirection_1[i])
			{
			case 0:
				triShape[i][0][0] += 0.01 * (i + 1);
				triShape[i][0][1] += 0.01 * (i + 1);
				triShape[i][1][0] += 0.01 * (i + 1);
				triShape[i][1][1] += 0.01 * (i + 1);
				triShape[i][2][0] += 0.01 * (i + 1);
				triShape[i][2][1] += 0.01 * (i + 1);
				for (int j = 0; j < 3; j++)
				{
					if (triShape[i][j][0] < -1 || triShape[i][j][0] > 1 || triShape[i][j][1] < -1 || triShape[i][j][1] > 1)
					{
						triDirection_1[i]++;
						if (triDirection_1[i] > 3)
						{
							triDirection_1[i] = 0;
						}

						triShape[i][0][0] -= 0.01 * (i + 1)*2;
						triShape[i][0][1] -= 0.01 * (i + 1)*2;
						triShape[i][1][0] -= 0.01 * (i + 1)*2;
						triShape[i][1][1] -= 0.01 * (i + 1)*2;
						triShape[i][2][0] -= 0.01 * (i + 1)*2;
						triShape[i][2][1] -= 0.01 * (i + 1)*2;

						rotation(i, triDirection_1[i]);
						break;
					}
				}
				break;
			case 1:
				triShape[i][0][0] -= 0.01* (i + 1);
				triShape[i][0][1] += 0.01* (i + 1);
				triShape[i][1][0] -= 0.01* (i + 1);
				triShape[i][1][1] += 0.01* (i + 1);
				triShape[i][2][0] -= 0.01* (i + 1);
				triShape[i][2][1] += 0.01* (i + 1);
				for (int j = 0; j < 3; j++)
				{
					if (triShape[i][j][0] < -1 || triShape[i][j][0] > 1 || triShape[i][j][1] < -1 || triShape[i][j][1] > 1)
					{
						triDirection_1[i]++;
						if (triDirection_1[i] > 3)
						{
							triDirection_1[i] = 0;
						}
						triShape[i][0][0] += 0.01 * (i + 1)*2;
						triShape[i][0][1] -= 0.01 * (i + 1)*2;
						triShape[i][1][0] += 0.01 * (i + 1)*2;
						triShape[i][1][1] -= 0.01 * (i + 1)*2;
						triShape[i][2][0] += 0.01 * (i + 1)*2;
						triShape[i][2][1] -= 0.01 * (i + 1)*2;

						rotation(i, triDirection_1[i]);
						break;
					}
				}
				break;
			case 2:
				triShape[i][0][0] -= 0.01* (i + 1);
				triShape[i][0][1] -= 0.01* (i + 1);
				triShape[i][1][0] -= 0.01* (i + 1);
				triShape[i][1][1] -= 0.01* (i + 1);
				triShape[i][2][0] -= 0.01* (i + 1);
				triShape[i][2][1] -= 0.01* (i + 1);
				for (int j = 0; j < 3; j++)
				{
					if (triShape[i][j][0] < -1 || triShape[i][j][0] > 1 || triShape[i][j][1] < -1 || triShape[i][j][1] > 1)
					{
						triDirection_1[i]++;
						if (triDirection_1[i] > 3)
						{
							triDirection_1[i] = 0;
						}
						triShape[i][0][0] += 0.01 * (i + 1)*2;
						triShape[i][0][1] += 0.01 * (i + 1)*2;
						triShape[i][1][0] += 0.01 * (i + 1)*2;
						triShape[i][1][1] += 0.01 * (i + 1)*2;
						triShape[i][2][0] += 0.01 * (i + 1)*2;
						triShape[i][2][1] += 0.01 * (i + 1)*2;

						rotation(i, triDirection_1[i]);
						break;
					}
				}
				break;
			case 3:
				triShape[i][0][0] += 0.01* (i + 1);
				triShape[i][0][1] -= 0.01* (i + 1);
				triShape[i][1][0] += 0.01* (i + 1);
				triShape[i][1][1] -= 0.01* (i + 1);
				triShape[i][2][0] += 0.01* (i + 1);
				triShape[i][2][1] -= 0.01* (i + 1);
				for (int j = 0; j < 3; j++)
				{
					if (triShape[i][j][0] < -1 || triShape[i][j][0] > 1 || triShape[i][j][1] < -1 || triShape[i][j][1] > 1)
					{
						triDirection_1[i]++;
						if (triDirection_1[i] > 3)
						{
							triDirection_1[i] = 0;
						}
						triShape[i][0][0] -= 0.01 * (i + 1)*2;
						triShape[i][0][1] += 0.01 * (i + 1)*2;
						triShape[i][1][0] -= 0.01 * (i + 1)*2;
						triShape[i][1][1] += 0.01 * (i + 1)*2;
						triShape[i][2][0] -= 0.01 * (i + 1)*2;
						triShape[i][2][1] += 0.01 * (i + 1)*2;

						rotation(i, triDirection_1[i]);
						break;
					}
				}
				break;
			}
		}

		break;
	case 2:
		for (int i = 0; i < 4; i++)
		{
			switch (triDirection_2[i])
			{
			case 0:	//��������
				triShape[i][0][0] -= 0.01* (i + 1);
				triShape[i][1][0] -= 0.01* (i + 1);
				triShape[i][2][0] -= 0.01* (i + 1);
				for (int j = 0; j < 3; j++)
				{
					if (triShape[i][j][0] < -1)
					{
						triDirection_2[i] = 1;
						/*for (int k = 0; k < 3; k++)
						{
							for (int t = 0; t < 3; t++)
							{
								temp2[i][k][t] = triShape[i][k][t];
							}
						}
						triShape[i][0][1] = temp2[i][2][1];
						triShape[i][1][1] = temp2[i][2][1];
						triShape[i][2][1] = temp2[i][1][1];*/

						rotation(i, triDirection_2[i]);

						break;
					}
				}
				
				break;
			case 1:	//�Ʒ�������
				triShape[i][0][1] -= 0.01* (i + 1);
				triShape[i][1][1] -= 0.01* (i + 1);
				triShape[i][2][1] -= 0.01* (i + 1);
				downCnt_2[i]++;
				for (int j = 0; j < 3; j++)
				{
					if (triShape[i][j][1] < -1)
					{
						triShape[i][0][1] += 2;
						triShape[i][1][1] += 2;
						triShape[i][2][1] += 2;
						break;
					}
				}
				if (downCnt_2[i] > 10)
				{
					triDirection_2[i] = 2;
					downCnt_2[i] = 0;
				}
				break;
			case 2:	//����������
				triShape[i][0][0] += 0.01* (i + 1);
				triShape[i][1][0] += 0.01* (i + 1);
				triShape[i][2][0] += 0.01* (i + 1);
				for (int j = 0; j < 3; j++)
				{
					if (triShape[i][j][0]>1)
					{
						triDirection_2[i] = 3;
						/*for (int k = 0; k < 3; k++)
						{
							for (int t = 0; t < 3; t++)
							{
								temp2[i][k][t] = triShape[i][k][t];
							}
						}
						triShape[i][0][1] = temp2[i][2][1];
						triShape[i][1][1] = temp2[i][2][1];
						triShape[i][2][1] = temp2[i][1][1];*/

						rotation(i, triDirection_2[i]);
						break;
					}
				}
				break;
			case 3:	//�Ʒ�������
				triShape[i][0][1] -= 0.01* (i + 1);
				triShape[i][1][1] -= 0.01* (i + 1);
				triShape[i][2][1] -= 0.01* (i + 1);
				downCnt_2[i]++;
				for (int j = 0; j < 3; j++)
				{
					if (triShape[i][j][1] < -1)
					{
						triShape[i][0][1] += 2;
						triShape[i][1][1] += 2;
						triShape[i][2][1] += 2;
						break;
					}
				}
				if (downCnt_2[i] > 10)
				{
					triDirection_2[i] = 0;
					downCnt_2[i] = 0;
				}
			}
		}
		break;


		//--------------����° �ִϸ��̼�----------------------------
	case 3:	
		for (int i = 0; i < 4; i++)
		{
			switch (triDirection_3[i])
			{
			case 0:
				//��������
				triShape[i][0][0] -= 0.01* (i + 1);
				triShape[i][1][0] -= 0.01* (i + 1);
				triShape[i][2][0] -= 0.01* (i + 1);
				for (int j = 0; j < 3; j++)
				{
					if (triShape[i][j][0]-triCnt_3[i]*0.1 < -1)
					{
						triDirection_3[i] = 1;
						triCnt_3[i]++;

						////���� �ٲ��
						//for (int k = 0; k < 3; k++)
						//{
						//	for (int t = 0; t < 3; t++)
						//	{
						//		temp3[i][k][t] = triShape[i][k][t];
						//	}
						//}

						//triShape[i][0][1] = temp3[i][2][1];
						//triShape[i][1][1] = temp3[i][2][1];
						//triShape[i][2][1] = temp3[i][1][1];
						rotation(i, triDirection_3[i]);

						break;
					}
					
				}
				break;
			case 1:
				//�Ʒ�������
				triShape[i][0][1] -= 0.01* (i + 1);
				triShape[i][1][1] -= 0.01* (i + 1);
				triShape[i][2][1] -= 0.01* (i + 1);
				for (int j = 0; j < 3; j++)
				{
					if (triShape[i][j][1] - triCnt_3[i] * 0.1 < -1)
					{
						triDirection_3[i] = 2;
						triCnt_3[i]++;

						////���� �ٲ��
						//for (int k = 0; k < 3; k++)
						//{
						//	for (int t = 0; t < 3; t++)
						//	{
						//		temp3[i][k][t] = triShape[i][k][t];
						//	}
						//}
						//triShape[i][0][1] = temp3[i][2][1];
						//triShape[i][1][1] = temp3[i][2][1];
						//triShape[i][2][1] = temp3[i][1][1];
						rotation(i, triDirection_3[i]);
						break;
					}
					
				}
				
				break;
			case 2:
				//����������
				triShape[i][0][0] += 0.01* (i + 1);
				triShape[i][1][0] += 0.01* (i + 1);
				triShape[i][2][0] += 0.01* (i + 1);
				for (int j = 0; j < 3; j++)
				{
					if (triShape[i][j][0] + triCnt_3[i] * 0.1 > 1)
					{
						triDirection_3[i] = 3;
						triCnt_3[i]++;

						//���� �ٲ��
						/*for (int k = 0; k < 3; k++)
						{
							for (int t = 0; t < 3; t++)
							{
								temp3[i][k][t] = triShape[i][k][t];
							}
						}
						triShape[i][0][1] = temp3[i][2][1];
						triShape[i][1][1] = temp3[i][2][1];
						triShape[i][2][1] = temp3[i][1][1];*/
						rotation(i, triDirection_3[i]);
						break;
					}
					
				}

				
				break;
			case 3:
				//��������
				triShape[i][0][1] += 0.01* (i + 1);
				triShape[i][1][1] += 0.01* (i + 1);
				triShape[i][2][1] += 0.01* (i + 1);
				for (int j = 0; j < 3; j++)
				{
					if (triShape[i][j][1] + triCnt_3[i] * 0.1 >1)
					{
						triDirection_3[i] = 0;
						triCnt_3[i]++;

						////���� �ٲ��
						//for (int k = 0; k < 3; k++)
						//{
						//	for (int t = 0; t < 3; t++)
						//	{
						//		temp3[i][k][t] = triShape[i][k][t];
						//	}
						//}
						//triShape[i][0][1] = temp3[i][2][1];
						//triShape[i][1][1] = temp3[i][2][1];
						//triShape[i][2][1] = temp3[i][1][1];
						rotation(i, triDirection_3[i]);
						break;
					}
				}
				
				break;
			}
			if (triCnt_3[i] > 9)
			{
				triCnt_3[i] = 0;
			}

		}
		break;
	case 4:
		if (start4)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					for (int k = 0; k < 4; k++)
					{
						temp4[i][j][k] = triShape[i][j][k];
					}
				}
			}
			start4 = false;
		}
		for (int i = 0; i < 4; i++) {
			
			theta[i] = triCnt_4[i] * 0.1;
			r[i] = 0.1 + 0.05 * theta[i];

			triShape[i][0][0] = r[i] * cos(theta[i])* (i * 0.1 + 1) +temp4[i][0][0];
			triShape[i][0][1] = r[i] * sin(theta[i])* (i * 0.1 + 1) +temp4[i][0][1];
			triShape[i][0][2] = 0.0;

			triShape[i][1][0] = r[i] * cos(theta[i])* (i * 0.1 + 1)+temp4[i][1][0];
			triShape[i][1][1] = r[i] * sin(theta[i])* (i * 0.1 + 1)+temp4[i][1][1];
			triShape[i][1][2] = 0.0;

			triShape[i][2][0] = r[i] * cos(theta[i])* (i * 0.1 + 1)+temp4[i][2][0];
			triShape[i][2][1] = r[i] * sin(theta[i])* (i * 0.1 + 1)+temp4[i][2][1];
			triShape[i][2][2] = 0.0;

			triCnt_4[i]++;
			if (triCnt_4[i] > 150)
			{
				triCnt_4[i] = 0;
			}
		}
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

void rotation(int i, int direction)
{
	double cx, cy;

	cx = (triShape[i][0][0] + triShape[i][1][0] + triShape[i][2][0])/3;
	cy = (triShape[i][0][1] + triShape[i][1][1] + triShape[i][2][1])/3;

	/*triShape[i][0][0] = cx;
	triShape[i][0][1] = cy;
	triShape[i][0][2] = 0.0;
	triShape[i][1][0] = cx;
	triShape[i][1][1] = cy;
	triShape[i][1][2] = 0.0;
	triShape[i][2][0] = cx;
	triShape[i][2][1] = cy;
	triShape[i][2][2] = 0.0;*/

	switch (direction)
	{
	case 0:
		//���� ����
		triShape[i][0][0] = cx - 0.15;
		triShape[i][0][1] = cy - 0.15;
		triShape[i][0][2] = 0.0;
		triShape[i][1][0] = cx + 0.15;
		triShape[i][1][1] = cy - 0.15;
		triShape[i][1][2] = 0.0;
		triShape[i][2][0] = cx;
		triShape[i][2][1] = cy + 0.25;
		triShape[i][2][2] = 0.0;
		break;
	case 1:
		//�Ʒ��� ����
		triShape[i][0][0] = cx + 0.15;
		triShape[i][0][1] = cy + 0.15;
		triShape[i][0][2] = 0.0;
		triShape[i][1][0] = cx - 0.15;
		triShape[i][1][1] = cy + 0.15;
		triShape[i][1][2] = 0.0;
		triShape[i][2][0] = cx;
		triShape[i][2][1] = cy - 0.25;
		triShape[i][2][2] = 0.0;
		break;
	case 2:
		//���� ����
		triShape[i][0][0] = cx + 0.15;
		triShape[i][0][1] = cy - 0.15;
		triShape[i][0][2] = 0.0;
		triShape[i][1][0] = cx + 0.15;
		triShape[i][1][1] = cy + 0.15;
		triShape[i][1][2] = 0.0;
		triShape[i][2][0] = cx - 0.25;
		triShape[i][2][1] = cy;
		triShape[i][2][2] = 0.0;
		break;
	case 3:
		//������ ����
		triShape[i][0][0] = cx - 0.15;
		triShape[i][0][1] = cy + 0.15;
		triShape[i][0][2] = 0.0;
		triShape[i][1][0] = cx - 0.15;
		triShape[i][1][1] = cy - 0.15;
		triShape[i][1][2] = 0.0;
		triShape[i][2][0] = cx + 0.25;
		triShape[i][2][1] = cy;
		triShape[i][2][2] = 0.0;
		break;
	}
}