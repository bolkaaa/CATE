# CATE
A corpus-based concatenative synthesis application written in C++ (work in progress). 

### Build dependencies
- portaudio
- portaudiocpp
- libsndfile
- libsamplerate
- boost/filesystem

### Building with CMake
```
mkdir build
cd build
cmake ..
make
```

### Using Python scripts
```
cd ./scripts
python3 -m venv venv
source ./venv/bin/activate
pip3 install matplotlib librosa
python3 ./script.py
```
