/*! \file   Render.cpp
\author FreakyBlast

\brief  Definition of Render.

In this file OpenGL function has been used obviously.
����������� ���� �� �������, ���� ����� ����������� � ������ �������.
*/

#include <windows.h>													
#include "renderer.h"

#include <gl/gl.h>														
#include <gl/glu.h>													


#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )


namespace Sad{

	Renderer::Renderer(void)
	{
	}

	Renderer::~Renderer(void)
	{
	}


	bool Renderer::init(Sad::Settings* _settings, sadWindow *window)
	{
        hst::log* curLog;
		bool result=true;
		PIXELFORMATDESCRIPTOR pfd =											// pfd ��������� Windows ��� ������������� OpenGL
		{
			sizeof (PIXELFORMATDESCRIPTOR),									// ������ ����� Pixel Format Descriptor'�
			1,																// ����� ������
			PFD_DRAW_TO_WINDOW |											// ������ ������ ������������ ��������� � ����
			PFD_SUPPORT_OPENGL |											// ������ ������ ������������ OpenGL
			PFD_DOUBLEBUFFER,												// ������ ������ ������������ Double Buffering
			PFD_TYPE_RGBA,													// ������ ������ ������������ RGBA �����
			_settings->bpp(),										// �������� ������� �����
			0, 0, 0, 0, 0, 0,												// ���������� ���� �����
			0,																// ��� ����� ������ (���� ����)
			0,																// ���������� ��� ������
			0,																// ���� Accumulation Buffer'�
			0, 0, 0, 0,														// Accumulation ���� ����������
			_settings->depth(),												// ������� Z-Buffer'� (Depth Buffer)  
			0,																// ��� Stencil Buffer'� (���� ����)
			0,																// ��� Auxiliary Buffer'�
			PFD_MAIN_PLANE,													// ������� ���� ��� ���������
			0,																// ���������������
			0, 0, 0															// ���������� ����� ����
		};

		GLuint PixelFormat;													// �������� ��������� ������

		PixelFormat = ChoosePixelFormat (window->hDC, &pfd);				// ���� ����������� Pixel Format
		if (!PixelFormat)												// �� �����?
		{
			// �������
			curLog = hst::log::inst();
			curLog->write(hString("PixelFormat() failure!\n"));
			curLog->write(hString("DATA: HWND: "));
			curLog->write(hString::number((int)window->hWnd));
			curLog->write(hString(" hDC: "));
			curLog->write(hString::number((int)window->hDC));
			curLog->write(hString("\n"));
			curLog->save("log.txt");
			ReleaseDC (window->hWnd, window->hDC);							// ����������� �������� ���������
			window->hDC = 0;												// �������� ���
			DestroyWindow (window->hWnd);									// ������� ����
			window->hWnd = 0;												// �������� ����� ����
			return false;													// ���������� False (����)
		}

		if (!SetPixelFormat (window->hDC, PixelFormat, &pfd))		// �������� ���������� ���� ������
		{
			// ��������
			hst::log* curLog = hst::log::inst();
			curLog->write(hString("SetPixelFormat() failure!\n"));
			curLog->write(hString("DATA: HWND: "));
			curLog->write(hString::number((int)window->hWnd));
			curLog->write(hString(" hDC: "));
			curLog->write(hString::number((int)window->hDC));
			curLog->write(hString("\n"));
			curLog->save("log.txt");
			ReleaseDC (window->hWnd, window->hDC);							// ����������� �������� ���������
			window->hDC = 0;												// �������� ���
			DestroyWindow (window->hWnd);									// ������� ����
			window->hWnd = 0;												// �������� ����� ����
			curLog = hst::log::inst();
			curLog->write(hString("PixelFormat() failure!"));
			curLog->save("log.txt");
			return false;													// ���������� False (����)
		}

		window->hRC = wglCreateContext (window->hDC);						// ������� �������� �������� �������
		if (!window->hRC)												// �� ��� ��������?
		{
			// ��������
			curLog = hst::log::inst();
			curLog->write(hString("wglCreateContext() failure!\n"));
			curLog->write(hString("DATA: HWND: "));
			curLog->write(hString::number((int)window->hWnd));
			curLog->write(hString(" hDC: "));
			curLog->write(hString::number((int)window->hDC));
			curLog->write(hString("\n"));
			curLog->save("log.txt");
			ReleaseDC (window->hWnd, window->hDC);							// ����������� �������� ���������
			window->hDC = 0;												// �������� ���
			DestroyWindow (window->hWnd);									// ������� ����
			window->hWnd = 0;												// �������� ����� ����
			return FALSE;													// ���������� False (����)
		}

		// ������ ���� ������� ������� �������
		if (!wglMakeCurrent(window->hDC, window->hRC))
		{
			// ��������
			curLog = hst::log::inst();
			curLog->write(hString("wglMakeCurrent() failure!\n"));
			curLog->write(hString("DATA: HWND: "));
			curLog->write(hString::number((int)window->hWnd));
			curLog->write(hString(" hDC: "));
			curLog->write(hString::number((int)window->hDC));
			curLog->write(hString("\n"));
			curLog->save("log.txt");
			wglDeleteContext (window->hRC);									// ������� �������� �������
			window->hRC = 0;												// �������� ���
			ReleaseDC (window->hWnd, window->hDC);							// ����������� �������� ���������
			window->hDC = 0;												// �������� ���
			DestroyWindow (window->hWnd);									// ������� ����
			window->hWnd = 0;												// �������� ����� ����
			return false;													// ���������� False (����)
		}

		return true;
	}

