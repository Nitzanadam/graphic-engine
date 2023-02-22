//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include <windows.h>									// Header File For Windows
#include <stdio.h>										// Header file for standard input output (IE, "FILE") (NEW)
#include <gl\gl.h>										// Header File For The OpenGL32 Library
#include <gl\glu.h>										// Header File For The GLu32 Library
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#pragma comment( lib, "winmm.lib" )                     // Search For WinMM Library While Linking


#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

using namespace cv;
using namespace std;


#define		MAP_SIZE	  1024							// Size Of Our Height Map 
#define		STEP_SIZE_high_res	  10
#define		STEP_SIZE_low_res	  35
#define		HEIGHT_RATIO_high_res  8.0f							// Ratio That The Y Is Scaled According To The X And Z
#define		HEIGHT_RATIO_low_res   ((STEP_SIZE_low_res/STEP_SIZE_high_res) + HEIGHT_RATIO_high_res)
bool high_res;
string fileName = "photo4.JPG";


HDC			hDC = NULL;									// Private GDI Device Context
HGLRC		hRC = NULL;									// Permanent Rendering Context
HWND		hWnd = NULL;									// Holds Our Window Handle
HINSTANCE	hInstance;									// Holds The Instance Of The Application

bool		keys[256];									// Array Used For The Keyboard Routine
bool		active = TRUE;								// Window Active Flag Set To TRUE By Default
bool		fullscreen = TRUE;							// Fullscreen Flag Set To TRUE By Default

int	mouse_x, mouse_y;
vector<vector<vector<float>>> triangles;
vector<vector<vector<float>>> triangles_picked_high_res;
vector<int> picked_low_res_index;

int col_size_x;
int row_size_y;

GLuint      texture[1];                         // Storage For One Texture ( NEW )
Mat img2_gray, img2_color;

float scaleValue = 0.15f;								// Scale Value For The Terrain (NEW)

const float piover180 = 0.0174532925f;
float heading;
float xpos;
float zpos;

GLfloat	yrot;				// Y Rotation
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;
GLfloat	z = 0.0f;				// Depth Into The Screen


#define MAX_PARTICLES 9000
#define RAIN	0
#define SNOW	1
#define	HAIL	2


float slowdown = 2.0;
float velocity = 0.0;
float zoom = -40.0;
float hailsize = 0.3;

int loop;
int fall = -1;

typedef struct {
	// Life
	bool alive;	// is the particle alive?
	float life;	// particle lifespan
	float fade; // decay
	// Position/direction
	float xpos;
	float ypos;
	float zpos;
	// Velocity/Direction, only goes down in y dir
	float vel;
	// Gravity
	float gravity;
}particles;

// Particle System
particles par_sys[MAX_PARTICLES];

void initParticles(int i) {
	par_sys[i].alive = true;
	par_sys[i].life = 1.0;
	par_sys[i].fade = float(rand() % 10) / 1000.0f + 0.003f;

	par_sys[i].xpos = (float)(rand() % col_size_x);
	par_sys[i].ypos = 70.0;
	par_sys[i].zpos = (float)(rand() % row_size_y);

	par_sys[i].vel = velocity;
	par_sys[i].gravity = -2;//-0.8;

}

GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 70.0f, 0.0f, 0.0f, 1.0f };

GLuint	fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
GLuint	fogfilter = -1;								// Which Fog Mode To Use 
GLfloat	fogColor[4] = { 0.5f,0.5f,0.5f,1.0f };		// Fog Color
bool	light;				// Lighting ON/OFF
bool	lp;					// L Pressed?
bool	gp;					// G Pressed? 

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void LoadToVector();
void drawRain();
void drawHail();
void drawSnow();
void drawSphere(double r, int lats, int longs);

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0)										// Prevent A Divide By Zero By
		height = 1;										// Making Height Equal One

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window.  Farthest Distance Changed To 500.0f (NEW)
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 500.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void LoadFile(string fileName) {
	std::string image_path = samples::findFile(fileName);
	Mat img_gray = imread(image_path, IMREAD_GRAYSCALE);
	Mat img_color = imread(image_path, IMREAD_COLOR);
	if (img_gray.empty())
	{
		std::cout << "Could not read the image: " << image_path << std::endl;
		return;
	}
	int col_size = img_gray.cols;
	int row_size = img_gray.rows;
	resize(img_gray, img2_gray, Size(MAP_SIZE, MAP_SIZE), INTER_LINEAR);
	resize(img_color, img2_color, Size(MAP_SIZE, MAP_SIZE), INTER_LINEAR);

	col_size_x = img2_gray.cols;
	row_size_y = img2_gray.rows;
	LoadToVector();
}

