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
#include <cpp3ds/Audio/SoundRecorder.hpp>
#include <cpp3ds/System/Sleep.hpp>
#include <cpp3ds/System/Err.hpp>
#include <cstring>
#include <malloc.h>
#include <cpp3ds/System/Service.hpp>


namespace cpp3ds
{

static bool isInitialized = false;


////////////////////////////////////////////////////////////
SoundRecorder::SoundRecorder() :
m_thread            (&SoundRecorder::record, this),
m_sampleRate        (0),
m_processingInterval(milliseconds(100)),
m_isCapturing       (false)
{
	// Set the device name to the default device
	m_deviceName = getDefaultDevice();

	if (!isInitialized){
		isInitialized = true;
		Service::enable(Microphone);
	}

	m_bufferSize = micGetSampleDataSize();
}


////////////////////////////////////////////////////////////
SoundRecorder::~SoundRecorder()
{
}


////////////////////////////////////////////////////////////
bool SoundRecorder::start(SampleRate sampleRate)
{
	// Check if the device can do audio capture
	if (!isAvailable())
	{
		err() << "Failed to start capture: your system cannot capture audio data (call SoundRecorder::isAvailable to check it)" << std::endl;
		return false;
	}

	// Check that another capture is not already running
	if (m_isCapturing)
	{
		err() << "Trying to start audio capture, but another capture is already running" << std::endl;
		return false;
	}

	// Clear the array of samples
	m_samples.clear();

	// Store the sample rate
	if (sampleRate == SampleRate_32730)
		m_sampleRate = 32730;
	else if (sampleRate == SampleRate_16360)
		m_sampleRate = 16360;
	else if (sampleRate == SampleRate_10910)
		m_sampleRate = 10910;
	else
		m_sampleRate = 8180;

	// Notify derived class
	if (onStart())
	{
		// Start the capture
		m_bufferPos = 0;
		if (R_FAILED(MICU_StartSampling(MICU_ENCODING_PCM16_SIGNED, static_cast<MICU_SampleRate>(sampleRate), 0, m_bufferSize, true)))
			return false;

//		memset(&m_ndspWaveBuf, 0, sizeof(ndspWaveBuf));
//		m_ndspWaveBuf.data_vaddr = &m_samples[0];
//		m_ndspWaveBuf.nsamples = m_samples.size() / 2;
//		m_ndspWaveBuf.looping = false;
//		m_ndspWaveBuf.status = NDSP_WBUF_FREE;
//
//		ndspSetCapture(&m_ndspWaveBuf);

		// Start the capture in a new thread, to avoid blocking the main thread
		m_isCapturing = true;
		m_thread.launch();

		return true;
	}

	return false;
}


////////////////////////////////////////////////////////////
void SoundRecorder::stop()
{
	MICU_StopSampling();

	// Stop the capturing thread
	m_isCapturing = false;
	m_thread.wait();

	// Notify derived class
	onStop();
}


////////////////////////////////////////////////////////////
unsigned int SoundRecorder::getSampleRate() const
{
	return m_sampleRate;
}


////////////////////////////////////////////////////////////
std::vector<std::string> SoundRecorder::getAvailableDevices()
{
	std::vector<std::string> deviceNameList;

	return deviceNameList;
}


////////////////////////////////////////////////////////////
std::string SoundRecorder::getDefaultDevice()
{
	return "3ds mic";
}


////////////////////////////////////////////////////////////
bool SoundRecorder::setDevice(const std::string& name)
{
	// Store the device name
	if (name.empty())
		m_deviceName = getDefaultDevice();
	else
		m_deviceName = name;

	if (m_isCapturing)
	{
		// Stop the capturing thread
		m_isCapturing = false;
		m_thread.wait();

		// Start the capture in a new thread, to avoid blocking the main thread
		m_isCapturing = true;
		m_thread.launch();
	}

	return true;
}


////////////////////////////////////////////////////////////
const std::string& SoundRecorder::getDevice() const
{
	return m_deviceName;
}


////////////////////////////////////////////////////////////
bool SoundRecorder::isAvailable()
{
	return Service::isEnabled(Microphone);
}


////////////////////////////////////////////////////////////
void SoundRecorder::setProcessingInterval(cpp3ds::Time interval)
{
	m_processingInterval = interval;
}


////////////////////////////////////////////////////////////
bool SoundRecorder::onStart()
{
	// Nothing to do
	return true;
}


////////////////////////////////////////////////////////////
void SoundRecorder::onStop()
{
	// Nothing to do
}


////////////////////////////////////////////////////////////
void SoundRecorder::record()
{
	while (m_isCapturing)
	{
		// Process available samples
		processCapturedSamples();
		// Don't bother the CPU while waiting for more captured data
		sleep(m_processingInterval);
	}

	// Capture is finished: clean up everything
	cleanup();
}


////////////////////////////////////////////////////////////
void SoundRecorder::processCapturedSamples()
{
	u32 lastOffset = micGetLastSampleOffset();

	if (lastOffset > m_bufferPos)
	{
		u32 samplesAvailable = lastOffset - m_bufferPos;

		// Forward them to the derived class
		if (!onProcessSamples(reinterpret_cast<Int16*>(&Service::m_micBuffer[m_bufferPos]), samplesAvailable / sizeof(Int16)))
		{
			// The user wants to stop the capture
			m_isCapturing = false;
		}

		m_bufferPos += samplesAvailable;
	}
	else
		m_bufferPos = 0;
}


////////////////////////////////////////////////////////////
void SoundRecorder::cleanup()
{
	// Get the samples left in the buffer
	processCapturedSamples();
}

} // namespace cpp3ds
