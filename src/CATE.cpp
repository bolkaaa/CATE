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

#include "./Database/Database.hpp"
#include "./Audio/AudioBuffer.hpp"
#include "./GUI/MainWindow.hpp"

using std::string;
using CATE::Database;
using CATE::MainWindow;

int main(int argc, char *argv[])
{
    bool test = 0;

    /* Command-line testing of data functionality. */
    if (test)
    {
        Database db;

        string audio_files_path = "/Users/lrwz/CATE/audio_files";
        string json_db_path = "Users/lrwz/CATE/database.json";

        db.add_directory(audio_files_path);
        db.to_json_file(json_db_path);
        db.load_buffers_from_db();
        db.sliding_window_analysis(1024, 256);

        return 0;
    }

    /* Main Application / GUI. */
    QApplication app(argc, argv);
    MainWindow main_window;
    main_window.show();
    return app.exec();
}
