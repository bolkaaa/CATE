#ifndef FEATURE_HPP
#define FEATURE_HPP

#include <iostream>

class Feature
{
public:
    Feature(uint16_t sample_rate);

protected:
    uint16_t sample_rate;
};

#endif
