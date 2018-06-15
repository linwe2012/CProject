#pragma once
#include<stdio.h>
#include<vector>
#include <time.h>
#include<stdlib.h>
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
#include <ft2build.h>
#include FT_FREETYPE_H
#include "camera.h"
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32
#include<Psapi.h>

#define CLOCK_PER_MS (CLOCKS_PER_SEC/1000.0)
#define BF_BLACK_WHITE 0x07
#define BF_BLACK_BWHITE 0x0F
#define BF_WHITE_BLACK 0x70
#define BF_BLACK_GREY 0x08

#define NAN_UL 0UL

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

struct FunctionInfo
{
	DWORD iniMem;
	DWORD maxMem;
	double maxTime;
};
struct  FunctionCost
{
	//memory Usage
	DWORD mem;
};

struct TimerCallBackPara {
	int k;
	HANDLE process;
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
	int cancelFunction(const char*s);
	void registerDataDealer(void(*_datadealer)(int *arr, int n, bool sorted, const char *name, double time));
	void benchmark();
	void setDataAmount(int _start, int _end, int _step);
	void printMem();

	void plot_new();
	void plot_cleanup();
	void plot_update();
	void plot_refresh();
	void plot_request();
	void plot_mem();
	void plot_name();

	std::vector<void(*)(T *a, int count)>fun1;
	std::vector<void(*)(T *a,T *b)>fun2;
	std::vector<const char *>funs;
	std::vector<int>which;
	std::vector<double>time;
	std::vector<FunctionCost>funcost;
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
	//static std::vector<std::shared_ptr(Analyzer<T>)>m_analyzer_ptrs;
	void larr(std::vector<Vertex>&res, std::vector<double>&src, int count, GLfloat stretch, GLfloat base);
	static Camera *camera;
	std::vector<Vertex>lines;
	GLuint VAO_lines, VBO_lines;
	std::vector<Vertex> MemGraph;
	std::vector<GLuint>Memindices;
	GLuint VAOM, VBOM, EBOM;
	//timers
	//VOID CALLBACK refreshGraph(PVOID lpParam, BOOLEAN TimerOrWaitFired);
	//DWORD refreshFrequency;
	static std::vector<FunctionInfo>funinfo;
	static VOID CALLBACK getMemInfo(PVOID lpParam, BOOLEAN TimerOrWaitFired);
	static HANDLE thisProcess;
	double TimeCutOff;

	//text
	Shader *textShader;
	// FreeType
	FT_Library ft;
	FT_Face face;
	GLuint textVAO, textVBO;
	std::map<GLchar, Character> Characters;
	void initText(int src_w, int scr_h);
	void renderText(const char *text, glm::vec3 pos, GLfloat scale, glm::vec3 color);
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
	int ifMemView;
	int firtTimeMemCall;
	int firstTimePlotNew;
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
	firstTimePlotNew = TRUE;
	axiscnt = 0;
	laSample = 4;
	shouldEnd = FALSE;
	datadealer = NULL;
	//refreshFrequency = 100;
	TimeCutOff = 2;
	ifMemView = FALSE;
	firtTimeMemCall = TRUE;
	textShader = NULL;
}

template<class T>
int Analyzer<T>::focused = 0;

template<class T>
int Analyzer<T>::passive = FALSE;

template<class T>
Camera* Analyzer<T>::camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

template<class T>
HANDLE Analyzer<T>::thisProcess = GetCurrentProcess();

