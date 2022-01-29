cmake_minimum_required(VERSION 3.14)
project(neuro-load C CXX)

add_subdirectory(lib/FFmpeg)
add_subdirectory(lib/glfw)

add_definitions(-DGL_SILENCE_DEPRECATION)

set(SOURCE_FILES
    src/source/MediaManager.cpp
    src/source/FileLoader.cpp
    src/source/VideoEditor.cpp
    src/source/AudioEditor.cpp
    src/source/SceneEditor.cpp
    src/source/Synchronizer.cpp
    src/source/Display.cpp
    src/utils/UserInterface.cpp
    src/utils/Log.cpp
)

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

set(OPENGL_LIBRARY "" CACHE PATH "/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/OpenGL.framework)" )
add_executable( neuro-load src/main.cpp ${SOURCE_FILES})
set_property(TARGET neuro-load PROPERTY CXX_STANDARD 17)
set_property(TARGET neuro-load PROPERTY -Wall )

target_link_libraries( neuro-load ${OPENGL_LIBRARY} ${OpenCV_LIBS} FFmpeg glfw)

