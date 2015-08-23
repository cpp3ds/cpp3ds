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

#ifndef CPP3DS_SOUNDFILEREADER_HPP
#define CPP3DS_SOUNDFILEREADER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Config.hpp>
#include <string>


namespace cpp3ds
{
class InputStream;

////////////////////////////////////////////////////////////
/// \brief Abstract base class for sound file decoding
///
////////////////////////////////////////////////////////////
class SoundFileReader
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Structure holding the audio properties of a sound file
    ///
    ////////////////////////////////////////////////////////////
    struct Info
    {
        Uint64       sampleCount;  ///< Total number of samples in the file
        unsigned int channelCount; ///< Number of channels of the sound
        unsigned int sampleRate;   ///< Samples rate of the sound, in samples per second
    };

    ////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~SoundFileReader() {}

    ////////////////////////////////////////////////////////////
    /// \brief Open a sound file for reading
    ///
    /// The provided stream reference is valid as long as the
    /// SoundFileReader is alive, so it is safe to use/store it
    /// during the whole lifetime of the reader.
    ///
    /// \param stream Source stream to read from
    /// \param info   Structure to fill with the properties of the loaded sound
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    virtual bool open(InputStream& stream, Info& info) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current read position to the given sample offset
    ///
    /// If the given offset exceeds to total number of samples,
    /// this function must jump to the end of the file.
    ///
    /// \param sampleOffset Index of the sample to jump to, relative to the beginning
    ///
    ////////////////////////////////////////////////////////////
    virtual void seek(Uint64 sampleOffset) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Read audio samples from the open file
    ///
    /// \param samples  Pointer to the sample array to fill
    /// \param maxCount Maximum number of samples to read
    ///
    /// \return Number of samples actually read (may be less than \a maxCount)
    ///
    ////////////////////////////////////////////////////////////
    virtual Uint64 read(Int16* samples, Uint64 maxCount) = 0;
};

} // namespace cpp3ds


#endif // CPP3DS_SOUNDFILEREADER_HPP


////////////////////////////////////////////////////////////
/// \class cpp3ds::SoundFileReader
/// \ingroup audio
///
/// This class allows users to read audio file formats not natively
/// supported by cpp3ds, and thus extend the set of supported readable
/// audio formats.
///
/// A valid sound file reader must override the open, seek and write functions,
/// as well as providing a static check function; the latter is used by
/// cpp3ds to find a suitable writer for a given input file.
///
/// To register a new reader, use the cpp3ds::SoundFileFactory::registerReader
/// template function.
///
/// Usage example:
/// \code
/// class MySoundFileReader : public cpp3ds::SoundFileReader
/// {
/// public:
///
///     static bool check(cpp3ds::InputStream& stream)
///     {
///         // typically, read the first few header bytes and check fields that identify the format
///         // return true if the reader can handle the format
///     }
///
///     virtual bool open(cpp3ds::InputStream& stream, Info& info)
///     {
///         // read the sound file header and fill the sound attributes
///         // (channel count, sample count and sample rate)
///         // return true on success
///     }
///
///     virtual void seek(cpp3ds::Uint64 sampleOffset)
///     {
///         // advance to the sampleOffset-th sample from the beginning of the sound
///     }
///
///     virtual cpp3ds::Uint64 read(cpp3ds::Int16* samples, cpp3ds::Uint64 maxCount)
///     {
///         // read up to 'maxCount' samples into the 'samples' array,
///         // convert them (for example from normalized float) if they are not stored
///         // as 16-bits signed integers in the file
///         // return the actual number of samples read
///     }
/// };
///
/// cpp3ds::SoundFileFactory::registerReader<MySoundFileReader>();
/// \endcode
///
/// \see cpp3ds::InputSoundFile, cpp3ds::SoundFileFactory, cpp3ds::SoundFileWriter
///
////////////////////////////////////////////////////////////
