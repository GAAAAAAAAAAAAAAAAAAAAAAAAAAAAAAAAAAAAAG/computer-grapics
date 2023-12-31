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
#include <vector>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> XYdis(-1, 1);
uniform_real_distribution<double> dis(0.0, 1.0);
uniform_real_distribution<double> speedDis(0.02, 0.07);
uniform_real_distribution<double> randomXYZ(-4.0, 4.0);

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
CUBE cube;
CUBE minicube;
CUBE plain;

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
vector<PYRAMID> pyramids;
PYRAMID Sierpinski[1000];

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
SPHERE sphere2;
SPHERE Rsphere;
SPHERE Gsphere;
SPHERE Bsphere;

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
		for (int i = 0; i < vertex_count; i++)
		{
			vertexdata[i] -= glm::vec3(0.5, 0.5, 0.5);
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
SNOW snow[100];

struct CIRCLE :OBJECT
{
	float r = 1;

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

			colordata[i].x = 1.0f;
			colordata[i].y = 1.0f;
			colordata[i].z = 1.0f;
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

	void update(int index)
	{
		float angle = 0;
		float x, y, z;

		switch (index)
		{
		case 1:
			for (int i = 0; i < vertex_count; ++i)
			{
				angle = (float)i / 360 * 2 * 3.1415926535;
				x = cos(angle) * 2;
				y = 0;
				z = sin(angle) * 3;

				vertexdata[i] = glm::vec4(x, y, z, 1.0f)*(glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0)));
			}
			break;
		case 2:
			for (int i = 0; i < vertex_count; ++i)
			{
				angle = (float)i / 360 * 2 * 3.1415926535;
				x = cos(angle) * 3;
				y = 0;
				z = sin(angle) * 3;

				vertexdata[i] = glm::vec4(x, y, z, 1.0f) * (glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0)))*(glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0)));
			}
			break;
		case 3:
			for (int i = 0; i < vertex_count; ++i)
			{
				angle = (float)i / 360 * 2 * 3.1415926535;
				x = cos(angle) * 3;
				y = 0;
				z = sin(angle) * 2;

				vertexdata[i] = glm::vec4(x, y, z, 1.0f) * (glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0)));
			}
			break;
		}

		
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), vertexdata, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}
};
CIRCLE Rcircle;
CIRCLE Gcircle;
CIRCLE Bcircle;

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

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.0f); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

GLuint vao, vbo[3];
GLuint TriPosVbo, TriColorVbo;

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

int windowWidth = 800;
int windowHeight = 800;

float openGLX, openGLY;
int movingRectangle = -1;

float ox = 0, oy = 0;
float x_angle = 0;
float y_angle = 0;
float z_angle = 0;
float pre_x_angle = 0;
float pre_y_angle = 0;
float wheel_scale = 0.15;
bool left_button = 0;
float fovy = 45;
float near_1 = 0.1;
float far_1 = 200.0;
float persfect_z = -2.0;

bool start = true;

