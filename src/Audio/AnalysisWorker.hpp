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
#include "src/Analysis/FeatureVector.hpp"
#include "src/Corpus/Corpus.hpp"
#include "src/Audio/RingBuffer.hpp"
#include "src/Audio/AudioSettings.hpp"

#include <QObject>

namespace CATE {

/* A pair of file position in samples and file path. */
typedef pair<int, Path> CorpusIndex;

class AnalysisWorker : public QObject
{
Q_OBJECT
public:
    AnalysisWorker(AudioSettings *audio_settings, Corpus *corpus);

    ~AnalysisWorker();

private:
    /* Calculate magnitude spectrum of input from ring buffer. */
    void calculate_magnitude_spectrum();

    AudioSettings *audio_settings;
    Corpus *corpus;
    const int num_search_results = 32;
    const int search_result_queue_size = 128;
    int counter;
    FeatureVector features;
    AudioBuffer buffer;
    Magspec magspec;
    RingBuffer<int> *search_results;
    FFT fft;

public slots:
    void input_data_received(RingBuffer<float> *ring_buffer);

signals:
    /* Emit search results buffer as signal. */
    void send_search_results(RingBuffer<int> *search_results);

    /* Emit analysis values as signals. */
    void send_marker(int *marker);
    void send_centroid(float *centroid);
    void send_flatness(float *flatness);
    void send_rolloff(float *rolloff);
};

} // CATE

#endif
