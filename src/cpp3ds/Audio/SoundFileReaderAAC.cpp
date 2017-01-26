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
#include <cpp3ds/Audio/SoundFileReaderAAC.hpp>
#include <cpp3ds/System/InputStream.hpp>
#include <cpp3ds/System/Err.hpp>
#include <algorithm>
#include <cctype>
#include <cassert>
#include <iostream>
#include <cstring>
#include <cpp3ds/System/FileInputStream.hpp>
#include <cpp3ds/System/FileSystem.hpp>


namespace
{
	const cpp3ds::Uint64 mainChunkSize = 64;
}

namespace cpp3ds
{
namespace priv
{
////////////////////////////////////////////////////////////
bool SoundFileReaderAAC::check(InputStream& stream)
{
	unsigned char mainChunk[mainChunkSize];
	if (stream.read(mainChunk, sizeof(mainChunk)) != sizeof(mainChunk))
		return false;

	NeAACDecHandle handle = NeAACDecOpen();
	unsigned long samplerate;
	unsigned char channels;
	long ret = NeAACDecInit(handle, mainChunk, sizeof(mainChunk), &samplerate, &channels);
	NeAACDecClose(handle);
	if (ret != 0)
		return false;

	std::cout << "AAC Samplerate: " << samplerate << " Channels: " << (int)channels << " BytesRead: " << (int)ret << std::endl;

	return true;
}


////////////////////////////////////////////////////////////
SoundFileReaderAAC::SoundFileReaderAAC() :
m_stream        (NULL),
m_bytesPerSample(0),
m_dataStart     (0)
{
	m_handle = NeAACDecOpen();

	// Configure the library to our needs
	NeAACDecConfigurationPtr conf = NeAACDecGetCurrentConfiguration(m_handle);
	conf->outputFormat = FAAD_FMT_16BIT; // We only support 16bit audio
	conf->downMatrix = 1;                // Convert from 5.1 to stereo if required
	NeAACDecSetConfiguration(m_handle, conf);
}


////////////////////////////////////////////////////////////
SoundFileReaderAAC::~SoundFileReaderAAC()
{
	close();
}


////////////////////////////////////////////////////////////
bool SoundFileReaderAAC::open(InputStream& stream, Info& info)
{
	m_stream = &stream;

	if (!parseHeader(info))
	{
		err() << "Failed to open AAC sound file (invalid or unsupported file)" << std::endl;
		return false;
	}

	return true;
}


////////////////////////////////////////////////////////////
void SoundFileReaderAAC::seek(Uint64 sampleOffset)
{
	assert(m_stream);

	m_stream->seek(m_dataStart + sampleOffset * m_bytesPerSample);

	m_inputBufferPosition = m_inputBuffer.size();
	readChunk();
}


////////////////////////////////////////////////////////////
bool SoundFileReaderAAC::readChunk()
{
	Int64 bytesRead = 0;

	if (m_inputBufferPosition == 0)
		return true;

	// If there still exists some data in the buffer,
	// move it to beginning and fill buffer from there.
	if (m_inputBufferPosition < m_inputBuffer.size())
	{
		int bytesRemaining = m_inputBuffer.size() - m_inputBufferPosition;
		memmove(m_inputBuffer.data(), &m_inputBuffer[m_inputBufferPosition], bytesRemaining);
		bytesRead = m_stream->read(&m_inputBuffer[bytesRemaining], m_inputBufferPosition);
		bytesRead += bytesRemaining;
	}
	else // Buffer has been completed consumed
	{
		bytesRead = m_stream->read(m_inputBuffer.data(), m_inputBuffer.size());
	}

	if (!bytesRead)
		return false;
	if (bytesRead < m_inputBuffer.size())
	{
		// Last bytes in file don't fill whole buffer, align accordingly
		m_inputBufferPosition = m_inputBuffer.size() - bytesRead;
		memmove(&m_inputBuffer[m_inputBufferPosition], m_inputBuffer.data(), bytesRead);
	}
	else
		m_inputBufferPosition = 0;

    return true;
}


////////////////////////////////////////////////////////////
Uint64 SoundFileReaderAAC::read(Int16* samples, Uint64 maxCount)
{
	assert(m_stream);

	NeAACDecFrameInfo frameInfo;
	Uint32 count = 0;
	Uint8 *buf = m_sampleBuffer.data();

	while (count < maxCount)
	{
		size_t bytesLeft = static_cast<size_t>(maxCount - count) * sizeof(Int16);

		// Keep input buffer full
		if (m_inputBufferPosition > m_inputBuffer.size() - 1024)
		{
			if (!readChunk())
				break;
		}

		// Output from sample buffer until it needs to be refilled
		size_t bytesToCopy = std::min(bytesLeft, m_sampleBuffer.size() - m_sampleBufferPosition);
		if (bytesToCopy > 0)
		{
			memcpy(samples + count, buf + m_sampleBufferPosition, bytesToCopy);
			count += (bytesToCopy/sizeof(Int16));
			m_sampleBufferPosition += bytesToCopy;
		}

		// Keep decoded sample buffer filled
		if (m_sampleBufferPosition == m_sampleBuffer.size())
		{
			NeAACDecDecode2(m_handle, &frameInfo,
							m_inputBuffer.data() + m_inputBufferPosition,
							m_inputBuffer.size() - m_inputBufferPosition,
							reinterpret_cast<void**>(&buf),
							m_sampleBuffer.size());

			if (frameInfo.error > 0)
			{
//				std::cout << "Error (" << (int)frameInfo.error << "): " << NeAACDecGetErrorMessage(frameInfo.error) << std::endl;
				if (m_inputBufferPosition == 0)
				{
					std::cout << "Error (" << (int)frameInfo.error << "): " << NeAACDecGetErrorMessage(frameInfo.error) << std::endl;
					break;
				}
				if (frameInfo.error != 13 && frameInfo.error != 15)
					std::cout << "Need to fill input buffer! error: " << (int)frameInfo.error << std::endl;
				if (!readChunk())
					break;
			}
			else // Success
			{
				m_inputBufferPosition += frameInfo.bytesconsumed;
				if (frameInfo.samples)
				{
					int sampleCapacity = m_sampleBuffer.size() / sizeof(Int16);
					if (frameInfo.samples < sampleCapacity)
					{
						m_sampleBufferPosition = (sampleCapacity - frameInfo.samples) * sizeof(Int16);
						memmove(&m_sampleBuffer[m_sampleBufferPosition], m_sampleBuffer.data(), frameInfo.samples * sizeof(Int16));
					}
					else
						m_sampleBufferPosition = 0;
				}
			}
		}
	}

	return count;
}


////////////////////////////////////////////////////////////
bool SoundFileReaderAAC::parseHeader(Info& info)
{
	assert(m_stream);

	unsigned char mainChunk[mainChunkSize];
	if (m_stream->read(mainChunk, sizeof(mainChunk)) != sizeof(mainChunk))
		return false;

	long ret = NeAACDecInit(m_handle, mainChunk, sizeof(mainChunk), &m_sampleRate, &m_channelCount);
	if (ret != 0)
		return false;

	info.sampleCount = 0;
	info.channelCount = m_channelCount;
	info.sampleRate = m_sampleRate;

	// TODO: Figure these out
	m_dataStart = ret;
	m_bytesPerSample = 1;

	m_inputBuffer.resize(m_sampleRate);
	m_inputBufferPosition = m_inputBuffer.size();
	m_sampleBuffer.resize(1024*2 * sizeof(uint16_t) * m_channelCount);
	m_sampleBufferPosition = m_sampleBuffer.size();

	m_stream->seek(ret);

	return true;
}


////////////////////////////////////////////////////////////
void SoundFileReaderAAC::close()
{
	NeAACDecClose(m_handle);
}


} // namespace priv

} // namespace cpp3ds
