set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.68.0 COMPONENTS filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package (Threads)

add_executable(CATE
  CATE.cpp
  AudioBuffer.hpp
  Synth.hpp
  AudioParameters.hpp
  AudioParameters.cpp
  FileTree.cpp
  FileTree.hpp
  RingBuffer.hpp
  Window.hpp
  FFT.hpp)

target_link_libraries(CATE portaudio portaudiocpp sndfile samplerate fftw3 ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
