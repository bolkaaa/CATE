#ifndef SPECTRAL_FEATURE_HPP
#define SPECTRAL_FEATURE_HPP

#include <vector>

#include "Feature.hpp"

using std::vector;

class SpectralFeature : public Feature
{
public:
    SpectralFeature(uint16_t sample_rate, uint16_t bin_size);

    /* Weighted mean of frequencies, with magnitudes as weights. Perceptually,
     * the "center of mass" of the spectrum. Characterises "brightness" of a
     * sound. */
    float centroid(const vector<float> &magspec);

    /* Ratio of geometric mean of magnitude spectrum to its arithmetic mean.
     * Characteristic of "noisiness" of a signal, where 1.0 is representative of
     * white noise, and 0.0 is a sinusoid. */
    float flatness(const vector<float> &magspec);

private:
    uint16_t bin_size;
};

#endif
