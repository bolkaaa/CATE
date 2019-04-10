#include "FeatureSet.hpp"

namespace CATE {

FeatureSet::FeatureSet(int bin_size, const vector<float> &magspec)
        : feature(bin_size)
{
    feature_map["centroid"] = feature.centroid(magspec);
    feature_map["flatness"] = feature.flatness(magspec);
    feature_map["kurtosis"] = feature.kurtosis(magspec);
}

} // CATE
