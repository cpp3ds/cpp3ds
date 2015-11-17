set(SRCROOT ${PROJECT_SOURCE_DIR}/src/cpp3ds/Graphics)

set(SRC
    ${RESOURCE_OUTPUT} # Embedded resources needed for graphics
    ${SRCROOT}/BlendMode.cpp
    ${SRCROOT}/CircleShape.cpp
    ${SRCROOT}/Color.cpp
    ${SRCROOT}/Console.cpp
    ${SRCROOT}/ConvexShape.cpp
    ${SRCROOT}/Font.cpp
    ${SRCROOT}/GLCheck.cpp
    ${SRCROOT}/GLExtensions.cpp
    ${SRCROOT}/Image.cpp
    ${SRCROOT}/ImageLoader.cpp
    ${SRCROOT}/RectangleShape.cpp
    ${SRCROOT}/RenderStates.cpp
    ${SRCROOT}/RenderTarget.cpp
    ${SRCROOT}/RenderTexture.cpp
    ${SRCROOT}/Shader.cpp
    ${SRCROOT}/Shape.cpp
    ${SRCROOT}/Sprite.cpp
    ${SRCROOT}/Text.cpp
    ${SRCROOT}/Texture.cpp
    ${SRCROOT}/TextureSaver.cpp
    ${SRCROOT}/Transform.cpp
    ${SRCROOT}/Transformable.cpp
    ${SRCROOT}/Vertex.cpp
    ${SRCROOT}/VertexArray.cpp
    ${SRCROOT}/View.cpp
)

unset(JPEG_INCLUDE_DIR CACHE)
unset(FREETYPE_INCLUDE_DIRS CACHE)

find_package(JPEG REQUIRED)
find_package(Freetype REQUIRED)

include_directories(${FREETYPE_INCLUDE_DIRS} ${JPEG_INCLUDE_DIR})

set_source_files_properties(${RESOURCE_OUTPUT} PROPERTIES GENERATED TRUE)

# ImageLoader.cpp must be compiled with the -fno-strict-aliasing
# when gcc is used; otherwise saving PNGs may crash in stb_image_write
set_source_files_properties(${SRCROOT}/ImageLoader.cpp PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)

add_cpp3ds_library(cpp3ds-graphics
                   SOURCES ${SRC}
                   DEPENDS cpp3ds-window cpp3ds-system cpp3ds-res)
