/*
  CATE: Concatenative Audio Processing Application
  Copyright (c) 2019 Liam Wyllie. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include <memory>

#include <QApplication>
#include "include/nanoflann.hpp"

#include "src/Corpus/PointCloud.hpp"
#include "src/Corpus/Corpus.hpp"
#include "src/Corpus/KdTree.hpp"
#include "src/Audio/AudioSettings.hpp"
#include "src/Synthesis/GrainParams.hpp"
#include "src/GUI/MainWindow.hpp"

using std::unique_ptr;
using std::make_unique;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /* Instantiate point cloud and k-d tree data structures. */
    auto point_cloud = make_unique<CATE::PointCloud>();
    auto adaptor_params(CATE::KdTreeParams::max_leaf);
    const auto num_features = CATE::FeatureMap::num_features;
    CATE::KdTree kd_tree(num_features,
                         *point_cloud,
                         adaptor_params);

    /* Instantiate audio objects. */
    auto audio_settings = make_unique<CATE::AudioSettings>();
    auto corpus = make_unique<CATE::Corpus>(audio_settings, point_cloud);
    auto grain_params = make_unique<CATE::GrainParams>();
    auto env_params = make_unique<CATE::EnvelopeParams>(grain_params->get_grain_size());
    auto audio_process = make_unique<CATE::AudioProcess>(audio_settings,
                                                         corpus,
                                                         point_cloud,
                                                         kd_tree);

    /* Instantiate and load GUI. */
    auto main_window = make_unique<CATE::MainWindow>(audio_settings,
                                                     audio_process,
                                                     corpus,
                                                     point_cloud,
                                                     grain_params,
                                                     env_params,
                                                     kd_tree);
    main_window->show();

    return app.exec();
}
