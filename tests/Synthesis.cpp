#include "TestHeader.hpp"

bool synthesis_test()
{
    int argc = 0;
    char *argv[] = {};

    QApplication app(argc, argv);

    /* Instantiate objects. */
    auto audio_settings = new CATE::AudioSettings;
    auto corpus = new CATE::Corpus(audio_settings);
    auto audio_process = new CATE::AudioProcess(audio_settings, corpus);

    /* Instantiate and load GUI. */
    MainWindow main_window(audio_process, audio_settings, corpus);

    main_window.start_playback_button_pressed();

    return true;
}

TEST_CASE("synthesis", "[single_file]")
{

    bool test_case = synthesis_test();
    REQUIRE(test_case);
}

