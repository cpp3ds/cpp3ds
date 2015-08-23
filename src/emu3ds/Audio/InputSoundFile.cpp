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
#include <cpp3ds/Audio/InputSoundFile.hpp>
#include <cpp3ds/Audio/SoundFileReader.hpp>
#include <cpp3ds/Audio/SoundFileFactory.hpp>
#include <cpp3ds/System/InputStream.hpp>
#include <cpp3ds/System/FileInputStream.hpp>
#include <cpp3ds/System/MemoryInputStream.hpp>
#include <cpp3ds/System/Err.hpp>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
InputSoundFile::InputSoundFile() :
m_reader      (NULL),
m_stream      (NULL),
m_streamOwned (false),
m_sampleCount (0),
m_channelCount(0),
m_sampleRate  (0)
{
}


////////////////////////////////////////////////////////////
InputSoundFile::~InputSoundFile()
{
}


////////////////////////////////////////////////////////////
bool InputSoundFile::openFromFile(const std::string& filename)
{
    return m_inputSoundFile.openFromFile(filename);
}


////////////////////////////////////////////////////////////
bool InputSoundFile::openFromMemory(const void* data, std::size_t sizeInBytes)
{
    return m_inputSoundFile.openFromMemory(data, sizeInBytes);
}


////////////////////////////////////////////////////////////
bool InputSoundFile::openFromStream(InputStream& stream)
{
	// TODO: don't cast here
    return m_inputSoundFile.openFromStream(reinterpret_cast<sf::InputStream&>(stream));
}


////////////////////////////////////////////////////////////
Uint64 InputSoundFile::getSampleCount() const
{
    return m_sampleCount;
}


////////////////////////////////////////////////////////////
unsigned int InputSoundFile::getChannelCount() const
{
    return m_channelCount;
}


////////////////////////////////////////////////////////////
unsigned int InputSoundFile::getSampleRate() const
{
    return m_sampleRate;
}


////////////////////////////////////////////////////////////
Time InputSoundFile::getDuration() const
{
    return seconds(m_inputSoundFile.getDuration().asSeconds());
}


////////////////////////////////////////////////////////////
void InputSoundFile::seek(Uint64 sampleOffset)
{
    if (m_reader)
        m_reader->seek(sampleOffset);
}


////////////////////////////////////////////////////////////
void InputSoundFile::seek(Time timeOffset)
{
    seek(static_cast<Uint64>(timeOffset.asSeconds() * m_sampleRate * m_channelCount));
}


////////////////////////////////////////////////////////////
Uint64 InputSoundFile::read(Int16* samples, Uint64 maxCount)
{
    if (m_reader && samples && maxCount)
        return m_reader->read(samples, maxCount);
    else
        return 0;
}


////////////////////////////////////////////////////////////
void InputSoundFile::close()
{
}


} // namespace cpp3ds
