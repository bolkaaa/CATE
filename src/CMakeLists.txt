add_executable(CATE CATE.cpp AudioBuffer.hpp Synth.hpp AudioParameters.hpp AudioParameters.cpp)
target_link_libraries(CATE portaudio portaudiocpp sndfile)
