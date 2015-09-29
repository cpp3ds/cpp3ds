set(SRCROOT ${PROJECT_SOURCE_DIR}/src/cpp3ds/Audio)

set(SRC
    ${SRCROOT}/InputSoundFile.cpp
    ${SRCROOT}/Music.cpp
    ${SRCROOT}/OutputSoundFile.cpp
    ${SRCROOT}/Sound.cpp
    ${SRCROOT}/SoundBuffer.cpp
    ${SRCROOT}/SoundBufferRecorder.cpp
    ${SRCROOT}/SoundFileFactory.cpp
    ${SRCROOT}/SoundFileReaderWav.cpp
    ${SRCROOT}/SoundFileWriterWav.cpp
    ${SRCROOT}/SoundRecorder.cpp
    ${SRCROOT}/SoundSource.cpp
    ${SRCROOT}/SoundStream.cpp
)

if(ENABLE_OGG)
    list(APPEND SRC
         ${SRCROOT}/SoundFileReaderOgg.cpp
         ${SRCROOT}/SoundFileWriterOgg.cpp)
endif()

add_cpp3ds_library(cpp3ds-audio
                   SOURCES ${SRC}
                   DEPENDS cpp3ds-system)
