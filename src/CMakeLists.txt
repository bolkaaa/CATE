set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.68.0 COMPONENTS filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package (Threads)

add_executable(CATE CATE.cpp AudioBuffer.hpp Synth.hpp AudioParameters.hpp AudioParameters.cpp FileTree.cpp FileTree.hpp)

target_link_libraries(CATE portaudio portaudiocpp sndfile samplerate boost_filesystem ${Boost_Libraries} ${CMAKE_THREAD_LIBS_INIT})