template<class T>
std::vector<FunctionInfo> Analyzer<T>::funinfo;

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
int Analyzer<T>::cancelFunction(const char *s)
{
	int itr = 0;
	int cnt[3] = { 0 };
	for (int len = funs.size(); itr < len; itr++) {
		if (strcmp(s, funs[itr] )== 0) {
			int a = which[itr];
			which.erase(which.begin() + itr);
			funs.erase(funs.begin() + itr);
			int b = cnt[a];
			switch (a)
			{
			case 1:
				fun1.erase(fun1.begin() + b); return 0;
				break;
			default:
				break;
			}
			break;
		}
		cnt[which[itr]] ++;
	}
	//error
	return -1;
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
void Analyzer<T>::printMem()
{
	int funsz = funs.size();
#ifdef _WIN32
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleColor, BF_BLACK_BWHITE);
#endif // _WIN32
	printf("Memory Usage: (Note that NaN happens when functions run too fast to detect)\n");
	printf("Count");
	for (int k = 0, len = funs.size(); k < len; k++) {
		printf("\t|%s", funs[k]);
	}
	printf("\n");
#ifdef _WIN32
	SetConsoleTextAttribute(hConsoleColor, BF_BLACK_WHITE);
#endif // _WIN32
	for (int i = start, itr = 0; i <= end;  i += step, itr ++) {
		printf("%d\t", i);
		for (int k = 0; k < funsz; k++) {
			DWORD memUsage = funcost[k + itr*funsz].mem;
			if (memUsage != NAN_UL) {
				printf("%lu\t", memUsage);// 1024UL);
			}
			else {
				printf("NaN\t");
			}
		}
		printf("\n");
	}
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
	if (genData == NULL) {
		printf("No Data generation functions");
		return;
	}
#ifdef _WIN32
	HANDLE hConsoleColor;
	HANDLE hTimerUseless = NULL;
	HANDLE hTimerPlot = NULL;
	HANDLE hMenInfoQuery = NULL;
	PROCESS_MEMORY_COUNTERS iniMemCounter;
	//DWORD minMem;
	int timerPlotArg = 3;
	hMenInfoQuery = CreateTimerQueue();
	//WAITORTIMERCALLBACK cll = static_cast<VOID CALLBACK(Analyzer<T>::*)(PVOID, BOOLEAN)>(refreshGraph);
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleColor, BF_BLACK_BWHITE);
#endif // _WIN32
	funinfo.clear();
	time.clear();
	funcost.clear();
	printf("Count");
	for (int k = 0, len = funs.size(); k < len; k++) {
		FunctionInfo fi = { NAN_UL, NAN_UL, -1.0 };
		printf("\t|%s", funs[k]);
		funinfo.push_back(fi);
	}
	printf("\n");
#ifdef _WIN32
	SetConsoleTextAttribute(hConsoleColor, BF_BLACK_WHITE);
