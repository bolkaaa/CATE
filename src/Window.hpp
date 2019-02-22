#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cmath>

#include "AudioBuffer.hpp"

template <class T>
class Window : public AudioBuffer<T>
{
public:
    Window(uint16_t sz);

    /* Fill data vector with the output of the Hann function. */
    void generate();

private:
    vector<T> data;
};

template <class T>
Window<T>::Window(uint16_t sz)
    : data(AudioBuffer<T>::data)
{
    generate();
}

template <class T>
void Window<T>::generate()
{
    for (uint16_t i = 0; i < data.size(); ++i)
    {
        data[i] = (1./2) * (1. - std::cos((2. * M_PI * i) / (data.size() - 1.)));
    }
}

#endif
