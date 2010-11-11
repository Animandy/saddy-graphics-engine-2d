
#include "..\include\Engine.h"
#include "gl\gl.h"
#include "gl\glu.h"

//���� �� ������� ��������� �����
float angle=0;
//������ ��� ������
const GLfloat boxVertexes[] =
{
	/*x,y,z*/
	-1, -1, -1,
	1, -1, -1,
	1,  1, -1,
	-1,  1, -1,
	-1, -1,  1,
	1, -1,  1,
	1,  1,  1,
	-1,  1,  1,
};

const float boxUVs[] =
{
	/*u,v ���������� ��� ���� ������ ������� �������� */
	0, 0,
	1, 0,
	1, 1,
	0, 1
};
//������� ��� ������ �����
const float boxNormals[] =
{
	0,  1,  0,
	0, -1,  0,
	1,  0,  0,
	-1,  0,  0,
	0,  0,  1,
	0,  0, -1
};
const int boxQuads[] =
{
	/* ������� ������, �� 4 �� ������� ������ */
	1, 0, 3, 2,
	4, 5, 6, 7,
	1, 2, 6, 5,
	0, 4, 7, 3,
	1, 5, 4, 0,
	3, 7, 6, 2
};
//������������� ��������
void InitSettings()
{
	//������ ����� 640�480�32
	MainSettings.SCREEN_WIDTH	=	640;							
	MainSettings.m_screenheight	=	480;
	MainSettings.BPP			=	32;
	//������� z-������� 24 ����
	MainSettings.DEPTH			=	24;
	//��������� � ������� ������, �� ������� F1 ����� ������������� � ����������
	MainSettings.m_isfullscreen		=	FALSE;
	//���� ������ (Quake ����)
	MainSettings.FOV			=	45.0f;
	//������� � ������� ��������� ���������
	MainSettings.ZNEAR			=	0.1f;
	MainSettings.ZFAR			=	100.0f;
	//��������� ����
	MainSettings.TITLE			=	"�������� ������ Saddy.������� 'F1' ��� �������������� ������, 'Esc' ��� ������.";

}
//������������� ������ �����
BOOL Initialize (sadWindow* window, Keys* keys)	// �������������
{
	//���� ��������
	angle = 0;

	return TRUE;
}
//���������������
void Deinitialize (void)							// ���������������
{
	//���� ��� �����
}
//����������� ����� ������ ������ ��� ��������� ��������� �������� � �.� � �.�.
void Update (__int64 DeltaTime)					// ������ ����� � ����������� �� ���������� �������
{
	//DeltaTime - ����� ��������� � ����������� �������
	angle+=100*DeltaTime/1000;

}

//������
void Draw (void)									// ��������� ���� �����
{
	//������� ������ ����� � �������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//������ ��������� ������� �������
	glLoadIdentity();

	//������������ �� ������ ����� �� ��� �����
	glTranslatef(0,0,-6);
	//������� ���
	glRotatef(angle,1,2,3);
	//������ 6-�� ����������
	glBegin(GL_QUADS);
	for(int i = 0; i < 6; i++ )
	{
		for (int j=0;j<4;j++)
		{
			//������������� ����
			glColor3fv(&boxVertexes[boxQuads[i*4+j]*3]);
			//������������� ����������
			glVertex3fv(&boxVertexes[boxQuads[i*4+j]*3]);
		}
	}
	glEnd();

	//��� �������� ������� ������������� ��� ����� �� ����������
	//� ����� ��� �������� ��������, � �� ���� ����� ��� ��������.
	//���� ��� glFinish(), �� ��� ���� ���� �� ����������, � � ��� ������� � �����
	glFlush();

}
