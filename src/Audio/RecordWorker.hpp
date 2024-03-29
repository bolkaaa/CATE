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

#include <list>

#include <QObject>

#include "src/Audio/AudioSettings.hpp"
#include "src/Audio/RingBuffer.hpp"

namespace CATE {

typedef std::list<float> AudioLinkedList;

class RecordWorker : public QObject
{
Q_OBJECT
public:
    RecordWorker(AudioSettings *audio_settings);

    /* Write record_buffer to output file. */
    void save_recording(const Path &output_path);

public slots:
    /* Pop available sample from ring buffer and write to record_buffer. */
    void output_data_received(RingBuffer<float> *ring_buffer);

private:
    AudioSettings *audio_settings;
    AudioLinkedList recording_data;
};

} // CATE

#endif
