#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
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
#include <string>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> XYdis(-1, 1);
uniform_real_distribution<double> dis(0.0, 1.0);
uniform_real_distribution<double> speedDis(0.03, 0.09);
uniform_real_distribution<double> startYDis(0.0, 2.0);
uniform_real_distribution<double> EndDis(-1, 1);
uniform_real_distribution<double> randomXZ(-5.0, 5.0);

struct Transform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);

	glm::mat4 GetTransform()
	{
		glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 S = glm::scale(glm::mat4(1.0), scale);
		glm::mat4 RX = glm::rotate(glm::mat4(1.0f), (float)glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 RY = glm::rotate(glm::mat4(1.0f), (float)glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 RZ = glm::rotate(glm::mat4(1.0f), (float)glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
		return T * RX * RY * RZ * S;
	}
};

struct OBJECT {
	GLuint vao, vbo[3];
	Transform worldmatrix;
	Transform modelmatrix;
	OBJECT* parent{ nullptr };

	glm::vec3* vertex;
	glm::vec3* face;
	glm::vec3* vertexdata;
	glm::vec3* normaldata;
	glm::vec3* colordata;

	int v_count = 0;
	int f_count = 0;
	int vertex_count = f_count * 3;

	void ReadObj(string fileName)
	{
		ifstream in{ fileName };

		string s;

		while (in >> s)
		{
			if (s == "v") v_count++;
			else if (s == "f") f_count++;
		}
		in.close();
		in.open(fileName);

		vertex_count = f_count * 3;

		vertex = new glm::vec3[v_count];
		face = new glm::vec3[f_count];
		vertexdata = new glm::vec3[vertex_count];
		normaldata = new glm::vec3[vertex_count];
		colordata = new glm::vec3[vertex_count];

		int v_incount = 0;
		int f_incount = 0;
		while (in >> s)
		{
			if (s == "v") {
				in >> vertex[v_incount].x >> vertex[v_incount].y >> vertex[v_incount].z;
				v_incount++;
			}
			else if (s == "f") {
				in >> face[f_incount].x >> face[f_incount].y >> face[f_incount].z;
				vertexdata[f_incount * 3 + 0] = vertex[static_cast<int>(face[f_incount].x - 1)];
				vertexdata[f_incount * 3 + 1] = vertex[static_cast<int>(face[f_incount].y - 1)];
				vertexdata[f_incount * 3 + 2] = vertex[static_cast<int>(face[f_incount].z - 1)];
				f_incount++;
			}
		}

		for (int i = 0; i < f_count; i++)
		{
			glm::vec3 normal = glm::cross(vertexdata[i * 3 + 1] - vertexdata[i * 3 + 0], vertexdata[i * 3 + 2] - vertexdata[i * 3 + 0]);
			//glm::vec3 normal = glm::vec3(0.0, 1.0, 0.0);
			normaldata[i * 3 + 0] = normal;
			normaldata[i * 3 + 1] = normal;
			normaldata[i * 3 + 2] = normal;
		}
	}

	glm::mat4 GetTransform()
	{
		if (parent)
			return parent->GetTransform() * worldmatrix.GetTransform();
		return worldmatrix.GetTransform();
	}

	glm::mat4 GetmodelTransform()
	{
		return modelmatrix.GetTransform();
	}
};

struct CUBE :OBJECT
{
	float speed;
	float start_Y;
	float direction_Y;	//1 : 1번 애니메이션 UP, 2 : 1번 애니메이션 Down;
	float cubesC1 = 1.0f, cubesC2 = 1.0f, cubesC3 = 1.0f;
	float EndDirectionX, EndDirectionY, EndDirectionZ;

	void Init()
	{
		speed = speedDis(gen);
		start_Y = startYDis(gen);
		direction_Y = 1;

		for (int i = 0; i < vertex_count; i++)
		{
			double random_color = 0.5;

			colordata[i].x = random_color;
			colordata[i].y = random_color;
			colordata[i].z = random_color;
		}
		for (int i = 0; i < vertex_count; i++)
		{
			vertexdata[i] -= glm::vec3(0.5, 0.0, 0.5);
		}

		glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
		glBindVertexArray(vao); //--- VAO를 바인드하기
		glGenBuffers(3, vbo); //--- 3개의 VBO를 지정하고 할당하기

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), vertexdata, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), colordata, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), normaldata, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}

	void draw(int shaderID)
	{
		unsigned int modelLocation = glGetUniformLocation(shaderID, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(GetTransform() * GetmodelTransform()));
		glBindVertexArray(vao);

		for (int i = 0; i < vertex_count; i++)
		{
			colordata[i].x = cubesC1;
			colordata[i].y = cubesC2;
			colordata[i].z = cubesC3;
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), colordata, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);

		for (int i = 0; i < vertex_count; i++)
		{
			colordata[i].x = 0.0f;
			colordata[i].y = 0.0f;
			colordata[i].z = 1.0f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), colordata, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_LINE_STRIP, 0, vertex_count);
	}

	void update()
	{
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), vertexdata, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}
};
CUBE cube;
CUBE minicube;
CUBE cubes[25][25];

