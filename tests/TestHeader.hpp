#ifndef TESTS_HPP
#define TESTS_HPP

#include "catch.hpp"

#include <memory>

#include "src/Analysis/FeatureMap.hpp"
#include "src/Audio/AudioSettings.hpp"
#include "src/Audio/AudioProcess.hpp"
#include "src/Audio/AudioFile.hpp"
#include "src/Corpus/PathTree.hpp"
#include "src/Corpus/PointCloud.hpp"
#include "src/Corpus/Corpus.hpp"
#include "src/Synthesis/Rand.hpp"

using namespace CATE;

namespace TestConstants {

const AudioSettings audio_settings;
const Path audio_test_file_path_root = "/Users/lrwz/CATE/audio_files";
const PathList audio_test_file_list = PathTree::get_paths(audio_test_file_path_root);

} // TestConstants

#endif
