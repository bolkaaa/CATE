#include "TestHeader.hpp"

/* Return true if audio data is not empty upon loading audio file from path. */
bool audio_data_loads_correctly(const Path &path)
{
    auto buffer = AudioBuffer();

    try
    {
        buffer = read_audio_file(path);
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Exception: " << e.what() << "while trying to read " << path << "\n\n";
    }

    auto loads_correctly = !buffer.empty();

    return loads_correctly;
}

/* Return true if audio data loads correctly for list of paths. */
bool all_audio_data_loads_correctly(const PathList &paths)
{
    auto loads_correctly = true;

    for (const auto &path : paths)
    {
        loads_correctly = audio_data_loads_correctly(path);
    }

    return loads_correctly;
}

/* Return true if audio data is written to output path and loaded again with the same data size. */
bool audio_data_writes_correctly(const Path &path)
{
    auto input_file = AudioBuffer();
    auto output_file = AudioBuffer();
    auto write_path = "/Users/lrwz/CATE/tests/test.wav";
    auto audio_settings = new AudioSettings;

    /* Read audio from path into AudioBuffer. */
    try
    {
        input_file = read_audio_file(path);
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Exception: " << e.what() << "while trying to read " << path << "\n\n";
        return false;
    }

    /* Write audio from previous AudioBuffer to output file . */
    try
    {
        write_audio_file(input_file, write_path, audio_settings->get_sample_rate().value);
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Exception: " << e.what() << "while trying to write to " << write_path << "\n\n";
        return false;
    }

    /* Read audio from previously created output file. */
    try
    {
        output_file = read_audio_file(write_path);
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Exception: " << e.what() << "while trying to read from " << write_path << "\n\n";
        return false;
    }

    bool same_buffer_size = (input_file.size() == output_file.size());

    return same_buffer_size;
}

/* Return true if audio data writes correctly for list of paths. */
bool all_audio_data_writes_correctly(const PathList &path_list)
{
    auto writes_correctly = true;

    for (const auto &path : path_list)
    {
        writes_correctly = audio_data_writes_correctly(path);
    }

    return writes_correctly;
}

/* Return true if audio file loading handles invalid path. */
bool read_from_invalid_path()
{
    auto empty = " ";

    try
    {
        auto buffer = read_audio_file(empty);
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Exception: " << e.what() << "while trying to read from " << empty << "\n\n";
    }

    return true;
}

/*********************************************************************************************************************/

TEST_CASE("Audio file data loads correctly.", "[single_file]")
{
    auto audio_file_list = PathTree::get_paths(TestConstants::audio_directory_base);
    auto test_case = all_audio_data_loads_correctly(audio_file_list);
    REQUIRE(test_case);
}

TEST_CASE("Audio file data writes correctly.", "[single_file]")
{
    auto audio_file_list = PathTree::get_paths(TestConstants::audio_directory_base);
    auto test_case = all_audio_data_writes_correctly(audio_file_list);
    REQUIRE(test_case);
}

TEST_CASE("Audio file reader works with invalid inputs.", "[single_file]")
{
    auto test_case = read_from_invalid_path();
    REQUIRE(test_case);
}

