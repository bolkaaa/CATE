add_executable(CATE CATE.cpp AudioBuffer.cpp AudioBuffer.hpp AudioFile.cpp AudioFile.hpp)
target_link_libraries(CATE portaudio portaudiocpp sndfile)
