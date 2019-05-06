#include "TestHeader.hpp"

/* Test class to quickly set up a working AudioProcess object. */
class AudioTest
{
public:
    AudioTest()
      : audio_settings(new AudioSettings),
      corpus(new Corpus(audio_settings)),
      audio_process(new AudioProcess(audio_settings, corpus))
    {
    }

    ~AudioTest()
    {
        delete audio_process;
        delete corpus;
        delete audio_settings;
    }

    AudioSettings *audio_settings;
    Corpus *corpus;
    AudioProcess *audio_process;
};

bool stream_starts_correctly()
{
    AudioTest audio_test;

    audio_test.audio_process->start_stream();
    audio_test.audio_process->sleep(4000);

    return true;
}

bool stream_stops_correctly()
{
    AudioTest audio_test;

    audio_test.audio_process->start_stream();
    audio_test.audio_process->sleep(1000);
    audio_test.audio_process->stop_stream();

    return true;
}

TEST_CASE("PortAudio starts successfully.", "[single_file]")
{
    bool test_case = stream_starts_correctly();
    REQUIRE(test_case);
}

TEST_CASE("PortAudio stops successfully.", "[single_file]")
{
    bool test_case = stream_stops_correctly();
    REQUIRE(test_case);
}

