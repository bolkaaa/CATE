---
layout: post
title: 'Discrete Fourier Transforms and Starting on Qt GUI'
date: '2019-02-24'
---
### This Week
This week, I've been working on the following parts of my program:

- *Discrete Fourier Transforms*

- *Starting on the Qt GUI*

- *More Work on Python Utilities*

- *Creating a More Generalised Audio Buffer Class*

### Implementing Discrete Fourier Transforms
For spectral audio analysis and visualisation, having some way of converting my
audio sample data to the frequency domain will be essential.

My requirements for a FFT library were that it should be a well-established,
standard library used by high-profile open source programs, and that it should
only offer functionality for doing discrete fourier transforms and not other
unnecessary DSP functions that I was already implementing myself. The only
library that really met this criteria was [fftw](http://www.fftw.org/). This is
a very significant library within open source/free-as-in-freedom software,
having been used by a lot of audio and signal processing applications in
general. I felt that it was a good opportunity for me to learn how to use it.
The only downside was that it's natively a C library rather than a C++ library,
but I could not find any natively C++ alternatives.
   
The library can be quite unforgiving to use, with lots of potential for
segmentation faults and other issues with array access due to how memory works
in C. After some experimentation, I eventually got a good system working. The
fftw library works in the following way:

1. Allocate memory for input and output arrays
2. Create a "plan" that defines the operations carried out, e.g
   *complex-to-complex* or *real-to-complex*, as well as specifying the arrays
   created in the last step.
3. Call the fftw_execute function to get the output spectrum array, which can be
   called as many times as needed (e.g, in a real-time loop).
4. Deallocate memory and call fftw_destroy_plan when done (e.g, when audio stream is finished).

I used the [one-dimensional
real-to-complex](http://www.fftw.org/fftw3_doc/One_002dDimensional-DFTs-of-Real-Data.html)
plan, which converts a one-dimensional input of floating point numbers to an
output array of complex numbers. The specified input array is *n* real numbers,
and the output is *n/2+1* complex numbers, which means the input array needs to
be filled with *N/2+1* elements and then padded with zeros. Additionally, the
array of data should be windowed, to avoid discontinuities at the starts and
ends. I just included an application of the Hann function within the function
that fills the data buffer, which is defined as:

$$
w(n) = \frac{1} {2} \left(1 - cos\left(\frac {2\pi n} {N - 1}\right)\right)
$$

where *n* is the sample index and *N* is the total number of samples.

Generally when using DFT data, the magnitude of the resultant complex output is taken. This can be calculated as follows:

$$
\sqrt{r^2 + i^2}
$$

where *r* and *i* are the real and imaginary parts of a complex number
respectively. When using std::complex in C++, you can just use the std::abs
function from the standard library to get the magnitude, making for a more
elegant implementation.

To use my spectrum array in a meaningful way, I will need to convert the
indices to their respective frequencies. The following formula can be used for
converting bin index to frequency:

$$
\frac{n  \frac{Fs} {2}} {N}
$$

where *n* is the bin index, *Fs* is the sampling rate and *N* is the total number
of bins. Hence, the 512th bin represents the magnitude of the frequency 11025Hz,
given a bin size of 1024 and a sample rate of 44100.

### Starting on the Qt GUI
I decided to commit to using Qt in my project and start working on it ahead of
schedule. This is mostly because I've realised that the backend of the code will
need to be redesigned quite a lot around the general practises of Qt
applications, such as the idea of QObjects. My reasons for using Qt are mostly
that it's a well-established GUI library/framework that has been used by a lot
of applications, including notable audio software like
[SuperCollider](https://supercollider.github.io) and
[SonicVisualiser](https://www.sonicvisualiser.org/).

I had to alter my CMake configuration to get Qt working, using the [following
github repo](https://github.com/euler0/mini-cmake-qt) as a guideline.

I then aimed to get a program working where my audio process is running in a
background with a Qt window interface in the foreground. I started with
[the following example from a github
repo](https://github.com/fabienpn/simple-qt-thread-example).

I also found [this
example](http://www.martin-kumm.de/wiki/doku.php?id=05Misc:A_Template_for_Audio_DSP_Applications)
of combining PortAudio, Qt and FFTW which was perfect for what I'm trying to do.

Learning from those examples, I now have the Qt graphics framework implemented
in my program. The program now launches as a window with buttons for starting
and stopping the audio engine. The downside of this, is that I was forced to
revert my audio code to use the C API of PortAudio rather than the C++ version.
Integrating the C++ PortAudio code with Qt was proving troublesome, and it was
considerably easier to wrap the low-level C code instead. I did want to avoid
this, but after reading some discussions in the PortAudio mailing list, this
seems to be what is generally done by people working with PortAudio in C++
anyway.

As for Qt, I will leave the actual bulk of the GUI design until much later, but
I want to get Qt integrated into my program as soon as possible. Hopefully
within the next month or so I will have a working prototype of the final system.
Then, I can improve upon it while working on the GUI. The final GUI will likely
be fairly simple, as I am new to Qt and it could be very time consuming to
create something complex in it. Additionally, the idea for my program is that it
should have a minimal interface and not require a lot of user input anyway!

### Creating a More Generalised Audio Buffer Class
I have also been thinking about the overall design of my program and its data
flow. As I've mostly been working on audio input, ring buffer and FFT code
recently, the earlier code I had written to read and write data from sound files
has become somewhat incompatible with the new code. My idea now is to have
interoperability between my classes that work with sample data. RingBuffer, FFT,
and AudioBuffer objects filled with data from audio files should be able to
communicate and share data easily. Therefore, I have looked into making my
AudioBuffer class a base class for the others and using inheritance to achieve
this.

### More Python Utilities
I also did a bit more work on the simple Python programs in the scripts
directory. I plan to compare the results of my C++ program's outputs for things
like magnitude spectrum and analysis data. With libraries like
[librosa](https://librosa.github.io/librosa/), I can check what the output
should look like for these processes very quickly and easily. My other
motivation for doing this, is related to the overall ethos I have with this in
project in thinking in the long-term as well as about the project itself. I
think that improving my proficiency at using libraries with librosa and Python
prototyping in general will be a useful skill for me in the future.

### Moving Onwards
My plan for the next week is to experiment with creating my own audio analysis
system with fftw and other low-level libraries. As a fallback option, I have
installed the Essentia library on my system and I will use it if this becomes
too much work. I am also thinking ahead to some of the next steps I will need to
take, such as the nearest-neighbours search of the parameter-space once I have
my analysis functionality sorted out. I have covered the K-Nearest neighbours
algorithm in other classes, where I learned that the K-d tree data structure
allows for a more efficient implementation of the algorithm, which will likely
be necessary so I can perform the comparisons efficiently in the audio loop.

Additionally, I want to implement better error handling with my PortAudio code
and clean it up in general. I will refrain from doing a lot of GUI work right
now, although I might add some kind of text output in the main window for
debugging pourposes.
