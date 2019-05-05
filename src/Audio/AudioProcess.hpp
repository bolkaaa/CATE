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

#include <memory>
#include <vector>

#include <QObject>

#include "AudioEngine.hpp"
#include "AudioBuffer.hpp"
#include "AudioSettings.hpp"
#include "RingBuffer.hpp"
#include "src/Analysis/FFT.hpp"
#include "src/Analysis/Feature.hpp"
#include "src/Corpus/Corpus.hpp"
#include "src/Corpus/KdTree.hpp"
#include "src/Synthesis/Granulator.hpp"

using std::unique_ptr;

namespace CATE {

/* The audio processing occurs in AudioProcess, which inherits from the
 * AudioEngine class that wraps the PortAudio functionality. It contains the
 * virtual audio callback function where the analysis and synthesis takes place. */
class AudioProcess : public QObject, public AudioEngine
{
Q_OBJECT

public:
    AudioProcess(const unique_ptr<AudioSettings> &audio_settings, const unique_ptr<Corpus> &db,
                 const unique_ptr<PointCloud> &point_cloud, const unique_ptr<GrainParams> &grain_params,
                 const unique_ptr<EnvelopeParams> &env_params, const KdTree &kd_tree);

    /* Reload granulator when database has changed. */
    void reload_granulator();

    /* Start recording audio output. */
    inline void start_recording() { recording = true; };

    /* Stop recording audio output. */
    inline void stop_recording() { recording = false; };

    /* Save current audio recording to disk. */
    void save_recording(const string &output_path);

    /* Get flag for whether audio process is ready to be used. */
    bool is_ready() { return ready; }

    /* Enable "ready" flag. */
    void enable() { ready = true; }

private:
    /* Determine the next segment from the corpus to access via KNN search. */
    Unit select_unit(const float *input, int n);

    /* Compute magnitude spectrum of input frame. */
    void compute_magspec(const float *input, int n);

    AudioSettings *audio_settings;
    Corpus *db;
    PointCloud *point_cloud;
    const KdTree &kd_tree;
    GrainParams *grain_params;
    EnvelopeParams *env_params;
    Granulator granulator;
    FFT fft;
    Magspec magspec;
    const size_t num_search_results = 1;
    vector<size_t> return_indices;
    vector<float> distances;
    const int ring_buffer_size = 512;
    unique_ptr<RingBuffer> ring_buffer;
    bool recording;
    bool ready;

signals:
    void send_record_data(RingBuffer *ring_buffer);

protected:
    int processing_callback(const void *input_buffer,
                            void *output_buffer,
                            unsigned long buffer_size,
                            const PaStreamCallbackTimeInfo *time_info,
                            PaStreamCallbackFlags status_flags) override;

};

} // CATE

#endif
