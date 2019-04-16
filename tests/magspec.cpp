#include "../include/catch.hpp"

float fft_sine()
{
    return 0.0f;
}

TEST_CASE("")
{
    float value = fft_sine();
    float expected_output = 0.0f;

    REQUIRE(value == expected_output);
}