#endif // _WIN32
	if (enablePlot == TRUE) {
		plot_new();
#ifdef _WIN32
		//if (!CreateTimerQueueTimer(&hTimerUseless, hTimerPlot,
		//	(WAITORTIMERCALLBACK)&Analyzer<T>::refreshGraph, &timerPlotArg, 0, refreshFrequency, 0));
		//{
		//	printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
		//	return 3;
		//}
#endif // _WIN32
	}
	
	for (i = start; i <= end;  i += step) {
		data = genData(i);
		//GetProcessMemoryInfo(GetCurrentProcess(), &iniMemCounter, sizeof(iniMemCounter));
		//minMem = iniMemCounter.WorkingSetSize + iniMemCounter.PagefileUsage;
		f1 = 0;  f2 = 0; mminTime = double(0x7fffffff);
		printf("%d\t", i);
		if(datadealer != NULL)
			datadealer(data, i, false, "__Unsorted__", 0);
		for (int k = 0; k < which_len; k++) {
			memcpy(tobetest, data, i * sizeof(int));
			glfwPollEvents();
			if (shouldEnd == TRUE) {
				printf("Analyzer::Benchmark Cancelled because of User's Action\n");
				free(tobetest);
				return;
			}
#ifdef _WIN32
			if (funinfo[k].maxTime >= 1.0) {
				DWORD period = static_cast<DWORD>(funinfo[k].maxTime) / 100 + 1;
				TimerCallBackPara para;
				para.k = k;
				para.process = GetCurrentProcess();
				
				GetProcessMemoryInfo(GetCurrentProcess(), &iniMemCounter, sizeof(iniMemCounter));
				period = iniMemCounter.WorkingSetSize;// +iniMemCounter.PagefileUsage;
				funinfo[k].iniMem = iniMemCounter.WorkingSetSize;// +iniMemCounter.PagefileUsage;
				//hMenInfoQuery = CreateTimerQueue();
				if (!CreateTimerQueueTimer(&hTimerUseless, hMenInfoQuery,
					(WAITORTIMERCALLBACK)&Analyzer::getMemInfo, &para, 0, 1, 0))
				{
					printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
					return;
				}
				//printf("a");
			}
#endif // _WIN32
			if (which[k] == 1) {
				st = clock();
				fun1[f1](tobetest, i);
				et = clock();
				ct = (et - st) / CLOCK_PER_MS;
				
				f1++;
				if (datadealer != NULL)
					datadealer(tobetest, i, true, funs[k], ct);
			}
			if (hTimerUseless != NULL) {
				DeleteTimerQueueTimer(hMenInfoQuery, hTimerUseless, NULL);
				//DeleteTimerQueue(hMenInfoQuery);
				//hMenInfoQuery = NULL;
				hTimerUseless = NULL;
			}
			glfwPollEvents();
			if (shouldEnd == TRUE) {
				printf("Analyzer::Benchmark Cancelled because of user Action\n");
				free(tobetest);
				return;
			}
			time.push_back(ct);
			if (ct > maxTime) {
				maxTime = ct;
			}
			else if (ct < mminTime)
			{
				mminTime = ct;
			}
			FunctionCost fc;
			if (funinfo[k].iniMem == NAN_UL || funinfo[k].maxMem == NAN_UL) {
				fc.mem = NAN_UL;
				funcost.push_back(fc);
				funinfo[k].maxMem = NAN_UL;
				funinfo[k].iniMem = NAN_UL;
			}
			else {
				fc.mem = funinfo[k].maxMem - funinfo[k].iniMem + 1UL;
				//printf("\n %lu", fc.mem);
				funcost.push_back(fc);
				funinfo[k].iniMem = NAN_UL;
				funinfo[k].maxMem = NAN_UL;
			}
			if (funinfo[k].maxTime < ct) {
				funinfo[k].maxTime = ct;
			}
		}
		int itr = time.size() - which_len;
		for (int k = 0; k < which_len; k++, itr++) {
			if (time[itr] == mminTime) {
#ifdef _WIN32
				SetConsoleTextAttribute(hConsoleColor, BF_WHITE_BLACK);
#endif // _WIN32
				printf("%.1f\t", time[itr]);
#ifdef _WIN32
				SetConsoleTextAttribute(hConsoleColor, BF_BLACK_WHITE);
#endif // _WIN32
			}
			else {
				printf("%.1f\t", time[itr]);
			}
		}
		free(data);
		plot_request();
		if (shouldEnd == TRUE) {
			printf("Analyzer::Benchmark Cancelled because of user Action\n");
			free(tobetest);
			return;
		}
		printf("\n");
	}
	free(tobetest);
	lines.clear();
	glBindVertexArray(VAO_lines);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_lines);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * lines.size(), &(lines[0]), GL_STATIC_DRAW);
	while (shouldEnd == FALSE) {
		plot_refresh();
	}
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
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	indices.clear();
	graph.clear();
	lines.clear();
	axiscnt = cidx = makeaxis();
	shader = new Shader("./shaders/3.3.shader.vs.c", "./shaders/3.3.shader.fs.c");
	shader->use();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(~0);
	glLineWidth(1.5f);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * graph.size(), &(graph[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &(indices[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	

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

	//text Settings
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initText(SCR_WIDTH, SCR_HEIGHT);
	
	if (ifMemView == TRUE) {
		camera->BudgeCamera(0, 0.5f);
		ifMemView = FALSE;
	}
	maxTime = -1;
	shouldEnd = FALSE;
	firstTimeCall = TRUE;
	firtTimeMemCall = TRUE;
	firstTimePlotNew = FALSE;
	plot_refresh();
}

template<class T>
void Analyzer<T>::plot_refresh()
{
	shader->use();
	static int allow_M = TRUE;
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
		if (ifMemView == FALSE) {
			camera->BudgeCamera(0, 0.5f);
		}
		else{
			camera->BudgeCamera(0, 0.7f, 177, 3.0f + 0.5f, 0.0f, -0.5f, -180.0f);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		if (allow_M == TRUE) {
			if (ifMemView == FALSE) {
				if (firtTimeMemCall == TRUE)
				{
					printMem();
					plot_mem();
					firtTimeMemCall = FALSE;
				}
				camera->BudgeCamera(0, 0.7, 177, 3.0f + 0.5f, 0.0f, -0.5f, -180.0f);
				ifMemView = TRUE;
				printf("\rCurrently In Memory Cost View.  ");
			}
			else {
				camera->BudgeCamera(0, 0.5);
				ifMemView = FALSE;
				printf("\rCurrently In Time Cost View.    ");
			}
			allow_M = FALSE;
		}
	}
	else{
		allow_M = TRUE;
	}
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearColor(0.15f, 0.15f, 0.16f, 1.0f);
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

	if (ifMemView == TRUE) {
		glBindVertexArray(VAOM);
		glDrawElements(GL_LINE_STRIP, Memindices.size(), GL_UNSIGNED_INT, 0);
		//printf("hi %d  ", Memindices.size());
	}

	plot_name();

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
	delete shader;
	shader = NULL;
	TimeCutOff = 2;
	if (ifMemView == TRUE) {
		camera->BudgeCamera(0.0f, 0.5f);
		ifMemView = FALSE;
	}
	firtTimeMemCall = TRUE;
	if (textShader != NULL) {
		delete textShader;
	}
	textShader = NULL;
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
	Vertex vtx = { glm::vec3(originx, originy, 0.0f) , glm::vec3(1.0f, 1.0f, 0.0f) };
	graph.push_back(vtx);
	//y & arrows  vbo used:1-3
	addvertex(glm::vec3(originx, originy + 1.8f, 0.0f),                         glm::vec3(1.0f, 0.0f, 0.0f)); addLineIdx(0, 1);
	addvertex(glm::vec3(originx - arrowSize, originy + 1.8f - arrowSize, 0.0f), glm::vec3(1.0f, 0.0f, 0.1f)); addLineIdx(1, 2);
	addvertex(glm::vec3(originx + arrowSize, originy + 1.8f - arrowSize, 0.0f), glm::vec3(1.0f, 0.0f, 0.1f)); addLineIdx(1, 3);
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

//timer callbacks
//template<class T>
//VOID CALLBACK Analyzer<T>::refreshGraph(PVOID lpParam, BOOLEAN TimerOrWaitFired)
//{
//	if (lpParam != NULL) {
//		plot_request();
//	}
//}
template<class T>
VOID CALLBACK Analyzer<T>::getMemInfo(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	TimerCallBackPara *para = (TimerCallBackPara*)lpParam;
	//if(TimerOrWaitFired == )
	if (lpParam != NULL) {
		PROCESS_MEMORY_COUNTERS memCounter;
		GetProcessMemoryInfo(para->process, &memCounter, sizeof(memCounter));
		if (funinfo[para->k].maxMem < memCounter.WorkingSetSize) //+ memCounter.PagefileUsage)
		{
			funinfo[para->k].maxMem = memCounter.WorkingSetSize; //+ memCounter.PagefileUsage;
		}
		//if(funinfo[para->k].iniMem > memCounter.WorkingSetSize + memCounter.PagefileUsage)
		//{
		//	funinfo[para->k].iniMem = memCounter.WorkingSetSize + memCounter.PagefileUsage;
		//}
		//printf("k=%d", para->k);
	}
}

template<class T>
void Analyzer<T>::plot_mem()
{
	int i, j, k, len;
	int funsz = funs.size();
	len = funcost.size() / funsz;
	int itr;
	DWORD maxMem = 0;
	GLfloat x, y, z;
	GLfloat range = end - start + 1;
	Vertex vtx;
	GLuint memIdx;
	MemGraph.clear();
	Memindices.clear();
	if (firtTimeMemCall == TRUE) {
		glGenVertexArrays(1, &VAOM);
		glGenBuffers(1, &VBOM);
		glGenBuffers(1, &EBOM);
		glBindVertexArray(VAOM);
		glBindBuffer(GL_ARRAY_BUFFER, VBOM);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MemGraph.size(), &(MemGraph[0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOM);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*Memindices.size(), &(Memindices[0]), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	}
	//z & arrows vbo
	vtx.position = glm::vec3(originx, originy, 0.0f);
	vtx.color = glm::vec3(1.0f, 1.0f, 0.0f);
	MemGraph.push_back(vtx); Memindices.push_back(0);

	vtx.position = glm::vec3(originx, originy, -1.8f);
	vtx.color = glm::vec3(0.1f, 0.4f, 1.0f);
	MemGraph.push_back(vtx); Memindices.push_back(1);

	vtx.position = glm::vec3(originx, originy + arrowSize, -1.8f + arrowSize);
	vtx.color = glm::vec3(0.3f, 0.4f, 0.9f);
	MemGraph.push_back(vtx); Memindices.push_back(2); Memindices.push_back(~0);

	vtx.position = glm::vec3(originx, originy - arrowSize, -1.8f + arrowSize);
	vtx.color = glm::vec3(0.3f, 0.4f, 0.9f);
	MemGraph.push_back(vtx); Memindices.push_back(1); Memindices.push_back(3); Memindices.push_back(~0);

	memIdx = 4;

	for (i = 0, itr = 0; i < funsz; i++) {
		for (j = 0; j < len; j++) {
			DWORD mem = funcost[itr].mem;
			if (mem > maxMem) {
				maxMem = mem;
			}
			itr++;
		}
	}
	if (maxMem == 0UL) {
		maxMem = 1UL;
	}
	for (i = 0, itr = 0; i < funsz; i++) {
		for (j = 0; j < len; j++) {
			x = originx;
			y = static_cast<GLfloat>(double(funcost[j*funsz + i].mem) / double(maxMem) * 1.8f + originy);
			z = - j * 1.8f / static_cast<GLfloat>(len);
			vtx = { glm::vec3(x,y,z), color[i] };
			MemGraph.push_back(vtx);
			Memindices.push_back(memIdx++);
			itr++;
		}
		Memindices.push_back(~0);
	}
	glBindVertexArray(VAOM);
	glBindBuffer(GL_ARRAY_BUFFER, VBOM);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MemGraph.size(), &(MemGraph[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOM);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*Memindices.size(), &(Memindices[0]), GL_STATIC_DRAW);
}

template<class T>
void Analyzer<T>::initText(int src_w, int scr_h)
{
	if (textShader == NULL) {
		textShader = new Shader("./shaders/3.3.text.vs.c", "./shaders/3.3.text.fs.c");
		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(src_w), 0.0f, static_cast<GLfloat>(scr_h));
		textShader->use();
		glUniformMatrix4fv(glGetUniformLocation(textShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&ft))
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		if (FT_New_Face(ft, "./fonts/arial.ttf", 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		// Set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);
		// Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		// Load first 128 characters of ASCII set
		for (GLubyte c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			Characters.insert(std::pair<GLchar, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		// Destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
		glGenVertexArrays(1, &textVAO);
		glGenBuffers(1, &textVBO);
	}

	
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

template<class T>
void  Analyzer<T>::renderText(const char *text, glm::vec3 pos, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state
	textShader->use();
	glUniform3f(glGetUniformLocation(textShader->ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);
	for (const char *s = text; *s != '\0'; s++) {
		Character ch = Characters[*s];
		GLfloat xpos = pos.x + ch.Bearing.x * scale;
		GLfloat ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },

		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		pos.x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

template<class T>
void  Analyzer<T>::plot_name()
{
	int cnt = funs.size();
	for (int i = 0; i < cnt; i++) {
		renderText(funs[i], glm::vec3(20.0f, static_cast<GLfloat>(i*32.0f)+40.0f, 0.0f), 0.5f, color[i]);
	}
}