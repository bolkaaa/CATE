#include "TestHeader.hpp"

using TestConstants::audio_test_file_list;

bool audio_data_loads_correctly(const Path &path)
{
    auto buffer = read_audio_file(path);
    auto loads_correctly = !buffer.empty();
    return loads_correctly;
}

bool all_audio_data_loads_correctly(const PathList &paths)
{
    auto loads_correctly = true;

    for (const auto &path : paths)
    {
        loads_correctly = audio_data_loads_correctly(path);
    }

    return loads_correctly;
}

bool audio_data_writes_correctly(const Path &path)
{
    auto input_file = read_audio_file(path);
    write_audio_file(input_file, "./test.wav", 2, 44100.0f);
    auto output_file = read_audio_file("./test.wav");

    return (input_file == output_file);
}

bool all_audio_data_writes_correctly(const PathList &path_list)
{
    auto writes_correctly = true;

    for (const auto &path : path_list)
    {
        writes_correctly = audio_data_writes_correctly(path);
    }

    return writes_correctly;
}

bool read_from_invalid_path()
{
    auto empty = " ";
    auto buffer = read_audio_file(empty);

    return true;
}

bool segmentation_works()
{
    auto path_list = audio_test_file_list;
    auto buffer = read_audio_file(audio_test_file_list[0]);
    auto frame_size = 256;
    auto hop_size = 128;
    auto frames = segment_frames(buffer, 8192, 8192);

    return (!frames.empty());
}

/*********************************************************************************************************************/

TEST_CASE("Audio file data loads correctly.", "[single_file]")
{
    bool test_case = all_audio_data_loads_correctly(audio_test_file_list);
    REQUIRE(test_case);
}

TEST_CASE("Audio file data writes correctly.", "[single_file]")
{
    bool test_case = all_audio_data_writes_correctly(audio_test_file_list);
    REQUIRE(test_case);
}

TEST_CASE("Audio file reader works with invalid inputs.", "[single_file]")
{
    bool test_case = read_from_invalid_path();
    REQUIRE(test_case);
}

TEST_CASE("Segmentation Works.", "[single_file]")
{
    bool test_case = segmentation_works();
    REQUIRE(test_case);
}



