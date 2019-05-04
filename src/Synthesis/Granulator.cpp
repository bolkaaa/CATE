#include "Granulator.hpp"

using std::vector;

namespace CATE {

Granulator::Granulator(AudioSettings *audio_settings, GrainParams *grain_params,
                       EnvelopeParams *env_params)
        : audio_settings(audio_settings),
          grain_params(grain_params),
          env_params(env_params),
          scheduler(audio_settings, grain_params, env_params)
{
}

float Granulator::synthesize(Unit unit)
{
    float output = scheduler.schedule(unit.marker, unit.file_path);
    return output;
}

void Granulator::load_files(const Corpus &corpus)
{
    scheduler.load_files(corpus.get_files());
}

} // CATE
