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

#ifndef ANALYSISWORKER_HPP
#define ANALYSISWORKER_HPP

#include "src/Analysis/FFT.hpp"
#include "src/Corpus/Corpus.hpp"
#include "src/Audio/RingBuffer.hpp"
#include "src/Audio/AudioSettings.hpp"
#include "src/Audio/AudioFile.hpp"

#include <QObject>

namespace CATE {

class AnalysisWorker : public QObject
{
Q_OBJECT
public:
    AnalysisWorker(AudioSettings *audio_settings);

public slots:
    void input_data_received(RingBuffer *ring_buffer);

private:
    /* Calculate magnitude spectrum of input from ring buffer. */
    void do_fft();

    AudioSettings *audio_settings;
    Corpus corpus;
    const int buffer_size = 512;
    int counter;
    AudioBuffer buffer = AudioBuffer(buffer_size);
    Magspec magspec = Magspec(buffer_size);
    FFT fft;
};

} // CATE

#endif
