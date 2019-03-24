---
layout: post
title: 'Expanding Upon Granular Synthesis'
date: '2019-03-24'
---
### Progress
This week I expanded upon the granular synthesis code I had created in the
previous week. The grains are now properly derived from the audio files matched
from the analysis process. The synthesis process is also more efficient and
correctly implemented, using object pooling techniques. I also started working
on a command line interface for managing the analysis process.

### Command Line Utility
One aspect of the program I wanted to develop was a basic command line utility
to generate new analysis data from specified directories of audio files. I used
a C++ library called "args" to handle the implementation details, allowing my
program to have a sophisticated CLI tool with argument flags. So far, it can be
used in the following way:

CATE_CLI -f 1024 -b 1024 analyse /directory/

where -f and -b specify the frames per buffer and FFT bin size parameters of the
analysis system respectively. I included pre-processor directives in my C++ code
and tweaked my build system so that multiple binaries are built from the source
code, the CLI tool and the GUI application. I will probably also have a separate
application that just executes the audio callback without a GUI.

###  Granular Synthesis Improvements
I fixed some issues I had with my granular synthesis model, relating to grain
activation/deactivation. I achieved this by incorporating a sample counter
within the synthesis function of the Grain class, so that the objects are able
to keep track of when they should be inactive. Essentially, the synthesize
function within the Grain class first checks if there are any more samples to
generate, and returns a zero-amplitude sample if the resultant boolean value is
false. Otherwise, it synthesizes a new sample and decrements the sample counter.

The grain creation function within the scheduler now iterates over the array of
grain objects until it finds an inactive grains, using that same is_active()
function. It then creates a new Grain object and immediately returns from the
function. This function is called on every interonset trigger.

The program now sounds reminiscent of classic granular synthesis. One issue is
there is a lack of control over grain duration, which limits the possibilities
in the sound. This is due to the fact that the grain duration is implemented as
a constant, tied to the segmentation sizes of the audio analysis system. I have
decided to just have a max duration that's tied to the maximum buffer size, and
then have the envelope duration be controllable.

### Relating the Synthesis and Analysis Processes
At the moment, the KNN function is called on each frame within the "frames per
buffer" loop in the audio callback function. It finds just one nearest neighbour
which is immediately used by the granular synthesis code. One approach may have
been to run this code on a separate thread, and have a queue of neighbours that
is consumed by the audio process. In the next week I will finalise this part of
the program, which will explore these options.
   
The audio files in the database are provided to the Scheduler in a C++ std::map
structure, which is keyed by a string filename. The filename is obtained from
the KNN function along with a segmentation marker position, which allows
creating a new audio buffer at the given position in the respective audio file.
The duration of the buffer must be fixed, however control over duration could be
allowed through manipulating the amplitude envelope of grains. 

### Next Steps
For the next week, I aim to improve the analysis and synthesis process in
general, adding more audio features and making the synthesis process more
aesthetically pleasing, such as envelope following of the microphone input's
amplitude to control the audio output's amplitude. I am hoping to finish most of
the functionality of the program in the next week or two so that I can focus on
the interface, error handling and unit tests.
