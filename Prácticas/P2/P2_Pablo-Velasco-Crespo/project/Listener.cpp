#include "Listener.h"

void Listener::setPosition(float x, float y, float z) 
{
    alListener3f(AL_POSITION, x, y, z);
}

void Listener::setVelocity(float x, float y, float z)
{
    alListener3f(AL_VELOCITY, x, y, z);
}

void Listener::setOrientation(float forwardX, float forwardY, float forwardZ,
    float upX, float upY, float upZ) 
{
    float orientation[] = { forwardX, forwardY, forwardZ, upX, upY, upZ };
    alListenerfv(AL_ORIENTATION, orientation);
}
