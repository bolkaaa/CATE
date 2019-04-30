---
layout: post
title: 'Overall Improvements to Program'
date: '2019-04-28'
---
Compared to last week, there were many improvements to the program this week, to
its features as well as the quality of the code through refactoring. While I am
trying to avoid adding significant new features, I am highly focused on
improving the existing ones. I will break down the improvements to the program I
have made this week.

### Synthesis
The granulation engine is now much more flexible, as the grain size can be
controlled in real time through a slider. Overall, I am quite happy with how it
sounds and find it quite aesthetically interesting compared to previous results
I had. 

### Interface
![Program Window](https://files.catbox.moe/1gctlq.png)
I have improved the interface of the program a lot:

- Value of parameters is now visible through number displays
- The layout of the window is now better-organised 
- The window is now fixed in size, so the UI elements cannot be displaced through resizing

The interface is now mostly complete, however I still need to add an option in the GUI to change the following audio settings:
- sample rate
- buffer size
- FFT bin size
- input device
- output device

This will be in the form of a secondary window that provides control over each
of these elements through dropdown menus, similar to how control over these
elements is provided in software such as PureData or Max/MSP.

Additionally, I made a minor improvement this week to the interface by altering
the signals and slots code such that the buttons in the interface that result in
dialog windows no longer stay highlighted after the action has been carried out.
This was solved by changing instances of the pressed() signal to clicked()
instances.

### Audio Settings Class
As a step towards solving the problem stated previously, I have created a new
class: AudioSettings, containing the parameters mentioned above. It is designed
to be used as a "singleton" object, with one instantiation in the main function
and a reference to it passed to other objects in the program, so that they can
share the same settings. To this end, I will design the class so that the copy
constructor and assignment operator functions are private. This is a common C++
technique when you wish to disallow copying or assigning of an object to another
object, which is relevant in this case.

###  Audio Analysis
Although I do not wish to add many more elements to the program, before
finalising things, I would like to improve the audio analysis code somewhat. The
spectral kurtosis feature was added recently to improve the overall unit
selection performance. I would like to spend some time if possible to improve
the feature extraction code and make it more modular. Additionally, I may add a
normalisation stage to the feature extractors.

### Future Plans
I am aiming to get the writing and documentation finished within the next week
or so, while also doing a bit more work and unit test writing for the program.
Then, the final week will be dedicated to improving the program, finalising the
CMake build/executables and tying up other loose ends.

