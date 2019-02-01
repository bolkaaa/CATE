#include "sndfile.h"
#include "Wave.hpp"

namespace CATE
{
    Wave::Wave(const std::string &path)
    {
	SF_INFO info;
	SNDFILE *snd = sf_open(path.c_str(), SFM_READ, &info);

	length = info.frames * info.channels;
	data = new float[length];

	sf_read_float(snd, data, length);
    }

    Wave::~Wave()
    {
	free(data);
    }


} // CATE
