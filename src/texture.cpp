#include "texture.h"
#include "texturemanager.h"
#include <errno.h>
#include "primitives/glext.h"
#include "extchecker.h"
#ifdef LINUX
#include <renderer.h>

extern void ( * glXGetProcAddressARB (const GLubyte *procName)) (void);
#endif

using namespace sad;

static PFNGLGENERATEMIPMAPEXTPROC glGenerateMipMapsSaddy  = NULL;


static PFNGLGENERATEMIPMAPEXTPROC getglGenerateMipMaps()
{
	if (glGenerateMipMapsSaddy == NULL)
	{
#ifdef WIN32
		glGenerateMipMapsSaddy = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress((LPCSTR)"glGenerateMipmap");
#else
		glGenerateMipMapsSaddy = (PFNGLGENERATEMIPMAPEXTPROC)(*glXGetProcAddressARB)((const GLubyte*)"glGenerateMipmap");
#endif
	}
	return glGenerateMipMapsSaddy;
}


 

Texture::Texture()
{
	m_mode = BORDER_CLAMP;	/*!< Set default mode of texture.		*/
	m_filter = 10;			/*!< Set default filter of texture.		*/
	m_generated=false;      /*!< Currently, this texture is not used */ 
}

Texture::~Texture()
{
	m_data.clear();			/*!< Cleaning the container of pixels.  */
	if (m_generated)
		glDeleteTextures(1,&m_id);
}
void Texture::disable()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::enable()
{
	glBindTexture(GL_TEXTURE_2D,m_id);
}

#include "log.h"

void Texture::buildMipMaps()
{
	// Строим текстуру
	GLuint type,components;
	switch (m_bpp)
	{
		case 24: // 24 бита
			type=GL_RGB;components=3; break;
		case 32: // 32 бита
			type=GL_RGBA;components=4; break;
	};
    
	glGenTextures(1, (GLuint *)&m_id);	
	glBindTexture(GL_TEXTURE_2D, m_id);
	m_generated=true;
	GLuint what;
	switch (m_mode)
	{
		case BORDER_CLAMP:	what=GL_CLAMP;break;
		case BORDER_REPEAT: what=GL_REPEAT;break;
	}

	makePOT();

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	// Устанавливаем параметры границ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, what);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, what);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	
    
	// Build Mip Maps	
	GLint res;
	hst::pair<int,int> version = ext::version();
	if (version.p1() < 3) // In OpenGL 3.0  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);  is deprecated
	{
		if (version.p1() == 1 && version.p2() < 4)
		{
			res=gluBuild2DMipmaps(GL_TEXTURE_2D, components, m_width, m_height, type, GL_UNSIGNED_BYTE, m_data.data());
		}
		else
		{
			  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
			  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, type, GL_UNSIGNED_BYTE, m_data.data());
			  res = 0;
		}
	} 
	else
	{
		float max  = (float)((m_width > m_height) ? m_width : m_height);
		int levels = (int)(log(max)/log(2.0f));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  m_width, m_height, 0, type, GL_UNSIGNED_BYTE, m_data.data());			  
		(*(getglGenerateMipMaps()))(GL_TEXTURE_2D);
		res = 0;
	}
	if (res)
	{
		hst::log::inst()->owrite(hst::string("Error: "));
		hst::log::inst()->owrite(hst::string((char*)gluErrorString(res) ));
		hst::log::inst()->owrite(hst::string("\n"));
	}

	
}

void Texture::setAlpha(Uint8 a)
{
	m_bpp=32;
	for (unsigned int i=3;i<m_data.count();i+=4)
	{
		m_data[i]=255-a;
	}
}
void Texture::setAlpha(Uint8 a, const hst::color & clr, Uint8 prec)
{
	m_bpp=32;
	for (unsigned int i=3;i<m_data.count();i+=4)
	{
	  bool rck=abs(m_data[i-3]-clr.r())<=prec;
	  bool gck=abs(m_data[i-2]-clr.g())<=prec;
	  bool bck=abs(m_data[i-1]-clr.b())<=prec;
	  if (rck && gck && bck)
		m_data[i]=255-a;
	}
}

void Texture::setAlpha(Uint8 a, const hst::color & clr,const hRectF & rect)
{
	hRectF tmp=rect;
	for (int i=0;i<4;i++)
	{
		if (tmp[i].x()<0) tmp[i].setX(0);
		if (tmp[i].y()<0) tmp[i].setY(0);
		if (tmp[i].x()>width()-1) tmp[i].setX(this->width()-1);
		if (tmp[i].y()>height()-1) tmp[i].setY(this->height()-1);
	}
	if (tmp[2].x()<tmp[0].x())
	{
		double f=tmp[2].x(); tmp[2].setX(tmp[0].x()); tmp[0].setX(f);
	}
	
	if (tmp[2].y()<tmp[0].y())
	{
		double f=tmp[2].y(); tmp[2].setY(tmp[0].y()); tmp[0].setY(f);
	}
	int minx=(int)tmp[0].x();
	int maxx=(int)tmp[2].x();
	int miny=(int)tmp[0].y();
	int maxy=(int)tmp[2].y();
	for (int row=minx;row<=maxx;row++)
	{
		for (int col=miny;col<=maxy;col++)
		{
			Uint8 * pix=this->pixel(row,col);
			bool rck=abs(clr.r()-pix[0])<10;
			bool gck=abs(clr.g()-pix[1])<10;
			bool bck=abs(clr.b()-pix[2])<10;
			if (rck && gck && bck)
			{
				setPixelAlpha(row,col,0);
			}
		}
	}

}
void Texture::setMode(Texture::Mode mode)
{
	m_mode=mode;
}

bool Texture::load(const hst::string & filename)
{
	hst::string ff(filename.getExtension());
	char * f=const_cast<char *>(ff.data());
	while(*f) { *f=toupper(*f); ++f; }

	sad::TextureLoader * l = sad::TextureManager::instance()->loader(ff);
	if (l)
	{
		FILE * fl = fopen(filename.data(), "rb");
		if (fl)
		{
			bool result = l->load(fl, this);
			fclose(fl);
			return result;
		}
	}
	return false;
}
bool Texture::load(const hst::wstring & filename)
{
	char * tmp=new char[2*filename.length()+2];
	wcstombs(tmp,filename.data(),2*filename.length()+2);
	hst::string tt(tmp);
	delete tmp;
	return load(tt);
}

void Texture::save(const char * method, const char * file)
{
	FILE * f=fopen(file,"wt");
	if (f)
	{
		fputs("#include \"texture.h\"\n\nstatic const unsigned char texdata[",f);
		fprintf(f,"%lu]=\n{\n",m_data.count());
		if (m_data.count()!=0)
			fprintf(f,"%d",m_data[0]);
		for (unsigned long i=1;i<m_data.count();i++)
               fprintf(f,", %d",m_data[i]);
		fputs("\n};\n\n",f);
		
		fprintf(f,"static const unsigned int cnt=%lu;\n\n",m_data.count());
		fprintf(f,"\nvoid sad::Texture::%s()\n{\n m_bpp=%d;m_width=%d;m_height=%d;\n for (unsigned int i=0;i<cnt;i++) m_data<<texdata[i]; \n}\n\n\n",method,m_bpp,m_width,m_height);
		fclose(f);
	}
	
}

TextureLoader::~TextureLoader()
{

}

