---
layout: post
title: Handling the Audio Database
date: '2019-02-10'
---
### Loading a Directory of Audio Files
My first objective was to find a way of automatically loading every
audio file from a directory into memory in the program.

After doing some research, I realised that this functionality only
exists natively in C++ with the C++17 standard, and I am using the
earlier C++14 standard in my project for compatibility
purposes. Therefore, I decided to use the filesystem header from the
boost library. This required installing the boost headers on my system
as I have not used them before when working with C++, and modifying my
CMakeLists file. After experimenting with the boost/filesystem
functions, I eventually implemented a way of getting the path of every
file deeper than a specified directory, using a recursive process:

1. A first, non-recursive function generates a list of all paths from a root directory.
2. The recursive function calls this function as its first statement.
3. It then iterates over that list.
4. If it encounters a directory path, the recursive function is called
   again, passing the path as a parameter.
5. Otherwise, the recursive function adds the path to the final path list.

This has been working well for the particular directory of example
sounds I am using. One particular issue that might arise is if the
user has a directory filled with a mixture of audio files and
non-audio files, which could result in the contents of those non-audio
files being treated as sample data. I may have to test for this case
and implement a way of checking if a file is an audio file or not.

### Pre-processing the Audio Database
My first idea was to convert every audio file loaded to the same
sample rate (the standard 44.1kHz CD-quality rate). I chose to do
this, as I had read in some papers relating to doing audio analysis on
a database of sounds that this would result in more effective
results. It also made it easier to test audio files with my program,
as it means I don't have to do any interpolation in the actual audio
processing function. I implemented a function for converting an audio
file to a new sample rate using the
[libsamplerate](https://github.com/erikd/libsamplerate)
library. Although I have been generally sticking to my philosophy of
avoiding C libraries and preferring native C++ code, I was only
implementing a very small function contained within my AudioBuffer
class, so I had no problems with it in this case. I found it
preferable to use this library than implementing my own sample rate
interpolation for a number of reasons:

1. Likely more efficient, faster, safer etc. than an implementation of my own.
3. Simplifies my own codebase by requiring less code.

That being said, I did become interested in sample rate interpolation when looking into this, and it's something that I would like to implement myself in the future.

### Python Utilities
It has been my plan with this project to make use of the Numpy and
Matplotlib libraries for the Python programming language, to help with
the creation of C++ DSP code. This week I created some simple plotting
tools which allow me to quickly view the waveform or spectrum of a
given audio file. When I am developing the audio analysis functions,
it will be helpful to prototype them in Python and plot their results
before implementing them in C++. 

### SDIF Files
I began to research the SDIF file format more, thinking about how I
could use it in my program. I plan to implement the simplest parts
of its functionality to begin with, reading and writing files and
printing their contents in my program. When I progress to the audio
analysis aspects later, I will start to flesh out the SDIF code more.

### Audio Buffer Segmentation
At this point I also wanted to experiment with splitting AudioBuffer
objects into sub-objects and playing them back with a modified version
of the Synth class. This was successful, and I was able to play back
short snippets of files from the database. It made me think about how
I will handle the segmentation process later on, as I realised that
pre-segmenting every audio file will likely be impractical and
inflexible. I looked back to how existing concatenative synthesis
implementations such as CataRT deal with this, and realised that
storing segmentation positions as numeric data may be the best
option. I can then do the actual segmentation in real-time in the
Synth::process function. Right now my understanding of this process
based on my research into concatenative synthesis is as follows:

1. The audio database is analysed, and segmentation markers are
   generated along with descriptors.
2. The information is stored in files to be recalled later. Each
   segment may have its own row in the database along with columns of
   features extracted from the audio analysis process.
3. During the synthesis stage, the user specifies target parameters,
   and a combination of segments from the database are generated

Once I begin working on the the analysis and synthesis stage of the
program, I will be able to experiment with the approach more. I plan
to first implement a non-realtime version which will allow me to
experiment with different target parameters through the command line
and create automated tests, which should make the process easier,
although I will also need a way of testing its real-time capabilities.

### Moving Forwards
At this point I have already partially completed some goals from weeks
3 and 4 of the production schedule. My goal at this point is to take
some time to improve the code that I already written along with the
rest of those production targets, while doing more reading and
planning of the analysis and synthesis stage.

I have still not implemented audio input recording, which I will
definitely aim to achieve in the next two weeks. I also want to
implement a better command line interface with best practises for C++
CLI programs, expanding upon the simple interface I currently have,
which directly uses the argc and argv parameters inherited from C.
