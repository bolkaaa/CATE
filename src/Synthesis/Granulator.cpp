#include "Granulator.hpp"

using std::vector;

namespace CATE {

Granulator::Granulator(vector<AudioFile> files, float sample_rate)
        : files(files),
          sample_rate(sample_rate),
          scheduler(files, sample_rate)
{
}

float Granulator::synthesize()
{
    float output = scheduler.schedule();
    return output;
}


} // CATE
