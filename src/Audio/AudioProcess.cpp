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

#include "AudioProcess.hpp"
#include "../Analysis/FFT.hpp"
#include "../Synthesis/Scheduler.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>

namespace CATE {

AudioProcess::AudioProcess(float sample_rate, int frames_per_buffer, int input_channels, int output_channels,
                           int fft_bin_size, Database &db,
                           PointCloud &point_cloud, KdTree &kd_tree)
        : AudioEngine(sample_rate, frames_per_buffer, input_channels, output_channels),
          fft(FFT(fft_bin_size, frames_per_buffer)),
          spectral_feature(sample_rate, fft_bin_size),
          magspec(vector<float>(fft_bin_size / 2 + 1)),
          db(db),
          point_cloud(point_cloud),
          kd_tree(kd_tree),
          return_indices(vector<size_t>(num_search_results)),
          distances(vector<float>(num_search_results)),
          granulator(db.get_files(), sample_rate)
{
}

int AudioProcess::processing_callback(const void *input_buffer,
                                      void *output_buffer,
                                      unsigned long frames_per_buffer,
                                      const PaStreamCallbackTimeInfo *time_info,
                                      PaStreamCallbackFlags status_flags)
{
    /* Casting callback parameters. */
    static_cast<void>(status_flags);
    static_cast<void>(time_info);
    auto *input = const_cast<float *>(static_cast<const float *>(input_buffer));
    auto *output = static_cast<float *>(output_buffer);

    /* FFT / spectral operations. */
    fft.fill(input);
    fft.compute();
    fft.get_magspec(magspec);
    centroid = spectral_feature.centroid(magspec);
    flatness = spectral_feature.flatness(magspec);

    /* Testing KNN search of point cloud. */
    const float search_points[2] = {centroid, flatness};
    kd_tree.knnSearch(&search_points[0], num_search_results, &return_indices[0],
                      &distances[0]);
    size_t ix = return_indices[0]; // Only using first result for now
    string file = point_cloud.points[ix].file_path;
    int marker = point_cloud.points[ix].marker;

    /* Main audio output block. */
    for (unsigned long i = 0; i < frames_per_buffer; ++i)
    {
        /* Temporary output logging. */
        // std::printf("File: %s\nMarker: %d\n\n", file.c_str(), marker);
        /* Input -> Output. */
        // *output++ = *input++;
        *output++ = granulator.synthesize();
    }

    return paContinue;
}

} // CATE