void LoadToVector()
{
	float height, height2, height3, height4;
	for (float x = 0; x < (col_size_x - STEP_SIZE_low_res); x += STEP_SIZE_low_res) {
		for (float y = 0; y < (row_size_y - STEP_SIZE_low_res); y += STEP_SIZE_low_res) {
			//first triangle
			height = (img2_gray.at<uchar>(y, x) / 256.0f)*HEIGHT_RATIO_low_res;

			height2 = (img2_gray.at<uchar>(y + STEP_SIZE_low_res, x) / 256.0f)*HEIGHT_RATIO_low_res;

			height3 = (img2_gray.at<uchar>(y, x + STEP_SIZE_low_res) / 256.0f)*HEIGHT_RATIO_low_res;

			triangles.push_back({ {x, height2, y + STEP_SIZE_low_res}, {x + STEP_SIZE_low_res, height3, y},  {x, height, y} });

			//second triangle

			height4 = (img2_gray.at<uchar>(y + STEP_SIZE_low_res, x + STEP_SIZE_low_res) / 256.0f)*HEIGHT_RATIO_low_res;

			triangles.push_back({ {x + STEP_SIZE_low_res, height3, y}, {x, height2, y + STEP_SIZE_low_res}, {x + STEP_SIZE_low_res, height4, y + STEP_SIZE_low_res} });
		}
	}
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	high_res = true;
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	LoadFile(fileName);

	//load texture
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]); // 2d texture (x and y size)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // scale linearly when image smalled than texture


	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image,
	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img2_color.cols, img2_color.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img2_color.data);


	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One


	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);					// We'll Clear To The Color Of The Fog
	//glFogi(GL_FOG_MODE, fogMode[fogfilter]);			// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);					// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.01f);						// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);					// Fog Hint Value
	glFogf(GL_FOG_START, 1.0f);							// Fog Start Depth
	glFogf(GL_FOG_END, 40.0f);							// Fog End Depth
	//glEnable(GL_FOG);									// Enables GL_FOG

	for (loop = 0; loop < MAX_PARTICLES; loop++) {
		initParticles(loop);
	}

	return TRUE;										// Initialization Went OK
}

void RenderHeightMap_high_res()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	float height, height2, height3, height4;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]); // choose the texture to use.

	int STEP_SIZE = STEP_SIZE_high_res;
	int HEIGHT_RATIO = HEIGHT_RATIO_high_res;

	for (float X = 0; X < (col_size_x - STEP_SIZE); X += STEP_SIZE) {
		for (float Y = 0; Y < (row_size_y - STEP_SIZE); Y += STEP_SIZE) {
			glBegin(GL_TRIANGLES);

			//glColor3f(0.0f, 0.0f, 1.0f);

			//first triangle
			height = (img2_gray.at<uchar>(Y, X) / 256.0f)*HEIGHT_RATIO;
			glTexCoord2f((float)X / col_size_x, (float)Y / row_size_y);
			glVertex3i(X, height, Y);

			height2 = (img2_gray.at<uchar>(Y + STEP_SIZE, X) / 256.0f)*HEIGHT_RATIO;
			glTexCoord2f((float)X / col_size_x, (float)(Y + STEP_SIZE) / row_size_y);
			glVertex3i(X, height2, Y + STEP_SIZE);

			height3 = (img2_gray.at<uchar>(Y, X + STEP_SIZE) / 256.0f)*HEIGHT_RATIO;
			glTexCoord2f((float)(X + STEP_SIZE) / col_size_x, (float)Y / row_size_y);
			glVertex3i(X + STEP_SIZE, height3, Y);

			//second triangle
			glTexCoord2f((float)(X + STEP_SIZE) / col_size_x, (float)Y / row_size_y);
			glVertex3i(X + STEP_SIZE, height3, Y);

			glTexCoord2f((float)X / col_size_x, (float)(Y + STEP_SIZE) / row_size_y);
			glVertex3i(X, height2, Y + STEP_SIZE);

			height4 = (img2_gray.at<uchar>(Y + STEP_SIZE, X + STEP_SIZE) / 256.0f)*HEIGHT_RATIO;
			glTexCoord2f((float)(X + STEP_SIZE) / col_size_x, (float)(Y + STEP_SIZE) / row_size_y);
			glVertex3i(X + STEP_SIZE, height4, Y + STEP_SIZE);

			glEnd();


			////lines
			//glBegin(GL_LINES);
			//glColor3f(0.7f, 0.7f, 0.7f);
			////first triangle

			////1->2
			//glVertex3f(X, height, Y); 
			//glVertex3f(X, height2, Y + STEP_SIZE);

			////2->3
			//glVertex3f(X, height2, Y + STEP_SIZE);
			//glVertex3f(X + STEP_SIZE, height3, Y);

			////3->1
			//glVertex3f(X + STEP_SIZE, height3, Y);
			//glVertex3f(X, height, Y);


			//glVertex3f(X, height2, Y + STEP_SIZE);
			//glVertex3f(X + STEP_SIZE, height4, Y + STEP_SIZE);

			//glVertex3f(X + STEP_SIZE, height4, Y + STEP_SIZE);
			//glVertex3f(X + STEP_SIZE, height3, Y);
			//
			//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);          // Reset The Color ////////////////////////3f???????4f???????????
			//glEnd();
		}
	}
	vector<vector<vector<float>>>::iterator iter;
	for (iter = triangles_picked_high_res.begin(); iter != triangles_picked_high_res.end(); ++iter) {
		vector<vector<float>> picked = *iter;

		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 1.0f);
		for (int i = 0; i < 3; i++) {
			vector<float> curr = picked[i];
			float X = curr[0];
			float height = curr[1];
			float Y = curr[2];
			glVertex3i(X, height, Y);
		}
		glColor3f(1.0f, 1.0f, 1.0f);          // Reset The Color
		glEnd();

	}

}

