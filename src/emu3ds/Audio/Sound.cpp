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

namespace cpp3ds
{
////////////////////////////////////////////////////////////
Sound::Sound() :
		m_buffer(NULL),
		m_sound()
{
}


////////////////////////////////////////////////////////////
Sound::Sound(const SoundBuffer& buffer) :
		m_buffer(NULL),
		m_sound()
{
	setBuffer(buffer);
}


////////////////////////////////////////////////////////////
Sound::Sound(const Sound& copy) :
		SoundSource(copy),
		m_buffer   (NULL),
		m_sound(copy.m_sound)
{
	if (copy.m_buffer)
		setBuffer(*copy.m_buffer);
	setLoop(copy.getLoop());
}


////////////////////////////////////////////////////////////
Sound::~Sound()
{

}


////////////////////////////////////////////////////////////
void Sound::play()
{
	m_sound.play();
}


////////////////////////////////////////////////////////////
void Sound::pause()
{
	m_sound.pause();
}


////////////////////////////////////////////////////////////
void Sound::stop()
{
	m_sound.stop();
}


////////////////////////////////////////////////////////////
void Sound::setBuffer(const SoundBuffer& buffer)
{
	m_sound.setBuffer(buffer.m_soundBuffer);
}


////////////////////////////////////////////////////////////
void Sound::setLoop(bool loop)
{
	m_sound.setLoop(loop);
}


////////////////////////////////////////////////////////////
void Sound::setPlayingOffset(Time timeOffset)
{
	m_sound.setPlayingOffset(sf::seconds(timeOffset.asSeconds()));
}


////////////////////////////////////////////////////////////
const SoundBuffer* Sound::getBuffer() const
{
	return m_buffer;
}


////////////////////////////////////////////////////////////
bool Sound::getLoop() const
{
	return m_sound.getLoop();
}


////////////////////////////////////////////////////////////
Time Sound::getPlayingOffset() const
{
	return seconds(m_sound.getPlayingOffset().asSeconds());
}


////////////////////////////////////////////////////////////
Sound::Status Sound::getStatus() const
{
	return static_cast<Sound::Status>(m_sound.getStatus());
}


////////////////////////////////////////////////////////////
Sound& Sound::operator =(const Sound& right)
{
	m_sound = right.m_sound;

	return *this;
}


////////////////////////////////////////////////////////////
void Sound::resetBuffer()
{
	m_sound.resetBuffer();
}

} // namespace cpp3ds
