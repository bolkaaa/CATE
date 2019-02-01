#ifndef WAVE_HPP
#define WAVE_HPP

#include <string>

namespace CATE
{
    class Wave
    {
    public:
	Wave(const std::string &path);

	~Wave();

	float *data;
	int length;
    };
}

#endif
