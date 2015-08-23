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
#include <SFML/System/InputStream.hpp>
#include <cpp3ds/Audio/SoundBuffer.hpp>
#include <cpp3ds/Audio/InputSoundFile.hpp>
#include <cpp3ds/Audio/OutputSoundFile.hpp>
#include <cpp3ds/System/InputStream.hpp>
#include <cpp3ds/Audio/Sound.hpp>
//#include <cpp3ds/Audio/AudioDevice.hpp>
//#include <cpp3ds/Audio/ALCheck.hpp>
#include <cpp3ds/System/Err.hpp>
#include <memory>
#include <cpp3ds/Resources.hpp>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer() :
m_buffer  (0),
m_duration()
{
}


////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer(const SoundBuffer& copy) :
m_buffer  (0),
m_samples (copy.m_samples),
m_duration(copy.m_duration),
m_sounds  () // don't copy the attached sounds
{
}


////////////////////////////////////////////////////////////
SoundBuffer::~SoundBuffer()
{
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromFile(const std::string& filename)
{
	return m_soundBuffer.loadFromFile(filename);
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromResource(const std::string& filename)
{
	return loadFromMemory(priv::resources[filename].data, priv::resources[filename].size);
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromMemory(const void* data, std::size_t sizeInBytes)
{
	return m_soundBuffer.loadFromMemory(data, sizeInBytes);
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromStream(InputStream& stream)
{
	// TODO: casting could break things
	return m_soundBuffer.loadFromStream(reinterpret_cast<sf::InputStream&>(stream));
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromSamples(const Int16* samples, Uint64 sampleCount, unsigned int channelCount, unsigned int sampleRate)
{
	return m_soundBuffer.loadFromSamples(samples, sampleCount, channelCount, sampleRate);
}


////////////////////////////////////////////////////////////
bool SoundBuffer::saveToFile(const std::string& filename) const
{
    // Create the sound file in write mode
    OutputSoundFile file;
    if (file.openFromFile(filename, getSampleRate(), getChannelCount()))
    {
        // Write the samples to the opened file
        file.write(&m_samples[0], m_samples.size());

        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
const Int16* SoundBuffer::getSamples() const
{
    return m_soundBuffer.getSamples();
}


////////////////////////////////////////////////////////////
Uint64 SoundBuffer::getSampleCount() const
{
    return m_soundBuffer.getSampleCount();
}


////////////////////////////////////////////////////////////
unsigned int SoundBuffer::getSampleRate() const
{
	return m_soundBuffer.getSampleRate();
}


////////////////////////////////////////////////////////////
unsigned int SoundBuffer::getChannelCount() const
{
    return m_soundBuffer.getChannelCount();
}


////////////////////////////////////////////////////////////
Time SoundBuffer::getDuration() const
{
    return seconds(m_soundBuffer.getDuration().asSeconds());
}


////////////////////////////////////////////////////////////
SoundBuffer& SoundBuffer::operator =(const SoundBuffer& right)
{
    SoundBuffer temp(right);

    std::swap(m_samples,  temp.m_samples);
    std::swap(m_buffer,   temp.m_buffer);
    std::swap(m_duration, temp.m_duration);
    std::swap(m_sounds,   temp.m_sounds); // swap sounds too, so that they are detached when temp is destroyed

    return *this;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::initialize(InputSoundFile& file)
{
	return true;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::update(unsigned int channelCount, unsigned int sampleRate)
{
    return true;
}


////////////////////////////////////////////////////////////
void SoundBuffer::attachSound(Sound* sound) const
{
}


////////////////////////////////////////////////////////////
void SoundBuffer::detachSound(Sound* sound) const
{
}

} // namespace cpp3ds
