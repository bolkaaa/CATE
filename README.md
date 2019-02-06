# CATE
A corpus-based concatenative synthesis application written in C++ (work in progress). 

### Build dependencies
- portaudio
- portaudiocpp
- libsndfile
- boost/filesystem

### Building with CMake
1. mkdir build
2. cd build
3. cmake ..
4. make

### Using Python scripts
1. cd ./scripts
2. python3 -m venv venv
3. source ./venv/bin/activate
4. pip3 install matplotlib librosa
5. python3 ./script.py
