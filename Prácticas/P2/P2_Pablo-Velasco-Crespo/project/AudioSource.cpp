#include <AudioSource.h>

AudioSource::AudioSource(AudioBuffer* buffer)
{
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer->getAlBuffer());
	setPitch(1.0f);
	setGain(1.0f);
	setLooping(true);
	setPosition(0.0f, 0.0f, 0.0f);
	setVelocity(0.0f, 0.0f, 0.0f);
}

AudioSource::~AudioSource()
{
	alDeleteSources(1, &source);
}

void AudioSource::setPitch(float pitch)
{
	alSourcef(source, AL_PITCH, pitch);
}

float AudioSource::getPitch() const
{
	float pitch;
	alGetSourcef(source, AL_PITCH, &pitch);
	return pitch;
}

void AudioSource::setGain(float gain)
{
	alSourcef(source, AL_GAIN, gain);
}

float AudioSource::getGain() const
{
	float gain;
	alGetSourcef(source, AL_GAIN, &gain);
	return gain;
}

void AudioSource::setLooping(bool loop)
{
	alSourcef(source, AL_LOOPING, loop);
}

void AudioSource::getPosition(float& x, float& y, float& z) const
{
	alGetSource3f(source, AL_POSITION, &x, &y, &z);
}

void AudioSource::setPosition(float x, float y, float z)
{
	alSource3f(source, AL_POSITION, x, y, z);
}

void AudioSource::setVelocity(float x, float y, float z)
{
	alSource3f(source, AL_VELOCITY, x, y, z);
}

void AudioSource::play()
{
	alSourcePlay(source);
}

void AudioSource::stop()
{
	alSourceStop(source);
}

void AudioSource::pause()
{
	alSourcePause(source);
}

bool AudioSource::isPlaying() const
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

ALuint AudioSource::getSource() const
{
	return source;
}