struct PYRAMID :OBJECT
{
	void Init()
	{
		for (int i = 0; i < vertex_count; i++)
		{
			double random_color = dis(gen);

			colordata[i].x = dis(gen);
			colordata[i].y = dis(gen);
			colordata[i].z = dis(gen);
		}
		for (int i = 0; i < vertex_count; i++)
		{
			vertexdata[i] -= glm::vec3(0.5, 0.5, 0.5);
		}

		glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
		glBindVertexArray(vao); //--- VAO를 바인드하기
		glGenBuffers(3, vbo); //--- 3개의 VBO를 지정하고 할당하기

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), vertexdata, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), colordata, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), normaldata, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}

	void draw(int shaderID)
	{
		unsigned int modelLocation = glGetUniformLocation(shaderID, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(GetTransform() * GetmodelTransform()));
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);
	}

	void update()
	{
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), vertexdata, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}
};
PYRAMID pyramid;

struct SPHERE :OBJECT
{
	void Init()
	{
		for (int i = 0; i < vertex_count; i++)
		{
			double random_color = dis(gen);

			colordata[i].x = dis(gen);
			colordata[i].y = dis(gen);
			colordata[i].z = dis(gen);
		}
		for (int i = 0; i < vertex_count; i++)
		{
			vertexdata[i] -= glm::vec3(0.5, 0.5, 0.5);
		}

		glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
		glBindVertexArray(vao); //--- VAO를 바인드하기
		glGenBuffers(3, vbo); //--- 3개의 VBO를 지정하고 할당하기

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), vertexdata, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), colordata, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), normaldata, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}

	void draw(int shaderID)
	{
		unsigned int modelLocation = glGetUniformLocation(shaderID, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(GetTransform() * GetmodelTransform()));
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);
	}

	void update(glm::vec3 color)
	{
		for (int i = 0; i < vertex_count; i++)
		{
			colordata[i] = color;
		}

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), colordata, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
	}
};
SPHERE sphere;

struct CIRCLE :OBJECT
{
	float r = 3.0;

	void Init()
	{
		vertex_count = 360;
		float angle = 0;
		float x, y, z;
		vertexdata = new glm::vec3[vertex_count];
		normaldata = new glm::vec3[vertex_count];
		colordata = new glm::vec3[vertex_count];

		for (int i = 0; i < vertex_count; ++i)
		{
			angle = (float)i / 360 * 2 * 3.1415926535;
			x = cos(angle) * r;
			y = 0;
			z = sin(angle) * r;
			vertexdata[i] = glm::vec3(x, y, z);
		}

		for (int i = 0; i < vertex_count; i++)
		{
			double random_color = dis(gen);

			colordata[i].x = dis(gen);
			colordata[i].y = dis(gen);
			colordata[i].z = dis(gen);
		}

		glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
		glBindVertexArray(vao); //--- VAO를 바인드하기
		glGenBuffers(3, vbo); //--- 3개의 VBO를 지정하고 할당하기

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), vertexdata, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), colordata, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), normaldata, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}

	void draw(int shaderID)
	{
		unsigned int modelLocation = glGetUniformLocation(shaderID, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(GetTransform() * GetmodelTransform()));
		glBindVertexArray(vao);
		glDrawArrays(GL_LINE_LOOP, 0, vertex_count);
	}

	void update()
	{
		float angle = 0;
		float x, y, z;

		for (int i = 0; i < vertex_count; ++i)
		{
			angle = (float)i / 360 * 2 * 3.1415926535;
			x = cos(angle) * r;
			y = 0;
			z = sin(angle) * r;
			vertexdata[i] = glm::vec3(x, y, z);
		}
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), vertexdata, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}
};
CIRCLE circle;

struct SNOW :OBJECT
{
	float speed;

	void Init()
	{
		for (int i = 0; i < vertex_count; i++)
		{
			colordata[i].x = 1.0;
			colordata[i].y = 1.0;
			colordata[i].z = 1.0;
		}
		speed = speedDis(gen);

		glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
		glBindVertexArray(vao); //--- VAO를 바인드하기
		glGenBuffers(3, vbo); //--- 3개의 VBO를 지정하고 할당하기

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), vertexdata, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), colordata, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), normaldata, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}

	void draw(int shaderID)
	{
		unsigned int modelLocation = glGetUniformLocation(shaderID, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(GetTransform() * GetmodelTransform()));
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);
	}

	void update(glm::vec3 color)
	{
		for (int i = 0; i < vertex_count; i++)
		{
			colordata[i] = color;
		}

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), colordata, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
	}
};
SNOW snow[500];