void RenderHeightMap_low_res()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer						
	float height, height2, height3, height4;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]); // choose the texture to use.

	int STEP_SIZE = STEP_SIZE_low_res;
	int HEIGHT_RATIO = HEIGHT_RATIO_low_res;

	for (float X = 0; X < (col_size_x - STEP_SIZE); X += STEP_SIZE) {
		for (float Y = 0; Y < (row_size_y - STEP_SIZE); Y += STEP_SIZE) {
			glBegin(GL_TRIANGLES);

			//glColor3f(0.0f, 0.0f, 1.0f);

			//first triangle
			height = (img2_gray.at<uchar>(Y, X) / 256.0f)*HEIGHT_RATIO;
			glTexCoord2f((float)X / col_size_x, (float)Y / row_size_y);
			glVertex3i(X, height, Y);

			height2 = (img2_gray.at<uchar>(Y + STEP_SIZE, X) / 256.0f)*HEIGHT_RATIO;
			glTexCoord2f((float)X / col_size_x, (float)(Y + STEP_SIZE) / row_size_y);
			glVertex3i(X, height2, Y + STEP_SIZE);

			height3 = (img2_gray.at<uchar>(Y, X + STEP_SIZE) / 256.0f)*HEIGHT_RATIO;
			glTexCoord2f((float)(X + STEP_SIZE) / col_size_x, (float)Y / row_size_y);
			glVertex3i(X + STEP_SIZE, height3, Y);

			//second triangle
			glTexCoord2f((float)(X + STEP_SIZE) / col_size_x, (float)Y / row_size_y);
			glVertex3i(X + STEP_SIZE, height3, Y);

			glTexCoord2f((float)X / col_size_x, (float)(Y + STEP_SIZE) / row_size_y);
			glVertex3i(X, height2, Y + STEP_SIZE);

			height4 = (img2_gray.at<uchar>(Y + STEP_SIZE, X + STEP_SIZE) / 256.0f)*HEIGHT_RATIO;
			glTexCoord2f((float)(X + STEP_SIZE) / col_size_x, (float)(Y + STEP_SIZE) / row_size_y);
			glVertex3i(X + STEP_SIZE, height4, Y + STEP_SIZE);

			glEnd();


			////lines
			//glBegin(GL_LINES);
			//glColor3f(0.7f, 0.7f, 0.7f);
			////first triangle

			////1->2
			//glVertex3f(X, height, Y); 
			//glVertex3f(X, height2, Y + STEP_SIZE);

			////2->3
			//glVertex3f(X, height2, Y + STEP_SIZE);
			//glVertex3f(X + STEP_SIZE, height3, Y);

			////3->1
			//glVertex3f(X + STEP_SIZE, height3, Y);
			//glVertex3f(X, height, Y);


			//glVertex3f(X, height2, Y + STEP_SIZE);
			//glVertex3f(X + STEP_SIZE, height4, Y + STEP_SIZE);

			//glVertex3f(X + STEP_SIZE, height4, Y + STEP_SIZE);
			//glVertex3f(X + STEP_SIZE, height3, Y);
			//
			//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);          // Reset The Color ////////////////////////3f???????4f???????????
			//glEnd();
		}
	}
	vector<int>::iterator iter;
	for (iter = picked_low_res_index.begin(); iter != picked_low_res_index.end(); ++iter) {
		int p = *iter;
		vector<vector<float>> picked = triangles[p];

		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 1.0f);
		for (int i = 0; i < 3; i++) {
			vector<float> curr = picked[i];
			float X = curr[0];
			float height = curr[1];
			float Y = curr[2];
			glVertex3i(X, height, Y);
		}
		glColor3f(1.0f, 1.0f, 1.0f);          // Reset The Color
		glEnd();

	}

}


