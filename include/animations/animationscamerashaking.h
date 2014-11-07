/*! \file animations/animationscamershaking.h
	\author HiddenSeeker

	Declares animation of shaking the camera
 */
#pragma once
#include "animationsanimation.h"
#include "animationsanimationfastcall.h"

#include "../sadpoint.h"
#include "../scene.h"

namespace sad
{
	
namespace animations
{

/*! An animation of shaking the camera
 */
class CameraShaking: public sad::animations::Animation
{
SAD_OBJECT
public:
	/*! An abstract fast call for rotating camera
	 */
	class AbstractFastCall: public sad::animations::AnimationFastCall
	{
	public: 
		/*! Translates a camera by vector
			\param[in] v vector
		 */
		virtual void call(const sad::Point3D& v) = 0;
		/*! Must be inherited
		 */
		virtual ~AbstractFastCall();
	};

	/*! A dummy fast call. Does nothing
	 */
	class DummyFastCall: public sad::animations::CameraShaking::AbstractFastCall
	{
	public: 
		/*! Does nothing
			\param[in] v vector
		 */
		virtual void call(const sad::Point3D& v);
		/*! Must be inherited
		 */
		virtual ~DummyFastCall();
	};

	/*! A real fast call for scene
	 */
	class FastCall: public  sad::animations::CameraShaking::AbstractFastCall
	{
	public: 
		/*! Sets a scene for call
			\param[in] s scene
		 */
		FastCall(sad::Scene* s);
		/*! Applies a translation to vector
			\param[in] v vector
		 */
		virtual void call(const sad::Point3D& v);
		/*! Must be inherited
		 */
		virtual ~FastCall();
	protected:
		/*! A sceme for rotating a camera
		 */
		sad::Scene* m_scene;
	};

	/*! Creates new empty animation
	 */
	CameraShaking();
	/*! Can be inherited
	 */
	virtual ~CameraShaking();
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v);
	/*! Sets offset of how far camera should be translated
		\param[in] offset offset of how far camera should be translated
	 */
	void setOffset(const sad::Point2D& offset);
	/*! Returns offset of how far camera should be translated
		\return offset
	 */
	const sad::Point2D & offset() const;
	/*! Sets frequency of how many times, should be changed offset of camera of scene
		\param[in] freq frequency
	 */
	void setFrequency(int freq);
	/*! Returns frequency of how many times, should be changed offset of camera of scene
		\return frequency
	 */
	int frequency() const;
	/*! Sets state of object from animation
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time);
    /*! Saves states of object in animation instance
        \param[in] i an animation instance
        \return whether we can work further with this object in instance
     */
    virtual bool saveState(sad::animations::Instance* i);
    /*! Resets state of object in animation instance, when animation ended
        \param[in] i an animation instance
     */
    virtual void resetState(sad::animations::Instance* i);
protected:
	/*! A vector of, how far should the camera be translated
	 */
	sad::Point2D m_offset;
	/*! A frequency of how many translations should be performed, when camera shake is finished
	 */
	int m_frequency;
};

}

}