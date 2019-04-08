---
layout: post
title: 'More GUI Work'
date: '2019-03-31'
---
### Progress
There has been less progress with the software this week as I have been trying to do more writing. One significant 
change I have added is that the user may now select a directory of audio files to use when running the program. They 
may also choose a JSON file of analysis data that they have already saved in an previous session. 

### GUI Improvements
To elaborate upon this, the program now has buttons that open dialog prompts for selecting directories of audio 
files and analysing them, creating a more user-friendly experience than the command line system I had previously 
implemented. This required learning more about Qt and its QFileDialog class.

### Testing
I plan to start implementing unit tests soon. After doing some research, I found that 
[Catch2](https://github.com/catchorg/Catch2) seems to be a good option. It can be included in a C++ project as a 
single header file, and œmakes use of modern, idiomatic C++. Along with unit tests, I also plan to start testing the 
audio functionality of my program with manual tests of different types of audio. A significant section of my report 
will be a study of how the program responds to different types of audio, whether the effects are desirable or less so. 
  
### Finalising Features
It will be helpful to finish adding features to the program in the next week, so that I can focus on testing and 
writing my report. Fortunately, I have kept to my schedule well, and I have around a month to not only work on the 
report, but tie up loose ends with the program as well as do unit testing. The features I would like to finish with are:

- More feature extraction functions

I will likely just add more spectral functions, since I already have the FFT code in place to easily implement them. 
So far, I am quite happy with how the current features relate the audio input to the synthesis output, but having 
more features would give more control over this process.

- Synthesis improvements

Implementing interpolation of sample values to have control over grain pitch would be a useful feature, but I have had 
trouble implementing this in the past.

- Amplitude following of microphone input to control the output

Currently, the amplitude of the output is static regardless of the current microphone input. I intend to have them be
 correlated, so that quiet input sounds result in a quieter output and vice versa.

- More controls in the GUI
The main controls I plan to add are for configuring the user's inputs and outputs. Right now, it just uses the system
 defaults. For specific audio setups such as those that make use of external audio interfaces, this may not be 
 desirable.
 
Once I've finished with adding features to the program, I can focus on improving error handling and testing. I can 
also do some refactoring to improve the code and make it more readable and efficient while keeping the functionality 
the same.