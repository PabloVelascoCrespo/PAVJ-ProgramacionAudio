#pragma once

#include "AudioBuffer.h"

class AudioSource
{
	ALuint source;

public:
	AudioSource(AudioBuffer* buffer);
	~AudioSource();



	void setPitch(float pitch);
	float getPitch() const;

	void setGain(float gain);
	float getGain() const;

	void setLooping(bool loop);
	void getPosition(float& x, float& y, float& z) const;

	void setPosition(float x, float y, float z);

	void setVelocity(float x, float y, float z);

	void play();
	void stop();
	void pause();
	bool isPlaying() const;
};

