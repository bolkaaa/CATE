#include "TestHeader.hpp"

/* Fill a random PointCloud object. */
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

/* Return true if corpus point cloud has data points after loading a file and rebuilding the cloud. */
bool corpus_load_files_working()
{
    auto audio_settings = new AudioSettings;
    Corpus corpus(audio_settings);
    auto test_path = TestConstants::analysis_file_paths[0];

    corpus.read_file(test_path);
    corpus.rebuild_point_cloud();
    bool has_data_points = corpus.size() > 0;

    return has_data_points;
}

/* Return true if corpus creates analysis file that results in the same data when read from another object. */
bool corpus_analysis_working()
{
    auto audio_settings = new AudioSettings;
    Corpus first_corpus(audio_settings);
    const auto frame_size = audio_settings->get_bin_size();
    const auto input_directory = TestConstants::audio_directory_paths[0];
    const auto output_file = TestConstants::analysis_file_paths[0];

    first_corpus.add_directory(input_directory);
    first_corpus.sliding_window_analysis(frame_size);
    first_corpus.write_file(output_file);

    Corpus second_corpus(audio_settings);
    second_corpus.read_file(output_file);

    bool same_data = (first_corpus==second_corpus);

    return same_data;
}

/* Return true if function reaches end after doing a KNN search of a random corpus. */
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

bool audio_frame_map_creation_works()
{
    auto audio_settings = new AudioSettings;
    Corpus corpus(audio_settings);
    auto path = TestConstants::analysis_file_paths[0];

    corpus.read_file(path);
    corpus.create_audio_frame_map();

    return true;
}

TEST_CASE("corpus is working.", "[single_file]")
{
    REQUIRE(corpus_load_files_working());
}

TEST_CASE("Analysis process is working.", "[single_file]")
{
    REQUIRE(corpus_analysis_working());
}

TEST_CASE("Nearest neighbours corpus search is working.", "[single_file]")
{
    REQUIRE(knn_working());
}

TEST_CASE("Audio frame map creation working.", "[single_file]")
{
    REQUIRE(audio_frame_map_creation_works());
}
