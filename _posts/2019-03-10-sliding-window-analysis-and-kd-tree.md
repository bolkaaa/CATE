---
layout: post
title: 'Sliding Window Analysis and K-d Tree'
date: '2019-03-10'
---
### Progress
This week I have been building upon the audio analysis code implemented
previously. I have added functionality to perform that analysis on fixed audio
files as well as on the live input that was already present. I also added the
K-d Tree implementation to perform nearest neighbour searches, using the
[nanoflann](https://github.com/jlblancoc/nanoflann) library. I also fixed some
critical bugs with my program, as well as simplifying the codebase.

### Code Simplifications
Previously, I had a class design for audio data which was meant to encompass
audio data loaded from files as well as sample data in general, for example
buffers created from mathematical functions. I wasn't happy with how I had
implemented this, where the sample data itself was encapsulated in a private
member variable, with public member functions for operations such as file
reading. The problem with this was that I had to make heavy usage of getter
functions to access the data in other classes, which defeated the purpose of it
for me. Therefore I decided to just work in a simpler way, passing std::vectors
of sample data around instead, and having specific, simple classes such as an
AudioFile class that just had the vector with public access. I used a typedef to
alias the vector to the name AudioBuffer to make it more symbolic and easier to
work with, and separate its meaning from the specific std::vector container. I
also removed a few functions from my program that I am not using currently,
which I can always add again later if necessary.

### Fixing a Painful Memory Bug
Recently I have been having some problems with my program relating to memory
that I was struggling to debug due to the nature of such issues. My program was
having issues with segmentation faults and other memory-related errors. I
eventually solved it by taking an approach where I would comment out all but the
most critical parts of my program and recompile, until the issue had gone away.
Then, I would continuously uncomment, recompile and test in a cycle, waiting to
find the combination of code that resulted in the memory errors. While going
through this process, I removed the spectrogram plotting code, as I no longer
planned to use it, and I also suspected it may be part of the problem. This
suspicion proved warranted, as my program is now free of those memory errors,
which is extremely good going forwards.

### Sliding Window Analysis of Audio Files
Previously, I had implemented FFT routines as well as functions for calculating
two different spectral features: spectal centroid and spectral flatness, which I
had successfully implemented in my audio callback function.

My next goal was to implement this same functionality, but on frames of audio
files. The frame size should be equal to the frames per buffer variable I was
using in my audio callback function, so that the extracted data was equivalent
in meaning. I created functions that iterated over the loaded audio files,
moving a window of <frame_size> frames, and passing those subvectors to my FFT
and spectral feature pipeline. Then, this data is able to be stored in the
analysis files, along with the marker positions and filenames.

### K-d Tree Implementation
A K-d tree is a data structure that partitions data in such a way that you can
perform k-nearest neighbour searches of the data much more efficiently. The
search time complexity when using a K-d tree is an average case of O(log n),
whereas regular KNN searches are at best O(nd) from the research I have done,
which is significantly slower. One tradeoff is there is some time needed to
build the tree, but this is not nearly as important as the time spent doing the
search itself, as the search will be taking place in the more critical real-time
audio part of my code.

As mentioned previously, I used the nanoflann library for this. I had done some
research, and it seemed like the perfect option, as one of its main priorities
is speed and efficiency, which is important for real-time concerns of my
program. My utilisation of it required some particular details. Notably, the
library is designed in such a way that means you may only have one instance of
the K-d tree object in place in your program at a time, as its copy constructor
and assignment operators are deleted. Therefore, I implemented the object in the
main function of my program. Then, a reference to that object is passed to
functionality that needs it. Essentially, you create a primitive data structure
where your data is stored as points. You create a std::vector of these point
objects, which is used by the K-d tree object to contruct the tree index. Then,
the workflow is as simple as calling one function that takes the following
arguments:

- An array of input points
- The number of search results to return
- A std::vector to store the return indices of your container of data points found by the search 
- Another std::vector to store the actual distances of those indices from the input points

I have added a call to it within my audio callback function, and it is operating
smoothly so far. I am passing it the centroid and flatness values generated from
the microphone input, and it is outputting the files and markers in the audio
file database that are closest in distance to the current values. My plan is to
use this information to drive a granular synthesis process, where the selected
grains are determined by the file IDs and segmentation markers.

### Moving Onwards
In my production schedule, the next two weeks were allocated towards the
granular synthesis implementation. I am happy about this, as it means that I
have been following and meeting my production schedule perfectly. I plan to take
my time and first implement a very simple synthesis system. Then I can add more
complicated features later. I'm particularly interested in how I can break down
the granular/concatenative synthesis process into different constituent parts,
in a flexible way. I will be looking to different code examples to help me
achieve the best results possible.
