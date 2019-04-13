#include "FeatureSet.hpp"

namespace CATE {

FeatureSet::FeatureSet(int bin_size)
        : feature(bin_size)
{
}

void FeatureSet::calculate(const vector<float> &magspec)
{
    centroid = feature.centroid(magspec);
    flatness = feature.flatness(magspec);
    kurtosis = feature.kurtosis(magspec);
}

} // CATE
