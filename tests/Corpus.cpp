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
    corpus.rebuild_point_cloud();

    return corpus.size() > 0;
}

void fill_point_cloud(PointCloud &point_cloud, int size, int dim)
{
    auto rand = Rand<float>(0.0f, 1.0f);

    for (auto i = 0; i < size; ++i)
    {
        auto p = Point();

        p.file_path = "";
        p.marker = 0;

        for (auto j = 0; j < dim; ++j)
        {
            p.features.emplace_back(rand.get());
        }

        point_cloud.add(p);
    }
}

bool knn_working()
{
    PointCloud point_cloud;
    const auto cloud_size = 1024;
    const auto dimensions = 3;
    const auto search_size = 1;
    const float query_pt[dimensions] = {0.1, 0.2, 0.3};
    auto indices = vector<size_t>(dimensions);
    auto distances = vector<float>(dimensions);
    KdTree kd_tree(3, point_cloud, KDTreeSingleIndexAdaptorParams(KdTreeParams::max_leaf));

    fill_point_cloud(point_cloud, cloud_size, dimensions);

    kd_tree.buildIndex();

    kd_tree.knnSearch(query_pt, search_size, &indices[0], &distances[0]);

    return true;
}

bool files_load_correctly()
{
    auto audio_settings = new AudioSettings;
    Corpus corpus(audio_settings);
    corpus.read_file("/Users/lrwz/CATE/analysis_data/piano_loops.json");
    corpus.load_audio_from_db();

    bool files_loaded = corpus.get_num_files() > 0;

    return files_loaded;
}

TEST_CASE("Ensure that feature map is working.", "[single_file]")
{
    REQUIRE(feature_map_working());
}

TEST_CASE("Ensure that corpus is working.", "[single_file]")
{
    REQUIRE(corpus_working());
}

TEST_CASE("Ensure that the nearest neighbours corpus search is working.", "[single_file]")
{
    REQUIRE(knn_working());
}

TEST_CASE("Ensure that files load from corpus correctly.", "[single_file]")
{
    REQUIRE(files_load_correctly());
}
