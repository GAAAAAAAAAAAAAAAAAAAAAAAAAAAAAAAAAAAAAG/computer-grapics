#include <iostream>
#include <fstream>
#include <string>

using namespace std;
struct vec3 {
	float x;
	float y;
	float z;
};

int main()
{
	ifstream in{ "cube.obj" };

	string s;
	float x = 0;
	float y = 0;
	float z = 0;
	int f_x = 0;
	int f_y = 0;
	int f_z = 0;

	int v_count = 0;
	int f_count = 0;
	int vertex_count = f_count * 3;
	while (in >> s)
	{
		if (s == "v") v_count++;
		else if (s == "f") f_count++;
	}
	cout << v_count << ": 버텍스 개수" << endl;
	cout << f_count << ": 페이스 개수" << endl;
	in.close();
	in.open("cube.obj");

	vec3* vertex = new vec3 [v_count];
	vec3* face = new vec3[f_count];
	vec3* vertexdata = new vec3[f_count*3];

	vertex_count = f_count * 3;

	int v_incount = 0;
	int f_incount = 0;
	while (in>>s)
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

	for (int i = 0; i < 6; i++)
	{
		glm::vec3 normal = glm::cross(vertexdata[i * 6 + 1] - vertexdata[i * 6 + 0], vertexdata[i * 6 + 2] - vertexdata[i * 6 + 0]);
		//glm::vec3 normal = glm::vec3(0.0, 1.0, 0.0);
		normaldata[i * 6 + 0] = normal;
		normaldata[i * 6 + 1] = normal;
		normaldata[i * 6 + 2] = normal;
		normaldata[i * 6 + 3] = normal;
		normaldata[i * 6 + 4] = normal;
		normaldata[i * 6 + 5] = normal;
	}

	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	//glGenBuffers(3, vbo);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), vertexdata, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), colordata, GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	//glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), normaldata, GL_STATIC_DRAW);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(2);
}