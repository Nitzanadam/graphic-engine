//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//#include <windows.h>									// Header File For Windows
//#include <stdio.h>										// Header file for standard input output (IE, "FILE") (NEW)
//#include <gl\gl.h>										// Header File For The OpenGL32 Library
//#include <gl\glu.h>										// Header File For The GLu32 Library
////#include "SDL.h"
////#include <gl\glaux.h>									// Header File For The Glaux Library
////#pragma warning(disable:4996)
////#pragma comment(lib, "opengl32.lib")					// Link OpenGL32.lib
////#pragma comment(lib, "glu32.lib")						// Link Glu32.lib
//#include <iostream>
//#include <vector>
//#include <opencv2/core.hpp>
//#include<opencv2/opencv.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
////#include "camera.h"
////#include "triangle.cpp"
//
//
//
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/quaternion.hpp"
//#include "glm/gtx/quaternion.hpp"
//using namespace glm;
//
//#ifndef GL_BGR
//#define GL_BGR 0x80E0
//#endif
//
//using namespace cv;
//using namespace std;
//
//#define		MAP_SIZE	  1024							// Size Of Our Height Map 
//#define		STEP_SIZE	  10							
//#define		HEIGHT_RATIO  10.0f							// Ratio That The Y Is Scaled According To The X And Z
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
//vector<vector<vector<float>>> triangels;
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
//LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
//
//GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
//{
//	if (height == 0)										// Prevent A Divide By Zero By
//		height = 1;										// Making Height Equal One
//
//	glViewport(0, 0, width, height);						// Reset The Current Viewport
//
//	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
//	glLoadIdentity();									// Reset The Projection Matrix
//
//	// Calculate The Aspect Ratio Of The Window.  Farthest Distance Changed To 500.0f (NEW)
//	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 500.0f);
//
//	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
//	glLoadIdentity();									// Reset The Modelview Matrix
//}
//
//void LoadFile(string fileName) {
//	std::string image_path = samples::findFile(fileName);
//	Mat img_gray = imread(image_path, IMREAD_GRAYSCALE);
//	Mat img_color = imread(image_path, IMREAD_COLOR);
//	if (img_gray.empty())
//	{
//		std::cout << "Could not read the image: " << image_path << std::endl;
//		return;
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
//}
//
//
//
//int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
//{
//	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
//	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
//	glClearDepth(1.0f);									// Depth Buffer Setup
//	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
//	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
//	glEnable(GL_TEXTURE_2D);
//
//	LoadFile(fileName);
//
//
//
//	//load texture
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
//	return TRUE;										// Initialization Went OK
//}
//
//
//
//void RenderHeightMap()					// This Renders The Height Map As Quads
//{
//	float height, height2, height3, height4;
//	glBindTexture(GL_TEXTURE_2D, texture[0]); // choose the texture to use.
//	//glBegin(GL_TRIANGLES);
//	
//	for (float X = 0; X < (col_size_x - STEP_SIZE); X += STEP_SIZE) {
//		for (float Y = 0; Y < (row_size_y - STEP_SIZE); Y += STEP_SIZE) {
//			glBegin(GL_TRIANGLES);
//			//first triangle
//			height = (img2_gray.at<uchar>(Y, X) / 256.0f)*HEIGHT_RATIO;
//			glTexCoord2f((float)X / col_size_x, (float)Y / row_size_y);
//			glVertex3i(X, height, Y);
//
//
//			height2 = (img2_gray.at<uchar>(Y + STEP_SIZE, X) / 256.0f)*HEIGHT_RATIO;
//			glTexCoord2f((float)X / col_size_x, (float)(Y + STEP_SIZE) / row_size_y);
//			glVertex3i(X, height2, Y + STEP_SIZE);
//
//			height3 = (img2_gray.at<uchar>(Y, X + STEP_SIZE) / 256.0f)*HEIGHT_RATIO;
//			glTexCoord2f((float)(X + STEP_SIZE) / col_size_x, (float)Y / row_size_y);
//			glVertex3i(X + STEP_SIZE, height3, Y);
//
//			triangels.push_back({ {X, height, Y}, {X, height2, Y + STEP_SIZE}, {X + STEP_SIZE, height3, Y} });
//
//			//second triangle
//			glTexCoord2f((float)(X + STEP_SIZE) / col_size_x, (float)Y / row_size_y);
//			glVertex3i(X + STEP_SIZE, height3, Y);
//
//			glTexCoord2f((float)X / col_size_x, (float)(Y + STEP_SIZE) / row_size_y);
//			glVertex3i(X, height2, Y + STEP_SIZE);
//
//			height4 = (img2_gray.at<uchar>(Y + STEP_SIZE, X + STEP_SIZE) / 256.0f)*HEIGHT_RATIO;
//			glTexCoord2f((float)(X + STEP_SIZE) / col_size_x, (float)(Y + STEP_SIZE) / row_size_y);
//			glVertex3i(X + STEP_SIZE, height4, Y + STEP_SIZE);
//
//			triangels.push_back({ {X + STEP_SIZE, height3, Y}, {X, height2, Y + STEP_SIZE}, {X + STEP_SIZE, height4, Y + STEP_SIZE} });
//
//			glEnd();
//
//
//			////lines
//			//glBegin(GL_LINES);
//			//glColor3f(0.7f, 0.7f, 0.7f);
//			////first triangle
//
//			////1->2
//			//glVertex3f(X, height, Y); 
//			//glVertex3f(X, height2, Y + STEP_SIZE);
//
//			////2->3
//			//glVertex3f(X, height2, Y + STEP_SIZE);
//			//glVertex3f(X + STEP_SIZE, height3, Y);
//
//			////3->1
//			//glVertex3f(X + STEP_SIZE, height3, Y);
//			//glVertex3f(X, height, Y);
//
//
//			//glVertex3f(X, height2, Y + STEP_SIZE);
//			//glVertex3f(X + STEP_SIZE, height4, Y + STEP_SIZE);
//
//			//glVertex3f(X + STEP_SIZE, height4, Y + STEP_SIZE);
//			//glVertex3f(X + STEP_SIZE, height3, Y);
//			//
//			//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);          // Reset The Color
//			//glEnd();
//		}
//	}
//
//}
//
//
//int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
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
//	glTranslatef(xtrans, ytrans, ztrans);
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
//	return TRUE;										// Keep Going
//}
//
//GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
//{
//	if (fullscreen)										// Are We In Fullscreen Mode?
//	{
//		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
//		ShowCursor(TRUE);								// Show Mouse Pointer
//	}
//
//	if (hRC)											// Do We Have A Rendering Context?
//	{
//		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
//		{
//			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
//		}
//
//		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
//		{
//			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
//		}
//		hRC = NULL;										// Set RC To NULL
//	}
//
//	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
//	{
//		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
//		hDC = NULL;										// Set DC To NULL
//	}
//
//	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
//	{
//		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
//		hWnd = NULL;										// Set hWnd To NULL
//	}
//
//	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
//	{
//		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
//		hInstance = NULL;									// Set hInstance To NULL
//	}
//}
//
///*	This Code Creates Our OpenGL Window.  Parameters Are:					*
// *	title			- Title To Appear At The Top Of The Window				*
// *	width			- Width Of The GL Window Or Fullscreen Mode				*
// *	height			- Height Of The GL Window Or Fullscreen Mode			*
// *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
// *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
//
//BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
//{
//	GLuint		PixelFormat;							// Holds The Results After Searching For A Match
//	WNDCLASS	wc;										// Windows Class Structure
//	DWORD		dwExStyle;								// Window Extended Style
//	DWORD		dwStyle;								// Window Style
//	RECT		WindowRect;								// Grabs Rectangle Upper Left / Lower Right Values
//	WindowRect.left = (long)0;							// Set Left Value To 0
//	WindowRect.right = (long)width;						// Set Right Value To Requested Width
//	WindowRect.top = (long)0;								// Set Top Value To 0
//	WindowRect.bottom = (long)height;						// Set Bottom Value To Requested Height
//
//	fullscreen = fullscreenflag;							// Set The Global Fullscreen Flag
//
//	hInstance = GetModuleHandle(NULL);		// Grab An Instance For Our Window
//	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
//	wc.lpfnWndProc = (WNDPROC)WndProc;			// WndProc Handles Messages
//	wc.cbClsExtra = 0;							// No Extra Window Data
//	wc.cbWndExtra = 0;							// No Extra Window Data
//	wc.hInstance = hInstance;					// Set The Instance
//	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);	// Load The Default Icon
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// Load The Arrow Pointer
//	wc.hbrBackground = NULL;							// No Background Required For GL
//	wc.lpszMenuName = NULL;							// We Don't Want A Menu
//	wc.lpszClassName = "OpenGL";						// Set The Class Name
//
//	if (!RegisterClass(&wc))							// Attempt To Register The Window Class
//	{
//		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;									// Return FALSE
//	}
//
//	if (fullscreen)										// Attempt Fullscreen Mode?
//	{
//		DEVMODE dmScreenSettings;						// Device Mode
//		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
//		dmScreenSettings.dmSize = sizeof(dmScreenSettings);	// Size Of The Devmode Structure
//		dmScreenSettings.dmPelsWidth = width;		// Selected Screen Width
//		dmScreenSettings.dmPelsHeight = height;		// Selected Screen Height
//		dmScreenSettings.dmBitsPerPel = bits;			// Selected Bits Per Pixel
//		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
//
//		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
//		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
//		{
//			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
//			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
//			{
//				fullscreen = FALSE;						// Windowed Mode Selected.  Fullscreen = FALSE
//			}
//			else
//			{
//				// Pop Up A Message Box Letting User Know The Program Is Closing.
//				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
//				return FALSE;							// Return FALSE
//			}
//		}
//	}
//
//	if (fullscreen)										// Are We Still In Fullscreen Mode?
//	{
//		dwExStyle = WS_EX_APPWINDOW;						// Window Extended Style
//		dwStyle = WS_POPUP;								// Windows Style
//		ShowCursor(FALSE);								// Hide Mouse Pointer
//	}
//	else
//	{
//		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// Window Extended Style
//		dwStyle = WS_OVERLAPPEDWINDOW;					// Windows Style
//	}
//
//	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	// Adjust Window To True Requested Size
//
//	// Create The Window
//	if (!(hWnd = CreateWindowEx(dwExStyle,				// Extended Style For The Window
//		"OpenGL",				// Class Name
//		title,					// Window Title
//		dwStyle |				// Defined Window Style
//		WS_CLIPSIBLINGS |		// Required Window Style
//		WS_CLIPCHILDREN,		// Required Window Style
//		0, 0,					// Window Position
//		WindowRect.right - WindowRect.left,	// Calculate Window Width
//		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
//		NULL,					// No Parent Window
//		NULL,					// No Menu
//		hInstance,				// Instance
//		NULL)))					// Dont Pass Anything To WM_CREATE
//	{
//		KillGLWindow();									// Reset The Display
//		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;									// Return FALSE
//	}
//
//	static	PIXELFORMATDESCRIPTOR pfd =					// pfd Tells Windows How We Want Things To Be
//	{
//		sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
//		1,												// Version Number
//		PFD_DRAW_TO_WINDOW |							// Format Must Support Window
//		PFD_SUPPORT_OPENGL |							// Format Must Support OpenGL
//		PFD_DOUBLEBUFFER,								// Must Support Double Buffering
//		PFD_TYPE_RGBA,									// Request An RGBA Format
//		bits,											// Select Our Color Depth
//		0, 0, 0, 0, 0, 0,								// Color Bits Ignored
//		0,												// No Alpha Buffer
//		0,												// Shift Bit Ignored
//		0,												// No Accumulation Buffer
//		0, 0, 0, 0,										// Accumulation Bits Ignored
//		16,												// 16Bit Z-Buffer (Depth Buffer)  
//		0,												// No Stencil Buffer
//		0,												// No Auxiliary Buffer
//		PFD_MAIN_PLANE,									// Main Drawing Layer
//		0,												// Reserved
//		0, 0, 0											// Layer Masks Ignored
//	};
//
//	if (!(hDC = GetDC(hWnd)))								// Did We Get A Device Context?
//	{
//		KillGLWindow();									// Reset The Display
//		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;									// Return FALSE
//	}
//
//	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))		// Did Windows Find A Matching Pixel Format?
//	{
//		KillGLWindow();									// Reset The Display
//		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;									// Return FALSE
//	}
//
//	if (!SetPixelFormat(hDC, PixelFormat, &pfd))			// Are We Able To Set The Pixel Format?
//	{
//		KillGLWindow();									// Reset The Display
//		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;									// Return FALSE
//	}
//
//	if (!(hRC = wglCreateContext(hDC)))					// Are We Able To Get A Rendering Context?
//	{
//		KillGLWindow();									// Reset The Display
//		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;									// Return FALSE
//	}
//
//	if (!wglMakeCurrent(hDC, hRC))						// Try To Activate The Rendering Context
//	{
//		KillGLWindow();									// Reset The Display
//		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;									// Return FALSE
//	}
//
//	ShowWindow(hWnd, SW_SHOW);							// Show The Window
//	SetForegroundWindow(hWnd);							// Slightly Higher Priority
//	SetFocus(hWnd);										// Sets Keyboard Focus To The Window
//	ReSizeGLScene(width, height);						// Set Up Our Perspective GL Screen
//
//	if (!InitGL())										// Initialize Our Newly Created GL Window
//	{
//		KillGLWindow();									// Reset The Display
//		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
//		return FALSE;									// Return FALSE
//	}
//
//	return TRUE;										// Success
//}
//
//LRESULT CALLBACK WndProc(HWND	hWnd,				// Handle For This Window
//	UINT	uMsg,				// Message For This Window
//	WPARAM	wParam,				// Additional Message Information
//	LPARAM	lParam)				// Additional Message Information
//{
//	switch (uMsg)										// Check For Windows Messages
//	{
//	case WM_ACTIVATE:								// Watch For Window Activate Message
//	{
//		if (!HIWORD(wParam))						// Check Minimization State
//		{
//			active = TRUE;							// Program Is Active
//		}
//		else
//		{
//			active = FALSE;							// Program Is No Longer Active
//		}
//
//		return 0;									// Return To The Message Loop
//	}
//
//	case WM_SYSCOMMAND:								// Intercept System Commands
//	{
//		switch (wParam)								// Check System Calls
//		{
//		case SC_SCREENSAVE:						// Screensaver Trying To Start?
//		case SC_MONITORPOWER:					// Monitor Trying To Enter Powersave?
//			return 0;								// Prevent From Happening
//		}
//		break;										// Exit
//	}
//
//	case WM_CLOSE:									// Did We Receive A Close Message?
//	{
//		PostQuitMessage(0);							// Send A Quit Message
//		return 0;									// Jump Back
//	}
//
//	case WM_LBUTTONDOWN:							// Did We Receive A Left Mouse Click?
//	{
//		bRender = !bRender;							// Change The Rendering State Between Fill And Wire Frame
//		return 0;									// Jump Back
//	}
//
//	case WM_KEYDOWN:								// Is A Key Being Held Down?
//	{
//		keys[wParam] = TRUE;						// If So, Mark It As TRUE
//		return 0;									// Jump Back
//	}
//
//	case WM_KEYUP:									// Has A Key Been Released?
//	{
//		keys[wParam] = FALSE;						// If So, Mark It As FALSE
//		return 0;									// Jump Back
//	}
//
//	case WM_SIZE:									// Resize The OpenGL Window
//	{
//		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
//		return 0;									// Jump Back
//	}
//	}
//
//	// Pass All Unhandled Messages To DefWindowProc
//	return DefWindowProc(hWnd, uMsg, wParam, lParam);
//}
//
//int WINAPI WinMain(HINSTANCE	hInstance,				// Instance
//	HINSTANCE	hPrevInstance,			// Previous Instance
//	LPSTR		lpCmdLine,				// Command Line Parameters
//	int			nCmdShow)				// Window Show State
//{
//	MSG		msg;										// Windows Message Structure
//	BOOL	done = FALSE;									// Bool Variable To Exit Loop
//
//	// Ask The User Which Screen Mode They Prefer
//	if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO)
//	{
//		fullscreen = FALSE;								// Windowed Mode
//	}
//
//	// Create Our OpenGL Window
//	if (!CreateGLWindow((char*)"Mini Project", 640, 480, 16, fullscreen))
//	{
//		return 0;										// Quit If Window Was Not Created
//	}
//	while (!done)										// Loop That Runs While done=FALSE
//	{
//		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))		// Is There A Message Waiting?
//		{
//			if (msg.message == WM_QUIT)					// Have We Received A Quit Message?
//			{
//				done = TRUE;								// If So done=TRUE
//			}
//			else										// If Not, Deal With Window Messages
//			{
//				TranslateMessage(&msg);					// Translate The Message
//				DispatchMessage(&msg);					// Dispatch The Message
//			}
//		}
//		else {											// If There Are No Messages
//
//		// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
//			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
//			{
//				done = TRUE;								// ESC or DrawGLScene Signalled A Quit
//			}
//			else if (active)							// Not Time To Quit, Update Screen
//			{
//				SwapBuffers(hDC);						// Swap Buffers (Double Buffering)
//				if (keys[VK_UP]) //UP ARROW key
//				{
//					scaleValue += 0.001f;					// Increase the scale value to zoom in
//					xpos -= (float)sin(heading*piover180) * 0.05f;
//					zpos -= (float)cos(heading*piover180) * 0.05f;
//					if (walkbiasangle >= 359.0f)
//					{
//						walkbiasangle = 0.0f;
//					}
//					else
//					{
//						walkbiasangle += 10;
//					}
//					walkbias = (float)sin(walkbiasangle * piover180) / 20.0f;
//				}
//
//				if (keys[VK_DOWN]) //DOWN ARROW key
//				{
//					scaleValue -= 0.001f;					// Decrease the scale value to zoom out
//					xpos += (float)sin(heading*piover180) * 0.05f;
//					zpos += (float)cos(heading*piover180) * 0.05f;
//					if (walkbiasangle <= 1.0f)
//					{
//						walkbiasangle = 359.0f;
//					}
//					else
//					{
//						walkbiasangle -= 10;
//					}
//					walkbias = (float)sin(walkbiasangle * piover180) / 20.0f;
//				}
//
//				if (keys[VK_RIGHT]) //RIGHT ARROW key
//				{
//					heading -= 0.5f;
//					yrot = heading;
//				}
//
//				if (keys[VK_LEFT]) //LEFT ARROW key
//				{
//					heading += 0.5f;
//					yrot = heading;
//				}
//
//				if (keys[VK_PRIOR]) //PAGE UP key
//				{
//					lookupdown -= 0.5f;
//				}
//
//				if (keys[VK_NEXT]) //PAGE DOWN key
//				{
//					lookupdown += 0.5f;
//				}
//
//				//if (keys[VK_UP])							// Is the UP ARROW key Being Pressed?
//				//	scaleValue += 0.001f;					// Increase the scale value to zoom in
//
//				//if (keys[VK_DOWN])							// Is the DOWN ARROW key Being Pressed?
//				//	scaleValue -= 0.001f;					// Decrease the scale value to zoom out
//
//			}
//
//			if (keys[VK_F1])							// Is F1 Being Pressed?
//			{
//				keys[VK_F1] = FALSE;						// If So Make Key FALSE
//				KillGLWindow();							// Kill Our Current Window
//				fullscreen = !fullscreen;					// Toggle Fullscreen / Windowed Mode
//				// Recreate Our OpenGL Window
//				if (!CreateGLWindow((char*)"Mini Project", 640, 480, 16, fullscreen))
//				{
//					return 0;							// Quit If Window Was Not Created
//				}
//			}
//		}
//	}
//	// Shutdown
//	KillGLWindow();										// Kill The Window
//	return (msg.wParam);								// Exit The Program
//}