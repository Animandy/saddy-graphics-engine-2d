/*! \file   Render.cpp
    \author FreakyBlast

	\brief  Definition of Render.
 
	In this file OpenGL function has been used obviously.
	����������� ���� �� �������, ���� ����� ����������� � ������ �������.
*/

#include <windows.h>													
#include "Render.h"

#include <gl/gl.h>														
#include <gl/glu.h>													

//���������� ����������, ����� �... �������, �� ���� �� ���� �������� � ���������� �������
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )


SadRender MainRender;

SadRender::SadRender(void)
{
}

SadRender::~SadRender(void)
{
}

//������������� OpenGL
BOOL SadRender::SetupOpenGL(GL_Window *window)
{
	PIXELFORMATDESCRIPTOR pfd =											// pfd ��������� Windows ��� ������������� OpenGL
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// ������ ����� Pixel Format Descriptor'�
		1,																// ����� ������
		PFD_DRAW_TO_WINDOW |											// ������ ������ ������������ ��������� � ����
		PFD_SUPPORT_OPENGL |											// ������ ������ ������������ OpenGL
		PFD_DOUBLEBUFFER,												// ������ ������ ������������ Double Buffering
		PFD_TYPE_RGBA,													// ������ ������ ������������ RGBA �����
		window->init.bitsPerPixel,										// �������� ������� �����
		0, 0, 0, 0, 0, 0,												// ���������� ���� �����
		0,																// ��� ����� ������ (���� ����)
		0,																// ���������� ��� ������
		0,																// ���� Accumulation Buffer'�
		0, 0, 0, 0,														// Accumulation ���� ����������
		MainSettings.DEPTH,												// ������� Z-Buffer'� (Depth Buffer)  
		0,																// ��� Stencil Buffer'� (���� ����)
		0,																// ��� Auxiliary Buffer'�
		PFD_MAIN_PLANE,													// ������� ���� ��� ���������
		0,																// ���������������
		0, 0, 0															// ���������� ����� ����
	};

	GLuint PixelFormat;													// �������� ��������� ������

	PixelFormat = ChoosePixelFormat (window->hDC, &pfd);				// ���� ����������� Pixel Format
	if (PixelFormat == 0)												// �� �����?
	{
		// �������
		ReleaseDC (window->hWnd, window->hDC);							// ����������� �������� ���������
		window->hDC = 0;												// �������� ���
		DestroyWindow (window->hWnd);									// ������� ����
		window->hWnd = 0;												// �������� ����� ����
		return FALSE;													// ���������� False (����)
	}

	if (SetPixelFormat (window->hDC, PixelFormat, &pfd) == FALSE)		// �������� ���������� ���� ������
	{
		// ��������
		ReleaseDC (window->hWnd, window->hDC);							// ����������� �������� ���������
		window->hDC = 0;												// �������� ���
		DestroyWindow (window->hWnd);									// ������� ����
		window->hWnd = 0;												// �������� ����� ����
		return FALSE;													// ���������� False (����)
	}

	window->hRC = wglCreateContext (window->hDC);						// ������� �������� �������� �������
	if (window->hRC == 0)												// �� ��� ��������?
	{
		// ��������
		ReleaseDC (window->hWnd, window->hDC);							// ����������� �������� ���������
		window->hDC = 0;												// �������� ���
		DestroyWindow (window->hWnd);									// ������� ����
		window->hWnd = 0;												// �������� ����� ����
		return FALSE;													// ���������� False (����)
	}

	// ������ ���� ������� ������� �������
	if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)
	{
		// ��������
		wglDeleteContext (window->hRC);									// ������� �������� �������
		window->hRC = 0;												// �������� ���
		ReleaseDC (window->hWnd, window->hDC);							// ����������� �������� ���������
		window->hDC = 0;												// �������� ���
		DestroyWindow (window->hWnd);									// ������� ����
		window->hWnd = 0;												// �������� ����� ����
		return FALSE;													// ���������� False (����)
	}

	return TRUE;
}

void SadRender::Reshape(int width, int height)
{
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// ����������������� ViewPort (������� ���������)
	glMatrixMode (GL_PROJECTION);										// �������� ������� ��������
	glLoadIdentity ();													// ���������� � �� ���������
	gluPerspective (MainSettings.FOV*(GLfloat)(height)/(GLfloat)(width), (GLfloat)(width)/(GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
		MainSettings.ZNEAR, MainSettings.ZFAR);		
	glMatrixMode (GL_MODELVIEW);										// �������� ������� �������
	glLoadIdentity ();													// ���������� � �� ���������


	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// ������������� �������� ���������� �����������
}


