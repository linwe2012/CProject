#pragma once
#include<stdio.h>
#include<vector>
#include <time.h>
#include<stdlib.h>
#include <Windows.h>
#include <glad\glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <string>
#include "shader.h"
#include <GLFW\glfw3.h>
#include <iostream>
#include "camera.h"

#define CLOCK_PER_MS (CLOCKS_PER_SEC/1000.0)
#define BF_BLACK_WHITE 0x07
#define BF_BLACK_BWHITE 0x0F
#define BF_WHITE_BLACK 0x70
struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

template<class T>
class Analyzer
{
public:
	Analyzer();
	void registerData(T *(*fun)(int count));
	void dataFunctionHint();
	void registerFunction(void(*fun)(T *a, int count), const char*s, glm::vec3 clr=glm::vec3(-1.0f, 0.0f, 0.0f));
	void registerFunction(void(*fun)(T *a, T *b));
	void registerDataDealer(void(*_datadealer)(int *arr, int n, bool sorted, const char *name, double time));
	void benchmark();
	void setDataAmount(int _start, int _end, int _step);

	void plot_new();
	void plot_cleanup();
	void plot_update();
	void plot_refresh();
	void plot_request();

	std::vector<void(*)(T *a, int count)>fun1;
	std::vector<void(*)(T *a,T *b)>fun2;
	std::vector<const char *>funs;
	std::vector<int>which;
	std::vector<double>time;
	std::vector<glm::vec3>color;
	T *(*genData)(int count);
	T *data;
	void (*datadealer)(int *arr, int n, bool sorted, const char *name, double time);
	int step;
	int start, end;

	int enablePlot;
	int anti_aliasing;
	std::vector<Vertex>graph;
	std::vector<Vertex>graph_des;
	std::vector<GLuint>indices;
	GLuint VAO, EBO, VBO;
	GLFWwindow* window;
	GLfloat originx, originy;
	GLfloat arrowSize;
	GLuint cidx;
	Shader *shader;
	int firstTimeCall;
	static int focused;
	static int passive;
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void window_focus_callback(GLFWwindow* window, int _focused);
	static void scroll_callback(GLFWwindow* mwindow, double xoffset, double yoffset);
	void larr(std::vector<Vertex>&res, std::vector<double>&src, int count, GLfloat stretch, GLfloat base);
	static Camera *camera;
	std::vector<Vertex>lines;
	GLuint VAO_lines, VBO_lines;

private:
	int makeaxis();
	void addvertex(glm::vec3 pos, glm::vec3 clr);
	void addLineIdx(GLuint start, GLuint end);
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	double maxTime;
	int axiscnt;
	int laSample;
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;
	int shouldEnd;
};

template<class T>
Analyzer<T>::Analyzer()
{
	genData = NULL;
	data = NULL;
	step = 1;
	start = 1;
	end = 100;
	originx = -0.9f;
	originy = -0.9f;
	arrowSize = 0.03f;
	enablePlot = TRUE;
	anti_aliasing = TRUE;
	shader = NULL;
	maxTime = 1;
	firstTimeCall = TRUE;
	axiscnt = 0;
	laSample = 4;
	shouldEnd = FALSE;
}

template<class T>
int Analyzer<T>::focused = 0;

template<class T>
int Analyzer<T>::passive = FALSE;

template<class T>
Camera* Analyzer<T>::camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

template<class T>
void Analyzer<T>::registerFunction(void(*fun)(T *a, int count), const char*s, glm::vec3 clr)
{
	static int mcnt = 0;
	fun1.push_back(fun);
	which.push_back(1);
	funs.push_back(s);
	if (clr.r == -1.0f) {
		//color.push_back(glm::vec3(1.0f, 0.5f, 0.3f));
		if (mcnt == 0) {
			color.push_back(glm::vec3(0.4f, 0.7f, 0.8f));
		}
		else if(mcnt == 1) {
			color.push_back(glm::vec3(1.0f, 0.5f, 0.3f));
		}
		else {
			GLfloat mr = (rand() % 100) / 100.0f;
			GLfloat mg = (rand() % 100) / 100.0f;
			GLfloat mb = (rand() % 100) / 100.0f;
			color.push_back(glm::vec3(mr, mg, mb));
		}
		mcnt++;
	}
	else {
		color.push_back(clr);
	}
}

