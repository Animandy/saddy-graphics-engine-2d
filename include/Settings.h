/*! \file   setting.h
\author FreakyBlast

\brief  Declaration of SadSettings and helper types.

This file contains a declaration of SadSettings - settings upon OpenGL. 
*/

#ifndef Settings_h__
#define Settings_h__


#include "types.h"

namespace Sad{
	/*!\class Setting class
	*/
	class Settings
	{

		unsigned int m_screenwidth;  //!< Width of screen

		unsigned int m_screenheight;  //!< Height of screen

		bool m_isfullscreen;  //!< Whether fullscreen

		Uint8 m_bpp;  //!< Depth color (bit per pixel)

		Uint8 m_depth;  //!< bit for z-buffer

		float m_fov; //!< Angle 

		float m_znear;  //!< ������� ��������� ���������

		float m_zfar;  //!< ������� ��������� ���������

	public:
		Settings(
			unsigned int screenwidth,
			unsigned int screenheight,
			bool isfullscreen,
			Uint8 bpp,
			Uint8 depth,
			float fov,
			float znear,
			float zfar);

		~Settings(void);

		inline unsigned int screenWidth(void)const {return m_screenwidth;     }
		inline unsigned int screenHeight(void)const{return m_screenheight;    }
		inline bool isFullscreen(void)const        {return m_isfullscreen;    }
		inline Uint8 bpp(void)const                {return m_bpp;             }
		inline Uint8 depth(void)const              {return m_depth;           }
		inline float fov(void)const                {return m_fov;             }
		inline float znear(void)const              {return m_znear;           }
		inline float zfar(void)const               {return m_zfar;            }

		inline void setIsFullscreen(bool value)         {m_isfullscreen=value;}
		inline void setWidthScreen(unsigned int value)  {m_screenwidth=value; }
		inline void setHeightScreen(unsigned int value) {m_screenheight=value;}
		inline void setBBP(Uint8 value)                 {m_bpp=value;         }
	};
}

#endif // Settings_h__