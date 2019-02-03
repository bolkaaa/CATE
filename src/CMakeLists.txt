add_executable(CATE CATE.cpp AudioBuffer.cpp AudioBuffer.hpp Synth.cpp Synth.hpp)
target_link_libraries(CATE portaudio portaudiocpp sndfile)
