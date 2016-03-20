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
#include <cpp3ds/Audio/SoundSource.hpp>
#include <3ds/ndsp/ndsp.h>

namespace cpp3ds
{
////////////////////////////////////////////////////////////
SoundSource::SoundSource()
: m_channel(-1)
{

}


////////////////////////////////////////////////////////////
SoundSource::SoundSource(const SoundSource& copy)
: m_channel(copy.m_channel)
{
    setPitch(copy.getPitch());
    setVolume(copy.getVolume());
    setPosition(copy.getPosition());
    setRelativeToListener(copy.isRelativeToListener());
    setMinDistance(copy.getMinDistance());
    setAttenuation(copy.getAttenuation());
}


////////////////////////////////////////////////////////////
SoundSource::~SoundSource()
{

}


////////////////////////////////////////////////////////////
void SoundSource::setPitch(float pitch)
{

}


////////////////////////////////////////////////////////////
void SoundSource::setVolume(float volume)
{

}

////////////////////////////////////////////////////////////
void SoundSource::setPosition(float x, float y, float z)
{

}


////////////////////////////////////////////////////////////
void SoundSource::setPosition(const Vector3f& position)
{
    setPosition(position.x, position.y, position.z);
}


////////////////////////////////////////////////////////////
void SoundSource::setRelativeToListener(bool relative)
{

}


////////////////////////////////////////////////////////////
void SoundSource::setMinDistance(float distance)
{

}


////////////////////////////////////////////////////////////
void SoundSource::setAttenuation(float attenuation)
{

}


////////////////////////////////////////////////////////////
float SoundSource::getPitch() const
{
	return 0;
}


////////////////////////////////////////////////////////////
float SoundSource::getVolume() const
{
	return 100;
}


////////////////////////////////////////////////////////////
Vector3f SoundSource::getPosition() const
{
    Vector3f position;

    return position;
}


////////////////////////////////////////////////////////////
bool SoundSource::isRelativeToListener() const
{
	return true;
}


////////////////////////////////////////////////////////////
float SoundSource::getMinDistance() const
{
	return 0;
}


////////////////////////////////////////////////////////////
float SoundSource::getAttenuation() const
{
	return 0;
}


////////////////////////////////////////////////////////////
SoundSource& SoundSource::operator =(const SoundSource& right)
{
	// Assign the sound attributes
	setPitch(right.getPitch());
	setVolume(right.getVolume());
	setPosition(right.getPosition());
	setRelativeToListener(right.isRelativeToListener());
	setMinDistance(right.getMinDistance());
	setAttenuation(right.getAttenuation());

	m_channel = right.m_channel;
	m_ndspWaveBuf = right.m_ndspWaveBuf;

	return *this;
}

////////////////////////////////////////////////////////////
SoundSource::Status SoundSource::getStatus() const
{
	if (m_channel == -1)
		return Stopped;

	if (ndspChnIsPaused(m_channel))
		return Paused;

	if (ndspChnIsPlaying(m_channel))
		return Playing;

	return Stopped;
}

} // namespace cpp3ds
