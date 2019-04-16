---
layout: post
title: 'Finalising Some Features and Starting Unit Testing'
date: '2019-04-14'
---
### Progress
While continuing to work on the report, I have also added some final features in
the software. These included adding amplitude following of the microphone input
for the synthesis output, and adding the functionality for recording output to a
file.

### Amplitude Following
One feature that I have intended to be in the program for a while but only
recently implemented was a relationship between the audio input and output.
Essentially, I wanted the amplitude of the synthesised output to be the same as
the amplitude of the microphone input. Without such a feature, the resulting
effect is that quiet and intense input gestures would result in the same level
of output, which I wanted to avoid. To achieve this, I implemented a calculation
of the *root mean square* (RMS) value of the input samples:

$$
\sqrt{\frac{1}{n}\sum_{i=0}^{n}x_i^{2}}
$$

This value is multiplied with the output sample for each sample in the callback
function, which has the effect of the output amplitude closely following the
input amplitude. 

### Recording to File
One drawback of creating a standalone audio synthesis/processing application as
opposed to a DAW/host-compatible plugin is that there is no immediate way of
having an output for the audio that can be used elsewhere. With a plugin, the
audio can go directly to the DAW and be used there. With my program, I had to
implement a way of having the audio output from the program be usable in other
contexts. My way of achieving this was to implement audio recording, so that the
user can experiment however they like, and save the resulting output to an audio
file. This approach is similar to how applications such as IRCAM's AudioSculpt
are designed to be used.

I delayed implementing this feature for some time as I was unsure of how to
achieve it in an efficient manner. Eventually, I realised it was more important
to have it working, even if the implementation was not ideal. The basic
algorithm I designed was as follows:

1. A high capacity, fixed-size array is instantiated along with the other objects in the AudioProcess class.
2. A boolean parameter determines whether the program is in "record mode" or not.
3. In the GUI, a button is provided that sets the parameter to *true*. 
4. In the main audio loop, the output samples are written to the array according to the condition of the recording parameter being equal to *true*.
5. This simply uses a counter that increments on each successive sample to index
   into the array, rather than some kind of "push back" container function,
   which would be too inefficient in an audio context.
6. When the user is done and has pressed the "Stop Recording" button, the size
   of the resultant file in samples is determined by the value of the counter
   after recording has stopped. A WAV/AIFF file is written to disk using the
   sample data from the array.
   
The main drawback I found with this approach is that it's quite
memory-inefficient, since a large array needs to be preallocated. I couldn't
find a better way of doing it however, as I figured that any approaches that
would be both space-efficient and time-efficient would likely require some kind
of buffering system with multiple threads, which I did not want to implement
with the constraints of how I had previously written the program.

### Unit Testing
While I have been busy adding the previously mentioned features and writing my
dissertation, I have also been spending more time reading documentation for the
Catch2 framework. In the next week I aim to get the functionality for creating
tests working with my build system. The first tests I will write will likely be
for testing the FFT and feature extraction code.


### Future Plans
Currently, there is approximately one month left to finish working on the
program and writing the dissertation. Ideally, as discussed with my supervisor,
I should try to have a feature freeze and stop working on new features, and
focus on bug fixing, error handling and unit testing. Currently, the only
features I really intend to work on are improving the analysis code and the GUI.
In the next week, I hope to have these finished while starting on unit tests. 
