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

} // CATE
