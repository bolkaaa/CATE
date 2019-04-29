# CATE
A corpus-based concatenative synthesis application written in C++ (work in progress). 

![Window](https://files.catbox.moe/1gctlq.png)

### Build dependencies
- portaudio
- libsndfile
- libsamplerate
- fftw3
- boost/filesystem
- qt5

### Building with CMake
Make sure the various dependencies are installed on your system. On MacOS, [homebrew](https://brew.sh/) has each of them. Most Linux distributions will also have them, and on Windows you should be able to install them manually. Setting the CMAKE_PREFIX_PATH variable within CMakeLists to your system's Qt installation path will probably be necessary so CMake can find it. The project should then build as follows: 
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
