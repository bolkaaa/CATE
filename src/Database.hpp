#include <string>
#include <vector>

#include "AudioBuffer.hpp"
#include "FileTree.hpp"

template <class T>
class Database
{
public:
    Database()
    {
    }

    void add_file(const std::string &path)
    {
	buffers.push_back(AudioBuffer<T>(path));
    }

    void add_directory(const std::string &path)
    {
	std::vector<std::string> paths;
	get_nested_files(paths, path);

	for (auto p : paths)
	{
	    buffers.push_back(AudioBuffer<T>(p));
	}

    }

    AudioBuffer<T>& operator[](unsigned int i)
    {
	return buffers[i];
    }

private:
    std::vector<AudioBuffer<T> > buffers;
};
