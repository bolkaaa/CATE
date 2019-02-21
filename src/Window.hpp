#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cmath>

#include "AudioBuffer.hpp"

template <class T>
class Window : public AudioBuffer<T>
{
public:
    Window();

    /* Fill data vector with the output of the Hann function. */
    void generate();

protected:
    vector<T> data;
};

template <class T>
Window<T>::Window()
    : data(AudioBuffer<T>::data)
{
    generate();
}

template <class T>
void Window<T>::generate()
{
    for (auto i = 0; i < data.size(); ++i)
    {
        data[i] = (1./2) * (1. - std::cos((2. * M_PI * i) / (data.size() - 1.)));
    }
}

#endif
