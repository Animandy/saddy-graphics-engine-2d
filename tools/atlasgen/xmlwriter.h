/*! \file xmlwriter.h
    

    Defines a writer for writing output configs atlases in XML Format
 */
#pragma once
#include "atlas.h"
#include "writer.h"

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>


/*! Writes output atlas config as XML
 */
class XMLWriter: public Writer
{
public:
    /*! Constructs new reader
     */
    XMLWriter();
    /*! Can be inherited
     */
    virtual ~XMLWriter();
    /*! Writes an atlas to output file
        \param[in] name a reading name
        \param[in, out] opts options
        \return result
     */
    virtual bool write(
        const Atlas& atlas,
        OutputOptions& opts
    );
};
