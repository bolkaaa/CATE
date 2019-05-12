---
layout: post
title: 'Final Post'
date: '2019-05-12'
---

# Final Improvements to Synthesis Engine
After improving the underlying granular synthesis engine last week, I have added
another feature, which is control over grain frequency. This was achieved by
reworking the wavetable lookup code so that the increment depends upon the grain
table size, frequency parameter and sample rate.

I have also now recombined the analysis and synthesis processes. The system
works by creating a queue of corpus indices from the analysis process, where
each index is a unit from the corpus that corresponds to the indices in the
GrainPool in the Granulator. I was briefly considering a system where the output
of the analysis process was a more complex data structure, and a table lookup
would convert this to a corresponding index to be used by Granulator. I later
realised this was unnecessary, as the corpus indices correspond 1:1 to the grain
indices anyway. The indices cycled through by the Granulator can then by updated
with indices from the queue. I am still not fully satisfied with the approach,
however I will likely finalise the synthesis system at this point.

# More Tests and Error Handling
I have also been working on other parts of the program, and writing more tests
has helped me improved certain areas. For example, I have added error handling
to the audio file reading/writing functionality, to handle cases such as an
invalid audio file being loaded. An exception is now thrown by these functions
which can be handled by the caller. I plan to write some more tests as I wrap up
writing the program, to ensure that there are no problematic bugs in the final
version.

# Future Work
With the deadline being extended until the 17th, I have some extra time to do
some more work on the program and dissertation. My plans for this are mostly to
focus on the following:

- Code cleanups/refactoring
- Polishing the GUI
- Final tests and bug fixes
- Compiling binary executables for deployment