GLfloat lineShape[10][2][3] = {};	//--- 선분 위치 값

glm::vec3 colors[12][3] = {};

GLfloat XYZShape[3][2][3] = {
	{{-1.0,0.0,0.0},{1.0,0.0,0.0}},
	{{0.0,-1.0,0.0},{0.0,1.0,0.0}},
	{{0.0,0.0,-1.0},{0.0,0.0,1.0}} };

GLfloat XYZcolors[6][3] = { //--- 축 색상
	{ 1.0, 0.0, 0.0 },	   	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },	   	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },	   	{ 0.0, 0.0, 1.0 }
};

//glm::vec3 cameraPos = glm::vec3(-5.0f, 20.5f, -5.0f); //--- 카메라 위치
//glm::vec3 cameraDirection = glm::vec3(0.0f, 10.0f, 0.0f); //--- 카메라 바라보는 방향

glm::vec3 cameraPos = glm::vec3(-10.0f, 20.5f, -10.0f); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(12.5f, 0.0f, 12.5f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

glm::vec3 cameraPos2 = glm::vec3(0, 10, 0); //--- 카메라 위치
glm::vec3 cameraDirection2 = glm::vec3(0,0,0); //--- 카메라 바라보는 방향
glm::vec3 cameraUp2 = glm::vec3(0.0f, 0.0f, 1.0f); //--- 카메라 위쪽 방향

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

glm::mat4 view2 = glm::mat4(1.0f);
glm::mat4 pTransform2 = glm::mat4(1.0f);

GLuint vao, vbo[3];
GLuint TriPosVbo, TriColorVbo;

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

int windowWidth = 1000;
int windowHeight = 1000;

float openGLX, openGLY;
int movingRectangle = -1;

float ox = 0, oy = 0;
float x_angle = 0;
float y_angle = 0;
float z_angle = 0;
float pre_x_angle = 0;
float pre_y_angle = 0;
float wheel_scale = 0.1;
bool left_button = 0;
float fovy = 45;
float near_1 = 0.1;
float far_1 = 200.0;
float persfect_z = -2.0;

bool start = true;

int in_cnt1 = 25, in_cnt2 = 25;
int animation = 1;	//1번, 2번, 3번 애니메이션

bool TSelection = true;
float LC_R, LC_G, LC_B;
int YSelection = 0;
int YSelectionCnt = 0;
float Ycnt = 0;
bool RSelection = false;
bool EndAnimation = false;
int EndCnt = 0;
bool LSelection = false;
int LCnt = 0;

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
GLvoid mouseWheel(int button, int dir, int x, int y);

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Example1");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW Initialized\n";
	}
	cube.ReadObj("cube.obj");
	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			cubes[i][j].ReadObj("cube.obj");
		}
	}
	
	minicube.ReadObj("cube.obj");
	pyramid.ReadObj("pyramid.obj");
	sphere.ReadObj("sphere.obj");
	for (int i = 0; i < 500; i++)
	{
		snow[i].ReadObj("sphere.obj");
	}

	LC_R=1.0f, LC_G=1.0f, LC_B= 1.0f;

	cout << "갯수를 입력하세요.(ex : 5 5) : " << endl;
	cin >> in_cnt1 >> in_cnt2;

	//--- 세이더 읽어와서 세이더 프로그램 만들기
	make_shaderProgram(); //--- 세이더 프로그램 만들기
	InitBuffer();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE); //--- 상태 설정은 필요한 곳에서 하면 된다.
	//glDisable(GL_DEPTH_TEST | GL_CULL_FACE);	//해제

	glutTimerFunc(10, TimerFunction, 1);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeys); // 방향키 콜백 함수 등록
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMouseWheelFunc(mouseWheel);

	glutMainLoop();
}

