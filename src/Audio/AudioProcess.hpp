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

#ifndef AUDIO_PROCESS_HPP
#define AUDIO_PROCESS_HPP

#include <vector>

#include <QObject>
#include <queue>

#include "AudioEngine.hpp"
#include "AudioBuffer.hpp"
#include "../Analysis/FFT.hpp"
#include "../Analysis/SpectralFeature.hpp"
#include "../Database/Database.hpp"
#include "../Database/KdTree.hpp"
#include "../Synthesis/Granulator.hpp"

using std::vector;
using std::queue;
using CATE::FFT;
using CATE::Database;
using CATE::KdTree;
using CATE::Granulator;

/* The audio processing occurs in AudioProcess, which inherits from the
 * AudioEngine class that wraps the PortAudio functionality. It contains the
 * virtual audio callback function where the analysis and synthesis takes place.
 * It also emits a signal when an FFT frame is processed, to be used by the Qt
 * GUI. */

namespace CATE {

class AudioProcess : public QObject, public AudioEngine
{
Q_OBJECT

public:
    AudioProcess(float sample_rate, int frames_per_buffer, int input_channels, int output_channels, int fft_bin_size,
                 Database &db, PointCloud &point_cloud, KdTree &kd_tree);

private:
    /* Feature extraction. */
    FFT fft;
    SpectralFeature spectral_feature;
    vector<float> magspec;
    float centroid;
    float flatness;

    /* Audio file management / K-d tree. */
    Database db;
    PointCloud &point_cloud;
    KdTree &kd_tree;
    const size_t num_search_results = 32;
    vector<size_t> return_indices;
    vector<float> distances;

    /* Synthesis. */
    vector<AudioBuffer> segments;
    Granulator granulator;

protected:
    int processing_callback(const void *input_buffer,
                            void *output_buffer,
                            unsigned long frames_per_buffer,
                            const PaStreamCallbackTimeInfo *time_info,
                            PaStreamCallbackFlags status_flags) override;
};

} // CATE

#endif
