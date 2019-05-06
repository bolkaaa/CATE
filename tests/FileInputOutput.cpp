#include "TestHeader.hpp"

using TestConstants::audio_test_file_list;

bool paths_not_empty(const PathList &file_paths)
{
    for (const auto &path : file_paths)
    {
        if (path.empty())
        {
            return false;
        }
    }

    return true;
}

bool audio_data_loads_correctly(const Path &path)
{
    AudioFile file(path);

    auto has_data = !file.data.empty();
    auto has_sr = file.sample_rate != 0;
    auto has_channels = file.channels != 0;
    auto loads_correctly = has_data && has_sr && has_channels;

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
    AudioFile input_file(path);

    auto sample_rate = 44100.0f;
    auto channels = 2;
    auto output_path = "./test.wav";

    input_file.write(channels, sample_rate, output_path);

    AudioFile output_file(output_path);

    auto same_size = input_file.data.size() == output_file.data.size();
    auto same_sr = input_file.sample_rate == output_file.sample_rate;
    auto same_channels = input_file.channels == output_file.channels;
    auto writes_correctly = same_size && same_sr && same_channels;

    return writes_correctly;
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

/*********************************************************************************************************************/

TEST_CASE("File tree walker works correctly.", "[single_file]")
{
    bool test_case = paths_not_empty(audio_test_file_list);
    REQUIRE(test_case);
}

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

