#include "imageformats/bmploader.h"

#include <iostream>


#include "util/chararrayibuf.h"
#include "util/fileistreambuf.h"

#define TAR7Z_SADDY

#include "3rdparty/tar7z/include/tar.h"

#pragma pack(push, 1)

namespace sad
{

namespace imageformats
{

struct BMPHeader
{
    unsigned short type;
    unsigned int  size;
    unsigned short reserv1;
    unsigned short reserv2;
    unsigned int  offsetbits;
};

struct BMPImageHeader
{
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitcount;
    unsigned int  compression;
    unsigned int  sizeimage;
    int           xpels;
    int           ypels;
    unsigned int  cused;
    unsigned int  cimportant;
};

}

}

#pragma pack(pop)

static const short bmp_magic_number = 19778;

// ========================================================================== PUBLIC METHODS ==========================================================================

bool sad::imageformats::BMPLoader::load(FILE * file, sad::Texture * texture)
{
    if (file == NULL || texture == NULL)
        return false;

    sad::util::FileIStreamBuf buf(file);
    return this->load(&buf, texture);
}

bool sad::imageformats::BMPLoader::load(tar7z::Entry* entry, sad::Texture* texture)
{
    if (entry == NULL || texture == NULL)
    {
        return false;
    }
    sad::util::CharArrayIBuf buf(entry->contents(), entry->contents() + entry->Size);
    return this->load(&buf, texture);
}

sad::imageformats::BMPLoader::~BMPLoader()
{

}

// ========================================================================== PROTECTED METHODS ==========================================================================

bool sad::imageformats::BMPLoader::load(std::streambuf* buf, sad::Texture* texture)
{
    if (buf == NULL || texture == NULL)
        return false;

    std::istream stream(buf);
    // Try to read header
    sad::imageformats::BMPHeader header;

    stream.read(reinterpret_cast<char*>(&header), sizeof(sad::imageformats::BMPHeader));
    if (stream.fail() || stream.eof())
    {
        return false;
    }

    // Check for magic number
    if (header.type != bmp_magic_number)
    {
        return false;
    }

    // Read image information
    sad::imageformats::BMPImageHeader image_header;
    stream.read(reinterpret_cast<char*>(&image_header), sizeof(sad::imageformats::BMPImageHeader));
    if (stream.fail() || stream.eof())
    {
        return false;
    }

    // Check whether image is supported
    bool image_unsupported = (image_header.bitcount != 24 && image_header.bitcount != 32);
    if (image_header.width == 0 || image_header.height == 0 || image_unsupported)
    {
        return false;
    }

    // Try to move to image data beginning to read it
    stream.seekg(header.offsetbits, std::ios_base::beg);
    if (stream.fail() || stream.eof())
    {
        return false;
    }

    unsigned char bypp = image_header.bitcount / 8;
    
    unsigned int width = image_header.width;
    unsigned int fileimagesize = image_header.width * image_header.height;  
    
    // Buffer
    sad::Texture::DefaultBuffer* newbuffer = new sad::Texture::DefaultBuffer();
    newbuffer->Data.resize(fileimagesize * 4, 255);

    int x = 0;
    int y = image_header.height - 1;
    
    sad::uchar * buffer = &(newbuffer->Data[0]);
    for (unsigned long i = 0; i < fileimagesize; i++)
    {
        unsigned int offset = 4 * (y * width + x);
        unsigned char * offset_buffer = buffer + offset;

        stream.read(reinterpret_cast<char*>(offset_buffer), bypp);
        if (stream.fail() || stream.eof())
        {
            delete newbuffer;
            return false;
        }
        
        std::swap(*offset_buffer, *(offset_buffer+2));
                
        x = x + 1;
        if (x == width)
        {
            y -= 1;
            x =  0;
        }
    }

    // Much better to do this in case
    texture->width() = width;
    texture->height() = image_header.height;
    texture->bpp() = 32;
    texture->Format = sad::Texture::SFT_R8_G8_B8_A8;
    delete texture->Buffer;
    texture->Buffer = newbuffer;
    return true;
}

