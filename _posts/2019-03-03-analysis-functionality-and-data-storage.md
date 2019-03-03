---
layout: post
title: 'Analysis Functionality and Data Storage'
date: '2019-03-03'
---
### Progress
I have been implementing functionality for audio analysis and storage for the program. I am on schedule with this and will continue working on it next week as well. 

This week's additions are:

- *Visualisation of the magnitude spectrum*

- *Implementing spectral analysis functions*

- *Implementing persistent storage of analysis data in JSON format*

- *General Changes to Code Design*

### Visualisation of the Magnitude Spectrum
I wanted a way of verifying the workings of my magnitude spectrum computation with visual feedback. I used a [code example](http://www.martin-kumm.de/wiki/doku.php?id=05Misc:A_Template_for_Audio_DSP_Applications) found online as a guideline when working on this. I will likely have a very different GUI by the completion of my program, but for now it has been helpful for this purpose, as well as teaching me more about Qt and the Qt plotting library. I may also add some extra plotting of my audio features, as a way of verifying their results easily as well.

### Implementing Spectral Analysis Functions
This week, I initially spent some time working with the [Essentia](https://essentia.upf.edu/) library, discovering if it would be suitable for my needs. In the end, I decided not to use it. All of the provided examples of its usage are non-real time utilisations, where analysis data is written to a file. It is important for my program to have real-time analysis. I found there to be very little guidance on doing this with Essentia, with the one example provided using an out of date version of Essentia, with numerous functions used within it that no longer exist in the library.

Therefore, I decided to implement my own audio analysis functionality, which was made easier by the fact that I have already worked on the FFT code. Leading on from that, I chose to implement two features in particular: spectral centroid and spectral flatness. Some reasons for this are as follows:

- They are relatively easy to calculate if you already have a magnitude spectrum computed, which I had functionality for with my previous week's work
- Compared to many other features, they are quite perceptually useful for a broad range of sounds that aren't necessarily tonal or rhythmic, which is ideal for the audio sources my program is intended to be making use of. Centroids are linked to brightness in timbre, and flatness is a noisiness ratio where 1.0 is white noise and 0.0 is sinusoidal.

I am focusing on implementing just two features right now, and then moving on to working on the nearest neighbours search functionality. Once that is functioning correctly, I will look to add more analysis functions.

Currently, the analysis code is structured in a way that there is a base "Feature" class that contains data like the sample rate of the system, which other feature classes can inherit from. I may simplify this later if the polymorphism-based approach seems unnecessary, but for now it seems like a logical way to do it.

### Implementing Persistent Storage of Analysis Data in JSON Format
It was important to add a way of persistently storing audio file paths and their associated analysis data arrays in some kind of data format.

My choices were:

- A database system / SQL
- Creating and parsing plain text files
- Using a data format like XML or JSON

I went with JSON for a number of reasons:
- It's very easy to work with

- It's a standard, portable format that is widely used and well-understood

- There is good functionality for working with it in C++ (through a third-party
library)

One potential drawback could be that the file size may grow large with a lot of
audio files and associated analysis data. The program likely won't be working
with many audio files at once however, and JSON is designed to store data in
much larger quantities than my program will be using.

I used the [following library](https://github.com/nlohmann/json) to get the functionality required. 

### Changes to Overall Code Design
One change was to change my usages of unsigned integer types with the simple "int" signed integer type. [I learned about some bugs that unsigned integers can result in](http://soundsoftware.ac.uk/c-pitfall-unsigned.html). Using ints for all integer types also simplifies the code and makes it more readable.


I also made some other simple type-related revisions, such as using double for sample rate variables, both for the reason that it is consistent with the libraries I am using, and that it makes the values easier to work with when doing calculations.

I also made more usage of the "auto" keyword in function bodies, which is considered good practice in modern C++. I plan to aim to write most of my code in this way, with the exception of some of audio code where I may have to focus more on efficiency and a more low-level way of doing things if necessary.

### Moving Onwards
My immediate plans moving forwards are as follows:

- Performing a sliding-window analysis of audio files, and storing the resulting data in the JSON data file

- Building Kd-tree from data and using KNN algorithm to find closest audio unit to current analysis frame. I plan to use the [following library](https://github.com/jlblancoc/nanoflann) for this

- Adding some more audio features, particularly spectral ones