bool RSelection = false;
float LC_R, LC_G, LC_B;
bool SSelection = false;
bool MSelection = true;
int CSelection = 0;
int PSelection = 0;
bool Selection[5]{};
int tri_level{};

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
void pyramid_Sierpinski(PYRAMID pyramid);
void init_tri(int level, float scale, float x, float y, float z, float height, float length);

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
	minicube.ReadObj("cube.obj");
	plain.ReadObj("cube.obj");
	pyramid.ReadObj("pyramid.obj");
	sphere.ReadObj("sphere.obj");
	sphere2.ReadObj("sphere.obj");
	Rsphere.ReadObj("sphere.obj");
	Gsphere.ReadObj("sphere.obj");
	Bsphere.ReadObj("sphere.obj");
	for (int i = 0; i < 100; i++)
	{
		snow[i].ReadObj("sphere.obj");
	}
	for (int i = 0; i < 1000; i++)
	{
		Sierpinski[i].ReadObj("pyramid.obj");
	}

	LC_R = 1.0, LC_G = 1.0, LC_B = 1.0;

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
	glUseProgram(shaderProgramID);
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //--- 깊이 버퍼를 클리어한다.

	glBindVertexArray(vao);

	// 색상 바꾸기
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), XYZcolors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	int modelLocation = glGetUniformLocation(shaderProgramID, "model"); //--- 버텍스 세이더에서 모델링 변환 행렬 변수값을 받아온다.
	int viewLocation = glGetUniformLocation(shaderProgramID, "view"); //--- 버텍스 세이더에서 뷰잉 변환 행렬 변수값을 받아온다.
	int projLocation = glGetUniformLocation(shaderProgramID, "projection"); //--- 버텍스 세이더에서 투영 변환 행렬 변수값을 받아온다.

	projection = glm::mat4(1.0f);
	projection = glm::scale(projection, glm::vec3(wheel_scale, wheel_scale, wheel_scale));
	projection = glm::rotate(projection, (float)glm::radians(x_angle + 30), glm::vec3(1.0, 0.0, 0.0));
	projection = glm::rotate(projection, (float)glm::radians(y_angle - 30), glm::vec3(0.0, 1.0, 0.0));

	unsigned int cameraLocation = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, glm::value_ptr(projection));

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
	glUniform3f(lightColorLocation, LC_R * (float)MSelection, LC_G * (float)MSelection, LC_B * (float)MSelection);
	unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 0.5, 0.3);

	if (start)
	{
		start = false;
	}

	model = glm::mat4(1.0f);

	//축 그리기
	for (int i = 0; i < 3; i++)
	{
		// 색상 바꾸기
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), XYZcolors[i * 2], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// modelTransform 변수에 변환 값 적용하기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), XYZShape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glLineWidth(2.0);
		glDrawArrays(GL_LINES, 0, 2);
	}

	//s r t p 코드 작성시에는 반대 방향으로.
	model = glm::mat4(1.0f);
	//cube.draw(shaderProgramID);
	//pyramid.draw(shaderProgramID);
	Rsphere.draw(shaderProgramID);
	Gsphere.draw(shaderProgramID);
	Bsphere.draw(shaderProgramID);
	minicube.draw(shaderProgramID);
	Rcircle.draw(shaderProgramID);
	Gcircle.draw(shaderProgramID);
	Bcircle.draw(shaderProgramID);
	plain.draw(shaderProgramID);
	if (SSelection)
	{
		for (int i = 0; i < 100; i++)
		{
			snow[i].draw(shaderProgramID);
		}
	}
	for (auto o : pyramids)
	{
		o.draw(shaderProgramID);
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
	minicube.Init();
	minicube.parent = &cube;
	pyramid.Init();
	sphere.Init();
	sphere2.Init();
	Rsphere.Init();
	Gsphere.Init();
	Bsphere.Init();
	Rcircle.Init();
	Gcircle.Init();
	Bcircle.Init();
	
	Rsphere.parent = &sphere;
	Gsphere.parent = &sphere;
	Bsphere.parent = &sphere;
	
	plain.Init();
	for (int i = 0; i < 100; i++)
	{
		snow[i].Init();
		snow[i].worldmatrix.position.x = randomXYZ(gen);
		snow[i].worldmatrix.position.y = 10;
		snow[i].worldmatrix.position.z = randomXYZ(gen);
		snow[i].worldmatrix.scale = glm::vec3(0.05, 0.05, 0.05);
	}
	for (int i = 0; i < 1000; i++)
	{
		Sierpinski[i].Init();
	}

	Rcircle.update(1);
	Gcircle.update(2);
	Bcircle.update(3);
	Rsphere.update({ 1.0, 0.0, 0.0 });
	Gsphere.update({ 0.0, 1.0, 0.0 });
	Bsphere.update({ 0.0, 0.0, 1.0 });

	plain.worldmatrix.position.y = -0.5;
	plain.worldmatrix.scale = glm::vec3(5, 0.01, 5);

	/*Rsphere.worldmatrix.position.y = 1;
	Rsphere.worldmatrix.position.z = -3;*/
	Rsphere.worldmatrix.scale = glm::vec3(0.25, 0.25, 0.25);
		
	/*Gsphere.worldmatrix.position.y = 1;
	Gsphere.worldmatrix.position.z = -2;*/
	Gsphere.worldmatrix.scale = glm::vec3(0.3, 0.3, 0.3);
		
	/*Bsphere.worldmatrix.position.x = 3;
	Bsphere.worldmatrix.position.y = 0;
	Bsphere.worldmatrix.position.z = 3;*/
	Bsphere.worldmatrix.scale = glm::vec3(0.5, 0.5, 0.5);

	minicube.worldmatrix.position.y = 3;
	minicube.worldmatrix.position.z = 3;
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

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
		pyramids.clear();
		tri_level = key - '0';
		init_tri(tri_level, 1.0f, 0, 0, 0, 1.0f, 1.0f);
		break;
	case 'p':
		PSelection += 1;
		if (PSelection > 3)
		{
			PSelection = 0;
		}
		switch (PSelection)
		{
		case 0:
			cube.worldmatrix.rotation.y = 0;
			cube.modelmatrix.rotation.y = 0;

			minicube.worldmatrix.position.x = 1.0;
			minicube.worldmatrix.position.y = 3.0;
			minicube.worldmatrix.position.z = 0.0;
			break;
		case 1:
			cube.worldmatrix.rotation.y = 0;
			cube.modelmatrix.rotation.y = 0;

			minicube.worldmatrix.position.x = -1.0;
			minicube.worldmatrix.position.y = 3.0;
			minicube.worldmatrix.position.z = 0.0;
			break;
		case 2:
			cube.worldmatrix.rotation.y = 0;
			cube.modelmatrix.rotation.y = 0;

			minicube.worldmatrix.position.x = 0.0;
			minicube.worldmatrix.position.y = 3.0;
			minicube.worldmatrix.position.z = 1.0;
			break;
		case 3:
			cube.worldmatrix.rotation.y = 0;
			cube.modelmatrix.rotation.y = 0;

			minicube.worldmatrix.position.x = 0.0;
			minicube.worldmatrix.position.y = 3.0;
			minicube.worldmatrix.position.z = -1.0;
			break;
		}
		break;
	case 'c':
		CSelection += 1;
		if (CSelection > 3)
		{
			CSelection = 0;
		}
		switch (CSelection)
		{
		case 0:
			LC_R = 1.0, LC_G = 1.0, LC_B = 1.0;
			break;
		case 1:
			LC_R = 1.0, LC_G = 1.0, LC_B = 0.0;
			break;
		case 2:
			LC_R = 0.0, LC_G = 1.0, LC_B = 1.0;
			break;
		case 3:
			LC_R = 1.0, LC_G = 0.0, LC_B = 1.0;
			break;
		}
		break;
	case 'm':
		MSelection = !MSelection;
		break;
	case 's':
		SSelection = !SSelection;
		break;
	case '-':
		if (LC_R > 0.1)
		{
			LC_R -= 0.1;
			LC_G -= 0.1;
			LC_B -= 0.1;
		}
		break;
	case '=':
		if (LC_R < 1.0)
		{
			LC_R += 0.1;
			LC_G += 0.1;
			LC_B += 0.1;
		}
		break;
	case 'n':
		minicube.worldmatrix.position.y -= 0.1;
		minicube.worldmatrix.position.z -= 0.1;
		break;
	case 'f':
		minicube.worldmatrix.position.y += 0.1;
		minicube.worldmatrix.position.z += 0.1;
		break;
	case 'r':
		RSelection = !RSelection;
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

float cnt = 0;

GLvoid TimerFunction(int value)
{
	switch (value)
	{
	case 1:
		glm::mat4 tempR_Z = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 tempR_X = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));

		Rsphere.worldmatrix.position.x = cos(cnt) * 2;
		Rsphere.worldmatrix.position.z = sin(cnt) * 3;
		Rsphere.worldmatrix.position = glm::vec4(Rsphere.worldmatrix.position.x, 0.0, Rsphere.worldmatrix.position.z, 1.0f) * tempR_X;

		Gsphere.worldmatrix.position.x = cos(cnt) * 3;
		Gsphere.worldmatrix.position.z = sin(cnt) * 3;
		Gsphere.worldmatrix.position = glm::vec4(Gsphere.worldmatrix.position.x, 0.0, Gsphere.worldmatrix.position.z, 1.0f) * tempR_X*tempR_Z;

		Bsphere.worldmatrix.position.x = cos(cnt)*3;
		Bsphere.worldmatrix.position.z = sin(cnt)*2;
		Bsphere.worldmatrix.position = glm::vec4(Bsphere.worldmatrix.position.x, 0.0, Bsphere.worldmatrix.position.z, 1.0f)*tempR_Z;
		cnt += 0.02;
		
		if (SSelection)
		{
			for (int i = 0; i < 100; i++)
			{
				snow[i].worldmatrix.position.y -= snow[i].speed;
				if (snow[i].worldmatrix.position.y < -0.5)
				{
					snow[i].worldmatrix.position.y = 10;
				}
			}
		}
		if (RSelection)
		{
			cube.worldmatrix.rotation.y += 1;
			cube.modelmatrix.rotation.y -= 1;
		}
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}
//update() : 아예 데이터를 바꾸고 싶을때 쓴다.

