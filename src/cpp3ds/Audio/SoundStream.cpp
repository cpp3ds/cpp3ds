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
#include <cpp3ds/Audio/SoundStream.hpp>
#include <cpp3ds/System/Sleep.hpp>
#include <cpp3ds/System/Err.hpp>
#include <cpp3ds/System/Lock.hpp>
#include <string.h>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
SoundStream::SoundStream()
: m_thread          (&SoundStream::streamData, this)
, m_threadMutex     ()
, m_threadStartState(Stopped)
, m_isStreaming     (false)
, m_channelCount    (0)
, m_sampleRate      (0)
, m_format          (0)
, m_loop            (false)
, m_samplesProcessed(0)
{

}


////////////////////////////////////////////////////////////
SoundStream::~SoundStream()
{
    // Stop the sound if it was playing

    // Request the thread to terminate
    {
        Lock lock(m_threadMutex);
        m_isStreaming = false;
    }

    // Wait for the thread to terminate
    m_thread.wait();
}


////////////////////////////////////////////////////////////
void SoundStream::initialize(unsigned int channelCount, unsigned int sampleRate)
{
    m_channelCount = channelCount;
    m_sampleRate   = sampleRate;

    // Check if the format is valid
    if (channelCount > 2)
    {
        m_channelCount = 0;
        m_sampleRate   = 0;
        err() << "Unsupported number of channels (" << m_channelCount << ")" << std::endl;
    }
	else
		m_format = 1;
}


////////////////////////////////////////////////////////////
void SoundStream::play()
{
    // Check if the sound parameters have been set
    if (m_format == 0)
    {
        err() << "Failed to play audio stream: sound parameters have not been initialized (call initialize() first)" << std::endl;
        return;
    }

	m_channel = 0;
	while (m_channel < 24 && ndspChnIsPlaying(m_channel))
		m_channel++;

	if (m_channel == 24) {
		err() << "Failed to play audio stream: all channels are in use." << std::endl;
		m_channel = -1;
		return;
	}

	ndspChnReset(m_channel);
	ndspChnSetInterp(m_channel, NDSP_INTERP_POLYPHASE);
	ndspChnSetRate(m_channel, float(m_sampleRate));
	ndspChnSetFormat(m_channel, (m_channelCount == 1) ? NDSP_FORMAT_MONO_PCM16 : NDSP_FORMAT_STEREO_PCM16);

	bool isStreaming = false;
    Status threadStartState = Stopped;

    {
        Lock lock(m_threadMutex);

        isStreaming = m_isStreaming;
        threadStartState = m_threadStartState;
    }


    if (isStreaming && (threadStartState == Paused))
    {
        // If the sound is paused, resume it
		setPlayingOffset(m_pauseOffset);

		Lock lock(m_threadMutex);
		m_threadStartState = Playing;

//		for (int i = 0; i < BufferCount; ++i)
//			m_endBuffers[i] = false;

		// Fill the queue
//		fillQueue();
        return;
    }
    else if (isStreaming && (threadStartState == Playing))
    {
        // If the sound is playing, stop it and continue as if it was stopped
        stop();
    }

    // Move to the beginning
    onSeek(Time::Zero);

    // Start updating the stream in a separate thread to avoid blocking the application
    m_samplesProcessed = 0;
    m_isStreaming = true;
    m_threadStartState = Playing;
    m_thread.launch();
}


////////////////////////////////////////////////////////////
void SoundStream::pause()
{
    // Handle pause() being called before the thread has started
    {
        Lock lock(m_threadMutex);

        if (!m_isStreaming)
            return;

        m_threadStartState = Paused;

		m_pauseOffset = getPlayingOffset();
		clearQueue();
    }
}


////////////////////////////////////////////////////////////
void SoundStream::stop()
{
    // Request the thread to terminate
    {
        Lock lock(m_threadMutex);
        m_isStreaming = false;
    }

    // Wait for the thread to terminate
    m_thread.wait();

    // Move to the beginning
    onSeek(Time::Zero);

    // Reset the playing position
    m_samplesProcessed = 0;
}


////////////////////////////////////////////////////////////
unsigned int SoundStream::getChannelCount() const
{
    return m_channelCount;
}


////////////////////////////////////////////////////////////
unsigned int SoundStream::getSampleRate() const
{
    return m_sampleRate;
}


////////////////////////////////////////////////////////////
SoundStream::Status SoundStream::getStatus() const
{
    Status status = SoundSource::getStatus();

    // To compensate for the lag between play() and alSourceplay()
    if (status == Stopped)
    {
        Lock lock(m_threadMutex);

        if (m_isStreaming)
            status = m_threadStartState;
    }

    return status;
}


////////////////////////////////////////////////////////////
void SoundStream::setPlayingOffset(Time timeOffset)
{
    // Get old playing status
    Status oldStatus = getStatus();

    // Stop the stream
    stop();

    // Let the derived class update the current position
    onSeek(timeOffset);

    // Restart streaming
    m_samplesProcessed = static_cast<Uint64>(timeOffset.asSeconds() * m_sampleRate * m_channelCount);

    if (oldStatus == Stopped)
        return;

    m_isStreaming = true;
    m_threadStartState = oldStatus;
    m_thread.launch();
}


