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

#include <iostream>

#include <QApplication>
#include "../include/nanoflann.hpp"

#include "./Database/PointCloud.hpp"
#include "./Database/Database.hpp"
#include "./Database/KdTree.hpp"
#include "./Audio/AudioBuffer.hpp"
#include "./GUI/MainWindow.hpp"

using std::string;
using CATE::KdTree;
using CATE::KdTreeParams;
using CATE::Database;
using CATE::PointCloud;
using CATE::MainWindow;

int main(int argc, char *argv[])
{
    /* Audio file database loading and K-d Tree Setup. */
    const string db_file_path = "/Users/lrwz/CATE/cello.json";
    Database db;
    db.read_json_file(db_file_path);
    db.load_buffers_from_db();
    PointCloud point_cloud = db.create_point_cloud();
    KdTree kd_tree(KdTreeParams::num_features,
                   point_cloud,
                   KDTreeSingleIndexAdaptorParams(KdTreeParams::max_leaf));
    kd_tree.buildIndex();

    /* Main Application. */
    QApplication app(argc, argv);
    MainWindow main_window(nullptr, db, point_cloud, kd_tree);
    main_window.show();
    return app.exec();
}