//void Tri_Sierpinski(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int depth) {
//	if (depth == 0) {
//		pyramid.m_vertex.emplace_back(x1, y1, z1);
//		pyramid.m_vertex.emplace_back(x2, y2, z2);
//		pyramid.m_vertex.emplace_back(x3, y3, z3);
//		pyramid.m_normal.emplace_back(0.0, 0.0, 1.0);
//		pyramid.m_normal.emplace_back(0.0, 0.0, 1.0);
//		pyramid.m_normal.emplace_back(0.0, 0.0, 1.0);
//	}
//	else {
//		float mid1x = (x1 + x2) / 2;
//		float mid1y = (y1 + y2) / 2;
//		float mid1z = (z1 + z2) / 2;
//		float mid2x = (x2 + x3) / 2;
//		float mid2y = (y2 + y3) / 2;
//		float mid2z = (z2 + z3) / 2;
//		float mid3x = (x1 + x3) / 2;
//		float mid3y = (y1 + y3) / 2;
//		float mid3z = (z1 + z3) / 2;
//
//		Tri_Sierpinski(x1, y1, z1, mid1x, mid1y, mid1z, mid3x, mid3y, mid3z, depth - 1);
//		Tri_Sierpinski(mid1x, mid1y, mid1z, x2, y2, z2, mid2x, mid2y, mid2z, depth - 1);
//		Tri_Sierpinski(mid3x, mid3y, mid3z, mid2x, mid2y, mid2z, x3, y3, z3, depth - 1);
//	}
//}

