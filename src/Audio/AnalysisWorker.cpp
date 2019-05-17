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

#include <string>

#include "AnalysisWorker.hpp"
#include "src/Analysis/Feature.hpp"

namespace CATE {

AnalysisWorker::AnalysisWorker(AudioSettings *audio_settings, Corpus *corpus)
        : audio_settings(audio_settings),
          corpus(corpus),
          fft(audio_settings),
          magspec(audio_settings->get_bin_size()),
          buffer(audio_settings->get_bin_size()),
          search_results(new RingBuffer<int>(search_result_queue_size))
{
}

AnalysisWorker::~AnalysisWorker()
{
    delete search_results;
}

void AnalysisWorker::input_data_received(RingBuffer<float> *ring_buffer)
{
    while (ring_buffer->samples_available())
    {
        ring_buffer->pop(buffer[counter]);
        ++counter;
    }

    /* When ring buffer is full, extract features and do a nearest-neighbours search of the corpus. */
    if (counter > buffer.size())
    {
        calculate_magnitude_spectrum();

        auto centroid = Feature::spectral_centroid(magspec);
        emit send_centroid(&centroid);

        auto flatness = Feature::spectral_flatness(magspec);
        emit send_flatness(&flatness);

        auto rolloff = Feature::spectral_rolloff(magspec);
        emit send_rolloff(&rolloff);

        const float query[Feature::num_features] = {
                centroid,
                flatness,
                rolloff
        };

        corpus->search(query);

        for (auto i = 0; i < num_search_results; ++i)
        {
            auto result = corpus->get_search_result(i);
            search_results->push(result);
        }

        emit send_search_results(search_results);

        counter -= buffer.size();
    }
}

void AnalysisWorker::calculate_magnitude_spectrum()
{
    fft.fill(buffer);
    fft.compute_spectrum();
    fft.compute_magspec();
    fft.get_magspec(magspec);
}

} // CATE
