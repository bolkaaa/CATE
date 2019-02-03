add_executable(CATE CATE.cpp AudioBuffer.cpp AudioBuffer.hpp AudioParameters.hpp AudioParameters.cpp Synth.cpp Synth.hpp)
target_link_libraries(CATE portaudio portaudiocpp sndfile)
