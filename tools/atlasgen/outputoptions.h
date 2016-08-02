/*! \file outputoptions.h

    An options for writing output files of texture atlas
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QString>
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QHash>
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QVariant>

namespace tar7z
{
class Archive;    
}

/*! An options for writing files
 */
struct OutputOptions
{
    /*! A program options list
     */
    QHash<QString, QVariant>* ProgramOptions;    
    /*! An archive to be written to (NULL if data should be written to file)
     */
    tar7z::Archive* Archive;
    /*! A return code for program
     */
    int ReturnCode;

    /*! Converts name to compatible with tar7z
        \param[in] name compatible name
        \return name
     */
    inline static QString tar7zCompatibleName(QString name)
    {
        QStringList parts = name.split("/");
        if (parts.size() == 1)
        {
            parts = name.split("\\");
        }
        if (parts.size() == 0)
        {
            parts << "";
        }
        return parts[parts.size() - 1];
    }
};
