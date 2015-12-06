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
#include <3ds.h>
#include <string.h>
#include <cpp3ds/System/Err.hpp>

namespace cpp3ds
{

////////////////////////////////////////////////////////////
Sound::Sound()
: m_buffer(nullptr)
, m_loop(false)
, m_isADPCM(false)
{
}


////////////////////////////////////////////////////////////
Sound::Sound(const SoundBuffer& buffer)
: m_buffer(nullptr)
, m_loop(false)
, m_isADPCM(false)
{
    setBuffer(buffer);
}


////////////////////////////////////////////////////////////
Sound::Sound(const Sound& copy)
: SoundSource(copy)
, m_buffer(nullptr)
{
    if (copy.m_buffer)
        setBuffer(*copy.m_buffer);
	m_pauseOffset = copy.m_pauseOffset;
    setLoop(copy.getLoop());
	setStateADPCM(copy.getStateADPCM());
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
	if (getStatus() == Playing)
		stop();

	m_channel = 0;
	while (m_channel < 24 && ndspChnIsPlaying(m_channel))
		m_channel++;

	if (m_channel == 24) {
		err() << "Sound::play() failed because all channels are in use." << std::endl;
		m_channel = -1;
		return;
	}

	setPlayingOffset(m_pauseOffset);

	if (m_pauseOffset != Time::Zero)
		m_pauseOffset = Time::Zero;

	u32 size = sizeof(Int16) * m_buffer->getSampleCount();

	ndspChnReset(m_channel);
	ndspChnSetInterp(m_channel, NDSP_INTERP_POLYPHASE);
	ndspChnSetRate(m_channel, float(m_buffer->getSampleRate()));
	ndspChnSetFormat(m_channel, (m_buffer->getChannelCount() == 1) ? NDSP_FORMAT_MONO_PCM16 : NDSP_FORMAT_STEREO_PCM16);

	DSP_FlushDataCache((u8*)m_buffer->getSamples(), size);

	ndspChnWaveBufAdd(m_channel, &m_ndspWaveBuf);
}


////////////////////////////////////////////////////////////
void Sound::pause()
{
	if (getStatus() != Playing)
		return;

	m_pauseOffset = getPlayingOffset();

	ndspChnWaveBufClear(m_channel);
}


////////////////////////////////////////////////////////////
void Sound::stop()
{
	if (getStatus() == Stopped)
		return;
	m_pauseOffset = Time::Zero;
	ndspChnWaveBufClear(m_channel);
	m_ndspWaveBuf.status = NDSP_WBUF_DONE;
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

	memset(&m_ndspWaveBuf, 0, sizeof(ndspWaveBuf));
	m_ndspWaveBuf.data_vaddr = buffer.getSamples();
	m_ndspWaveBuf.nsamples = buffer.getSampleCount();
	m_ndspWaveBuf.looping = m_loop; // Loop enabled
	m_ndspWaveBuf.status = NDSP_WBUF_FREE;

    // Assign and use the new buffer
    m_buffer = &buffer;
    m_buffer->attachSound(this);
}


////////////////////////////////////////////////////////////
void Sound::setStateADPCM(bool enable)
{
	m_isADPCM = enable;
}


////////////////////////////////////////////////////////////
bool Sound::getStateADPCM() const
{
	return m_isADPCM;
}


////////////////////////////////////////////////////////////
void Sound::setLoop(bool loop)
{
	m_loop = loop;
	m_ndspWaveBuf.looping = m_loop;
}


////////////////////////////////////////////////////////////
void Sound::setPlayingOffset(Time timeOffset)
{
	Status status = getStatus();
	stop();
	m_playOffset = timeOffset;
	int offset = m_buffer->getSampleRate() * m_buffer->getChannelCount() * timeOffset.asSeconds();
	m_ndspWaveBuf.data_vaddr = m_buffer->getSamples() + offset;
	m_ndspWaveBuf.nsamples = m_buffer->getSampleCount() - offset;
	if (status == Playing)
		ndspChnWaveBufAdd(m_channel, &m_ndspWaveBuf);
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
	if (getStatus() == Stopped)
		return Time::Zero;
	if (getStatus() == Paused)
		return m_pauseOffset;

	u32 samplePos = ndspChnGetSamplePos(m_channel);
	return seconds(static_cast<float>(samplePos) / m_buffer->getSampleRate()) + m_playOffset;
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
