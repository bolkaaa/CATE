#include "EnvelopeParams.hpp"

namespace CATE {

EnvelopeParams::EnvelopeParams(int sample_size)
        : sample_size(sample_size),
          attack(0.5f),
          sustain(0.5f),
          release(0.5f)
{
}

} // CATE