template<class T>
void Analyzer<T>::registerFunction(void(*fun)(T *a, T *b))
{
	fun2.push_back(fun);
	which.push_back(2);
}

template<class T>
void Analyzer<T>::registerData(T *(*fun)(int count)) {
	genData = fun;
}

template<class T>
void Analyzer<T>::registerDataDealer(void(*_datadealer)(int *arr, int n, bool sorted, const char *name, double time)) {
	datadealer = _datadealer;
}

template<class T>
void Analyzer<T>::setDataAmount(int _start, int _end, int _step)
{
	start = _start;
	end = _end;
	step = _step;
}

template<class T>
void Analyzer<T>::benchmark()
{
	clock_t  st, et;
	double ct;
	int f1=0, f2=0;
	int i;
	int which_len = which.size();
	double mminTime;
	unsigned long largestTime = (~0L) >> 1;
	int *tobetest = (int *)malloc(sizeof(int) * (end+1));
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleColor, BF_BLACK_BWHITE);
	printf("Count");
	for (int k = 0, len = funs.size(); k < len; k++) {
		printf("\t|%s", funs[k]);
	}
	printf("\n");
	SetConsoleTextAttribute(hConsoleColor, BF_BLACK_WHITE);
	if (enablePlot == TRUE) {
		plot_new();
	}
	for (i = start; i <= end;  i += step) {
		f1 = 0;  f2 = 0; mminTime = double(0x7fffffff);
		data = genData(i);
		printf("%d\t", i);
		datadealer(data, i, false, "__Unsorted__", 0);
		for (int k = 0; k < which_len; k++) {
			memcpy(tobetest, data, i * sizeof(int));
			glfwPollEvents();
			if (which[k] == 1) {
				st = clock();
				fun1[f1](tobetest, i);
				et = clock();
				ct = (et - st) / CLOCK_PER_MS;
				f1++;
				datadealer(tobetest, i, true, funs[k], ct);
			}
			glfwPollEvents();
			time.push_back(ct);
			if (ct > maxTime) {
				maxTime = ct;
			}
			else if (ct < mminTime)
			{
				mminTime = ct;
			}
		}
		int itr = time.size() - which_len;
		for (int k = 0; k < which_len; k++, itr++) {
			if (time[itr] == mminTime) {
				SetConsoleTextAttribute(hConsoleColor, BF_WHITE_BLACK);
				printf("%.1f\t", time[itr]);
				SetConsoleTextAttribute(hConsoleColor, BF_BLACK_WHITE);
			}
			else {
				printf("%.1f\t", time[itr]);
			}
		}
		free(data);
		plot_request();
		printf("\n");
	}
	lines.clear();
	glBindVertexArray(VAO_lines);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_lines);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * lines.size(), &(lines[0]), GL_STATIC_DRAW);
	while (shouldEnd == FALSE) {
		plot_refresh();
	}
	free(tobetest);
}

template<class T>
void Analyzer<T>::plot_new()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (anti_aliasing == TRUE) {
		glfwWindowHint(GLFW_SAMPLES, 4);
	}
