cmake_minimum_required(VERSION 3.12.2)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(CATE VERSION 1.0)
set(COMPANY "CATE")
set(COPYRIGHT "Copyright (c) 2019 Liam Wyllie. All rights reserved.")
set(IDENTIFIER "com.cate.CATE")

set(SOURCE_FILES
        src/CATE.cpp
        src/Audio/AudioEngine.hpp
        src/Audio/AudioEngine.cpp
        src/Audio/AudioProcess.hpp
        src/Audio/AudioProcess.cpp
        src/Audio/AudioBuffer.hpp
        src/Audio/AudioBuffer.cpp
        src/Audio/AudioFile.hpp
        src/Audio/AudioFile.cpp
        src/Audio/AudioRecorder.cpp
        src/Audio/AudioRecorder.hpp
        src/Audio/AudioSettings.cpp
        src/Audio/AudioSettings.hpp
        src/Analysis/FFT.hpp
        src/Analysis/FFT.cpp
        src/Analysis/Feature.cpp
        src/Analysis/Feature.hpp
        src/Analysis/FeatureMap.cpp
        src/Analysis/FeatureMap.hpp
        src/Corpus/FileTree.hpp
        src/Corpus/FileTree.cpp
        src/Corpus/Corpus.hpp
        src/Corpus/Corpus.cpp
        src/Corpus/PointCloud.hpp
        src/Corpus/KdTree.hpp
        src/GUI/MainWindow.hpp
        src/GUI/MainWindow.cpp
        src/GUI/AudioSettingsWindow.cpp
        src/GUI/AudioSettingsWindow.hpp
        src/GUI/QtUtil.hpp
        src/Synthesis/Granulator.cpp
        src/Synthesis/Granulator.hpp
        src/Synthesis/Envelope.cpp
        src/Synthesis/Envelope.hpp
        src/Synthesis/Source.cpp
        src/Synthesis/Source.hpp
        src/Synthesis/Grain.cpp
        src/Synthesis/Grain.hpp
        src/Synthesis/Scheduler.cpp
        src/Synthesis/Scheduler.hpp
        src/Synthesis/Rand.hpp
        src/Synthesis/EnvelopeParams.cpp
        src/Synthesis/GrainParams.cpp
        src/Synthesis/GrainParams.hpp
        src/Analysis/Math.hpp src/Util/NonCopyable.hpp)

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

# Main Executable
add_executable(${PROJECT_NAME}
        ${OS_BUNDLE}
        ${SOURCE_FILES}
        ${META_FILES_TO_INCLUDE}
        ${RESOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
        portaudio
        sndfile
        fftw3
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        )

# Qt Modules
qt5_use_modules(${PROJECT_NAME} Widgets)
