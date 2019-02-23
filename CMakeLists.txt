cmake_minimum_required(VERSION 3.13.3)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(CATE VERSION 1.0)
set(COMPANY "CATE")
set(COPYRIGHT "Copyright (c) 2019 Liam Wyllie. All rights reserved.")
set(IDENTIFIER "com.cate.CATE")

set(SOURCE_FILES
  src/CATE.cpp
  src/AudioEngine.hpp
  src/AudioEngine.cpp
  src/AudioProcess.hpp
  src/AudioProcess.cpp
  src/AudioBuffer.hpp
  src/RingBuffer.hpp
  src/FFT.hpp
  src/Synth.hpp
  src/Synth.cpp
  src/FileTree.cpp
  src/FileTree.hpp
  src/MainWindow.hpp
  src/MainWindow.cpp
)

# Boost
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.68.0 COMPONENTS filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Qt
set(CMAKE_PREFIX_PATH /usr/local/Cellar/qt/5.12.0) # Set to system Qt path
include(QtCommon)
find_package(Qt5Widgets REQUIRED)

add_executable(${PROJECT_NAME}
    ${OS_BUNDLE}
    ${SOURCE_FILES}
    ${META_FILES_TO_INCLUDE}
    ${RESOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME}
    portaudio
    portaudiocpp
    sndfile
    samplerate
    fftw3
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
)

# Qt Modules
qt5_use_modules(${PROJECT_NAME} Widgets)
