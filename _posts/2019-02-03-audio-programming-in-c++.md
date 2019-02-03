---
layout: post
title: Audio Programming in C++
date: '2019-02-02'
---
### Starting Out

My main goal when starting the project was to first establish which
libraries for working with audio in C++ would form the basis of the
program, and how I would design and structure the audio backend of the
program. I have found that when creating audio applications in C++,
choosing between C and C++ libraries is an important choice. Usually,
the option is to either use established, widely-used C libraries like
portaudio and libsndfile, and hide their C functions behind a C++
interface, or use native C++ libraries. I have noticed that certain
applications like SuperCollider opt for the former, however wrapping C
code in C++ can be time consuming and error-prone and should generally
be avoided when possible, as I have discussed with my supervisor. In
general, I think that it's better to use the STL and modern C++
features, rather than using the backward-compatible C parts of
C++. This post recaps my experiences with exploring both options,
comparing them and expanding upon my motivations for ultimately
deciding upon using the native C++ bindings for the portaudio and
libsndfile libraries.

### Reading Audio File Data
My first objective was dealing with uncompressed audio file formats
like WAV and AIFF; Implementing functionality for reading existing
files and storing their sample values in data structures, as well as
creating output files from sequences of samples.

I initially used the C library libsndfile, which provides an easy way
of dealing with audio files without having to worry about issues like
file headers and endianness. This involved creating a wrapper class
that hid the C functions behind a C++ interface.

One immediate issue I found was that due to C's lack of templates and
function overloading, each return type for the sf_read functions has a
separate function. Working around this to create generic functions
would add unnecessary complexity, if I wanted my functions to work
with both floats and doubles for instance.

Therefore, I eventually decided to use the C++ API instead, which
provides a SndfileHandle class with overloaded member functions,
fitting better into a C++ program. 

### Audio I/O API

Prior to starting the project, I decided upon using the C PortAudio
library as I had experience using it in the past. I achieved good
results initially, using it in a procedural manner to play back audio
file data. When I wanted to adapt the code to a more templated
object-oriented C++ style that I intended the rest of my program to be
written in, I encountered some challenges that made me reassess my
options going forwards. Wrapping the functionality in C++ classes was
proving to be time consuming and requiring a lot of workarounds, so I
decided to use the C++ bindings for the library rather than create my
own wrappers. As well as being less time-consuming and allowing me to
focus more on writing the application itself, this has the advantage
of being better tested and more reliable due to being created by the
developers of the library.

### Progress Summary

So far, the program is able to load audio file data into an
AudioBuffer class, and output them through the Synth::process member
function of the Synth class. Later, these components will be expanded
upon, so that the AudioBuffers can be split into smaller segmented
buffers that can be played back simultaneously.

The program also has some basic multithreading capabilities so far using the std::thread class from C++11, with the audio stream starting on a separate thread while the main thread continues (as a while (true) loop for now as user input isn't needed yet).

My plans moving forward are to add functionality for storing multiple
AudioBuffers in memory from a directory of audio files, in preparation
for the audio analysis part of my program. I may use the Boost library
for a portable way of interacting with the filesystem to achieve
this. I will also look ahead to how I can store the analysis data in
some kind of lightweight database, which may involve utilising the SDIF file format.