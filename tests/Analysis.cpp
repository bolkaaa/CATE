#include "TestHeader.hpp"

bool magspec_working()
{
    auto path = TestConstants::audio_test_file_list[0];
    auto audio_settings = new AudioSettings();
    FFT fft(audio_settings);
    auto data = read_audio_file(path);
    auto frame_size = 256;
    auto segment = AudioBuffer(frame_size);

    for (auto i = 0; i < frame_size; ++i)
    {
        segment[i] = data[i];
    }

    fft.fill(segment);
    fft.compute_spectrum();
    fft.compute_magspec();
    auto magspec = fft.get_magspec();

    auto i = 0;
    for (auto x : magspec)
    {
        auto freq = (i * (44100.0f/2.0f)) / static_cast<float>(magspec.size());
        std::cout << freq << ": " << x << "\n";
        ++i;
    }

    return true;
}

TEST_CASE("Magnitude spectrum calculation working correctly.", "[single_file]")
{
    bool test_case = magspec_working();
    REQUIRE(test_case);
}