	void Renderer::reshape(int width, int height, Sad::Settings* sets)
	{
		glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// ����������������� ViewPort (������� ���������)
		glMatrixMode (GL_PROJECTION);										// �������� ������� ��������
		glLoadIdentity ();													// ���������� � �� ���������
		gluPerspective (sets->fov()*(GLfloat)(height)/(GLfloat)(width), (GLfloat)(width)/(GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
			sets->znear(), sets->zfar());		
		glMatrixMode (GL_MODELVIEW);										// �������� ������� �������
		glLoadIdentity ();													// ���������� � �� ���������


		glEnable(GL_DEPTH_TEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// ������������� �������� ���������� �����������
	}

	Renderer& Renderer::instance()
	{
		static Renderer aloneRenderer;
		return aloneRenderer;
	}

	void Renderer::mainLoop(){

		bool isMessagePumpActive;
        LPMSG msg;


        Sad::Settings* _GLSets;
		Sad::Window* mainWin;
		mainWin->initSettings(_GLSets);

		Sad::sadWindow* wndparams;
        bool isSuccessCreationW = mainWin->createWindow(wndparams);
		if (!Renderer::instance().init(_GLSets, wndparams)){
			// to log
		}else{

			isProgramLooping = true;											// Program Looping Is Set To TRUE
			createFullScreen = _GLSets->isFullscreen();						// g_createFullScreen Is Set To User Default

			Renderer::setTimer();

			while (isProgramLooping)											// Loop Until WM_QUIT Is Received
			{
				// Create A Window
				_GLSets->setIsFullscreen(createFullScreen);					// Set Init Param Of Window Creation To Fullscreen?
				if (isSuccessCreationW)							// Was Window Creation Successful?
				{
					// At This Point We Should Have A Window That Is Setup To Render OpenGL
					if (!mainWin->createGLContext(_GLSets, wndparams))					// Call User Intialization
					{
						// Failure
						mainWin->terminateApplication (wndparams);							// Close Window, This Will Handle The Shutdown
					}
					else														// Otherwise (Start The Message Pump)
					{	// Initialize was a success
						isMessagePumpActive = true;								// Set isMessagePumpActive To TRUE
						Renderer::instance().setCurrentWindow(wndparams);
						while (isMessagePumpActive == true)						// While The Message Pump Is Active
						{
							// Success Creating Window.  Check For Window Messages
							if (PeekMessage (msg, wndparams->hWnd, 0, 0, PM_REMOVE) != 0)
							{
								// Check For WM_QUIT Message
								if (msg->message != WM_QUIT)						// Is The Message A WM_QUIT Message?
								{
									DispatchMessage (msg);						// If Not, Dispatch The Message
								}
								else											// Otherwise (If Message Is WM_QUIT)
								{
									isMessagePumpActive = false;				// Terminate The Message Pump
								}
							}
							else												// If There Are No Messages
							{
								if (wndparams->isVisible == false)					// If Window Is Not Visible
								{
									WaitMessage();								// Application Is Minimized Wait For A Message
								}
								else											// If Window Is Visible
								{
									// Process Application Loop

									mainWin->update(Renderer::instance().elapsedInMSeconds());
									Renderer::instance().setTimer();


									SwapBuffers (wndparams->hDC);					// Swap Buffers (Double Buffering)
								}
							}
						}														// Loop While isMessagePumpActive == TRUE
					}															// If (Initialize (...

					// Application Is Finished
					//Deinitialize ();											// User Defined DeInitialization

					mainWin->destroyWindowGL(wndparams, _GLSets);									// Destroy The Active Window
				}
				else															// If Window Creation Failed
				{
					// Error Creating Window
					MessageBox (HWND_DESKTOP, LPCSTR("Error Creating OpenGL Window"), LPCSTR("Error"), MB_OK | MB_ICONEXCLAMATION);
					isProgramLooping = false;									// Terminate The Loop
				}
			}
		}// While (isProgramLooping)
	}	


	void Renderer::quit(){

	}
}


