#pragma once
#include <AL/al.h>
#include <fstream>
#include <vector>


class AudioBuffer
{
	ALuint buffer;

public:
	static AudioBuffer* load(const char* filename);
	ALuint getAlBuffer() const;

	~AudioBuffer();
};

