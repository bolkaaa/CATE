#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

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
	AudioBuffer<T> buffer(path);
	std::pair<std::string, AudioBuffer<T>> pair(path, buffer);
	buffers.insert(pair);
    }

    void add_directory(const std::string &directory_path)
    {
	std::vector<std::string> file_paths;
	get_nested_files(file_paths, directory_path);

	for (auto path :file_paths)
	{
	    AudioBuffer<T> buffer(path);
	    std::pair<std::string, AudioBuffer<T>> pair(path, buffer);
	    buffers.insert(pair);
	}
    }

    void clear()
    {
    	buffers.clear();
   }

    void convert_sample_rates(unsigned int new_sr)
    {
    	for (auto &buffer : buffers)
    	{
    	    buffer.second.convert_sample_rate(new_sr);
    	}
    }

    AudioBuffer<T>& operator[](std::string filename)
    {
    	return buffers[filename];
    }

    unsigned int size() const { return buffers.size(); }

    std::unordered_map<std::string, AudioBuffer<T>> buffers;

private:
    // std::unordered_map<std::string, AudioBuffer<T>> buffers;

};

#endif
