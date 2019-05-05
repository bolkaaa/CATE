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
    AudioProcess(AudioSettings *audio_settings, Corpus *corpus, PointCloud *point_cloud, KdTree &kd_tree);

    /* Reload granulator when database has changed. */
    void reload_granulator();

    /* Start recording audio output. */
    inline void start_recording() { recording = true; };

    /* Stop recording audio output. */
    inline void stop_recording() { recording = false; };

    /* Analyse the audio from a particular directory. */
    void analyse_directory(const Path &directory_path);

    /* Rebuild k-d tree index, point cloud and granulator files. */
    void rebuild_data_points();

    /* Load new corpus. */
    void load_corpus(const Path &corpus_path);

    /* Return status of whether granulator is ready to used. */
    bool granulator_has_files() { return granulator.is_ready(); }

    /* Set parameters. */
    void set_grain_attack(float attack) { granulator.set_grain_attack(attack); }
    void set_grain_sustain(float sustain) { granulator.set_grain_sustain(sustain); }
    void set_grain_release(float release) { granulator.set_grain_release(release); }
    void set_grain_size(int size) { granulator.set_grain_size(size); env_params.set_sample_size(size); }
    void set_grain_density(float density) { granulator.set_grain_density(density); }
    void set_sample_rate(int selection_index) { audio_settings->set_sample_rate(selection_index); }
    void set_buffer_size(int selection_index) { audio_settings->set_buffer_size(selection_index); }
    void set_bin_size(int selection_index) { audio_settings->set_bin_size(selection_index); }
    void set_max_grains(int selection_index) { granulator.set_max_grains(selection_index); }

    /* Get parameters. */
    float get_sample_rate() const { return audio_settings->get_sample_rate(); }
    const SampleRateVector get_available_sample_rates() const { return audio_settings->get_available_sample_rates(); }
    const BinSizeVector get_available_bin_sizes() const { return audio_settings->get_available_bin_sizes(); }
    const BufferSizeVector get_available_buffer_sizes() const { return audio_settings->get_available_buffer_sizes(); }
    const MaxGrainsVector get_available_max_grains() const { return grain_params.get_available_max_grains(); }
    const int get_default_sample_rate_index() const { return audio_settings->get_default_sample_rate_index(); }
    const int get_default_bin_size_index() const { return audio_settings->get_default_bin_size_index(); }
    const int get_default_buffer_size_index() const { return audio_settings->get_default_buffer_size_index(); }
    const int get_default_max_grains_index() const { return grain_params.get_default_max_grains_index(); }

private:
    /* Determine the next segment from the corpus to access via KNN search. */
    Unit select_unit(const float *input, int n);

    /* Compute magnitude spectrum of input frame. */
    void compute_magspec(const float *input, int n);

    AudioSettings *audio_settings;
    Corpus *corpus;
    PointCloud *point_cloud;
    KdTree &kd_tree;
    GrainParams grain_params;
    EnvelopeParams env_params;
    Granulator granulator;
    FFT fft;
    Magspec magspec;
    const size_t num_search_results = 1;
    vector<size_t> return_indices;
    vector<float> distances;
    const int ring_buffer_size = 512;
    RingBuffer *ring_buffer;
    bool recording;

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
