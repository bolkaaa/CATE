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
#include "include/nanoflann.hpp"
#include "include/args.hxx"

#include "src/Database/PointCloud.hpp"
#include "src/Database/Database.hpp"
#include "src/Database/KdTree.hpp"
#include "src/Audio/AudioBuffer.hpp"
#include "src/GUI/MainWindow.hpp"

using std::string;
using std::cout;
using std::cerr;
using CATE::KdTree;
using CATE::KdTreeParams;
using CATE::Database;
using CATE::PointCloud;
using CATE::MainWindow;

int main(int argc, char *argv[])
{
#ifdef CLI
    const std::string program_info = "CATE is an audio processing application that performs content-based "
                                     "granular synthesis / concatenative synthesis.";
    args::ArgumentParser parser(program_info);
    args::Group arguments("arguments");
    args::HelpFlag help_flag(arguments, "help", "help", {'h', "help"});
    args::PositionalList<string> paths(arguments, "paths", "directories to analyse");
    args::Group commands(parser, "commands");
    args::GlobalOptions globals(parser, arguments);

    /* Flags */
    const string bin_size_flag_info = "Specify FFT bin size for analysis process.";
    args::ValueFlag<int> bin_size_flag(parser, "bin size", bin_size_flag_info, {'b'});

    const string frames_per_buffer_flag_info = "Specify frames per buffer for analysis process.";
    args::ValueFlag<int> frames_per_buffer_flag(parser, "frames per buffer", frames_per_buffer_flag_info, {'f'});

    const string db_file_path_flag_info = "Specify JSON output file path for analysis process";
    args::ValueFlag<string> db_file_path_flag(parser, "db file path", db_file_path_flag_info, {'d'});


    /* Commands */
    const string analyse_info = "Analyse specified directories and create JSON file from data.";
    args::Command analyse(commands, "analyse", analyse_info, [&](args::Subparser &parser)
    {
        auto bin_size = bin_size_flag ? args::get(bin_size_flag) : 1024;
        auto frames_per_buffer = frames_per_buffer_flag ? args::get(frames_per_buffer_flag) : 4096;
        auto db_file_path = db_file_path_flag ? args::get(db_file_path_flag) : "db.json";
        Database db(db_file_path);

        parser.Parse();

        for (auto &&p : paths)
        {
            db.add_directory(p);
        }

        db.load_files();
        db.sliding_window_analysis(bin_size, frames_per_buffer);
        db.write_json_file();
    });

    try
    {
        parser.ParseCLI(argc, argv);
    }

    catch (args::Help &)
    {
        cout << parser;
    }

    catch (args::Error &e)
    {
        cerr << e.what() << "\n" << parser;
        return 1;
    }

    return 0;
#endif // CLI

    Database db("/Users/lrwz/CATE/speech.json");
    db.load_files();
    db.convert_sample_rates(96000);
    PointCloud point_cloud = db.create_point_cloud();
    KdTree kd_tree(KdTreeParams::num_features,
                   point_cloud,
                   KDTreeSingleIndexAdaptorParams(KdTreeParams::max_leaf));
    kd_tree.buildIndex();

    QApplication app(argc, argv);
    MainWindow main_window(nullptr, db, point_cloud, kd_tree);
    main_window.show();

    return app.exec();
}