int pyramid_Sierpinski_cnt{};

void pyramid_Sierpinski(PYRAMID pyramid)
{
	glm::vec3 V1 = pyramid.vertexdata[0];
	glm::vec3 V2 = pyramid.vertexdata[1];
	glm::vec3 V3 = pyramid.vertexdata[2];
	glm::vec3 V4 = pyramid.vertexdata[3];
	glm::vec3 V5 = pyramid.vertexdata[4];

	glm::vec3 M1 = glm::vec3((V2.x- V1.x) / 2 , V1.y , V1.z);
	glm::vec3 M2 = glm::vec3(V2.x ,V2.y , (V3.z - V2.z) / 2);
	glm::vec3 M3 = glm::vec3((V3.x- V4.x) / 2 , V3.y , V3.z);
	glm::vec3 M4 = glm::vec3(V4.x , V4.y , (V4.z - V1.z) / 2);
	glm::vec3 M5 = glm::vec3((V3.x - V1.x) / 2 , V1.y , (V3.z - V1.z) / 2);

	glm::vec3 M6 = glm::vec3((M5.x - V1.x) / 2 , (V5.y - M5.y) / 2 , (M5.z - V1.z) / 2);
	glm::vec3 M7 = glm::vec3((M2.x - M1.x) / 2 , (V5.y - M5.y) / 2 , (M2.z - M1.z) / 2);
	glm::vec3 M8 = glm::vec3((V3.x - M5.x) / 2 , (V5.y - M5.y) / 2 , (V3.z - M5.z) / 2);
	glm::vec3 M9 = glm::vec3((M3.x - M4.x) / 2 , (V5.y - M5.y) / 2 , (M3.z - M4.z) / 2);

	//PYRAMID *Sierpinski_pyramid = new PYRAMID [5];
	
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[0] = V1;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[1] = M1;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[2] = M5;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[3] = M4;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[4] = M6;
	Sierpinski[pyramid_Sierpinski_cnt].update();

	pyramid_Sierpinski_cnt++;

	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[0] = M1;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[1] = V2;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[2] = M2;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[3] = M5;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[4] = M7;
	Sierpinski[pyramid_Sierpinski_cnt].update();

	pyramid_Sierpinski_cnt++;

	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[0] = M5;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[1] = M2;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[2] = V3;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[3] = M3;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[4] = M8;
	Sierpinski[pyramid_Sierpinski_cnt].update();

	pyramid_Sierpinski_cnt++;

	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[0] = M4;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[1] = M5;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[2] = M3;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[3] = V4;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[4] = M9;
	Sierpinski[pyramid_Sierpinski_cnt].update();

	pyramid_Sierpinski_cnt++;

	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[0] = M6;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[1] = M7;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[2] = M8;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[3] = M9;
	Sierpinski[pyramid_Sierpinski_cnt].vertexdata[4] = V5;
	Sierpinski[pyramid_Sierpinski_cnt].update();

	pyramid_Sierpinski_cnt++;
}

void init_tri(int level, float scale, float x, float y, float z, float height, float length)
{
	if (level == 0)
	{
		pyramid.modelmatrix.scale = glm::vec3(scale * 1.1, scale * 1.1, scale * 1.1);
		pyramid.modelmatrix.position = glm::vec3(x, y, z);
		pyramids.push_back(pyramid);
	}
	else
	{
		init_tri(level - 1, scale / 2, x, y + height / 2, z, height / 2, length / 2);
		init_tri(level - 1, scale / 2, x - length / 4, y, z - length / 4, height / 2, length / 2);
		init_tri(level - 1, scale / 2, x + length / 4, y, z - length / 4, height / 2, length / 2);
		init_tri(level - 1, scale / 2, x - length / 4, y, z + length / 4, height / 2, length / 2);
		init_tri(level - 1, scale / 2, x + length / 4, y, z + length / 4, height / 2, length / 2);
	}
}