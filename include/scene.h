/*! \file   scene.h
\author HiddenSeeker
\brief  ����� ����������� ����� �����
*/
#include "templates/hlvector.hpp"
#include "templates/hhash.hpp"
#include <assert.h>
#pragma once

namespace sad
{

/*! ����� �������� ������� �����
*/
class BasicNode
{
	float m_x;    //!< �������
	float m_y;    //!< �������
	int   m_type; //!< ��� ������� (������������)
public:
	static int Type; //!< ����������� ���
	/*! ���
	*/
	int type() const;
	/*! ���������, ������� ����� ����������� ��� ���������� �������.
	*/
	virtual void render()=0;
	/*! ����������
	*/
	virtual ~BasicNode();
};


/*! ����� �����
*/
class Scene
{
private:
	hst::vector<BasicNode *>   m_layers;              //!< ����, ������ ������� � �� �� ����������
	hst::hash<hst::string,unsigned long>  m_nodehash; //!< ������� �� ����
public:
	Scene();    //!< ������ �����
	~Scene();   //!< ����������

	/*! ��������� ������ � �����
	\param[in] node  ������ 
	\param[in] name  ���
	\param[in] lay   ����
	*/
	void add(
		BasicNode * node, 
		const hst::string & name=hst::string(),
		unsigned long lay=(unsigned long)-1
		);
	/*! ������� ������ �� �����
	\param[in] name ��� �������
	*/
	void remove(const hst::string & name);
	/*! �������� �����
	*/
	void render();
	/*! ��������� ������� �� �����. ���� �� �������, �� ������ NULL
	\param[in] name ���
	*/
	template<typename T> T * get(const hst::string & name); 
};

}

typedef sad::BasicNode BasicNode;
typedef sad::Scene     Scene;

//=================�������� ��� ��������� �����====================
template<typename T> T * sad::Scene::get(const hst::string & name)
{
	if (!m_nodehash.contains(name)) return NULL;

	BasicNode *  ind=m_layers[m_nodehash[name]];
	assert(ind->type()==T::Type && "Inconsistent types!");
	return ind;
}
