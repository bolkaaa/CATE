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

#ifndef RECORDWORKER_HPP
#define RECORDWORKER_HPP

#include <QObject>

#include "src/Audio/AudioSettings.hpp"
#include "src/Audio/AudioFile.hpp"
#include "src/Audio/RingBuffer.hpp"

using std::string;

namespace CATE {

class RecordWorker : public QObject
{
Q_OBJECT
public:
    /* Write record_buffer to output file. */
    void save_recording(const std::string &output_path, float sample_rate, int channels);

public slots:
    /* Pop available sample from ring buffer and write to record_buffer. */
    void record_data_received(RingBuffer *ring_buffer);

private:
    AudioFile record_data;
};

} // CATE

#endif
