---
layout: post
title: 'Implementing Simple Granular Synthesis'
date: '2019-03-17'
---
### Progress
My goal for this week and the following week was working on a granular synthesis implementation. For the first week, I
 have aimed to just get a simple system working that has the core functionality needed going forwards. Then I plan to
  integrate it with my audio analysis code, so that the grains are derived from the matches found by the KNN search.

### Establishing the Granular Synthesis Model
I wanted to find a system for implementing granular synthesis in C++ that would have robust foundations and be 
modular/expandable. 
After doing a 
bit of research, I found the following article written by Ross Bencina, who also designed the PortAudio library I am 
using in my program. It seemed to be an ideal starting point, so I set out to understand it and implement the class 
structure proposed within it. In the article, a model for granular synthesis is broken down into different objects, 
each of which I will write briefly about: 

#### Source
The source is where audio used in the granulation comes from, which can be of the following types:
##### Tapped Delay Line
Sample data is stored from a real-time stream as a delay line. This is
particularly useful for granulators that operate as an audio effect. It can be
seen as the most complicated type of granular synthesis to implement, because
the delay line must be maintained and variable rate delay taps have to be
implemented for each grain.

##### Synthetic Grains
Each grain is synthesized through synthesis techniques such
as FM synthesis. Potentially simple to implement, because state doesn't need to
be shared between grains.

##### Stored Samples
Sample values for grains are stored in wavetables, which can be used to process
pre-generated waveforms or sample data from audio files.


As the goal of CATE is to resynthesize the audio from audio files, stored
samples will be the type of source.

#### Envelope
Generates sample values for an amplitude envelope. Types of envelope include simple parabolic envelopes, 
trapezoidal envelopes, or raised cosine bell envelopes. Grain envelopes can introduce spectral artifacts.
 Trapezoidal envelopes introduce the most spectral distortion due to discontinuities at the edges. An 
 alternative is raised cosine bell envelopes which are better in this regard, at the expense of greater processing 
 time.  
 
 There is a general time/space tradeoff with implementing amplitude envelopes, where you
 may choose to either compute the coefficients on demand or use a pre-computed
 wavetable. Bencina states that modern processors allow for a more efficient
 implementation when not using stored tables, due to the memory access overhead
 of accessing stored tables.
 
 I chose to implement a Parabolic envelope for now due to its relative
 simplicity, which I can always swap out later for a raised cosine bell envelope,
 which allows for more control over the envelope parameters.

#### Grain
Combines an Envelope and Source object to form an overall model of a grain in granular synthesis. The
 source sample is multiplied by the envelope sample when synthesized.
 
#### Scheduler
Generates time periods between grains and handles activating them. It also manages mixing and outputting active 
grains. A simple counting system generates a new [interonset](https://en.wikipedia.org/wiki/Time_point#Interonset_interval) 
time whenever the sample counter is equal to 0, and activates the 
next grain. Then, that sample counter is decremented. When it hits 0 again, the cycle continues and the next grain is
 activated. The interonset time can be generated with a simple function that uses a random number generator. I used 
functionality from the random header in C++ to get uniform distribution random numbers.

#### Granulator
The top-level class that contains the scheduler and any other necessary high-level functionality for 
using the synth. This is the class that will have a spawned object in the AudioProcess class of CATE, while the 
previous classes will all be hidden.

### Challenges to Overcome Going Forwards
After having implemented some basic granular synthesis, I feel that I have now
come to one of the more challenging aspects of implementing a granular system
that works with a large set of audio files, where the sliced segments within the files to be used as grains is 
constantly changing. Currently, I know that I have the following functionality to work with:

- A granular synthesis system that requires each grain to be provided with a source wavetable array.
- An audio analysis pipeline that relates lists of analysis features and segmentation positions with audio files
- A real-time analysis system that outputs the same features that are used in the stored analysis data
- A k-nearest neighbours function that returns the N nearest indices from the list of segmentation markers that 
correspond to the 
current block of input from the microphone
        
My task now is to tie these systems together, so that the segments extracted
from the audio files are granulated according to their features that are matched
with the microphone input.

### Current Progress Check and Future Plans
I am generally satisfied with my progress this week, as the most difficult aspect was approaching a topic like 
granular synthesis and understanding how to implement it in C++, while I am still new to low-level audio programming.
 I'm happy with the article written by Bencina that I found, as it improved my understanding of granular synthesis 
 and helped me get started on implementing it. I plan to spend the next week improving upon what I have, and properly
  combining it with the analysis aspects of the program. I may look into the concept of [object pooling](https://en.wikipedia.org/wiki/Object_pool_pattern), which is a 
  design pattern that specifies a set of pre-initialised objects that can reused, rather than destroying objects and 
  allocating new ones. Pooled objects can be recalled in a determinant amount of time as opposed to allocated 
  objects, which may take an indeterminant amount of time (an undesirable property in audio programming).
  