GLvoid drawScene()
{
	glViewport(0, 0, windowWidth, windowHeight);

	glUseProgram(shaderProgramID);
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //--- 깊이 버퍼를 클리어한다.

	glBindVertexArray(vao);

	int modelLocation = glGetUniformLocation(shaderProgramID, "model"); //--- 버텍스 세이더에서 모델링 변환 행렬 변수값을 받아온다.
	int viewLocation = glGetUniformLocation(shaderProgramID, "view"); //--- 버텍스 세이더에서 뷰잉 변환 행렬 변수값을 받아온다.
	int projLocation = glGetUniformLocation(shaderProgramID, "projection"); //--- 버텍스 세이더에서 투영 변환 행렬 변수값을 받아온다.

	cameraPos = glm::vec3(-10.0f, 20.5f, -10.0f);
	cameraDirection = glm::vec3(12.5f, 0.0f, 12.5f);

	if (YSelection > 0)
	{
		cameraDirection = glm::vec3(6.25f, 10.0f, 6.25f);
		cameraPos = glm::rotate(glm::mat4(1.0f), glm::radians(Ycnt), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cameraPos, 1.0);
		cameraPos += cameraDirection;
	}
	if (LSelection)
	{
		cameraPos = glm::vec3(-30.0f, 20.5f, -30.0f);
	}
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	glm::mat4 perspect = glm::mat4(1.0f);
	perspect = glm::perspective(glm::radians(fovy), (float)windowWidth / (float)windowHeight, near_1, far_1);
	perspect = glm::translate(perspect, glm::vec3(0.0, 0.0, persfect_z));
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(perspect));

	glm::mat4 lightmatrix = minicube.GetTransform(); // 주어진 mat4 행렬
	glm::vec3 lightposition = glm::vec3(lightmatrix[3]); // 행렬의 마지막 열을 사용하여 위치 추출

	unsigned int lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, lightposition.x, lightposition.y, lightposition.z);
	unsigned int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, LC_R * (float)TSelection, LC_G * (float)TSelection, LC_B * (float)TSelection);
	unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 0.5, 0.3);

	if (start)
	{
		start = false;
	}

	model = glm::mat4(1.0f);

	//minicube.draw(shaderProgramID);
	for (int i = 0; i < in_cnt1; i++)
	{
		for (int j = 0; j < in_cnt2; j++)
		{
			cubes[i][j].draw(shaderProgramID);
		}
	}
	if (LSelection)
	{
		for (int i = 0; i < LCnt; i++)
		{
			snow[i].draw(shaderProgramID);
		}
	}

	glViewport(700, 700, 300, 300);
	//뷰변환
	view2 = glm::mat4(1.0f);
	view2 = glm::lookAt(cameraPos2, cameraDirection2, cameraUp2);
	unsigned int viewLocation2 = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(viewLocation2, 1, GL_FALSE, glm::value_ptr(view2));

	//투영 변환
	perspect = glm::mat4(1.0f);
	perspect = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, -20.0f, 20.0f);
	unsigned int projectionLocation2= glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projectionLocation2, 1, GL_FALSE, glm::value_ptr(perspect));

	//minicube.draw(shaderProgramID);

	for (int i = 0; i < in_cnt1; i++)
	{
		for (int j = 0; j < in_cnt2; j++)
		{
			cubes[i][j].draw(shaderProgramID);
		}
	}
	if (LSelection)
	{
		for (int i = 0; i < 100; i++)
		{
			snow[i].draw(shaderProgramID);
		}
	}

	glutSwapBuffers(); //--- 화면에 출력하기
}

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	cube.Init();
	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			cubes[i][j].Init();
		}
	}
	minicube.Init();
	minicube.parent = &cube;
	pyramid.Init();
	sphere.Init();
	circle.Init();

	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			cubes[i][j].worldmatrix.position.x = i * 0.5;
			cubes[i][j].worldmatrix.position.z = j * 0.5;
			cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5+cubes[i][j].start_Y, 0.5);
		}
	}
	/*for (int i = 0; i < 500; i++)
	{
		snow[i].Init();
		snow[i].worldmatrix.position.x = randomXYZ(gen);
		snow[i].worldmatrix.position.y = 0;
		snow[i].worldmatrix.position.z = randomXYZ(gen);
		snow[i].worldmatrix.scale = glm::vec3(0.1, 0.1, 0.1);
	}*/

	minicube.worldmatrix.position.x = 6.25;
	minicube.worldmatrix.position.y = 20;
	minicube.worldmatrix.position.z = 6.25;
	minicube.modelmatrix.scale = glm::vec3(0.5, 0.5, 0.5);
}

void make_shaderProgram()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}