int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The Matrix

	GLfloat x_m, y_m, z_m, u_m, v_m;
	GLfloat xtrans = -xpos;
	GLfloat ztrans = -zpos;
	GLfloat ytrans = -walkbias - 0.25f;
	GLfloat sceneroty = 360.0f - yrot;

	glTranslatef(xtrans, ytrans, ztrans);
	glRotatef(lookupdown, 1.0f, 0, 0);
	glRotatef(sceneroty, 0, 1.0f, 0);


	// 			 Position	      View		Up Vector
	gluLookAt(212, 55, 194, 186, 55, 171, 0, 1, 0);	// This Determines Where The Camera's Position And View Is

	glScalef(scaleValue, scaleValue * HEIGHT_RATIO_high_res, scaleValue); /////////////////////////////

	if (high_res) {
		RenderHeightMap_high_res();						// Render The Height Map
	}
	else {
		RenderHeightMap_low_res();
	}

	if (fall == RAIN) {
		drawRain();
	}
	else if (fall == HAIL) {
		drawHail();
	}
	else if (fall == SNOW) {
		drawSnow();
	}
	return TRUE;										// Keep Going
}


void selection() {

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	float height, height2, height3, height4;
	int i = 0;
	glBegin(GL_TRIANGLES);
	for (float X = 0; X < (col_size_x - STEP_SIZE_low_res); X += STEP_SIZE_low_res) {
		for (float Y = 0; Y < (row_size_y - STEP_SIZE_low_res); Y += STEP_SIZE_low_res) {

			int r1 = (i & 0x000000FF) >> 0;
			int g1 = (i & 0x0000FF00) >> 8;
			int b1 = (i & 0x00FF0000) >> 16;

			glColor3f(r1 / 255.0f, g1 / 255.0f, b1 / 255.0f);

			//first triangle
			height = (img2_gray.at<uchar>(Y, X) / 256.0f)*HEIGHT_RATIO_low_res;
			glVertex3i(X, height, Y);

			height2 = (img2_gray.at<uchar>(Y + STEP_SIZE_low_res, X) / 256.0f)*HEIGHT_RATIO_low_res;
			glVertex3i(X, height2, Y + STEP_SIZE_low_res);

			height3 = (img2_gray.at<uchar>(Y, X + STEP_SIZE_low_res) / 256.0f)*HEIGHT_RATIO_low_res;
			glVertex3i(X + STEP_SIZE_low_res, height3, Y);


			i = i + 1;

			//second triangle

			int r2 = (i & 0x000000FF) >> 0;
			int g2 = (i & 0x0000FF00) >> 8;
			int b2 = (i & 0x00FF0000) >> 16;


			glColor3f(r2 / 255.0f, g2 / 255.0f, b2 / 255.0f);

			height4 = (img2_gray.at<uchar>(Y + STEP_SIZE_low_res, X + STEP_SIZE_low_res) / 256.0f)*HEIGHT_RATIO_low_res;
			glVertex3i(X + STEP_SIZE_low_res, height4, Y + STEP_SIZE_low_res);

			glVertex3i(X + STEP_SIZE_low_res, height3, Y);

			glVertex3i(X, height2, Y + STEP_SIZE_low_res);


			i = i + 1;

		}
	}
	glEnd();



	// The Size Of The Viewport. [0] Is <x>, [1] Is <y>, [2] Is <length>, [3] Is <width>
	GLint	viewport[4];

	// This Sets The Array <viewport> To The Size And Location Of The Screen Relative To The Window
	glGetIntegerv(GL_VIEWPORT, viewport);


	unsigned char data[4];


	cv::Mat img(480, 640, CV_8UC3);
	glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);
	glPixelStorei(GL_PACK_ROW_LENGTH, img.step / img.elemSize());
	glReadPixels(0, 0, img.cols, img.rows, GL_BGR_EXT, GL_UNSIGNED_BYTE, img.data);
	cv::Mat flipped(img);
	cv::flip(img, flipped, 0);
	cv::imwrite("snapshot.png", img);


	glReadPixels(mouse_x, viewport[3] - mouse_y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// Convert the color back to an integer ID
	int pickedID =
		data[0] +
		data[1] * 256 +
		data[2] * 256 * 256;

	if (pickedID == 0x00ffffff) { // Full white, must be the background !
		MessageBox(NULL, "chosen background", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	}
	else {
		float height, height2, height3;
		/*string t = to_string(pickedID);
		char const *n_char = t.c_str();
		MessageBox(NULL, n_char, "picked id", MB_OK | MB_ICONEXCLAMATION);*/
		picked_low_res_index.push_back(pickedID);
		vector<vector<float>> picked = triangles[pickedID];
		vector<float> p1 = picked[0];
		vector<float> p2 = picked[1];
		vector<float> p3 = picked[2];
		bool up;
		if (p3[2] > p1[2]) {
			up = true;
		}
		else if (p3[2] < p1[2]) {
			up = false;
		}
		float x2 = p2[0];
		float x3 = p3[0];
		float y1 = p1[2];
		float y3 = p3[2];
		vector<float> curr_x;
		vector<float> curr_y;

		if (up) {
			int j = 0;
			for (int i = 0; i <= x3; i = i + STEP_SIZE_high_res) {
				if (i >= x2) {
					curr_x.push_back(i);
					j++;
				}
			}
			int mid = j / 2;
			float new_x = curr_x[mid + 1];
			int k = 0;
			for (int i = 0; i <= y3; i = i + STEP_SIZE_high_res) {
				if (i >= y1) {
					curr_y.push_back(i);
					k++;
				}
			}
			mid = k / 2;
			float new_y = curr_y[mid + 1];
			height = (img2_gray.at<uchar>(new_y, new_x) / 256.0f)*HEIGHT_RATIO_high_res;

			height2 = (img2_gray.at<uchar>(new_y - STEP_SIZE_high_res, new_x) / 256.0f)*HEIGHT_RATIO_high_res;

			height3 = (img2_gray.at<uchar>(new_y, new_x - STEP_SIZE_high_res) / 256.0f)*HEIGHT_RATIO_high_res;

			triangles_picked_high_res.push_back({ {new_x, height2, new_y - STEP_SIZE_high_res}, {new_x - STEP_SIZE_high_res, height3, new_y},  {new_x, height, new_y} });

		}
		else {
			int j = 0;
			for (int i = 0; i <= x2; i = i + STEP_SIZE_high_res) {
				if (i >= x3) {
					curr_x.push_back(i);
					j++;
				}
			}
			int mid = j / 2;
			float new_x = curr_x[mid - 1];
			int k = 0;
			for (int i = 0; i <= y1; i = i + STEP_SIZE_high_res) {
				if (i >= y3) {
					curr_y.push_back(i);
					k++;
				}
			}
			mid = k / 2;
			float new_y = curr_y[mid - 1];
			height = (img2_gray.at<uchar>(new_y, new_x) / 256.0f)*HEIGHT_RATIO_high_res;

			height2 = (img2_gray.at<uchar>(new_y + STEP_SIZE_high_res, new_x) / 256.0f)*HEIGHT_RATIO_high_res;

			height3 = (img2_gray.at<uchar>(new_y, new_x + STEP_SIZE_high_res) / 256.0f)*HEIGHT_RATIO_high_res;

			triangles_picked_high_res.push_back({ {new_x, height2, new_y + STEP_SIZE_high_res}, {new_x + STEP_SIZE_high_res, height3, new_y},  {new_x, height, new_y} });

		}


	}
}

// For Rain
void drawRain() {
	glDisable(GL_TEXTURE_2D);
	float x, y, z;
	for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
		if (par_sys[loop].alive == true) {
			x = par_sys[loop].xpos;
			y = par_sys[loop].ypos;
			z = par_sys[loop].zpos;

			// Draw particles
			glColor3f(0.9, 0.9, 0.9);
			glBegin(GL_LINES);
			glVertex3f(x, y, z);
			glVertex3f(x, y + 0.5, z);
			glEnd();

			// Update values
			//Move
			// Adjust slowdown for speed!
			par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 1000);
			par_sys[loop].vel += par_sys[loop].gravity;
			// Decay
			par_sys[loop].life -= par_sys[loop].fade;

			if (par_sys[loop].ypos <= -10) {
				par_sys[loop].life = -1.0;
			}
			//Revive
			if (par_sys[loop].life < 0.0) {
				initParticles(loop);
			}
		}
	}
}


