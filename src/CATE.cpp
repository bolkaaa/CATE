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

#include <QApplication>
#include "include/nanoflann.hpp"

#include "src/Corpus/PointCloud.hpp"
#include "src/Corpus/Corpus.hpp"
#include "src/Corpus/KdTree.hpp"
#include "src/Audio/AudioSettings.hpp"
#include "src/GUI/MainWindow.hpp"

using CATE::KdTree;
using CATE::KdTreeParams;
using CATE::Corpus;
using CATE::PointCloud;
using CATE::MainWindow;
using CATE::AudioSettings;
using CATE::AudioProcess;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PointCloud point_cloud;
    KdTree kd_tree(KdTreeParams::num_features, point_cloud, KDTreeSingleIndexAdaptorParams(KdTreeParams::max_leaf));
    AudioSettings audio_settings;
    Corpus corpus(audio_settings);
    AudioProcess audio_process(audio_settings, corpus, point_cloud, kd_tree);
    MainWindow main_window(audio_process, audio_settings, corpus, point_cloud, kd_tree);

    main_window.show();

    return app.exec();
}
