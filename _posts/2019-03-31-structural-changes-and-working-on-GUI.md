---
layout: post
title: 'Structural Changes and Working on GUI'
date: '2019-03-31'
---
### Progress
I have focused on writing my report this week, but have also made some
improvements and additions to the program, which I will discuss in the following sections.

### Structural Changes
Previously, my program was designed such that the AudioProcess object that
contains most of the audio functionality of the program was a member of the
MainWindow class. This caused a coupling of backend and frontend logic that I
wanted to move away from. Therefore I am now instantiating the AudioProcess
object earlier in the main function and passing it by reference to MainWindow,
which is a more flexible way of working.

### More Granular Synthesis Improvements
The grain envelope implementation now functions much more smoothly, with control
over attack and release of each grain, using an ASR envelope shape. I first
prototyped this in Python which was very helpful due to being able to plot the
tests with Matplotlib. I also implementing functionality for modifying the
envelope parameters, which I will come back to in a later section of this post.
Another improvement was in the grain inter-onset generator. An average onset
time in samples is calculated by dividing the audio system's sample rate by a
*grain density* parameter, which is effectively the average number of grains per
second. A *grain width* parameter determines how far apart a minimum and maximum
onset are from the average onset. Then, the random inter-onset time is
calculated based upon these values.

The overall sound is still not perfect, but has improved a lot in the past few
weeks. I will now be aiming to focus on improving other parts of the program
rather than the sound of the granulation in particular.

### Starting To Work More On The Qt GUI
This week I also started doing more work on the Qt GUI of the program. I am
using *Qt Designer* for a WYSIWYG (What You See Is What You Get) approach to GUI
design. At the moment, the interface has buttons for starting and stopping audio
playback, along with sliders for controlling certain synthesis parameters. As
mentioned earlier, I had to create a new class for storing parameters for the
grain envelopes, which is passed around in the granular synthesis object system.
This allows for control of the grain attack and release from the top-level
object in the hierarchy. The list of controllable parameters so far is as
follows:

- Overall Amplitude
- Grain Attack
- Grain Release
- Grain Density

I will likely add more parameters later, along with other potential additions
such as analysis visualisations.

### Next Goals
I had meant to include functionality for recording audio output in one of my
earlier weekly schedule targets, but I never got around to doing so. One
disadvantage of creating a standalone audio processing application as opposed to
a VST/AU plugin, is that this kind of functionality must be implemented, which
can be difficult. For the program to be truly *useful*, it should have this kind
of functionality. Otherwise, it is more of a program solely for experimentation
rather than musical usefulness. Therefore I will try to get this functionality
working as soon as possible. Besides that, I will be doing more work on the GUI
as well as thinking about including unit tests to systematically test the
correctness of certain aspects of the program.