void drawHail() {
	float x, y, z;
	glDisable(GL_TEXTURE_2D);

	for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
		if (par_sys[loop].alive == true) {
			x = par_sys[loop].xpos;
			y = par_sys[loop].ypos;
			z = par_sys[loop].zpos;

			// Draw particles
			glColor3f(0.9, 0.9, 0.9);
			glBegin(GL_QUADS);
			// Front
			glVertex3f(x - hailsize, y - hailsize, z + hailsize); // lower left
			glVertex3f(x - hailsize, y + hailsize, z + hailsize); // upper left
			glVertex3f(x + hailsize, y + hailsize, z + hailsize); // upper right
			glVertex3f(x + hailsize, y - hailsize, z + hailsize); // lower left
			//Left
			glVertex3f(x - hailsize, y - hailsize, z + hailsize);
			glVertex3f(x - hailsize, y - hailsize, z - hailsize);
			glVertex3f(x - hailsize, y + hailsize, z - hailsize);
			glVertex3f(x - hailsize, y + hailsize, z + hailsize);
			// Back
			glVertex3f(x - hailsize, y - hailsize, z - hailsize);
			glVertex3f(x - hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z - hailsize);
			//Right
			glVertex3f(x + hailsize, y + hailsize, z + hailsize);
			glVertex3f(x + hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z + hailsize);
			//Top
			glVertex3f(x - hailsize, y + hailsize, z + hailsize);
			glVertex3f(x - hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y + hailsize, z + hailsize);
			//Bottom
			glVertex3f(x - hailsize, y - hailsize, z + hailsize);
			glVertex3f(x - hailsize, y - hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z + hailsize);
			glEnd();

			// Update values
			//Move
			if (par_sys[loop].ypos <= -10) {
				par_sys[loop].vel = par_sys[loop].vel * -1.0;
			}
			par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 1000); // * 1000
			par_sys[loop].vel += par_sys[loop].gravity;

			// Decay
			par_sys[loop].life -= par_sys[loop].fade;

			//Revive
			if (par_sys[loop].life < 0.0) {
				initParticles(loop);
			}
		}
	}
}