void make_vertexShaders()
{
	vertexSource = filetobuf("vertex3.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cout << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment3.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cout << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
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

bool speedUp = false;
bool speedDown = false;

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		animation = 1;
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				cubes[i][j].speed = speedDis(gen);
				cubes[i][j].start_Y = startYDis(gen);
				cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5 + cubes[i][j].start_Y, 0.5);
				cubes[i][j].direction_Y = 1;
			}
		}
		break;
	case '2':
		animation = 2;
		for (int i = 0; i < 25; ++i)
		{
			for(int j = 0; j<25; ++j)
			{
				cubes[i][j].speed = 0.05;
				cubes[i][j].start_Y = 0;
				cubes[i][j].start_Y = i * 0.5;
				cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5 + cubes[i][j].start_Y, 0.5);
				cubes[i][j].direction_Y = 1;
			}
		}
		break;
	case '3':
		animation = 3;
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				cubes[i][j].speed = 0.05;
				cubes[i][j].start_Y = 0;
				cubes[i][j].start_Y += (int)(in_cnt1 / 2) - (abs)((int)(in_cnt1 / 2 - i));
				cubes[i][j].start_Y += (int)(in_cnt2 / 2) - (abs)((int)(in_cnt2 / 2 - j));
				cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5 + cubes[i][j].start_Y, 0.5);
				cubes[i][j].direction_Y = 1;
			}
		}
		break;
	case '4':
		animation = 4;
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				cubes[i][j].speed = 0.05;
				cubes[i][j].start_Y = 0;
				cubes[i][j].start_Y = i * 0.5 + j * 0.5;
				cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5 + cubes[i][j].start_Y, 0.5);
				cubes[i][j].direction_Y = 1;
			}
		}
		break;
	case '5':
		animation = 5;
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				cubes[i][j].speed = 0.05;
				cubes[i][j].start_Y = 0;
				cubes[i][j].start_Y = (i%2) * 7.5 + (j%2) * 7.5;
				cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5 + cubes[i][j].start_Y, 0.5);
				cubes[i][j].direction_Y = 1;
			}
		}
		break;
	case '6':
		animation = 6;
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				cubes[i][j].speed = 0.05;
				cubes[i][j].start_Y = 0;
				if (i < (int)(in_cnt1 / 2) && j < (int)(in_cnt2 / 2))
				{
					cubes[i][j].start_Y += 10;
				}
				if (i >= (int)(in_cnt1 / 2) && j >= (int)(in_cnt2 / 2))
				{
					cubes[i][j].start_Y += 10;
				}
				cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5 + cubes[i][j].start_Y, 0.5);
				cubes[i][j].direction_Y = 1;
			}
		}
		break;
	case '7':
		animation = 7;
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				cubes[i][j].speed = 0.05;
				cubes[i][j].start_Y = 0;
				cubes[i][j].start_Y += (int)((in_cnt1+in_cnt2) / 2) - (abs)((int)((in_cnt1 + in_cnt2) / 2 - (i + j)));
				cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5 + cubes[i][j].start_Y, 0.5);
				cubes[i][j].direction_Y = 1;
			}
		}
		break;
	case '8':
		animation = 8;
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				cubes[i][j].speed = 0.05;
				cubes[i][j].start_Y = 0;
				cubes[i][j].start_Y = (abs)(i-j)*0.5;
				cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5 + cubes[i][j].start_Y, 0.5);
				cubes[i][j].direction_Y = 1;
			}
		}
		break;
	case 't':
		TSelection = !TSelection;
		break;
	case 'c':
		LC_R = dis(gen);
		LC_G = dis(gen);
		LC_B = dis(gen);
		break;
	case 'y':
		if (YSelectionCnt % 2 == 0)
		{
			YSelection = 1;
		}
		else
		{
			YSelection = 0;
		}
		YSelectionCnt += 1;
		break;
	case 'Y':
		if (YSelectionCnt % 2 == 0)
		{
			YSelection = 2;
		}
		else
		{
			YSelection = 0;
		}
		YSelectionCnt += 1;
		break;
	case '-':
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				if (cubes[i][j].speed > 0.01)
				{
					cubes[i][j].speed -= 0.01;
				}
			}
		}
		break;
	case '=':
	case '+':
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				cubes[i][j].speed += 0.01;
			}
		}
		break;
	case 'r':
		EndAnimation = true;
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				cubes[i][j].EndDirectionX = EndDis(gen);
				cubes[i][j].EndDirectionY = EndDis(gen);
				cubes[i][j].EndDirectionZ = EndDis(gen);
			}
		}
		break;
	case 'l':
		LSelection = true;
		animation = -1;
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				cubes[i][j].speed = 0.05;
				cubes[i][j].start_Y = 0;
				cubes[i][j].start_Y += (float)((in_cnt1+1) / 2) - (float)(abs)((in_cnt1 + 1) / 2 - i);
				cubes[i][j].start_Y += (float)((in_cnt2 + 1) / 2) - (float)(abs)((in_cnt2 + 1) / 2 - j);
				cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5 + cubes[i][j].start_Y, 0.5);
				cubes[i][j].direction_Y = 1;
			}
		}
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid SpecialKeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	}
	glutPostRedisplay(); // 화면 갱신
}

int movingMouse = -1;
float beforeX, beforeY;

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		ox = x;
		oy = y;
		left_button = true;
	}
	else
	{
		ox = 0;
		oy = 0;
		pre_x_angle = x_angle;
		pre_y_angle = y_angle;
		left_button = false;
	}
}

