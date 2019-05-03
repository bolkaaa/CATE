#include "Granulator.hpp"

using std::vector;

namespace CATE {

Granulator::Granulator(float sample_rate)
        : sample_rate(sample_rate),
          envelope_params(grain_params.get_grain_size()),
          scheduler(grain_params, envelope_params, sample_rate)
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

void Granulator::set_grain_attack(float new_grain_attack)
{
    envelope_params.set_attack(new_grain_attack);
}

void Granulator::set_grain_sustain(float new_grain_sustain)
{
    envelope_params.set_sustain(new_grain_sustain);
}

void Granulator::set_grain_release(float new_grain_release)
{
    envelope_params.set_release(new_grain_release);
}

void Granulator::set_grain_density(int new_grain_density)
{
    grain_params.set_grain_density(new_grain_density);
}

void Granulator::set_grain_size(int new_grain_size)
{
    grain_params.set_grain_size(new_grain_size);
    envelope_params.set_sample_size(new_grain_size);
}

} // CATE
