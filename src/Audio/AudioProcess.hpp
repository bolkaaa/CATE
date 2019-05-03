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

#include "AudioEngine.hpp"
#include "AudioBuffer.hpp"
#include "AudioRecorder.hpp"
#include "AudioSettings.hpp"
#include "src/Analysis/FFT.hpp"
#include "src/Analysis/Feature.hpp"
#include "src/Corpus/Corpus.hpp"
#include "src/Corpus/KdTree.hpp"
#include "src/Synthesis/Granulator.hpp"

namespace CATE {

/* The audio processing occurs in AudioProcess, which inherits from the
 * AudioEngine class that wraps the PortAudio functionality. It contains the
 * virtual audio callback function where the analysis and synthesis takes place. */
class AudioProcess : public QObject, public AudioEngine
{
Q_OBJECT

public:
    AudioProcess(AudioSettings &audio_settings, Corpus &db, PointCloud &point_cloud, KdTree &kd_tree);

    /* Reload granulator when database has changed. */
    void reload_granulator();

    /* Start recording audio output. */
    void start_recording();

    /* Stop recording audio output. */
    void stop_recording();

    /* Set attack value of synthesis grains. */
    void set_grain_attack(float new_grain_attack);

    /* Set decay value of synthesis grains. */
    void set_grain_release(float new_grain_release);

    /* Set amplitude of synthesis output. */
    void set_grain_sustain(float new_grain_sustain);

    /* Set density value of synthesis grains. */
    void set_grain_density(int new_grain_density);

    /* Set grain size of synthesis grains. */
    void set_grain_size(int new_grain_size);

    /* Get flag for whether audio process is ready to be used. */
    bool is_ready() { return ready; }

    /* Enable "ready" flag. */
    void enable() { ready = true; }

    /* Save current audio recording to disk. */
    void save_recording(const string &output_path);

private:
    /* Determine the next segment from the corpus to access via KNN search. */
    Unit select_unit(const float *input);

    /* Compute magnitude spectrum of input frame. */
    void compute_magspec(const float *input);

    AudioSettings &audio_settings;
    Corpus &db;
    PointCloud &point_cloud;
    KdTree &kd_tree;
    Granulator granulator;
    AudioRecorder audio_recorder;
    FFT fft;
    MagSpec magspec;
    const size_t num_search_results = 1;
    int next_marker;
    string next_file_path;
    vector<size_t> return_indices;
    vector<float> distances;
    bool recording;
    bool ready;

protected:
    int processing_callback(const void *input_buffer,
                            void *output_buffer,
                            unsigned long frames_per_buffer,
                            const PaStreamCallbackTimeInfo *time_info,
                            PaStreamCallbackFlags status_flags) override;
};

} // CATE

#endif
