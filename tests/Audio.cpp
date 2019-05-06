#include "TestHeader.hpp"

bool test_audio()
{
    return true;
}

TEST_CASE("PortAudio starts successfully.", "[single_file]")
{
    bool test_case = test_audio();
    REQUIRE(test_case);
}

