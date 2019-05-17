# CATE
A corpus-based concatenative synthesis application written in C++ (work in progress). The user may load directories of audio files, which are segmented and analysed with various spectral analysis functions. The segments are stored in memory as grains of audio data, and the analysis data is written to a stored JSON file. During playback, the microphone input is analysed with same analysis functions. A nearest-neighbors search of the currently loaded audio corpus selects the grains that are played back through a granular synthesis process.

![Window](https://files.catbox.moe/x7mdsg.png)

### Build dependencies
- portaudio
- libsndfile
- fftw3
- boost
- qt5

### Building with CMake
Make sure the various dependencies are installed on your system. On MacOS, [homebrew](https://brew.sh/) has each of them. Most Linux distributions will also have them, and on Windows you should be able to install them manually. The project should then build as follows: 
```
mkdir build
cd build
cmake ..
make
```

### Using the Python scripts
```
cd ./scripts
python3 -m venv venv
source ./venv/bin/activate
pip3 install matplotlib librosa
python3 ./script.py
```
