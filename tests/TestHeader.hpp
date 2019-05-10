#ifndef TESTS_HPP
#define TESTS_HPP

#include "catch.hpp"

#include <memory>
#include <QApplication>

#include "src/Audio/AudioSettings.hpp"
#include "src/Audio/AudioProcess.hpp"
#include "src/Corpus/PathTree.hpp"
#include "src/Corpus/PointCloud.hpp"
#include "src/Corpus/Corpus.hpp"
#include "src/Synthesis/Rand.hpp"
#include "src/GUI/MainWindow.hpp"

using namespace CATE;

/* Constants used in unit tests. */
namespace TestConstants {

const Path audio_directory_base = "/Users/lrwz/CATE/tests/audio_files";
const PathList audio_directory_paths  = {
        audio_directory_base + "/violin",
        audio_directory_base + "/piano_loops",
};

const Path analysis_test_base = "/Users/lrwz/CATE/tests/analysis_data";
const PathList analysis_file_paths  = {
        analysis_test_base + "/violin.json",
        analysis_test_base + "/piano_loops.json",
};

} // TestConstants

#endif
