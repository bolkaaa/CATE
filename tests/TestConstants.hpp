#ifndef TESTS_HPP
#define TESTS_HPP

#include "src/Audio/AudioSettings.hpp"
#include "src/Corpus/PathTree.hpp"

using CATE::AudioSettings;
using CATE::Path;
using CATE::PathList;
using CATE::PathTree;

namespace TestConstants {

const AudioSettings audio_settings;
const Path audio_test_file_path_root = "/Users/lrwz/CATE/tests/audio_files";
const PathList audio_test_file_list = PathTree::get_paths(audio_test_file_path_root);

} // TestConstants


#endif
