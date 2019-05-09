---
layout: post
title: 'Many Improvements and More Testing'
date: '2019-04-28'
---

This was the busiest week yet of working on the program, as I wanted to improve
it as much as possible before the completion date. Due to the writing being more
or less finished by this point, I had a lot of time to do this. I tried to focus
on improving existing features rather than adding new ones. This included the
following alterations:

### Structural Improvements
In the past week, I have greatly simplified the class hierarchy of the program,
making it more readable and logical. Previously, I was instantiated the KdTree
object in main(), and passing it around the program as a reference. This was due
to my lack of understanding of an alternative way of instantiating it, which I
have now corrected. I have now moved it to the Corpus class, where it is stored
as a pointer. As its functionality is integral to the Corpus class and it can
now simply be accessed through that, I feel this is a much better way of doing
things. Likewise, I moved the PointCloud object to Corpus. I also made these
classes non-copyable, as I wanted to design the program such that only one
instance of objects created from these classes should exist. I created a base
class, *NonCopyable*, which other classes can inherit from. This is a class
where the assignment operator and copy constructor are deleted, meaning copies
cannot be created as those functions are normally used for.

### Multithreading
There were several parts of the program that I was unhappy with, in terms of
efficiency. My main concern was that all operations were taking place
sequentially on the same thread. This placed limitations on how the analysis and
synthesis code could interact. It also meant that my naive recording solution
was very memory-inefficient and unacceptable as a long term solution. Due to
this, I looked into the possiblity of moving these pieces of functionality to
dedicated threads, and having them operate independently of the synthesis code.
To achieve this, I used the QThread class from the Qt library, which worked well
with the rest of the Qt code in the program. I brought back the RingBuffer class
to the project, and used it extensively as a container for the data being passed
between the threads. Its usefulness for this comes from the fact that by design
it has protection against the possibility of operations from two threads
modifying the same access point at once.

Within the audio callback function, input (microphone) data is pushed into one
ring buffer, and otuput (synthesized) data is pushed into another. These are
emitted as a signal to the *AnalysisWorker* and *RecordWorker* objects
respectively, each of which occupy separate threads. In AnalysisWorker, the
spectral features are extracted and a KNN search finds the k-nearest data points
in the point cloud. In RecordWorker, the output data is pushed to a linked list,
which has the benefit for this use case of faster insertion time compared to a
dynamic array. The recording functionality is now working quite well, though it
still seems to drop samples at times, so it may need to be optimised further.

### Reworked Feature Extraction System
I wanted to create a more flexible feature extraction system that wasn't as
dependent upon manually referring to the defined functions. The *FeatureVector*
class contains the following members:

1. A vector of FeatureExtractors, which is a simple object comprised of a string to identify the extractor, and a function pointer to the corresponding function.

2. An unordered/hash map of <string, vector<float>> pairs. This allows the lookup of a vector of extracted values via the name of the feature.

3. A vector of markers (file positions)

Given a magnitude spectrum as input, the class can compute each feature value
and store it in the map. This class is now used within *Corpus* as part of the
sliding window analysis process.

### Improving Feature Extraction Functions
As I only had two features implemented (spectral centroid and spectral
flatness), and I wasn't happy with their implementations, I improved upon them.
Particularly for the spectral flatness function, I separate the calculations
into different functions, for calculating arithmetic and geometric mean. This
made the actual function much easier to break down and understand.

I also added a new feature, spectral rolloff. This is defined as the point of
the spectrum at which a particular percentage of the overall spectrum's energy,
often defined as 85%, is located within this point and below.

### Large Improvements to Synthesis Code
After struggling with the synthesis implementation earlier in the program's
development and being unsatisfied with its sonic results, I had to move on to
other parts of the program and accept how it turned out. Now that I have time to
improve things, I have come back to it and reworked it entirely. The main change
is that the grains for the current audio file corpus are now computed when it's
loaded. This means that I could remove the inefficient buffer-filling loops that
were slowing down the execution. Due to the fact that the audio files had to be
loaded into memory at all times with the previous method regardless, there is no
disadvantage to doing this. One issue is that I now have to find a strategy for
determining which grains to play back. As the actual number of available grains
created from the corpus will be very high, there needs to be a way of cycling
through a fixed set of them, as well as being able to update that set. Improving
this system will be my main priority from this point.

### Moving Towards a Better Input Analysis + Synthesis System
Moving on from the last point, I had the plan to improve how grains in the
corpus would be selected, based upon the results of the KNN search. My plan is
to have a hash table (std::unordered_map in C++) comprised of a particular
key/value pair. The key will be a integer/string pair where the integer is a
file position and the string is a file path. The value will be an integer that
corresponds to the index of the aforementioned grain container. In ideal
conditions, this will allow for an O(1) lookup of the corresponding corpus
grain, given positional file information. My priority now is to focus on taking
the marker/path pairs generated by the AnalysisWorker thread, and transporting
them to the Granulator object to calculate new indices in an effective manner.

### GUI Improvements
The sliders in the GUI now initialise to the same values as the actual
parameters, which was a much-needed improvement. Also, due to reworking the
synthesis code, the changes in audio from manipulating the sliders is now
smoother as well. I plan to improve the GUI a bit more before the completion
date, as I want to display some more information now that I have more threads to
work with.

### The Unit Tests
The unit testing has also been well underway in the past week. It has made me
regret not writing them sooner and taking a more test-driven development focused
approach, as I have already improved my code a lot due to issues discovered from
the tests I've written. My tests have been separated into the following categories, so far:

*Audio*: The PortAudio backend and ensuring it starts and stops properly.

*Analysis*: The creation of magnitude spectrum arrays
from audio file segments. It will also test the various feature extractors used in the program.

*Corpus*: The operations contained within the Corpus class, such as reading and writing the JSON files correctly.

*FileInputOutput*: The reading and writing of audio files.

*Synthesis*: The synthesis processes used in the program.

I plan to write a lot more tests over the next week as I finalise the program
and make sure that it's as good and bug-free as it can be.
