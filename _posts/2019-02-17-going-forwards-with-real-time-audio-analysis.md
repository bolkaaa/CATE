---
layout: post
title: 'Moving Onwards: Real-time Audio Analysis'
date: '2019-02-17'
---
### Thoughts on the Program's Direction
I have been thinking a lot about my aims with the program and my report, and the
particular niche of musical concatenative synthesis I would like to explore. One
aspect I would like to focus more strongly on, is the notion of working with
real-time audio input. Thus, CATE would primarily be an experimental tool for
generating variations of an audio source in real-time. Essentially, this will be
a type of audio effect, where the incoming audio is transformed according to its
analysed features. 

*CataRT* offers this functionality, but overall it's a more general-purpose
realisation of corpus-based concatenative synthesis in a musical context. I
would aim to focus on this aspect of the technique and see how it can be
expanded upon and made more usable by electronic music practitioners such as
myself. One aspect that would be important to me, would be for the system to
easily "playable" by a solo performer/composer. Thus, the control of the system
should require little intervention after an initial configuration process. It
may be useful to do more research into current studies in improvisation-based
music software systems. The paper *Electro/Acoustic Improvisation and Deeply
Listening Machines* touches upon some ideas related to this.

Ideally, the relationship between the source sound and the new sound
concatenated from the corpus should be identifiable. As discussed with my
supervisor, one potentially interesting approach could be for there to be some
level of control over the distance function of the system. For instance, the
user of the program can specify for the output to be close in features to the
audio input, or this could be inverted and the resulting output could have
features that are the opposite of this. Some probabilistic systems could be
added, so that the chance of similar or dissimilar sonic output is controllable.

### Technical Aspects of Processing Real-Time Data
The first thing that I wanted to achieve this week was to implement a lock-free
circular buffer/ring buffer class for my program. In real-time audio processing
applications, circular buffers are a very useful data structure, as they allow
the program to operate on fixed blocks of samples that are updated as new data
comes into the system.

After doing a lot of research, I found a suitable implementation in the book
*Audio Anecdotes II: Tools, Tips, and Techniques for Digital Audio*, and I am
now using it in my Synth class. Data from the microphone is continuously pushed
into the buffer, which can then be popped into auxillary buffers for processing.
At the moment, I am just directing the input straight to output in this manner.


![Ring Buffer](https://upload.wikimedia.org/wikipedia/commons/thumb/f/fd/Circular_Buffer_Animation.gif/400px-Circular_Buffer_Animation.gif)


### Audio Analysis
I have also been weighing up my options when approaching the audio analysis
aspects of my program. I had the decision to either use a library that provided
this functionality for me, or to implement it myself. I wanted to keep with my
plan of implementing most features at a low level, but I am also wary of
balancing my time spent and not taking too long implementing certain aspects of
the program. I will compare my findings when looking at several libraries for C++:

#### **Essentia**
This is a well-established library for doing audio analysis in C++, which
includes many of the most widely-used audio feature extraction algorithms. The
advantage of using it would be the time saved in not implementing such code
myself, and that it may possess less errors than self-written code. That being
said, integrating a large library's functionality into my existing codebase also
brings with it some potential issues. Additionally, it would also be doing
much of the work for me, getting in the way of my learning how to implement such
processes myself. Although the end-result is of course important, one of my main
aims with this project is to get better at audio programming and understanding
signal processing, even if the process is more time consuming and error-prone.

#### **fftw3**
This is a comparatively low-level library that primarily computes discrete
fourier transforms of signal vectors, through a highly efficient FFT algorithm.
This approach intrigued me as I wanted to gain more experience working with DFTs
in C++ and it would mean that although I wouldn't have to create my own FFT
algorithm, I would have to implement much of the analysis code myself.

In the end, I moved forwards with using the fftw3 library and planning the
creation of my own analysis functions, while keeping time constraints in mind. I
am aiming to give myself plenty of time and have the option to revert to
fallback approaches if the approach turns out to be too demanding. I can also
switch to using Essentia or another option later on this way, if needed.

### Plans moving forwards
I have been working at quite a low level and the progress towards a working
prototype has been slow, due to the time that developing components from the
ground up can take. I want to stand by my approach, as it has been invaluable in
learning how to write audio/DSP code in C++, and I have been enjoying the
experience. That being said, it has made it difficult to reason about the aims
and success of my program, since I am still relatively far away from a working
prototype. With that in mind, I am aiming to move a bit faster with my
production schedule. I am now starting the audio analysis aspects of the program
and then working on the synthesis aspect shortly after.

I am aiming to achieve a basic working command-line-only prototype within the
next several weeks, which I will flesh out further from there after assessing
its results.