////////////////////////////////////////////////////////////
Time SoundStream::getPlayingOffset() const
{
    if (m_sampleRate && m_channelCount)
    {
		u32 samplePos = ndspChnGetSamplePos(m_channel);

		// In case current buffer has finished and yet to be updated to next in queue
		if (m_ndspWaveBuf.status == NDSP_WBUF_DONE)
		if (ndspChnGetWaveBufSeq(m_channel) != m_ndspWaveBuf.sequence_id)
			samplePos += m_ndspWaveBuf.nsamples;

        return seconds(static_cast<float>(m_samplesProcessed + samplePos) / m_sampleRate / m_channelCount);
    }
    else
    {
        return Time::Zero;
    }
}


////////////////////////////////////////////////////////////
void SoundStream::setLoop(bool loop)
{
    m_loop = loop;
}


////////////////////////////////////////////////////////////
bool SoundStream::getLoop() const
{
    return m_loop;
}


////////////////////////////////////////////////////////////
void SoundStream::streamData()
{
    bool requestStop = false;

    {
        Lock lock(m_threadMutex);

        // Check if the thread was launched Stopped
        if (m_threadStartState == Stopped)
        {
            m_isStreaming = false;
            return;
        }
    }

    // Create the buffers
    for (int i = 0; i < BufferCount; ++i)
        m_endBuffers[i] = false;

    // Fill the queue to start playing
    requestStop = fillQueue();

    {
        Lock lock(m_threadMutex);

        // Check if the thread was launched Paused
        if (m_threadStartState == Paused) {
//			std::cout << "thread launched while paused" << std::endl;
		}
    }

    for (;;)
    {
        {
            Lock lock(m_threadMutex);
            if (!m_isStreaming)
                break;
        }

        // The stream has been interrupted!
        if (SoundSource::getStatus() == Stopped)
        {
            if (!requestStop)
            {
                // Just continue
//                alCheck(alSourcePlay(m_source));
            }
            else
            {
                // End streaming
                Lock lock(m_threadMutex);
                m_isStreaming = false;
            }
        }

		if (SoundSource::getStatus() != Paused)
        // Get the number of buffers that have been processed (i.e. ready for reuse)
		for (int i = 0; i < BufferCount; ++i)
		{
			if (m_ndspWaveBuffers[i].status == NDSP_WBUF_DONE)
			{
				// Retrieve its size and add it to the samples count
				if (m_endBuffers[i]) {
					// This was the last buffer: reset the sample count
					m_samplesProcessed = 0;
					m_endBuffers[i] = false;
				}
				else {
					m_samplesProcessed += m_ndspWaveBuffers[i].nsamples;
				}

				// Fill it and push it back into the playing queue
				if (!requestStop) {
					if (fillAndPushBuffer(i))
						requestStop = true;
				}
			}
		}
        // Leave some time for the other threads if the stream is still playing
        if (SoundSource::getStatus() != Stopped)
            sleep(milliseconds(10));
    }

    // Stop the playback
    // Dequeue any buffer left in the queue
    clearQueue();

	m_ndspWaveBuf.status = NDSP_WBUF_DONE;
	m_pauseOffset = Time::Zero;

	for (int i = 0; i < BufferCount; ++i)
		m_buffers->clear();
}


////////////////////////////////////////////////////////////
bool SoundStream::fillAndPushBuffer(unsigned int bufferNum)
{
    bool requestStop = false;

    // Acquire audio data
    Chunk data = {NULL, 0};
    if (!onGetData(data))
    {
        // Mark the buffer as the last one (so that we know when to reset the playing position)
        m_endBuffers[bufferNum] = true;

        // Check if the stream must loop or stop
        if (m_loop)
        {
            // Return to the beginning of the stream source
            onSeek(Time::Zero);

            // If we previously had no data, try to fill the buffer once again
            if (!data.samples || (data.sampleCount == 0))
            {
                return fillAndPushBuffer(bufferNum);
            }
        }
        else
        {
            // Not looping: request stop
            requestStop = true;
        }
    }

    // Fill the buffer if some data was returned
    if (data.samples && data.sampleCount)
    {
        auto& buffer = m_buffers[bufferNum];
		auto& ndspBuffer = m_ndspWaveBuffers[bufferNum];

        // Fill the buffer
		buffer.assign(data.samples, data.samples + data.sampleCount);

		memset(&ndspBuffer, 0, sizeof(ndspWaveBuf));
		ndspBuffer.data_vaddr = reinterpret_cast<u32>(&buffer[0]);
		ndspBuffer.nsamples = data.sampleCount;
		ndspBuffer.looping = false;
		ndspBuffer.status = NDSP_WBUF_FREE;

		DSP_FlushDataCache((u8*)&buffer[0], data.sampleCount);

        // Push it into the sound queue
		ndspChnWaveBufAdd(m_channel, &ndspBuffer);

		m_ndspWaveBuf = ndspBuffer;
    }

    return requestStop;
}


////////////////////////////////////////////////////////////
bool SoundStream::fillQueue()
{
    // Fill and enqueue all the available buffers
    bool requestStop = false;
    for (int i = 0; (i < BufferCount) && !requestStop; ++i)
    {
        if (fillAndPushBuffer(i))
            requestStop = true;
		sleep(milliseconds(10));
    }

    return requestStop;
}


////////////////////////////////////////////////////////////
void SoundStream::clearQueue()
{
	ndspChnWaveBufClear(m_channel);

	m_ndspWaveBuf.status = NDSP_WBUF_FREE;
}

} // namespace cpp3ds
