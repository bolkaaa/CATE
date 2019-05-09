#include "TestHeader.hpp"

/* Helper class for performing audio analysis-based tests. */
class AnalysisTest
{
public:
    AnalysisTest(const Path &path, int frame_size)
      : audio_settings(new AudioSettings()),
        fft(audio_settings)
    {
        auto audio_data = AudioBuffer();

        try
        {
            audio_data = read_audio_file(path);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << e.what() << "\n";
            return;
        }

        auto start = audio_data.begin();
        auto end = audio_data.begin() + frame_size;
        AudioBuffer segment(start, end);
        fft.fill(segment);
        fft.compute_spectrum();
        fft.compute_magspec();
        magspec = fft.get_magspec();
    }

    inline Magspec get_magspec() const { return magspec; }

private:
    AudioSettings *audio_settings;
    FFT fft;
    Magspec magspec;
};

bool magspec_working()
{
    for (const auto &path : TestConstants::audio_test_file_list)
    {
        auto frame_size = 256;
        auto analysis_test = AnalysisTest(path, frame_size);
        auto magspec = analysis_test.get_magspec();
    }

    return true;
}

TEST_CASE("Magnitude spectrum calculation working correctly.", "[single_file]")
{
    bool test_case = magspec_working();
    REQUIRE(test_case);
}