#ifdef  __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //something for mac
#endif //  __APPLE__

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Graph", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, &Analyzer::framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	if (anti_aliasing == TRUE) {
		glEnable(GL_MULTISAMPLE);
	}
	if (shader != NULL) {
		delete shader;
	}
	glfwSetWindowFocusCallback(window, &Analyzer::window_focus_callback);
	glfwSetScrollCallback(window, &Analyzer::scroll_callback);
	shader = new Shader("./shaders/3.3.shader.vs.c", "./shaders/3.3.shader.fs.c");
	indices.clear();
	graph.clear();
	lines.clear();
	axiscnt = cidx = makeaxis();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * graph.size(), &(graph[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &(indices[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(~0);

	//lines
	glGenVertexArrays(1, &VAO_lines);
	glGenBuffers(1, &VBO_lines);
	glBindVertexArray(VAO_lines);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_lines);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * lines.size(), &(lines[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	maxTime = -1;
	shouldEnd = FALSE;
	plot_refresh();
	firstTimeCall = TRUE;

}

template<class T>
void Analyzer<T>::plot_refresh()
{
	//get key input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			glfwDestroyWindow(window);
			plot_cleanup();
			shouldEnd = TRUE;
		}
		else {
			camera->BudgeCamera(0, 0.5);
		}
	}
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	shader->use();
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	//pass delta time to determine if it should move back
	camera->PassDeltaTime(deltaTime);
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	shader->setMat4("projection", projection);

	// camera/view transformation
	glm::mat4 view = camera->GetViewMatrix();
	shader->setMat4("view", view);
	glBindVertexArray(VAO);
	glDrawElements(GL_LINE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(VAO_lines);
	glDrawArrays(GL_LINES, 0, lines.size());
	glfwSwapBuffers(window);
	glfwPollEvents();
	if (glfwWindowShouldClose(window)) {
		glfwDestroyWindow(window);
		plot_cleanup();
		shouldEnd = TRUE;
	}
}

template<class T>
void Analyzer<T>::plot_request()
{
	if (enablePlot == TRUE) {
		if (passive) {
			if (focused) {
				plot_refresh();
			}
			else {
				glfwPollEvents();
			}
		}
		else {
			plot_update();
			//plot_refresh();
		}
	}
}

template<class T>
void Analyzer<T>::plot_update()
{
	GLfloat y, x;
	GLfloat range = end - start + 1;
	GLfloat length = range / step;
	//GLfloat cnt = range * funs.size();

	if (firstTimeCall) {
		for (int i = 0, len = funs.size(); i<len; i++) {
			y = time[i] / maxTime * 1.8f + originy;
			for (int j = 0; j < length-1; j++) {
				x = j / range * 1.8f * step + originx;
				addvertex(glm::vec3(x, y, 0.0f), color[i]);
				indices.push_back(cidx++);
			}
			indices.push_back(~0);
		}
		graph_des = graph;
		firstTimeCall = FALSE;
		Vertex vtx = { glm::vec3(originx + 0.001f, -2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) };
		lines.push_back(vtx);
		vtx.position.y = 2.0f;
		lines.push_back(vtx);
	}
	else {
		int sz = time.size() / funs.size();
		larr(graph, time, length, 1.8f, -0.9f);
		lines[1].position.x = lines[0].position.x = originx + sz / length * 1.8f;
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * graph.size(), &(graph[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &(indices[0]), GL_STATIC_DRAW);
	glBindVertexArray(VAO_lines);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_lines);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * lines.size(), &(lines[0]), GL_STATIC_DRAW);
	plot_refresh();
}

template<class T>
void Analyzer<T>::plot_cleanup()
{
	indices.clear();
	graph.clear();
	graph_des.clear();
	firstTimeCall = TRUE;
}

template<class T>
void Analyzer<T>::larr(std::vector<Vertex>&res, std::vector<double>&src, int count, GLfloat stretch, GLfloat base)
{
	int func = funs.size();
	int cnt = src.size() / func;
	int srcsz = src.size();
	int lastep;
	//y coordinate y0,y1,,,
	static double arr[20];
	//coefficient y0/(x0-x1)(x0-x2)
	static double coeff[20];
	//denominator (x0-x1)(x0-x2)
	static double dnmtr[20];
	double tempt;
	double temp;
	int allowance;
	double slope;
	double tbase;
	//sampling
	allowance = cnt;
	if (cnt > laSample) {
		lastep = cnt / laSample;
		cnt = laSample;
	}
	else {
		lastep = 1;
	}
	//plot existing point
	for (int i = 0; i < func; i++) {
		for (int k = 0; k < allowance; k++) {
			res[count*i + k + axiscnt].position.y = src[i + k * func] / maxTime * stretch + base;
		}
	}
	//for (int i = 0; i < func; i++) {
	//	tbase = src[srcsz - func + i];
	//	slope = (src[srcsz - func + i] - src[srcsz - 2 * func + i]) ;
	//	for (int k = allowance; k <= count+1; k++) {
	//		res[count*i + k + axiscnt].position.y = ((k - allowance + 1) * slope +tbase) / maxTime * stretch + base;
	//	}
	//}
	//(x0-x1)(x0-x2)...(x1-x0)(x1-x2)...
	//for (int k = 0; k < cnt; k++) {
	//	temp = 1.0;
	//	for (int j = 0; j < cnt; j++) {
	//		if (j == k)
	//			continue;
	//		temp *= double(k - j) / count * stretch * lastep * step; //(Xk-Xj)
	//	}
	//	dnmtr[k] = temp;
	//	//printf("dnmtr[%d]=%.10f, step=%d\t",k, temp, lastep);
	//}
	//for (int i = 0; i < func; i++) {
	//	//y0,y1,y2...
	//	//printf("\n");
	//	for (int k = 0; k < cnt; k++) {
	//		tempt = src[srcsz-k*func*lastep - func + i];
	//		arr[cnt - k - 1] = tempt / maxTime * stretch + base;
	//		//y0/(x0-x1)(x0-x2)...
	//		coeff[cnt-k - 1] = arr[cnt - k -1] / dnmtr[cnt - k - 1];
	//		//printf("tempt[%d]=%.2f, coeff[%d]=%.10f\t", srcsz - k * func*lastep - func + i, tempt, cnt - k - 1, coeff[cnt - k - 1]);
	//	}
	//	for (int k = allowance; k < count; k++) {
	//		double ypos = 0.0;
	//		for (int j = 0; j < cnt; j++) {
	//			temp = coeff[j];
	//			for (int m = 0; m < cnt; m++) {
	//				if (j == m)
	//					continue;
	//				temp *= double(k - m*lastep)*stretch / count;
	//			}
	//			ypos += temp;
	//		}
	//		res[k+axiscnt+i*count].position.y = GLfloat(ypos); // = glm::vec3(float(k) / count * stretch + base, ypos, 0.0f);
	//	}
	//}
}

template<class T>
int Analyzer<T>::makeaxis()
{
	//origin index:0
	Vertex vtx = { glm::vec3(originx, originy, 0.0f) , glm::vec3(1.0f, 0.0f, 0.0f) };
	graph.push_back(vtx);
	//y & arrows  vbo used:1-3
	addvertex(glm::vec3(originx, originy + 1.8f, 0.0f),                         glm::vec3(0.0f, 1.0f, 0.0f)); addLineIdx(0, 1);
	addvertex(glm::vec3(originx - arrowSize, originy + 1.8f - arrowSize, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); addLineIdx(1, 2);
	addvertex(glm::vec3(originx + arrowSize, originy + 1.8f - arrowSize, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); addLineIdx(1, 3);
	//x & arrows vbo used:4-6
	addvertex(glm::vec3(originx + 1.8f, originy, 0.0f),                         glm::vec3(0.0f, 0.0f, 1.0f)); addLineIdx(0, 4);
	addvertex(glm::vec3(originx + 1.8f - arrowSize, originy - arrowSize, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); addLineIdx(4, 5);
	addvertex(glm::vec3(originx + 1.8f - arrowSize, originy + arrowSize, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); addLineIdx(4, 6);

	return 7;
}

template<class T>
void Analyzer<T>::addvertex(glm::vec3 pos, glm::vec3 clr)
{
	Vertex vtx = { pos, clr };
	graph.push_back(vtx);
}

template<class T>
void Analyzer<T>::addLineIdx(GLuint start, GLuint end)
{
	indices.push_back(start);
	indices.push_back(end);
	indices.push_back(~0);
}

template<class T>
void Analyzer<T>::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

template<class T>
void Analyzer<T>::window_focus_callback(GLFWwindow* window, int _focused)
{
	focused = _focused;
}

template<class T>
void Analyzer<T>::scroll_callback(GLFWwindow* mwindow, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(yoffset);
}