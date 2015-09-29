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
#include <cpp3ds/Audio/Sound.hpp>
#include <cpp3ds/Audio/SoundBuffer.hpp>
//#include <cpp3ds/Audio/ALCheck.hpp>
#include <3ds.h>
#include <iostream>
#include <string.h>

namespace cpp3ds
{
////////////////////////////////////////////////////////////
Sound::Sound() :
m_buffer(NULL),
m_loop(false)
{
}


////////////////////////////////////////////////////////////
Sound::Sound(const SoundBuffer& buffer) :
m_buffer(NULL),
m_currentChannel(-1),
m_loop(false)
{
    setBuffer(buffer);
}


////////////////////////////////////////////////////////////
Sound::Sound(const Sound& copy) :
SoundSource(copy),
m_buffer   (NULL)
{
    if (copy.m_buffer)
        setBuffer(*copy.m_buffer);
    setLoop(copy.getLoop());
}


////////////////////////////////////////////////////////////
Sound::~Sound()
{
    stop();
    if (m_buffer)
        m_buffer->detachSound(this);
}


////////////////////////////////////////////////////////////
void Sound::play()
{
	if (!m_buffer || m_buffer->getSampleCount() == 0)
		return;

	// 24 possible CSND channels to cycle through (0-7) reserved for DSP
	static int channel;
	channel++;
	channel %= 24;

	u32 flags = SOUND_FORMAT_16BIT;
	if (m_loop)
		flags |= SOUND_REPEAT;
	u32 size = sizeof(Int16) * m_buffer->getSampleCount();
	std::cout << "Playing sound: " << m_buffer->getSampleCount();
	GSPGPU_FlushDataCache(NULL, (u8*)m_buffer->getSamples(), size);
	csndPlaySound(channel+8, flags, m_buffer->getSampleRate(), 1.0, 0.0, (u32*)m_buffer->getSamples(), (u32*)m_buffer->getSamples(), size);
}


////////////////////////////////////////////////////////////
void Sound::pause()
{

}


////////////////////////////////////////////////////////////
void Sound::stop()
{

}


////////////////////////////////////////////////////////////
void Sound::setBuffer(const SoundBuffer& buffer)
{
    // First detach from the previous buffer
    if (m_buffer)
    {
        stop();
        m_buffer->detachSound(this);
    }

    // Assign and use the new buffer
    m_buffer = &buffer;
    m_buffer->attachSound(this);
}


////////////////////////////////////////////////////////////
void Sound::setLoop(bool loop)
{
	m_loop = loop;
}


////////////////////////////////////////////////////////////
void Sound::setPlayingOffset(Time timeOffset)
{

}


////////////////////////////////////////////////////////////
const SoundBuffer* Sound::getBuffer() const
{
    return m_buffer;
}


////////////////////////////////////////////////////////////
bool Sound::getLoop() const
{
    return m_loop;
}


////////////////////////////////////////////////////////////
Time Sound::getPlayingOffset() const
{
    return seconds(0);
}


////////////////////////////////////////////////////////////
Sound::Status Sound::getStatus() const
{
    return SoundSource::getStatus();
}


////////////////////////////////////////////////////////////
Sound& Sound::operator =(const Sound& right)
{
    // Here we don't use the copy-and-swap idiom, because it would mess up
    // the list of sound instances contained in the buffers

    // Detach the sound instance from the previous buffer (if any)
    if (m_buffer)
    {
        stop();
        m_buffer->detachSound(this);
        m_buffer = NULL;
    }

    // Copy the sound attributes
    if (right.m_buffer)
        setBuffer(*right.m_buffer);
    setLoop(right.getLoop());
    setPitch(right.getPitch());
    setVolume(right.getVolume());
    setPosition(right.getPosition());
    setRelativeToListener(right.isRelativeToListener());
    setMinDistance(right.getMinDistance());
    setAttenuation(right.getAttenuation());

    return *this;
}


////////////////////////////////////////////////////////////
void Sound::resetBuffer()
{
    // First stop the sound in case it is playing
    stop();

    // Detach the buffer
    if (m_buffer)
    {
        m_buffer->detachSound(this);
        m_buffer = NULL;
    }
}

} // namespace cpp3ds
