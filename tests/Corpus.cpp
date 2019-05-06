#include "TestHeader.hpp"

bool feature_map_working()
{
    auto audioSettings = new AudioSettings;
    FeatureMap feature_map(audioSettings);
    auto feature_name_vector = feature_map.get_feature_names();
    bool not_empty = !feature_name_vector.empty();
    return not_empty;
}

bool corpus_working()
{
    auto audio_settings = new AudioSettings;

    Corpus corpus(audio_settings);

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

