#include "Granulator.hpp"

using std::vector;

namespace CATE {

Granulator::Granulator(map<string, AudioFile> files, float sample_rate)
        : files(files),
          sample_rate(sample_rate),
          scheduler(files, sample_rate)
{
}

float Granulator::synthesize(int marker, string filename)
{
    float output = scheduler.schedule(marker, filename);
    return output;
}

void Granulator::load_files(const Corpus &db)
{
    files = db.get_files();
    scheduler.load_files(files);
}

void Granulator::set_grain_attack(float new_grain_attack)
{
    scheduler.set_grain_attack(new_grain_attack);
}

void Granulator::set_grain_release(float new_grain_release)
{
    scheduler.set_grain_release(new_grain_release);
}

void Granulator::set_grain_density(int new_grain_density)
{
    scheduler.set_grain_density(new_grain_density);
}

void Granulator::set_grain_width(int new_grain_width)
{
    scheduler.set_grain_width(new_grain_width);
}

void Granulator::set_grain_size(int new_grain_size)
{
    scheduler.set_grain_size(new_grain_size);
}

} // CATE
