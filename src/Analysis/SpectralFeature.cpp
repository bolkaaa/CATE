#include <cmath>

#include "Feature.hpp"
#include "SpectralFeature.hpp"

SpectralFeature::SpectralFeature(uint16_t sample_rate, uint16_t bin_size)
    : Feature(sample_rate),
      bin_size(bin_size)
{
}

float SpectralFeature::centroid(const vector<float> &magspec)
{
    auto magnitudes = 0.0;
    auto weighted_magnitudes = 0.0;

    for (std::size_t i = 1; i <= magspec.size(); ++i)
    {
        magnitudes += magspec[i];
        weighted_magnitudes += magspec[i] * i;
    }

    /* Protect against divide-by-zero errors. */
    if (magnitudes > 0)
    {
        return weighted_magnitudes / magnitudes;
    }

    return 0.0;
}

float SpectralFeature::flatness(const vector<float> &magspec)
{
    double sum = 0.0;
    double log_sum = 0.0;
    double n = (double) (magspec.size());

    for (std::size_t i = 0; i < magspec.size(); ++i)
    {
        double v = (double) (1 + magspec[i]);
        sum += v;
        log_sum += std::log(v);
    }

    sum /= n;
    log_sum /= n;

    /* Protect against divide-by-zero errors. */
    if (sum > 0)
    {
        float f = (float) (std::exp(log_sum / sum));
        return f;
    }
    else
    {
        return 0.0;
    }
}
