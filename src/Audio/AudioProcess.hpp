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
#include <boost/functional/hash.hpp>

#include "AudioEngine.hpp"
#include "AudioBuffer.hpp"
#include "AudioSettings.hpp"
#include "RingBuffer.hpp"
#include "src/Corpus/Corpus.hpp"
#include "src/Synthesis/Granulator.hpp"
#include "AnalysisWorker.hpp"

namespace CATE {

/* The audio processing occurs in AudioProcess, which inherits from the
 * AudioEngine class that wraps the PortAudio functionality. It contains the
 * virtual audio callback function where the analysis and synthesis takes place. */
class AudioProcess : public QObject, public AudioEngine
{
Q_OBJECT

public:
    AudioProcess(AudioSettings *audio_settings, Corpus *corpus);

    ~AudioProcess();

    /* Start recording audio output. */
    inline void start_recording() { recording = true; };

    /* Stop recording audio output. */
    inline void stop_recording() { recording = false; };


    /* Analyse the audio from a particular directory. */
    void analyse_directory(const Path &directory_path);

    /* Load new corpus. */
    void load_corpus(const Path &corpus_path);

    /* Fill granulator with audio segments from corpus. */
    void fill_grain_buffers();

    /* Return status of whether granulator is ready to used. */
    bool granulator_has_files() { return granulator.is_ready(); }

    /* Set parameters. */
    void set_grain_attack(float attack) { granulator.set_grain_attack(attack); }
    void set_grain_sustain(float sustain) { granulator.set_grain_sustain(sustain); }
    void set_grain_release(float release) { granulator.set_grain_release(release); }
    void set_grain_size(int size) { granulator.set_grain_size(size); }
    void set_grain_density(float density) { granulator.set_grain_density(density); }
    void set_sample_rate(int selection_index) { audio_settings->set_sample_rate(selection_index); }
    void set_buffer_size(int selection_index) { audio_settings->set_buffer_size(selection_index); }
    void set_max_grains(int selection_index) { granulator.set_max_grains(selection_index); }

    const FixedParam<float> get_sample_rate() { return audio_settings->get_sample_rate(); }
    const FixedParam<unsigned long> get_buffer_size() { return audio_settings->get_buffer_size(); }
    const int get_bin_size() { return audio_settings->get_bin_size(); }
    const FixedParam<int> get_max_grains() { return granulator.get_max_grains(); }
    const Param<float> get_grain_attack() { return granulator.get_grain_attack(); }
    const Param<float> get_grain_sustain() { return granulator.get_grain_sustain(); }
    const Param<float> get_grain_release() { return granulator.get_grain_release(); }
    const Param<float> get_grain_density() { return granulator.get_grain_density(); }
    const Param<int> get_grain_size() { return granulator.get_grain_size(); }

private:
    AudioSettings *audio_settings;
    Corpus *corpus;
    Granulator granulator;
    const int ring_buffer_size = 512;
    RingBuffer<float> *input_ring_buffer;
    RingBuffer<float> *output_ring_buffer;
    RingBuffer<AudioIndex> *audio_index_queue;
    AudioFrameMap audio_frame_map;
    bool recording;

public slots:
    /* Action to take when signal received from AnalysisWorker. */
    void search_results_received(RingBuffer<AudioIndex> *search_results);

signals:
    /* Emit input ring buffer as signal. */
    void send_input_data(RingBuffer<float> *input_ring_buffer);

    /* Emit output ring buffer as signal. */
    void send_output_data(RingBuffer<float> *output_ring_buffer);

protected:
    int processing_callback(const void *input_buffer,
                            void *output_buffer,
                            unsigned long buffer_size,
                            const PaStreamCallbackTimeInfo *time_info,
                            PaStreamCallbackFlags status_flags) override;
};

} // CATE

#endif
