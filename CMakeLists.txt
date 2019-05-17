cmake_minimum_required(VERSION 3.12.2)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
SET(BUILD_SHARED_LIBS OFF)

project(CATE VERSION 1.0)
set(COMPANY "CATE")
set(COPYRIGHT "Copyright (c) 2019 Liam Wyllie. All rights reserved.")
set(IDENTIFIER "com.cate.CATE")

set(SOURCE_FILES
        src/Audio/AudioEngine.hpp
        src/Audio/AudioEngine.cpp
        src/Audio/AudioProcess.hpp
        src/Audio/AudioProcess.cpp
        src/Audio/AudioBuffer.hpp
        src/Audio/AudioBuffer.cpp
        src/Audio/AudioSettings.hpp
        src/Audio/AudioSettings.cpp
        src/Audio/RecordWorker.hpp
        src/Audio/RecordWorker.cpp
        src/Audio/AnalysisWorker.hpp
        src/Audio/AnalysisWorker.cpp
        src/Audio/RingBuffer.hpp
        src/Analysis/FFT.hpp
        src/Analysis/FFT.cpp
        src/Analysis/Feature.hpp
        src/Analysis/Feature.cpp
        src/Analysis/Math.hpp
        src/Analysis/FeatureVector.hpp
        src/Analysis/FeatureVector.cpp
        src/Corpus/PathTree.hpp
        src/Corpus/PathTree.cpp
        src/Corpus/Corpus.hpp
        src/Corpus/Corpus.cpp
        src/Corpus/PointCloud.hpp
        src/Corpus/KdTree.hpp
        src/GUI/MainWindow.hpp
        src/GUI/MainWindow.cpp
        src/GUI/AudioSettingsWindow.hpp
        src/GUI/AudioSettingsWindow.cpp
        src/GUI/QtUtil.hpp
        src/Synthesis/Granulator.hpp
        src/Synthesis/Granulator.cpp
        src/Synthesis/Envelope.hpp
        src/Synthesis/Envelope.cpp
        src/Synthesis/Source.hpp
        src/Synthesis/Source.cpp
        src/Synthesis/Grain.hpp
        src/Synthesis/Grain.cpp
        src/Synthesis/Scheduler.hpp
        src/Synthesis/Scheduler.cpp
        src/Synthesis/Rand.hpp
        src/Util/Param.hpp
        src/Util/NonCopyable.hpp)

set(TEST_FILES
        ${SOURCE_FILES}
        tests/main.cpp
        tests/TestHeader.hpp
        tests/FileInputOutput.cpp
        tests/Corpus.cpp
        tests/Audio.cpp
        tests/Synthesis.cpp
        tests/Analysis.cpp src/Analysis/FeatureVector.cpp src/Analysis/FeatureVector.hpp)

# Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.68.0 COMPONENTS filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Qt
include(QtCommon)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Main Executable
add_executable(${PROJECT_NAME}
        ${OS_BUNDLE}
        ${SOURCE_FILES} src/cate.cpp
        ${META_FILES_TO_INCLUDE}
        ${RESOURCE_FILES})

# Tests Executable
add_executable(TESTS
        ${TEST_FILES})

set(LIBRARIES
        portaudio
        sndfile
        fftw3
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
target_link_libraries(TESTS ${LIBRARIES})

# Qt Modules
qt5_use_modules(${PROJECT_NAME} Widgets)
qt5_use_modules(TESTS Widgets)
