//#include <windows.h>											// Header File For Windows
//#include <stdio.h>												// Header File For Standard Input / Output
//#include <stdarg.h>												// Header File For Variable Argument Routines
//#include <gl\gl.h>												// Header File For The OpenGL32 Library
//#include <gl\glu.h>												// Header File For The GLu32 Library
//#include <time.h>												// For Random Seed
//#include "NeHeGL.h"												// Header File For NeHeGL
//#include <iostream>
//#include <vector>
//#include <opencv2/core.hpp>
//#include<opencv2/opencv.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
//
//#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
//#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking
//#pragma comment( lib, "winmm.lib" )								// Search For WinMM Library While Linking
//
//using namespace cv;
//using namespace std;
//
//#ifndef		CDS_FULLSCREEN										// CDS_FULLSCREEN Is Not Defined By Some
//#define		CDS_FULLSCREEN 4									// Compilers. By Defining It This Way,
//#endif															// We Can Avoid Errors
//
//
//GL_Window*	g_window;
//Keys*		g_keys;
//
//
//#ifndef GL_BGR
//#define GL_BGR 0x80E0
//#endif
//
//#define		MAP_SIZE	  1024							// Size Of Our .RAW Height Map (NEW)
//#define		STEP_SIZE	  10							// Width And Height Of Each Quad (NEW)
//#define		HEIGHT_RATIO  8.0f							// Ratio That The Y Is Scaled According To The X And Z (NEW)
//string fileName = "photo1.JPG";
//
//
//HDC			hDC = NULL;									// Private GDI Device Context
//HGLRC		hRC = NULL;									// Permanent Rendering Context
//HWND		hWnd = NULL;									// Holds Our Window Handle
//HINSTANCE	hInstance;									// Holds The Instance Of The Application
//
//bool		keys[256];									// Array Used For The Keyboard Routine
//bool		active = TRUE;								// Window Active Flag Set To TRUE By Default
//bool		fullscreen = TRUE;							// Fullscreen Flag Set To TRUE By Default
//bool		bRender = TRUE;								// Polygon Flag Set To TRUE By Default (NEW)
//
//int col_size_x;
//int row_size_y;
//
//GLuint      texture[1];                         // Storage For One Texture ( NEW )
//Mat img2_gray, img2_color;
//
//float scaleValue = 0.15f;								// Scale Value For The Terrain (NEW)
//
//const float piover180 = 0.0174532925f;
//float heading;
//float xpos;
//float zpos;
//
//GLfloat	yrot;				// Y Rotation
//GLfloat walkbias = 0;
//GLfloat walkbiasangle = 0;
//GLfloat lookupdown = 0.0f;
//GLfloat	z = 0.0f;				// Depth Into The Screen
//
//bool LoadFile(string fileName) {
//	std::string image_path = samples::findFile(fileName);
//	Mat img_gray = imread(image_path, IMREAD_GRAYSCALE);
//	Mat img_color = imread(image_path, IMREAD_COLOR);
//	if (img_gray.empty())
//	{
//		std::cout << "Could not read the image: " << image_path << std::endl;
//		return false;
//	}
//	//imshow("Display window", img);
//	//Mat scaled_f_up, scaled_f_down;
//	//Mat img2_gray, img2_color;
//	int col_size = img_gray.cols;
//	int row_size = img_gray.rows;
//	// ratio =  1000 / row_size;
//	//cout << ratio << endl;
//	resize(img_gray, img2_gray, Size(MAP_SIZE, MAP_SIZE), INTER_LINEAR);
//	resize(img_color, img2_color, Size(MAP_SIZE, MAP_SIZE), INTER_LINEAR);
//
//	col_size_x = img2_gray.cols;
//	row_size_y = img2_gray.rows;
//
//	glGenTextures(1, &texture[0]);
//	glBindTexture(GL_TEXTURE_2D, texture[0]); // 2d texture (x and y size)
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // scale linearly when image bigger than texture
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // scale linearly when image smalled than texture
//
//
//	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image,
//	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
//	glTexImage2D(GL_TEXTURE_2D, 0, 3, img2_color.cols, img2_color.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img2_color.data);
//
//	return true;
//}
//
//BOOL Initialize(GL_Window* window, Keys* keys)					// Any OpenGL Initialization Goes Here
//{
//	g_window = window;
//	g_keys = keys;
//
//	srand((unsigned)time(NULL));							// Randomize Things
//
//	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
//	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
//	glClearDepth(1.0f);									// Depth Buffer Setup
//	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
//	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
//	glEnable(GL_TEXTURE_2D);
//	
//	if (!LoadFile(fileName)) {
//		return false;
//	}
//
//	return TRUE;												// Return TRUE (Initialization Successful)
//}
//
//void Selection(void)											// This Is Where Selection Is Done
//{
//	GLuint	buffer[512];										// Set Up A Selection Buffer
//	GLint	hits;												// The Number Of Objects That We Selected
//
//
//	// The Size Of The Viewport. [0] Is <x>, [1] Is <y>, [2] Is <length>, [3] Is <width>
//	GLint	viewport[4];
//
//	// This Sets The Array <viewport> To The Size And Location Of The Screen Relative To The Window
//	glGetIntegerv(GL_VIEWPORT, viewport);
//	glSelectBuffer(512, buffer);								// Tell OpenGL To Use Our Array For Selection
//
//	// Puts OpenGL In Selection Mode. Nothing Will Be Drawn.  Object ID's and Extents Are Stored In The Buffer.
//	(void)glRenderMode(GL_SELECT);
//
//	glInitNames();												// Initializes The Name Stack
//	glPushName(0);												// Push 0 (At Least One Entry) Onto The Stack
//
//	glMatrixMode(GL_PROJECTION);								// Selects The Projection Matrix
//	glPushMatrix();												// Push The Projection Matrix
//	glLoadIdentity();											// Resets The Matrix
//
//	// This Creates A Matrix That Will Zoom Up To A Small Portion Of The Screen, Where The Mouse Is.
//	gluPickMatrix((GLdouble)mouse_x, (GLdouble)(viewport[3] - mouse_y), 1.0f, 1.0f, viewport);
//
//	// Apply The Perspective Matrix
//	gluPerspective(45.0f, (GLfloat)(viewport[2] - viewport[0]) / (GLfloat)(viewport[3] - viewport[1]), 0.1f, 100.0f);
//	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
//	//DrawTargets();												// Render The Targets To The Selection Buffer
//	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
//	glPopMatrix();												// Pop The Projection Matrix
//	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
//	hits = glRenderMode(GL_RENDER);								// Switch To Render Mode, Find Out How Many
//																// Objects Were Drawn Where The Mouse Was
//	if (hits > 0)												// If There Were More Than 0 Hits
//	{
//		int	choose = buffer[3];									// Make Our Selection The First Object
//		int depth = buffer[1];									// Store How Far Away It Is 
//
//		for (int loop = 1; loop < hits; loop++)					// Loop Through All The Detected Hits
//		{
//			// If This Object Is Closer To Us Than The One We Have Selected
//			if (buffer[loop * 4 + 1] < GLuint(depth))
//			{
//				choose = buffer[loop * 4 + 3];						// Select The Closer Object
//				depth = buffer[loop * 4 + 1];						// Store How Far Away It Is
//			}
//		}
//
//	}
//}
//
//void RenderHeightMap()					// This Renders The Height Map As Quads
//{
//
//	glBindTexture(GL_TEXTURE_2D, texture[0]); // choose the texture to use.
//
//	glBegin(GL_TRIANGLES);
//	for (int X = 0; X < (col_size_x - STEP_SIZE); X += STEP_SIZE)
//		for (int Y = 0; Y < (row_size_y - STEP_SIZE); Y += STEP_SIZE)
//		{
//			//first triangle
//			float height = (img2_gray.at<uchar>(Y, X) / 256.0f)*HEIGHT_RATIO;
//			glTexCoord2f((float)X / col_size_x, (float)Y / row_size_y);
//			glVertex3i(X, height, Y);						// Send This Vertex To OpenGL To Be Rendered (Integer Points Are Faster)
//
//
//			float height2 = (img2_gray.at<uchar>(Y + STEP_SIZE, X) / 256.0f)*HEIGHT_RATIO;
//			glTexCoord2f((float)X / col_size_x, (float)(Y + STEP_SIZE) / row_size_y);
//			glVertex3i(X, height2, Y + STEP_SIZE);
//
//			float height3 = (img2_gray.at<uchar>(Y, X + STEP_SIZE) / 256.0f)*HEIGHT_RATIO;
//			glTexCoord2f((float)(X + STEP_SIZE) / col_size_x, (float)Y / row_size_y);
//			glVertex3i(X + STEP_SIZE, height3, Y);
//
//
//			//second triangle
//			glTexCoord2f((float)(X + STEP_SIZE) / col_size_x, (float)Y / row_size_y);
//			glVertex3i(X + STEP_SIZE, height3, Y);
//
//			glTexCoord2f((float)X / col_size_x, (float)(Y + STEP_SIZE) / row_size_y);
//			glVertex3i(X, height2, Y + STEP_SIZE);
//
//			float height4 = (img2_gray.at<uchar>(Y + STEP_SIZE, X + STEP_SIZE) / 256.0f)*HEIGHT_RATIO;
//			glTexCoord2f((float)(X + STEP_SIZE) / col_size_x, (float)(Y + STEP_SIZE) / row_size_y);
//			glVertex3i(X + STEP_SIZE, height4, Y + STEP_SIZE);
//
//
//		}
//	glEnd();
//
//}
//
//void DrawScene(GLvoid)									// Here's Where We Do All The Drawing
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
//	glLoadIdentity();									// Reset The Matrix
//
//	GLfloat x_m, y_m, z_m, u_m, v_m;
//	GLfloat xtrans = -xpos;
//	GLfloat ztrans = -zpos;
//	GLfloat ytrans = -walkbias - 0.25f;
//	GLfloat sceneroty = 360.0f - yrot;
//
//	glRotatef(lookupdown, 1.0f, 0, 0);
//	glRotatef(sceneroty, 0, 1.0f, 0);
//
//
//	// 			 Position	      View		Up Vector
//	gluLookAt(212, 55, 194, 186, 55, 171, 0, 1, 0);	// This Determines Where The Camera's Position And View Is
//
//	glScalef(scaleValue, scaleValue * HEIGHT_RATIO, scaleValue);
//
//	//cam.Control();
//	//cam.UpdateCamera();
//
//	RenderHeightMap();						// Render The Height Map
//
//}
//
