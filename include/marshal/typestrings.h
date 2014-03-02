/*! \file typestrings.h
	\author HiddenSeeker

	Here described a type string generator, which defines data properties
 */
#include <sadstring.h>
#include <sadvector.h>
#pragma once
namespace abstract_names
{

/*! This class defines uniques string fore each type used in program
	
	This strings are used, by Property class to check, whether valid PropertyEditor they
	return and also are used by PropertyFactory to identify which class to return
 */
template<typename T>
class type_string
{
public:
	/*! A type string which must be returned by PropertyEditor
		\return type string
	 */
	static sad::String type();  
};

}

#define DEFINE_PROPERTY_TYPESTRING( _type )  namespace abstract_names              \
										     {                                     \
											                                       \
                                              template<>	                       \
										      class type_string< _type >           \
                                              {                                    \
											  public:                              \
                                               static sad::String type()           \
											   { return sad::String( #_type ); }   \
                                              };                                   \
                                            }                                   
