
#include "fontmanager.h"

/*	function create font object and return pointer
	\param[in] name name of file with font, it will be key for this font object
	\return pointer to font object
*/
pFont FontManager::loadFont(hString &name, int size) {
	pFont temp;
	if (!m_fonts.contains(name)) {
		temp =  new Font(name, size);
		m_fonts.insert(name, temp);
	} else {
		temp = NULL;
	}
	return temp;

}
/*	function return font object with key
	\param[in] key key for object for returning
	\return pointer to font object
*/
pFont FontManager::getFont(hString &key) {
	pFont result = NULL;
	if (m_fonts.contains(key)) {
		result = m_fonts[key];
	}
	return result;
}
/*	function delete font object
	\param[in] key key of deleting object
*/
void FontManager::removeFont(hString &key) {
	if (m_fonts.contains(key)) {
		m_fonts.remove(key);
	}
}
/* destructor delete all fonts object keeping in this manager
*/
FontManager::~FontManager() {
	pFont temp;
	hhash<hString, Font*>::iterator font_iterator;
	font_iterator = m_fonts.begin();
	while (font_iterator != m_fonts.end()) {
		temp = font_iterator.value();
		delete temp;
		++font_iterator;
	}
}

//����� �������� �� ������� �� ���������� ���������� ��������� ������� ������� �����������, ��������.

/*! Destructor free loaded font
*/
Font::~Font() {
	glDeleteLists(m_font, 96);//������� ��� 96 ������� �����������, �.�. ����������� ��������
}
/*!	Default constructor, no load anything
*/
Font::Font() {
	m_font_loaded = false;
}
/*! Copying constructor, copy all properties of getting object
	\param[in] other object for copying, this object will be copy of other
*/
Font::Font(Font &other) {
	m_font_loaded = other.m_font_loaded;
	m_font = other.m_font;
#ifdef WIN32
	m_hdc = other.m_hdc;
#endif
}
/*! Constructor load font and size
	\param[in] name name (and path, if need) of file with true type font
*/
Font::Font(hString &name, int size) {
	loadFont(name, size);
}
#ifdef WIN32
/*!	Default constructor, no load anything
	\param hdc HDC
*/
Font::Font(HDC hdc) {
	m_hdc = hdc;
}
/*! Constructor load font and size
	\param[in] file_name name (and path, if need) of file with true type font
	\param hdc HDC
*/
Font::Font(hString &name, int size, HDC hdc) {
	m_hdc = hdc;
	loadFont(name, size);
}
#endif
/*! function load font from file with name
	\param[in] name name of file with font
	\return font pointer
*/
void Font::loadFont(hString &name, int size) {
#ifdef WIN32
	tfont temp_font = CreateFont(
		-size, //������ ������, ���� ����� ��������: ������� ������ ������� � �� ������
		0, //������ ������
		0, //���� ���������
		0, //���� �������
		FW_NORMAL, //�������� ������
		false, //������
		false, //�����������
		false, //�����������
		ANSI_CHARSET, //������������� ������ ��������
		OUT_TT_PRECIS, //������������ ttf ������, ���� ���� ��������� � ����� ������
		CLIP_DEFAULT_PRECIS, //�������� ���������
		ANTIALIASED_QUALITY, //���-�� ������
		FF_DONTCARE|DEFAULT_PITCH, //��������� � ���
		name.data() //��� ������
		);
	m_font = glGenLists(96);
	SelectObject(m_hdc, temp_font);//����� ����� �����
	wglUseFontBitmaps(m_hdc, 32, 96, m_font);//��������� ������ ����������� ��� 96 �������� ������� � �������
#endif
}