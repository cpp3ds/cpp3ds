find_package(Qt5Widgets REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(ui_header ${PROJECT_SOURCE_DIR}/include/cpp3ds/Emulator/ui_emulator.h)

add_custom_command(
        OUTPUT ${ui_header}
        COMMAND uic ${PROJECT_SOURCE_DIR}/res/emu/emulator.ui -o ${ui_header}
        DEPENDS ${PROJECT_SOURCE_DIR}/res/emu/emulator.ui
        COMMENT "Generating ui_emulator.h"
)

qt5_wrap_cpp(tst_hdr_moc ${PROJECT_SOURCE_DIR}/include/cpp3ds/Emulator/Emulator.hpp)
qt5_add_resources(RESOURCE_HEADER ${PROJECT_SOURCE_DIR}/res/emu/emulator.qrc)
set_source_files_properties(${RESOURCE_OUTPUT} PROPERTIES GENERATED TRUE)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
)

set(SRCROOT ${PROJECT_SOURCE_DIR}/src/cpp3ds)
set(EMUSRCROOT ${PROJECT_SOURCE_DIR}/src/emu3ds)

set(SRC
        # Emulator
        ${EMUSRCROOT}/emu3ds.cpp
        ${EMUSRCROOT}/Emulator/Emulator.cpp
        ${EMUSRCROOT}/Emulator/SFMLWidget.cpp

        # Audio
        ${EMUSRCROOT}/Audio/ALCheck.cpp
        ${EMUSRCROOT}/Audio/AlResource.cpp
        ${EMUSRCROOT}/Audio/AudioDevice.cpp
        ${SRCROOT}/Audio/InputSoundFile.cpp
        ${SRCROOT}/Audio/Music.cpp
        ${SRCROOT}/Audio/OutputSoundFile.cpp
        ${EMUSRCROOT}/Audio/Sound.cpp
        ${EMUSRCROOT}/Audio/SoundBuffer.cpp
        ${SRCROOT}/Audio/SoundBufferRecorder.cpp
        ${SRCROOT}/Audio/SoundFileFactory.cpp
        ${SRCROOT}/Audio/SoundFileReaderWav.cpp
        ${SRCROOT}/Audio/SoundFileWriterWav.cpp
        ${EMUSRCROOT}/Audio/SoundRecorder.cpp
        ${EMUSRCROOT}/Audio/SoundSource.cpp
        ${EMUSRCROOT}/Audio/SoundStream.cpp

        # Graphics
        ${SRCROOT}/Graphics/BlendMode.cpp
        ${SRCROOT}/Graphics/CircleShape.cpp
        ${SRCROOT}/Graphics/Color.cpp
        ${SRCROOT}/Graphics/Console.cpp
        ${SRCROOT}/Graphics/ConvexShape.cpp
        ${SRCROOT}/Graphics/Font.cpp
        ${EMUSRCROOT}/Graphics/GLCheck.cpp
        ${SRCROOT}/Graphics/GLExtensions.cpp
        ${SRCROOT}/Graphics/Image.cpp
        ${SRCROOT}/Graphics/ImageLoader.cpp
        ${SRCROOT}/Graphics/RectangleShape.cpp
        ${SRCROOT}/Graphics/RenderStates.cpp
        ${EMUSRCROOT}/Graphics/RenderTarget.cpp
        ${SRCROOT}/Graphics/RenderTexture.cpp
        ${EMUSRCROOT}/Graphics/Shader.cpp
        ${SRCROOT}/Graphics/Shape.cpp
        ${SRCROOT}/Graphics/Sprite.cpp
        ${SRCROOT}/Graphics/Text.cpp
        ${EMUSRCROOT}/Graphics/Texture.cpp
        ${EMUSRCROOT}/Graphics/TextureSaver.cpp
        ${EMUSRCROOT}/Graphics/Transform.cpp
        ${SRCROOT}/Graphics/Transformable.cpp
        ${SRCROOT}/Graphics/Vertex.cpp
        ${SRCROOT}/Graphics/VertexArray.cpp
        ${SRCROOT}/Graphics/View.cpp

        # Network
        ${SRCROOT}/Network/Ftp.cpp
        ${SRCROOT}/Network/Http.cpp
        ${SRCROOT}/Network/IpAddress.cpp
        ${SRCROOT}/Network/Packet.cpp
        ${SRCROOT}/Network/Socket.cpp
        ${SRCROOT}/Network/SocketImpl.cpp
        ${SRCROOT}/Network/SocketSelector.cpp
        ${SRCROOT}/Network/TcpListener.cpp
        ${SRCROOT}/Network/TcpSocket.cpp
        ${SRCROOT}/Network/UdpSocket.cpp

        # System
        ${EMUSRCROOT}/System/Clock.cpp
        ${SRCROOT}/System/Err.cpp
        ${SRCROOT}/System/FileInputStream.cpp
        ${SRCROOT}/System/FileSystem.cpp
        ${SRCROOT}/System/I18n.cpp
        ${SRCROOT}/System/Lock.cpp
        ${SRCROOT}/System/MemoryInputStream.cpp
        ${SRCROOT}/System/Mutex.cpp
        ${EMUSRCROOT}/System/Service.cpp
        ${EMUSRCROOT}/System/Sleep.cpp
        ${SRCROOT}/System/String.cpp
        ${EMUSRCROOT}/System/Thread.cpp
        ${SRCROOT}/System/ThreadLocal.cpp
        ${SRCROOT}/System/Time.cpp

        # Window
        ${SRCROOT}/Window/Context.cpp
        ${EMUSRCROOT}/Window/EventManager.cpp
        ${EMUSRCROOT}/Window/Game.cpp
        ${EMUSRCROOT}/Window/GlContext.cpp
        ${EMUSRCROOT}/Window/GlResource.cpp
        ${EMUSRCROOT}/Window/Keyboard.cpp
        ${SRCROOT}/Window/Sensor.cpp
        ${EMUSRCROOT}/Window/Window.cpp
)

if(ENABLE_OGG)
    list(APPEND SRC
            ${SRCROOT}/Audio/SoundFileReaderOgg.cpp
            ${SRCROOT}/Audio/SoundFileWriterOgg.cpp)
endif()
if(ENABLE_MP3)
	find_package(mpg123 REQUIRED)
	include_directories(${MPG123_INCLUDE_DIRS})
	list(APPEND SRC
			${SRCROOT}/Audio/SoundFileReaderMp3.cpp)
endif()

# ImageLoader.cpp must be compiled with the -fno-strict-aliasing
# when gcc is used; otherwise saving PNGs may crash in stb_image_write
set_source_files_properties(${SRCROOT}/ImageLoader.cpp PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)

unset(JPEG_INCLUDE_DIR CACHE)
unset(FREETYPE_INCLUDE_DIRS CACHE)

find_package(JPEG REQUIRED)
find_package(Freetype REQUIRED)

include_directories(${FREETYPE_INCLUDE_DIRS} ${JPEG_INCLUDE_DIR})

add_library(cpp3ds-emu STATIC
        ${SRC}
        ${tst_hdr_moc}
        ${RESOURCE_HEADER}
        ${RESOURCE_OUTPUT}
)
set_target_properties(cpp3ds-emu PROPERTIES COMPILE_DEFINITIONS "EMULATION")
set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_EMU_FLAGS}")

add_custom_target(ui ALL DEPENDS ${ui_header})
add_dependencies(cpp3ds-emu ui cpp3ds-res)
qt5_use_modules(cpp3ds-emu Core Gui Widgets)
