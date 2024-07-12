#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <cmath>
#include <Windows.h>
#include <cstring>
#include <unordered_map>
#include <utility>
#include "ShaderClass.h"
using namespace std;
int l = 12;
bool b = 0;

const int WIDTH = 1024, HEIGHT = 1024, N = 10000;
unsigned int VAO, VBO, dx = 0;
const float ofs = 0.05f;
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 acolor;\n"
"out vec3 color;"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"	color = acolor;"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 color;"
"void main()\n"
"{\n"
"   FragColor = vec4(color, 1.0);\n"
"}\0";
// vec4(R,G,B,alpha)
// 
// 自定义哈希函数
struct pair_hash {
	template <class T1, class T2>
	size_t operator () (const pair<T1, T2>& p) const {
		auto h1 = hash<T1>{}(p.first);
		auto h2 = hash<T2>{}(p.second);
		return h1 ^ h2;
	}
};

// 自定义相等比较函数
struct pair_equal {
	bool operator()(const pair<float, float>& lhs, const pair<float, float>& rhs) const {
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}
};
unordered_map<pair<float, float>, int, pair_hash, pair_equal> mp;
struct Vertex {
	float x, y, z, R, G, B;
};

Vertex v[N] = {
	//pos				   // color
	{-0.05f, -0.05f, 0.0f, 1.0f, 1.0f, 1.0f} ,
	{-0.05f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
	{0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
	{-0.05f, -0.05f, 0.0f, 1.0f, 1.0f, 1.0f},
	{0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
	{0.0f, -0.05f, 0.0f, 1.0f, 1.0f, 1.0f},
};
Vertex Pt[6] = {
	{-0.05f - 2 * ofs, -0.05f, 0.0f, 1.0f, 1.0f, 0.0f} ,
	{-0.05f - 2 * ofs, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
	{0.0f - 2 * ofs, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
	{-0.05f - 2 * ofs, -0.05f, 0.0f, 1.0f, 1.0f, 0.0f},
	{0.0f - 2 * ofs, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
	{0.0f - 2 * ofs, -0.05f, 0.0f, 1.0f, 1.0f, 0.0f},
};
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Move(GLFWwindow* w);
void Check(int d);
void Draw(Vertex q[], int Len);

void Debug(float q[]) {
	for (int i = 0; i < 9; i++) printf("%f\n", q[i]);
}

int RandInt() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(-19, 19);
	return distrib(gen);
}

void Generate() {
GTE: bool fl = 1;
	float ulcx = ofs * RandInt(), ulcy = ofs * RandInt();//upper left corner
	for (int i = 0; i < l; i += 6) {
		if (ulcx == v[i + 1].x && ulcy == v[i + 1].y) {
			fl = 0;
			break;
		}
	}
	if (!fl || mp[{ulcx, ulcy}] > 0) goto GTE;
	Pt[0] = { ulcx, ulcy - 0.05f, 0.0f, 1.0f, 1.0f, 0.0f };
	Pt[1] = { ulcx, ulcy, 0.0f, 1.0f, 1.0f, 0.0f };
	Pt[2] = { ulcx + 0.05f, ulcy, 0.0f, 1.0f, 1.0f, 0.0f };
	Pt[3] = { ulcx, ulcy - 0.05f, 0.0f, 1.0f, 1.0f, 0.0f };
	Pt[4] = { ulcx + 0.05f, ulcy, 0.0f, 1.0f, 1.0f, 0.0f };
	Pt[5] = { ulcx + 0.05f, ulcy - 0.05f, 0.0f, 1.0f, 1.0f, 0.0f };

}

void IfEat() {
	const float ep = 0.02f;
	if (abs(Pt[1].x - v[1].x) < ep && abs(Pt[1].y - v[1].y) < ep) {
		memset(Pt, 0, sizeof Pt);
		for (int i = l; i < l + 6; i++) {
			v[i] = v[i - 6];
		}
		b = 1;
		Generate();
	}

}

void DeathOrWin(GLFWwindow* w) {
	if (l == 1444) {
		glfwSetWindowShouldClose(w, true);
		printf("You win!!!!!!!!!!!!!!!!");
	}
	if (mp[{v[1].x, v[1].y}] == 2) {
		glfwSetWindowShouldClose(w, true);
		printf("You lose!!!!!!!!!!!!!!!!");
		printf("%c", 7);
	}
}

int main() {

	//mp[{v[1].x, v[1].y}] += 1;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// make version OpenGL3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// make mode core-profile
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Snake(Utimate Edition)", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(0);
	}
	glfwMakeContextCurrent(window);
	// create an object -------------  (width,height,title,?,?)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(0);
	}
	//initialize GLAD
	glViewport(0, 0, WIDTH, HEIGHT);
	// glViewport(x,y,width,height) the coordinate of the window's lower left corner
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	Shader myShader("ShaderFile/v_Shader.vs", "ShaderFile/F_Shader.fs");
	myShader.use();
	Generate();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 0.6f);
		glClear(GL_COLOR_BUFFER_BIT);
		Check(dx);
		DeathOrWin(window);
		IfEat();
		Move(window);
		

		
		Draw(v, l);
		Draw(Pt, 6);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}

void Move(GLFWwindow* w) {

	if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS && dx != 2) {
		dx = 1;
	}
	else if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS && dx != 1) {
		dx = 2;
	}
	else if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS && dx != 4) {
		dx = 3;
	}
	else if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS && dx != 3) {
		dx = 4;
	}
	Sleep(50);
}

void Check(int d) {
	mp[{v[l - 5].x, v[l - 5].y}] -= 1;
	for (int i = l - 1; i >= 6; i--) {
		v[i] = v[i - 6];
	}
	if (d == 1) for (int i = 0; i < 6; i++) v[i].x += ofs;//right
	if (d == 2) for (int i = 0; i < 6; i++) v[i].x -= ofs;//left
	if (d == 3) for (int i = 0; i < 6; i++) v[i].y += ofs;//up
	if (d == 4) for (int i = 0; i < 6; i++) v[i].y -= ofs;//down
	if (d == 1 || d == 2) { // left or right
		for (int i = 0; i < 6; i++) {
			if (v[i].x >= 1.05f) {
				for (int j = 0; j < 6; j++) {
					v[j].x -= 2.05f;
				}
				break;
			}
			else if (v[i].x <= -1.05f) {
				for (int j = 0; j < 6; j++) {
					v[j].x += 2.05f;
				}
				break;
			}
		}
	}
	else { // up or down
		for (int i = 0; i < 6; i++) {
			if (v[i].y > 1.0f) {
				for (int j = 0; j < 6; j++) {
					v[j].y = -1.0f + (v[j].y - 1.0f);
				}
				break;
			}
			else if (v[i].y < -1.0f) {
				for (int j = 0; j < 6; j++) {
					v[j].y += 2.05f;
				}
				break;
			}
		}
	}
	mp[{v[1].x, v[1].y}] += 1;
	if (b) l += 6, b = 0, mp[{v[l - 5].x, v[l - 5].y}] += 1;
}

void Draw(Vertex q[], int Len) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Len * sizeof(Vertex), q, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// parameter--> (pos value,VertexAttributeSize,DataForm,IfNormalizeData) 1,2,3
	//Stride(interval between consecutive vertex attribute groups) 4
	//Offset of the starting pos of pos data in buffer 5

	glDrawArrays(GL_TRIANGLES, 0, Len);
	//(PrimitiveForm,startingPos,NumberToDraw)
}