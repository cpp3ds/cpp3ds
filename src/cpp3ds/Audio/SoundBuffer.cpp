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
#include <cpp3ds/Audio/SoundBuffer.hpp>
#include <cpp3ds/Audio/InputSoundFile.hpp>
#include <cpp3ds/Audio/OutputSoundFile.hpp>
#include <cpp3ds/Audio/Sound.hpp>
//#include <cpp3ds/Audio/AudioDevice.hpp>
//#include <cpp3ds/Audio/ALCheck.hpp>
#include <cpp3ds/System/Err.hpp>
#include <cpp3ds/Resources.hpp>
#include <3ds.h>
#include <memory>
#include <string.h>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer() :
m_duration()
{
}


////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer(const SoundBuffer& copy) :
m_samples (copy.m_samples),
m_duration(copy.m_duration),
m_sounds  () // don't copy the attached sounds
{
    // Update the internal buffer with the new samples
    update(copy.getChannelCount(), copy.getSampleRate());
}


////////////////////////////////////////////////////////////
SoundBuffer::~SoundBuffer()
{
    // To prevent the iterator from becoming invalid, move the entire buffer to another
    // container. Otherwise calling resetBuffer would result in detachSound being
    // called which removes the sound from the internal list.
    SoundList sounds;
    sounds.swap(m_sounds);

    // Detach the buffer from the sounds that use it (to avoid OpenAL errors)
    for (SoundList::const_iterator it = sounds.begin(); it != sounds.end(); ++it)
        (*it)->resetBuffer();
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromFile(const std::string& filename)
{
    InputSoundFile file;
    if (file.openFromFile(filename))
        return initialize(file);
    else
        return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromMemory(const void* data, std::size_t sizeInBytes)
{
    InputSoundFile file;
    if (file.openFromMemory(data, sizeInBytes))
        return initialize(file);
    else
        return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromStream(InputStream& stream)
{
    InputSoundFile file;
    if (file.openFromStream(stream))
        return initialize(file);
    else
        return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromSamples(const Int16* samples, Uint64 sampleCount, unsigned int channelCount, unsigned int sampleRate)
{
	if (samples && sampleCount && channelCount && sampleRate)
	{
		// Copy the new audio samples
		m_samples.assign(samples, samples + sampleCount);

		// Update the internal buffer with the new samples
		return update(channelCount, sampleRate);
	}
	else
	{
		// Error...
		err() << "Failed to load sound buffer from samples ("
		<< "array: "      << samples      << ", "
		<< "count: "      << sampleCount  << ", "
		<< "channels: "   << channelCount << ", "
		<< "samplerate: " << sampleRate   << ")"
		<< std::endl;

		return false;
	}
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
    return m_samples.empty() ? NULL : &m_samples[0];
}


////////////////////////////////////////////////////////////
Uint64 SoundBuffer::getSampleCount() const
{
    return m_samples.size();
}


////////////////////////////////////////////////////////////
unsigned int SoundBuffer::getSampleRate() const
{
	return m_sampleRate;
}


////////////////////////////////////////////////////////////
unsigned int SoundBuffer::getChannelCount() const
{
    return m_channelCount;
}


////////////////////////////////////////////////////////////
Time SoundBuffer::getDuration() const
{
    return m_duration;
}


////////////////////////////////////////////////////////////
SoundBuffer& SoundBuffer::operator =(const SoundBuffer& right)
{
    SoundBuffer temp(right);

    std::swap(m_samples,     temp.m_samples);
    std::swap(m_duration,    temp.m_duration);
    std::swap(m_sounds,      temp.m_sounds); // swap sounds too, so that they are detached when temp is destroyed

    return *this;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::initialize(InputSoundFile& file)
{
    // Retrieve the sound parameters
    Uint64       sampleCount  = file.getSampleCount();
    unsigned int channelCount = file.getChannelCount();
    unsigned int sampleRate   = file.getSampleRate();

	// Read the samples from the provided file
	m_samples.resize(static_cast<std::size_t>(sampleCount));
	if (file.read(&m_samples[0], sampleCount) == sampleCount)
	{
		// Update the internal buffer with the new samples
		return update(channelCount, sampleRate);
	}
	else
	{
		return false;
	}
}


////////////////////////////////////////////////////////////
bool SoundBuffer::update(unsigned int channelCount, unsigned int sampleRate)
{
    // Check parameters
    if (!channelCount || !sampleRate || m_samples.empty())
        return false;

    // Check if the format is valid
    if (channelCount > 2){
        err() << "Failed to load sound buffer (unsupported number of channels: " << channelCount << ")" << std::endl;
        return false;
    }

	m_channelCount = channelCount;
	m_sampleRate = sampleRate;

    // First make a copy of the list of sounds so we can reattach later
    SoundList sounds(m_sounds);

    // Detach the buffer from the sounds that use it (to avoid OpenAL errors)
    for (SoundList::const_iterator it = sounds.begin(); it != sounds.end(); ++it)
        (*it)->resetBuffer();

    // Compute the duration
    m_duration = seconds(static_cast<float>(m_samples.size()) / sampleRate / channelCount);

    // Now reattach the buffer to the sounds that use it
    for (SoundList::const_iterator it = sounds.begin(); it != sounds.end(); ++it)
        (*it)->setBuffer(*this);

    return true;
}


////////////////////////////////////////////////////////////
void SoundBuffer::attachSound(Sound* sound) const
{
    m_sounds.insert(sound);
}


////////////////////////////////////////////////////////////
void SoundBuffer::detachSound(Sound* sound) const
{
    m_sounds.erase(sound);
}

} // namespace cpp3ds