// For Snow
void drawSnow() {
	float x, y, z;
	glDisable(GL_TEXTURE_2D);
	for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
		if (par_sys[loop].alive == true) {
			x = par_sys[loop].xpos;
			y = par_sys[loop].ypos;
			z = par_sys[loop].zpos;

			// Draw particles
			glColor3f(0.9, 0.9, 0.9);
			glPushMatrix();
			glTranslatef(x, y, z);
			drawSphere(0.05, 5, 5);
			glPopMatrix();

			// Update values
			//Move
			par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 1000);
			par_sys[loop].vel += par_sys[loop].gravity;
			// Decay
			par_sys[loop].life -= par_sys[loop].fade;

			//Revive
			if (par_sys[loop].life < 0.0) {
				initParticles(loop);
			}
		}
	}
}

float M_PI = 3.14;

void drawSphere(double r, int lats, int longs) {
	int i, j;
	for (i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = M_PI * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);

		}
		glEnd();

	}

}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;							// Holds The Results After Searching For A Match
	WNDCLASS	wc;										// Windows Class Structure
	DWORD		dwExStyle;								// Window Extended Style
	DWORD		dwStyle;								// Window Style
	RECT		WindowRect;								// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;							// Set Left Value To 0
	WindowRect.right = (long)width;						// Set Right Value To Requested Width
	WindowRect.top = (long)0;								// Set Top Value To 0
	WindowRect.bottom = (long)height;						// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;							// Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);		// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;			// WndProc Handles Messages
	wc.cbClsExtra = 0;							// No Extra Window Data
	wc.cbWndExtra = 0;							// No Extra Window Data
	wc.hInstance = hInstance;					// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);	// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// Load The Arrow Pointer
	wc.hbrBackground = NULL;							// No Background Required For GL
	wc.lpszMenuName = NULL;							// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";						// Set The Class Name

	if (!RegisterClass(&wc))							// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if (fullscreen)										// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;						// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);	// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;		// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;		// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;			// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;						// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;							// Return FALSE
			}
		}
	}

	if (fullscreen)										// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;						// Window Extended Style
		dwStyle = WS_POPUP;								// Windows Style
		ShowCursor(FALSE);								// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;					// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,				// Extended Style For The Window
		"OpenGL",				// Class Name
		title,					// Window Title
		dwStyle |				// Defined Window Style
		WS_CLIPSIBLINGS |		// Required Window Style
		WS_CLIPCHILDREN,		// Required Window Style
		0, 0,					// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,					// No Parent Window
		NULL,					// No Menu
		hInstance,				// Instance
		NULL)))					// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();									// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =					// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		1,												// Version Number
		PFD_DRAW_TO_WINDOW |							// Format Must Support Window
		PFD_SUPPORT_OPENGL |							// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,								// Must Support Double Buffering
		PFD_TYPE_RGBA,									// Request An RGBA Format
		bits,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,								// Color Bits Ignored
		0,												// No Alpha Buffer
		0,												// Shift Bit Ignored
		0,												// No Accumulation Buffer
		0, 0, 0, 0,										// Accumulation Bits Ignored
		16,												// 16Bit Z-Buffer (Depth Buffer)  
		0,												// No Stencil Buffer
		0,												// No Auxiliary Buffer
		PFD_MAIN_PLANE,									// Main Drawing Layer
		0,												// Reserved
		0, 0, 0											// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))								// Did We Get A Device Context?
	{
		KillGLWindow();									// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))		// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();									// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))			// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();									// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))					// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();									// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))						// Try To Activate The Rendering Context
	{
		KillGLWindow();									// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);							// Show The Window
	SetForegroundWindow(hWnd);							// Slightly Higher Priority
	SetFocus(hWnd);										// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);						// Set Up Our Perspective GL Screen

	if (!InitGL())										// Initialize Our Newly Created GL Window
	{
		KillGLWindow();									// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	return TRUE;										// Success
}

LRESULT CALLBACK WndProc(HWND	hWnd,				// Handle For This Window
	UINT	uMsg,				// Message For This Window
	WPARAM	wParam,				// Additional Message Information
	LPARAM	lParam)				// Additional Message Information
{
	switch (uMsg)										// Check For Windows Messages
	{
	case WM_ACTIVATE:								// Watch For Window Activate Message
	{
		if (!HIWORD(wParam))						// Check Minimization State
		{
			active = TRUE;							// Program Is Active
		}
		else
		{
			active = FALSE;							// Program Is No Longer Active
		}

		return 0;									// Return To The Message Loop
	}

	case WM_SYSCOMMAND:								// Intercept System Commands
	{
		switch (wParam)								// Check System Calls
		{
		case SC_SCREENSAVE:						// Screensaver Trying To Start?
		case SC_MONITORPOWER:					// Monitor Trying To Enter Powersave?
			return 0;								// Prevent From Happening
		}
		break;										// Exit
	}

	case WM_CLOSE:									// Did We Receive A Close Message?
	{
		PostQuitMessage(0);							// Send A Quit Message
		return 0;									// Jump Back
	}

	case WM_LBUTTONDOWN:
	{
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		if (high_res) {
			MessageBox(NULL, "Can't pick at high res", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		}
		else {
			selection();
		}
		return 0;
	}
	break;

	case WM_MOUSEMOVE:
	{
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		return 0;
	}

	case WM_KEYDOWN:								// Is A Key Being Held Down?
	{
		keys[wParam] = TRUE;						// If So, Mark It As TRUE
		return 0;									// Jump Back
	}

	case WM_KEYUP:									// Has A Key Been Released?
	{
		keys[wParam] = FALSE;						// If So, Mark It As FALSE
		return 0;									// Jump Back
	}

	case WM_SIZE:									// Resize The OpenGL Window
	{
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;									// Jump Back
	}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,				// Instance
	HINSTANCE	hPrevInstance,			// Previous Instance
	LPSTR		lpCmdLine,				// Command Line Parameters
	int			nCmdShow)				// Window Show State
{
	MSG		msg;										// Windows Message Structure
	BOOL	done = FALSE;									// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		fullscreen = FALSE;								// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow((char*)"Mini Project", 640, 480, 16, fullscreen))
	{
		return 0;										// Quit If Window Was Not Created
	}
	while (!done)										// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))		// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)					// Have We Received A Quit Message?
			{
				done = TRUE;								// If So done=TRUE
			}
			else										// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);					// Translate The Message
				DispatchMessage(&msg);					// Dispatch The Message
			}
		}
		else {											// If There Are No Messages

		// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done = TRUE;								// ESC or DrawGLScene Signalled A Quit
			}
			else if (active)							// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);						// Swap Buffers (Double Buffering)
				if (keys[VK_UP]) //UP ARROW key
				{
					scaleValue += 0.001f;					// Increase the scale value to zoom in
					xpos -= (float)sin(heading*piover180) * 0.05f;
					zpos -= (float)cos(heading*piover180) * 0.05f;
					if (walkbiasangle >= 359.0f)
					{
						walkbiasangle = 0.0f;
					}
					else
					{
						walkbiasangle += 10;
					}
					walkbias = (float)sin(walkbiasangle * piover180) / 20.0f;
				}

				if (keys[VK_DOWN]) //DOWN ARROW key
				{
					scaleValue -= 0.001f;					// Decrease the scale value to zoom out
					xpos += (float)sin(heading*piover180) * 0.05f;
					zpos += (float)cos(heading*piover180) * 0.05f;
					if (walkbiasangle <= 1.0f)
					{
						walkbiasangle = 359.0f;
					}
					else
					{
						walkbiasangle -= 10;
					}
					walkbias = (float)sin(walkbiasangle * piover180) / 20.0f;
				}

				if (keys[VK_RIGHT]) //RIGHT ARROW key
				{
					heading -= 0.5f;
					yrot = heading;
				}

				if (keys[VK_LEFT]) //LEFT ARROW key
				{
					heading += 0.5f;
					yrot = heading;
				}

				if (keys[VK_PRIOR]) //PAGE UP key
				{
					lookupdown -= 0.5f;
				}

				if (keys[VK_NEXT]) //PAGE DOWN key
				{
					lookupdown += 0.5f;
				}
				if (keys[VK_SPACE])
				{
					high_res = false;
					DrawGLScene();
				}
				if (keys[VK_RETURN])
				{
					high_res = true;
					DrawGLScene();
				}
				if (keys['G'] && !gp)
				{
					gp = TRUE;
					glEnable(GL_FOG);
					fogfilter += 1;
					if (fogfilter > 2)
					{
						fogfilter = -1;
						glDisable(GL_FOG);
					}
					else
						glFogi(GL_FOG_MODE, fogMode[fogfilter]);	// Fog Mode
				}
				if (!keys['G'])
				{
					gp = FALSE;
				}
				if (keys['L'] && !lp)
				{
					lp = TRUE;
					light = !light;
					if (!light)
					{

						glDisable(GL_LIGHTING);
					}
					else
					{
						glEnable(GL_LIGHT1);
						glEnable(GL_LIGHTING);
					}
				}
				if (!keys['L'])
				{
					lp = FALSE;
				}
				if (keys['R']) { // Rain
					fall = RAIN;
					PlaySound("rain-01.wav", NULL, SND_ASYNC);

				}
				if (keys['H']) { // Hail
					fall = HAIL;
					PlaySound(NULL, NULL, NULL);
				}
				if (keys['S']) { // Snow
					fall = SNOW;
					PlaySound(NULL, NULL, NULL);
				}
				if (keys['Z']) { // restart
					fall = -1;
					PlaySound(NULL, NULL, NULL);
				}
				if (keys[VK_OEM_PLUS]) { //'+' make hail bigger
					hailsize += 0.01;
				}
				if (keys[VK_OEM_MINUS]) { //'-' make hail smaller
					if (hailsize > 0.1)
						hailsize -= 0.01;
				}
				if (keys[VK_OEM_COMMA]) { // '<' slow down fall
					if (slowdown < 4.0)
						slowdown += 0.01;
				}
				if (keys[VK_OEM_PERIOD]) { // '>' speed up fall
					if (slowdown > 1.0)
						slowdown -= 0.01;
				}

			}

			if (keys[VK_F1])							// Is F1 Being Pressed?
			{
				keys[VK_F1] = FALSE;						// If So Make Key FALSE
				KillGLWindow();							// Kill Our Current Window
				fullscreen = !fullscreen;					// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow((char*)"Mini Project", 640, 480, 16, fullscreen))
				{
					return 0;							// Quit If Window Was Not Created
				}
			}
		}
	}
	// Shutdown
	KillGLWindow();										// Kill The Window
	return (msg.wParam);								// Exit The Program
}