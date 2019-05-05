#include "catch.hpp"
#include "TestConstants.hpp"

#include <memory>

#include "src/Audio/AudioSettings.hpp"
#include "src/Corpus/PointCloud.hpp"
#include "src/Corpus/Corpus.hpp"
#include "src/Analysis/FeatureMap.hpp"

using CATE::AudioSettings;
using CATE::PointCloud;
using CATE::Corpus;
using CATE::FeatureMap;
using CATE::FeatureNameVector;
using std::unique_ptr;
using std::make_unique;

bool feature_map_working()
{
    auto audioSettings = make_unique<AudioSettings>();
    FeatureMap feature_map(audioSettings);
    auto feature_name_vector = feature_map.get_feature_names();
    bool not_empty = !feature_name_vector.empty();
    return not_empty;
}

bool corpus_working()
{
    auto audio_settings = make_unique<AudioSettings>();
    auto point_cloud = make_unique<PointCloud>();
    Corpus corpus(audio_settings, point_cloud);

    corpus.read_file("/Users/lrwz/CATE/analysis_data/piano_loops.json");
    corpus.load_audio_from_db();
    corpus.rebuild_point_cloud();

    return true;
}

TEST_CASE("Ensure that feature map is working.", "[single_file]")
{
    REQUIRE(feature_map_working());
}

TEST_CASE("Ensure that corpus is working.", "[single_file]")
{
    REQUIRE(corpus_working());
}

