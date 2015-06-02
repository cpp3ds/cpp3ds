////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/System/FileInputStream.hpp>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
FileInputStream::FileInputStream()
: m_file(NULL)
{

}


////////////////////////////////////////////////////////////
FileInputStream::~FileInputStream()
{
    if (m_file)
        std::fclose(m_file);
}


////////////////////////////////////////////////////////////
bool FileInputStream::open(const std::string& filename)
{
    if (m_file)
        std::fclose(m_file);

	#ifdef EMULATION
		std::string rel_filename = "res/sdmc/" + filename;
		m_file = std::fopen(rel_filename.c_str(), "rb");
	#else
    	m_file = std::fopen(filename.c_str(), "rb");
	#endif

    return m_file != NULL;
}


////////////////////////////////////////////////////////////
Int64 FileInputStream::read(void* data, Int64 size)
{
    if (m_file)
        return std::fread(data, 1, static_cast<std::size_t>(size), m_file);
    else
        return -1;
}


////////////////////////////////////////////////////////////
Int64 FileInputStream::seek(Int64 position)
{
    if (m_file)
    {
        std::fseek(m_file, static_cast<std::size_t>(position), SEEK_SET);
        return tell();
    }
    else
    {
        return -1;
    }
}


////////////////////////////////////////////////////////////
Int64 FileInputStream::tell()
{
    if (m_file)
        return std::ftell(m_file);
    else
        return -1;
}


////////////////////////////////////////////////////////////
Int64 FileInputStream::getSize()
{
    if (m_file)
    {
        cpp3ds::Int64 position = tell();
        std::fseek(m_file, 0, SEEK_END);
        cpp3ds::Int64 size = tell();
        seek(position);
        return size;
    }
    else
    {
        return -1;
    }
}

} // namespace cpp3ds