GLvoid Motion(int x, int y)
{
	if (left_button)
	{
		y_angle = x - ox;
		x_angle = y - oy;
		x_angle += pre_x_angle;
		y_angle += pre_y_angle;

		y_angle /= 2;
		x_angle /= 2;
	}
	glutPostRedisplay();
}

GLvoid mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		wheel_scale += dir * 0.1;
	}
	else if (dir < 0)
	{
		wheel_scale += dir * 0.1;
		if (wheel_scale < 0.1)
		{
			wheel_scale = 0.1;
		}
	}
	glutPostRedisplay();
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
		if (YSelection == 1)
		{
			Ycnt+=1;
		}
		if (YSelection == 2)
		{
			Ycnt-= 1;
		}

		//높이에 따른 색깔 바꾸기
		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				cubes[i][j].cubesC1 = cubes[i][j].worldmatrix.scale.y / 10.0;
				cubes[i][j].cubesC2 = cubes[i][j].worldmatrix.scale.y / 10.0;
				cubes[i][j].cubesC3 = cubes[i][j].worldmatrix.scale.y / 10.0;
			}
		}

		if (LSelection)
		{
			if (LCnt>0 && LCnt < 50 || (LCnt>100 && LCnt < 150) || (LCnt > 200 && LCnt < 250))
			{
				for (int i = 0; i < 25; ++i)
				{
					for (int j = 0; j < 25; ++j)
					{
						cubes[i][j].worldmatrix.position.y += 0.05;
					}
				}
			}
			if (LCnt > 50 && LCnt < 100 || (LCnt > 150 && LCnt < 200) || (LCnt > 250 && LCnt < 300))
			{
				for (int i = 0; i < 25; ++i)
				{
					for (int j = 0; j < 25; ++j)
					{
						cubes[i][j].worldmatrix.position.y -= 0.05;
					}
				}
			}
			else if (LCnt >= 300)
			{
				for (int i = 0; i < 25; ++i)
				{
					for (int j = 0; j < 25; ++j)
					{
						cubes[i][j].worldmatrix.position.y += 0.3;
					}
				}
			}

			LCnt++;
			snow[LCnt].Init();
			snow[LCnt].worldmatrix.position.x = cubes[in_cnt1 / 2][in_cnt2 / 2].worldmatrix.position.x + randomXZ(gen);
			snow[LCnt].worldmatrix.position.y = cubes[in_cnt1 / 2][in_cnt2 / 2].worldmatrix.position.y;
			snow[LCnt].worldmatrix.position.z = cubes[in_cnt1 / 2][in_cnt2 / 2].worldmatrix.position.z + randomXZ(gen);
			//snow[LCnt].worldmatrix.scale = glm::vec3(0.1, 0.1, 0.1);
			
			for (int i = 0; i < LCnt; ++i)
			{
				snow[i].worldmatrix.position.y -= 0.05;
			}

			if (LCnt >= 500)
			{
				LCnt = 0;

				cout << "갯수를 입력하세요.(ex : 5 5) : " << endl;
				cin >> in_cnt1 >> in_cnt2;

				animation = 1;
				for (int i = 0; i < 25; ++i)
				{
					for (int j = 0; j < 25; ++j)
					{
						cubes[i][j].worldmatrix.position.x = i * 0.5;
						cubes[i][j].worldmatrix.position.y = 0.0f;
						cubes[i][j].worldmatrix.position.z = j * 0.5;
						cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5 + cubes[i][j].start_Y, 0.5);
						cubes[i][j].worldmatrix.rotation.x = 0.0f;
						cubes[i][j].worldmatrix.rotation.y = 0.0f;
						cubes[i][j].worldmatrix.rotation.z = 0.0f;
						cubes[i][j].speed = speedDis(gen);
						cubes[i][j].start_Y = startYDis(gen);
						cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5 + cubes[i][j].start_Y, 0.5);
						cubes[i][j].direction_Y = 1;
					}
				}
				TSelection = true;
				LC_R = 1.0f, LC_G = 1.0f, LC_B = 1.0f;
				YSelection = 0;
				YSelectionCnt = 0;
				Ycnt = 0;
				animation = 1;
				LSelection = false;
			}
		}

		//애니메이션
		switch (animation)
		{
		case 1:
			for (int i = 0; i < 25; ++i)
			{
				for (int j = 0; j < 25; ++j)
				{
					if (cubes[i][j].direction_Y == 1)
					{
						cubes[i][j].worldmatrix.scale.y += cubes[i][j].speed;
					}
					else if (cubes[i][j].direction_Y == 2)
					{
						cubes[i][j].worldmatrix.scale.y -= cubes[i][j].speed;
					}
					if (cubes[i][j].worldmatrix.scale.y > 10)
					{
						cubes[i][j].direction_Y = 2;
						cubes[i][j].worldmatrix.scale.y = 20 - cubes[i][j].worldmatrix.scale.y;
					}
					else if (cubes[i][j].worldmatrix.scale.y < 0)
					{
						cubes[i][j].direction_Y = 1;
						cubes[i][j].worldmatrix.scale.y = -cubes[i][j].worldmatrix.scale.y;
					}
				}
			}
			break;
		case 2:
			for (int i = 0; i < 25; ++i)
			{
				for (int j = 0; j < 25; ++j)
				{
					if (cubes[i][j].direction_Y == 1)
					{
						cubes[i][j].worldmatrix.scale.y += cubes[i][j].speed;
					}
					else if (cubes[i][j].direction_Y == 2)
					{
						cubes[i][j].worldmatrix.scale.y -= cubes[i][j].speed;
					}
					if (cubes[i][j].worldmatrix.scale.y > 15)
					{
						cubes[i][j].direction_Y = 2;
						cubes[i][j].worldmatrix.scale.y = 30 - cubes[i][j].worldmatrix.scale.y;
					}
					else if (cubes[i][j].worldmatrix.scale.y < 0)
					{
						cubes[i][j].direction_Y = 1;
						cubes[i][j].worldmatrix.scale.y = -cubes[i][j].worldmatrix.scale.y;
					}
				}
			}
			break;
		case 3:
			for (int i = 0; i < 25; ++i)
			{
				for (int j = 0; j < 25; ++j)
				{
					if (cubes[i][j].direction_Y == 1)
					{
						cubes[i][j].worldmatrix.scale.y += cubes[i][j].speed;
					}
					else if (cubes[i][j].direction_Y == 2)
					{
						cubes[i][j].worldmatrix.scale.y -= cubes[i][j].speed;
					}
					if (cubes[i][j].worldmatrix.scale.y > 15)
					{
						cubes[i][j].direction_Y = 2;
						cubes[i][j].worldmatrix.scale.y = 30 - cubes[i][j].worldmatrix.scale.y;
					}
					else if (cubes[i][j].worldmatrix.scale.y < 0)
					{
						cubes[i][j].direction_Y = 1;
						cubes[i][j].worldmatrix.scale.y = -cubes[i][j].worldmatrix.scale.y;
					}
				}
			}
			break;
		case 4:
			for (int i = 0; i < 25; ++i)
			{
				for (int j = 0; j < 25; ++j)
				{
					if (cubes[i][j].direction_Y == 1)
					{
						cubes[i][j].worldmatrix.scale.y += cubes[i][j].speed;
					}
					else if (cubes[i][j].direction_Y == 2)
					{
						cubes[i][j].worldmatrix.scale.y -= cubes[i][j].speed;
					}
					if (cubes[i][j].worldmatrix.scale.y > 10)
					{
						cubes[i][j].direction_Y = 2;
						cubes[i][j].worldmatrix.scale.y = 20 - cubes[i][j].worldmatrix.scale.y;
					}
					else if (cubes[i][j].worldmatrix.scale.y < 0)
					{
						cubes[i][j].direction_Y = 1;
						cubes[i][j].worldmatrix.scale.y = -cubes[i][j].worldmatrix.scale.y;
					}
				}
			}
			break;
		case 5:
			for (int i = 0; i < 25; ++i)
			{
				for (int j = 0; j < 25; ++j)
				{
					if (cubes[i][j].direction_Y == 1)
					{
						cubes[i][j].worldmatrix.scale.y += cubes[i][j].speed;
					}
					else if (cubes[i][j].direction_Y == 2)
					{
						cubes[i][j].worldmatrix.scale.y -= cubes[i][j].speed;
					}
					if (cubes[i][j].worldmatrix.scale.y > 10)
					{
						cubes[i][j].direction_Y = 2;
						cubes[i][j].worldmatrix.scale.y = 20 - cubes[i][j].worldmatrix.scale.y;
					}
					else if (cubes[i][j].worldmatrix.scale.y < 0)
					{
						cubes[i][j].direction_Y = 1;
						cubes[i][j].worldmatrix.scale.y = -cubes[i][j].worldmatrix.scale.y;
					}
				}
			}
			break;
		case 6:
			for (int i = 0; i < 25; ++i)
			{
				for (int j = 0; j < 25; ++j)
				{
					if (cubes[i][j].direction_Y == 1)
					{
						cubes[i][j].worldmatrix.scale.y += cubes[i][j].speed;
					}
					else if (cubes[i][j].direction_Y == 2)
					{
						cubes[i][j].worldmatrix.scale.y -= cubes[i][j].speed;
					}
					if (cubes[i][j].worldmatrix.scale.y > 10)
					{
						cubes[i][j].direction_Y = 2;
						cubes[i][j].worldmatrix.scale.y = 20 - cubes[i][j].worldmatrix.scale.y;
					}
					else if (cubes[i][j].worldmatrix.scale.y < 0)
					{
						cubes[i][j].direction_Y = 1;
						cubes[i][j].worldmatrix.scale.y = -cubes[i][j].worldmatrix.scale.y;
					}
				}
			}
			break;
		case 7:
			for (int i = 0; i < 25; ++i)
			{
				for (int j = 0; j < 25; ++j)
				{
					if (cubes[i][j].direction_Y == 1)
					{
						cubes[i][j].worldmatrix.scale.y += cubes[i][j].speed;
					}
					else if (cubes[i][j].direction_Y == 2)
					{
						cubes[i][j].worldmatrix.scale.y -= cubes[i][j].speed;
					}
					if (cubes[i][j].worldmatrix.scale.y > 10)
					{
						cubes[i][j].direction_Y = 2;
						cubes[i][j].worldmatrix.scale.y = 20 - cubes[i][j].worldmatrix.scale.y;
					}
					else if (cubes[i][j].worldmatrix.scale.y < 0)
					{
						cubes[i][j].direction_Y = 1;
						cubes[i][j].worldmatrix.scale.y = -cubes[i][j].worldmatrix.scale.y;
					}
				}
			}
			break;
		case 8:
			for (int i = 0; i < 25; ++i)
			{
				for (int j = 0; j < 25; ++j)
				{
					if (cubes[i][j].direction_Y == 1)
					{
						cubes[i][j].worldmatrix.scale.y += cubes[i][j].speed;
					}
					else if (cubes[i][j].direction_Y == 2)
					{
						cubes[i][j].worldmatrix.scale.y -= cubes[i][j].speed;
					}
					if (cubes[i][j].worldmatrix.scale.y > 10)
					{
						cubes[i][j].direction_Y = 2;
						cubes[i][j].worldmatrix.scale.y = 20 - cubes[i][j].worldmatrix.scale.y;
					}
					else if (cubes[i][j].worldmatrix.scale.y < 0)
					{
						cubes[i][j].direction_Y = 1;
						cubes[i][j].worldmatrix.scale.y = -cubes[i][j].worldmatrix.scale.y;
					}
				}
			}
			break;
		}

		if (EndAnimation)
		{
			for (int i = 0; i < 25; ++i)
			{
				for (int j = 0; j < 25; ++j)
				{
					cubes[i][j].worldmatrix.position.x += cubes[i][j].EndDirectionX;
					cubes[i][j].worldmatrix.position.y += cubes[i][j].EndDirectionY;
					cubes[i][j].worldmatrix.position.z += cubes[i][j].EndDirectionZ;

					cubes[i][j].worldmatrix.rotation.x += cubes[i][j].EndDirectionX;
					cubes[i][j].worldmatrix.rotation.y += cubes[i][j].EndDirectionY;
					cubes[i][j].worldmatrix.rotation.z += cubes[i][j].EndDirectionZ;
				}
			}

			EndCnt++;
			if (EndCnt > 100)
			{
				EndCnt = 0;

				cout << "갯수를 입력하세요.(ex : 5 5) : " << endl;
				cin >> in_cnt1 >> in_cnt2;

				animation = 1;
				for (int i = 0; i < 25; ++i)
				{
					for (int j = 0; j < 25; ++j)
					{
						cubes[i][j].worldmatrix.position.x = i * 0.5;
						cubes[i][j].worldmatrix.position.y = 0.0f;
						cubes[i][j].worldmatrix.position.z = j * 0.5;
						cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5 + cubes[i][j].start_Y, 0.5);
						cubes[i][j].worldmatrix.rotation.x = 0.0f;
						cubes[i][j].worldmatrix.rotation.y = 0.0f;
						cubes[i][j].worldmatrix.rotation.z = 0.0f;
						cubes[i][j].speed = speedDis(gen);
						cubes[i][j].start_Y = startYDis(gen);
						cubes[i][j].worldmatrix.scale = glm::vec3(0.5, 0.5 + cubes[i][j].start_Y, 0.5);
						cubes[i][j].direction_Y = 1;
					}
				}
				TSelection = true;
				LC_R = 1.0f, LC_G = 1.0f, LC_B = 1.0f;
				YSelection = 0;
				YSelectionCnt = 0;
				Ycnt = 0;

				EndAnimation = false;
			}
		}
		
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

//update() : 아예 데이터를 바꾸고 싶을때 쓴다.