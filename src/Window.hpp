#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cmath>

#include "AudioBuffer.hpp"

template <class T>
class Window : public AudioBuffer<T>
{
public:
    Window();

protected:
    vector<T> data;
};

template <class T>
Window<T>::Window()
    : data(AudioBuffer<T>::data)
{
}

template <class T>
class HannWindow : public Window<T>
{
public:
    HannWindow();

    /* Generate points of Hann window within data vector. */
    void generate();

private:
    vector<T> data;
};

template <class T>
HannWindow<T>::HannWindow()
    : data(Window<T>::data)
{
    generate();
}

template <class T>
void HannWindow<T>::generate()
{
    for (auto i = 0; i < data.size(); ++i)
    {
        data[i] = (1./2) * (1. - std::cos((2. * M_PI * i) / (data.size() - 1.)));
    }
}

#endif
